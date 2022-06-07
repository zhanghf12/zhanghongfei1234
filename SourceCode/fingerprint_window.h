#ifndef FINGERPRINT_WINDOW_H
#define FINGERPRINT_WINDOW_H
#include <QMainWindow>

#include "threading.h"

#include"ui_Generator.h"
class Fingerprint_Window : public QMainWindow
{
    Q_OBJECT
public:
    Fingerprint_Window(QWidget* parent = 0);
    ~Fingerprint_Window();
    QString OpenDir_Dialog();
    bool setVals();
    //void Fingerprint_load_config();
public slots:
    //void Finger_load_defaults();
    //void Finger_save_config();
    void Opendir_Clicked();
    void Finger_Generate_Clicked();
    void worker_finished();
    void custom_Clicked(bool);
    //void load_conf_clicked();
    void translation_sliderChanged(int val);
    void rotation_sliderChanged(int val);
    void sliderChanged(int low, int high);

private:
    Ui::GeneratorWindow* fingerprintW;
    threaded_app* app;
    dialog Dialog;
    QTimer* t;
    QThreadEx* th;
    //void load_config(QString path);
    //void save_config(QString path);
};

#endif
