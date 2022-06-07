#include "fingerprint_window.h"
#include <iostream>
#include <QFileDialog>
#include <QtXml/QDomDocument>
#include <QTextStream>
#include <QMessageBox>
#include <QDesktopWidget>
#include <iomanip>
#include <QToolTip>
#include <time.h>
#include "threading.h"
#include <windows.h>

Fingerprint_Window::Fingerprint_Window(QWidget* parent)
    :QMainWindow(parent), fingerprintW(new Ui::GeneratorWindow)
{
    fingerprintW->setupUi(this);

    fingerprintW->ClassDistri->setToolTip("Select the distribution of fingerprints to be generated.");
    fingerprintW->FingersPerFolder->setToolTip("Set the number of fingerprints per directory. Some filesystems \nhave limit on maximum number of files per directory and \nlisting will be slower if directory contains to many files.");
    fingerprintW->Generate->setToolTip("Click to start the generation of fingerprints.");
    fingerprintW->ImageType->setToolTip("Choose the type of fingerprint image.");
    fingerprintW->ImprPerFinger->setToolTip("Set the number of impressions to be generated per fingerprint.");
    //fingerprintW->metaSave->setToolTip("Check if meta information, like fingerprint type, \nis to be saved in text file.");
    fingerprintW->NumOfFinger->setToolTip("Set the number of fingerprints to be generated.");
    fingerprintW->OutputDir->setToolTip("Set the directory where fingerprints, impressions \nand meta information is to be saved.");
    fingerprintW->StartFinger->setToolTip("Set the number from where the numbers to \nfingerprint images should be given.");
    fingerprintW->threads->setToolTip("Set the number of threads to run to generate \nfingerprints simultaneously.");
    fingerprintW->customCheck->setToolTip("Check if custom seed value is to be given for \nramdom number generator.");
    fingerprintW->seedVal->setToolTip("Enter the value of seed for random number generator.");
    fingerprintW->noise_level->setToolTip("Change the interval for noise level");
    fingerprintW->num_scratches->setToolTip("Change the interval for number of scatches per Impression");
    fingerprintW->trans_slider->setToolTip("Change the translation (in pixels).");
    fingerprintW->rot_slider->setToolTip("Change the rotation (in degrees).");

    connect(fingerprintW->pushButton, SIGNAL(clicked()), this, SLOT(Opendir_Clicked()));
    connect(fingerprintW->Generate, SIGNAL(clicked()), this, SLOT(Finger_Generate_Clicked()));
    connect(fingerprintW->customCheck, SIGNAL(toggled(bool)), SLOT(custom_Clicked(bool)));


    connect(fingerprintW->rot_slider, SIGNAL(valueChanged(int)), SLOT(rotation_sliderChanged(int)));
    connect(fingerprintW->trans_slider, SIGNAL(valueChanged(int)), SLOT(translation_sliderChanged(int)));
    connect(fingerprintW->noise_level, SIGNAL(spanChanged(int, int)), SLOT(sliderChanged(int, int)));
    connect(fingerprintW->num_scratches, SIGNAL(spanChanged(int, int)), SLOT(sliderChanged(int, int)));

    setFixedSize(this->width(), this->height() - 25);

    QRect frect = frameGeometry();
    frect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(frect.topLeft());
    fingerprintW->customCheck->setChecked(false);
    fingerprintW->seedVal->setEnabled(false);
}
Fingerprint_Window::~Fingerprint_Window()
{

}

/*
  Handles the range slider change event and shows the selected range as tool tip.
  */
void Fingerprint_Window::sliderChanged(int low, int high)
{
    std::stringstream value;
    value << low << " - " << high;
    QToolTip::showText(QCursor::pos(), value.str().c_str());
}

/*
  Handles the change event of rotation slider to show the current
  value chosen.
  */
void Fingerprint_Window::rotation_sliderChanged(int val)
{
    std::stringstream value;
    value << val << "<sup>o</sup>";

    fingerprintW->rotation_label->setText(value.str().c_str());
}

/*
  Handles the change event of translation slider to show the current
  value chosen.
  */
