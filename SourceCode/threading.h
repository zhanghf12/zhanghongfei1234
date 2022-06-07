#ifndef THREADING_H
#define THREADING_H
#include <QThread>
#include <QObject>
#include <QProgressDialog>
#include "generation.h"
#include "dialog.h"

using namespace zhanghongfei;
using namespace std;

class QThreadEx : public QThread
{
public:
    void run() { exec(); }
};

class thread_work : public QObject, public Generator
{
    Q_OBJECT
public:
    thread_work() { }
    ~thread_work() { }
public slots:
    void generate();
    void generate_density_maps();
signals:
    void update(int y);
    void finish();
public:
    int start_finger;
    dialog* Dialog;
    console_progressDialog* console_progress;
};

class noise_work : public QObject, public noise::Noise
{
    Q_OBJECT
public:
    noise_work() { }
    ~noise_work() { }
public slots:
    void generate_impressions();
signals:
    void update(int y);
    void finish();
public:
    dialog* Dialog;
    console_progressDialog* console_progress;
    QString input_dir;
    QString output_dir;
    int id;
};

//class threaded_app : public QObject
//{
//    Q_OBJECT
//public:
//    threaded_app() { }
//    ~threaded_app() { }
//    bool creat_dir(const char* path);
//    void setDialog(dialog* dialog)
//    {
//        Dialog = dialog;
//    }
//    void setConsoleProgress(console_progressDialog* dialog)
//    {
//        console_progress = dialog;
//    }
//    void delete_filter();
//    void save_config(QString path);
//    void showErr(QString err);
//public slots:
//    void start();
//    void noise_start();
//    void child_finished();
//signals:
//    void update(int y);
//    void finish();
//private:
//    dialog* Dialog;
//    console_progressDialog* console_progress;
//    vector <QThreadEx*> thread_pool;
//    vector <thread_work*> work_pool;
//    vector <noise_work*> nwork_pool;
//    int childs_finished;
//
//};
class threaded_app : public QObject
{
    Q_OBJECT
public:
    threaded_app() { childs_finished = 0; }
    ~threaded_app() { }
    bool create_dir(const char* path);
    void setDialog(dialog* dialog)
    {
        Dialog = dialog;
    }
    void setConsoleProgress(console_progressDialog* dialog)
    {
        console_progress = dialog;
    }
    void delete_filter();
    //void save_config(QString path);
    void showErr(QString err);
public slots:
    void start();
    void noise_start();
    void child_finished();
signals:
    void update(int y);
    void finish();
private:
    dialog* Dialog;
    console_progressDialog* console_progress;
    vector <QThreadEx*> thread_pool;
    vector <thread_work*> work_pool;
    vector <noise_work*> nwork_pool;
    int childs_finished;
};
#endif
