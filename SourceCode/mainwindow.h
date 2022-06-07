#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "fingerprint_window.h"

using namespace zhanghongfei;

namespace Ui {
    class MainWindow;
    class GeneratorWindow;
}



class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();
public slots:
    void fingerprints_Clicked();
    //void impressions_Clicked();
    //void about_Clicked();
protected:
    void changeEvent(QEvent* e);

private:
    Ui::MainWindow* ui;
    Fingerprint_Window* f_window;
};
#endif