void Fingerprint_Window::translation_sliderChanged(int val)
{
    std::stringstream value;
    value << val;
    fingerprintW->translation_label->setText(value.str().c_str());
}

/*
  Handles the check event of custom seed check box to enable or disable the
  input box for custo seed value.
  */
void Fingerprint_Window::custom_Clicked(bool val)
{
    if (val)
    {
        fingerprintW->seedVal->setEnabled(true);
    }
    else
    {
        fingerprintW->seedVal->setEnabled(false);
    }
}

/*
  Displays the dialog box for selecting the Folder.
  Returns the selected path.
  */
QString Fingerprint_Window::OpenDir_Dialog() {
    QString path;
    QFileDialog dialog;

    dialog.setWindowModality(Qt::WindowModal);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    path = dialog.getExistingDirectory(this, tr("Select Directory"), ".", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    return path;
}

/*
  Handle the clicked signal of Output Directory browse
  button.
  */
void Fingerprint_Window::Opendir_Clicked()
{
    QString path;
    path = OpenDir_Dialog();

    if (path.size() != 0)
        fingerprintW->OutputDir->setText(path);
}

/*
  Handles the clicked signal of Generate Button. Initializes the
  variables, makes appropriate connections between signals and
  handler routines.
  */
void Fingerprint_Window::Finger_Generate_Clicked()
{
    
    app = new threaded_app();   
    if (!setVals())
        return;
    Dialog.reset();
    
    Dialog.setLabelText("Generating Fingerprints");
    Dialog.setMaximum(100);
    Dialog.connect(app, SIGNAL(update(int)), SLOT(set_values(int)));
    Dialog.setWindowModality(Qt::WindowModal);
    Dialog.setFixedSize(400, 120);
    Dialog.show();
    app->setDialog(&Dialog);
    this->connect(app, SIGNAL(finish()), SLOT(worker_finished()));
    app->start();
    fingerprintW->Generate->setEnabled(false);
    start_time = QDateTime::currentDateTime();
}

/*
  Handles the finish signal emitted by main worker thread created in Generate_Clicked()
  function. Does the cleanup and re-enables the Generate button.
  */
void Fingerprint_Window::worker_finished()
{
    delete app;
    th = NULL;
    app = NULL;
    fingerprintW->Generate->setEnabled(true);
    Dialog.hide();
    if (!Dialog.wasCanceled())
        QMessageBox::information(this, "zhanghongfei", "Fingerprint Generation is Complete!", QMessageBox::Ok, QMessageBox::Ok);
}

/*
  Initializes the variables with appropriate values from GUI components.
  This sould be called before generation starts.
  */
bool Fingerprint_Window::setVals()
{
    no_fin = atoi(fingerprintW->NumOfFinger->text().toStdString().c_str());
    size_fo = atoi(fingerprintW->FingersPerFolder->text().toStdString().c_str());
    n_impr = atoi(fingerprintW->ImprPerFinger->text().toStdString().c_str());

    path_fing = fingerprintW->OutputDir->text().toStdString();


    n_threads = atoi(fingerprintW->threads->text().toStdString().c_str());
    start_from_finger = atoi(fingerprintW->StartFinger->text().toStdString().c_str());
    class_distribution = fingerprintW->ClassDistri->currentIndex();
    Wid = fingerprintW->ImageType_2->currentIndex();
    image_type = fingerprintW->ImageType->currentText().toStdString();
    generated = 0;
    use_custom_seed = (fingerprintW->customCheck->checkState() == Qt::Checked);
    if (use_custom_seed)
        custom_seed = fingerprintW->seedVal->value();
    else
        custom_seed = time(NULL);

    fingerprintW->seedVal->setValue(custom_seed);

    max_noise = fingerprintW->noise_level->upperPosition();
    min_noise = fingerprintW->noise_level->lowerPosition();
    max_scratches = fingerprintW->num_scratches->upperPosition();
    min_scratches = fingerprintW->num_scratches->lowerPosition();
    rotation = fingerprintW->rot_slider->value();
    translation = fingerprintW->trans_slider->value();

    if (app->create_dir(path_fing.c_str()) == false) {
        Dialog.hide();
        fingerprintW->Generate->setEnabled(true);
        return false;
    }
    return true;
}

