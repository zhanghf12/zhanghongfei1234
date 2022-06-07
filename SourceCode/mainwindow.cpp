#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QToolTip>

using namespace std;
using namespace zhanghongfei;



MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(this->width(), this->height() - 25);
    f_window = NULL;

    ui->fingerprints->setToolTip("Generate New Fingerprints Database.");
    ui->exit->setToolTip("Exit zhanghongfei.");
    connect(ui->fingerprints, SIGNAL(clicked()), this, SLOT(fingerprints_Clicked()));
    connect(ui->exit, SIGNAL(clicked()), qApp, SLOT(quit()));
    QRect frect = frameGeometry();
    frect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(frect.topLeft());
}

/*
*  MainWindow Destructor.
*/
MainWindow::~MainWindow()
{
    delete ui;
}

/*
*  Autocreated from QT
*/
void MainWindow::changeEvent(QEvent* e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/*
  Handles the clicked event of Generate Fingerprint button.
  */
void MainWindow::fingerprints_Clicked()
{
    if (f_window == NULL)
    {

        f_window = new Fingerprint_Window(this);
        f_window->setWindowModality(Qt::WindowModal);
        f_window->setWindowTitle("zhanghongfei: 123");
    }
    //f_window->Fingerprint_load_config();

    f_window->show();
}
