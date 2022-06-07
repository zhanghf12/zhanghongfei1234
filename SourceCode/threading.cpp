#include "threading.h"

#include "filter_image.h"
#include "generation.h"
#include "densitymap.h"
#include "n_noise.h"
#include "n_cv_type_converter.h"
//#include<qthread.h>
#include <QThread>
#include <QApplication>
#include <QDir>
#include <QtXml/QDomDocument>
#include <QTextStream>
#include <QMessageBox>
#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include <time.h>
#include "GL.h"

using namespace std;
//using namespace zhanghongfei;

/*
  Generates the density maps.
  */
void thread_work::generate_density_maps()
{
    for (n_fp = 1; n_fp <= density_map_count; n_fp = n_fp + 1) {
        random_densitymap();
    }
}

/*
  Generates the fingerprints from starting number to end number which are set before
  calling this function.
  Handle the console mode and GUI mode appropriately.
  */
void thread_work::generate()
{
    for (n_fp = start_finger; n_fp < (no_fin + start_from_finger);) {
        n_fo = ceil((float)n_fp / size_fo);
        //Dialog->setLabelText("dd");
        generation();

        if (is_GUI)
        {
            Dialog->lockMutex();
            if (Dialog->wasPaused())
            {
                Dialog->waitForCondition();
            }
            Dialog->unloMutex();
        }

        emit update(no_fin);
        n_fp += n_threads;

        /*if (is_GUI && Dialog->wasCanceled())
        {
            cout << "Canceled!\n";
            break;
        }*/
    }
    emit finish();
}

/*
  Generates the impressions of fingerprints and stores them in appropriate
  folder.
  */
void noise_work::generate_impressions()
{

    QDir dire(input_dir);

    dire.setFilter(QDir::Dirs);
    QStringList entries = dire.entryList();

    QStringList::ConstIterator entry = entries.begin();

    cvReleaseImage(&f_print1);
    cvReleaseImage(&f_print2);

    f_print1 = 0;
    f_print2 = 0;

    QString str;

    do
    {
        QString name = *entry;
        if (name == "..")
        {
            entry++;
            continue;
        }
        QString path(input_dir);
        path.append("/");
        path.append(name);
        QDir files(path);
        files.setFilter(QDir::Files);
        QStringList filterList;
        filterList << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp" << "*.tif" << "*.tiff";
        files.setNameFilters(filterList);
        files.setSorting(QDir::Name);
        QStringList fileentries = files.entryList();
        QStringList::ConstIterator fileentry = fileentries.begin();
        for (int j = 0; j < id && fileentry != fileentries.end(); j++)
        {
            fileentry++;
        }

        for (; fileentries.size() > 0 && fileentry != fileentries.end();)
        {
            QApplication::processEvents();
            QString file_name = *fileentry;
            str = QString(input_dir).append("/").append(name).append("/").append(file_name);
            f_print1 = cvLoadImage(str.toStdString().c_str(), CV_LOAD_IMAGE_GRAYSCALE);
            noise::Ipltomat2D(f_print1, f_print1_2Dmat);
            for (int i = 0; i < noise::H + noise::margin; i++) {
                for (int j = 0; j < noise::W + noise::margin; j++) {
                    if (f_print1_2Dmat[i][j] > 127)
                        f_print1_2Dmat[i][j] = 255;
                    else
                        f_print1_2Dmat[i][j] = 0;
                }
            }

            int num_scratches = min_scratches + (max_scratches - min_scratches) * noise::ahaq_rand();

            for (int num = 0; num < num_scratches; num++)
            {
                int width = f_print1->width;
                int height = f_print1->height;
                int part = 10;
                int x1 = (width / part) + noise::ahaq_rand() * (((part - 2) * width / part));

                int y1 = (height / part) + noise::ahaq_rand() * (((part - 2) * height / part));

                int x2 = (width / part) + noise::ahaq_rand() * (((part - 2) * width / part));

                int y2 = (height / part) + noise::ahaq_rand() * (((part - 2) * height / part));

                int wide = 1 + noise::ahaq_rand() * 6;

                CvScalar white = CV_RGB(255, 255, 255);
                cvLine(f_print1, cvPoint(x1, y1), cvPoint(x2, y2), white, wide, 4, 0);
            }

            fixed_noise(f_print1);

            f_print2 = cvCloneImage(f_print1);

            for (int i = 0; i < noise::H + noise::margin; i++) {
                for (int j = 0; j < noise::W + noise::margin; j++) {
                    f_print2_2Dmat[i][j] = f_print1_2Dmat[i][j];
                }
            }

            for (int k = 1; k <= n_impr; k++)
            {
                noise::Ipltomat2D(f_print1, f_print1_2Dmat);
                for (int i = 0; i < noise::H + noise::margin; i++) {
                    for (int j = 0; j < noise::W + noise::margin; j++) {
                        if (f_print1_2Dmat[i][j] > 127)
                            f_print1_2Dmat[i][j] = 255;
                        else
                            f_print1_2Dmat[i][j] = 0;
                    }
                }
                random_noise(f_print1);
                str = name + "/" + file_name;
                std::stringstream stream;
                stream << path_fing << "_" << k << "/" << str.toStdString().c_str();

                distortion(stream.str().c_str());

                cvCopy(f_print2, f_print1);
            }
            if (is_GUI)
            {
                Dialog->lockMutex();
                if (Dialog->wasPaused())
                {
                    Dialog->waitForCondition();
                }
                Dialog->unloMutex();
            }

            if (is_GUI && Dialog->wasCanceled())
            {
                cout << "Canceled!\n";
                break;
            }
            emit update(no_fin);
            cvReleaseImage(&f_print1);
            cvReleaseImage(&f_print2);

            for (int j = 0; j < (n_threads) && fileentry != fileentries.end(); j++, fileentry++);
        }
        entry++;

        if (is_GUI && Dialog->wasCanceled())
            break;
    } while (entry != entries.end());

    emit finish();
}

/*
  Checks if the directory, whose path is given as argument, is present or not.
  if directory is not present and creates the directory.
  Return true if directory is present or mkdir does not give error, else return
  false.
  */
bool threaded_app::create_dir(const char* path) {

    if (!QDir(path).exists())
    {
        if (QDir().mkdir(path))
            return true;
    }
    else
        return true;
    showErr(QString("Cannon create directory: ") + path);
    return false;
}

/* Called when a child(worker) thread exits. When all worker
   threads are finished then emits finish signal and releases
   allocated memory.
   */
void threaded_app::child_finished()
{
    childs_finished++;

    if (childs_finished == thread_pool.size())
    {
        while (!thread_pool.empty())
        {
            QThreadEx* tr = thread_pool.back();
            while (tr->isRunning())
            {
                QApplication::processEvents();

                tr->wait(1000);
            }
            thread_pool.pop_back();
            if (!noise_only) {
                thread_work* work = work_pool.back();
                work_pool.pop_back();
                delete work;
            }
            else
            {
                noise_work* work = nwork_pool.back();
                nwork_pool.pop_back();
                delete work;
            }
            delete tr;
        }
        emit finish();
        childs_finished = 0;
    }
}

/*
  Displays the error accroding to mode.
  */
void threaded_app::showErr(QString err)
{
    if (is_GUI)
    {
        QMessageBox::information(Dialog, "zhanghongfei", err, QMessageBox::Ok, QMessageBox::Ok);
    }
    else
    {
        cout << "\rWARNING: " << err.toStdString() << endl;
        exit(0);
    }
}

/*
  Starting point of Generation process. Creates the required directories.
  Creates the threads as per the setting, initializes the worker threads
  appropriately.
  In command line mode waits for each worker thread.
  */
void threaded_app::start()
{
    int start_fo, end_fo, n_fo;
    start_fo = ceil(((float)start_from_finger) / size_fo);
    end_fo = ceil((float)(start_from_finger + no_fin - 1) / size_fo);

    if (create_dir(path_fing.c_str()) == false) {

        showErr("Cannot create Fingerprint Directory");
        return;
    }

    path_o_f = path_fing;
    path_o_f.append("/Fingerprints");

    path_M_F = path_fing;
    path_M_F.append("/Master Fingerprints");


    path_impr = path_fing;
    path_impr.append("/Impression");

    density_map_count = 2000;

    int i;
    for (i = 1; i <= n_impr; i++)
    {
        std::stringstream stream;
        stream << path_impr << "_" << i;
        if (create_dir(stream.str().c_str()) == false) {
            return;
        }
    }
    //cout << endl;
    if (is_GUI)
        Dialog->setLabelText("Creating directories");


    for (n_fo = start_fo; n_fo <= end_fo; n_fo = n_fo + 1) {

        std::stringstream stream;
        stream << path_o_f << "/fp_" << n_fo;

        if (create_dir(stream.str().c_str()) == false) {
            return;
        }


        for (i = 1; i <= n_impr; i++)
        {
            stream.str("");
            stream << path_impr << "_" << i << "/fp_" << n_fo;
            if (create_dir(stream.str().c_str()) == false) {
                return;
            }
        }
    }

    int start_fingerprint = start_from_finger;



    int threads_to_run = n_threads;

    i = 1;

    for (; i <= threads_to_run; i++)
    {
        QThreadEx* tr = new QThreadEx();
        thread_work* work = new thread_work();
        
        work->moveToThread(tr);
        if (is_GUI)
            work->Dialog = Dialog;
        else
            work->console_progress = console_progress;

#ifdef WIN32
        work->generation_seed = Noise::rand_r(&custom_seed);
        work->rand_seed = Noise::rand_r(&custom_seed);
        
#else
        work->generation_seed = rand_r(&custom_seed);
        work->rand_seed = rand_r(&custom_seed);
#endif
        work->start_finger = start_fingerprint++;
        work->connect(tr, SIGNAL(started()), SLOT(generate()));
        tr->connect(work, SIGNAL(finish()), SLOT(quit()));
        connect(work, SIGNAL(finish()), SLOT(child_finished()));
        
        tr->start();

        if (is_GUI)
            Dialog->connect(work, SIGNAL(update(int)), SLOT(set_values(int)));

        else
            console_progress->connect(work, SIGNAL(update(int)), SLOT(console_progress->set_values(int)));
        thread_pool.push_back(tr);
        work_pool.push_back(work);
        //Dialog->setLabelText("dd");
    }

    while (!is_GUI && !thread_pool.empty())
    {
        QApplication::processEvents();
#ifdef WIN32
        _sleep(1000);
#else
        sleep(1);
#endif
    }
}

/*
  Starting point of Generating impressions. Creates the required directories.
  Creates the threads as per the setting, initializes the worker threads
  appropriately.
  In command line mode waits for each worker thread.
  */
void threaded_app::noise_start()
{
    int i;
    QDir dir;
    cout << endl;
    if (!dir.exists(path_noise_in.c_str()))
    {
        if (is_GUI)
        {
            Dialog->canceled();
        }
        showErr(QString("Path does not exists: ") + path_noise_in.c_str());
        emit finish();
        return;
    }

    if (create_dir(path_fing.c_str()) == false) {
        return;
    }

    /*if (!is_GUI)
        save_config(QString(path_fing.c_str()) + "/zhanghongfei.conf");*/

    path_fing.append("/Impression");

    for (i = 1; i <= n_impr; i++) {
        std::stringstream cmd;
        cmd << path_fing << "_" << i;
        if (create_dir(cmd.str().c_str()) == false) {
            return;
        }
    }

    QDir dire(path_noise_in.c_str());
    dire.setFilter(QDir::Dirs);
    QStringList entries = dire.entryList();
    no_fin = 0;
    for (QStringList::const_iterator iter = entries.begin(); iter != entries.end(); iter++)
    {
        QString file_name = *iter;
        if ((entries.size() > 2 && file_name == ".") || file_name == "..")
            continue;
        QString str = dire.absolutePath() + "/" + file_name;
        QDir dir(str);
        dir.setFilter(QDir::Files);
        QStringList filterList;
        filterList << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp" << "*.tif" << "*.tiff";
        dir.setNameFilters(filterList);
        no_fin += dir.entryList().size();
    }
    if (no_fin == 0)
    {
        showErr("No Image Files!");
        return;
    }
    else
    {
        for (i = 1; i <= n_impr; i++)
        {
            std::stringstream cmd;
            cmd << path_fing << "_" << i;
            for (QStringList::ConstIterator entry = entries.begin(); entry != entries.end(); entry++)
            {
                QString name = *entry;
                if (name == "." || name == "..")
                    continue;
                QString path(cmd.str().c_str());
                path.append("/");
                path.append(name);
                if (create_dir(path.toStdString().c_str()) == false)
                {
                    return;
                }
            }
        }
    }


    if (is_GUI)
        Dialog->show();


    for (int i = 0; i < n_threads; i++)
    {
        noise_work* wthread = new noise_work();

        wthread->rotmax = ((double)rotation * 22.0) / (7.0 * 180.0);
        wthread->rotmin = -(wthread->rotmax);

        wthread->transmax = (double)translation;
        wthread->transmin = -(wthread->transmax);

        wthread->max_noiseLevel = max_noise;
        wthread->min_noiseLevel = min_noise;

        wthread->input_dir = path_noise_in.c_str();
        wthread->output_dir = path_fing.c_str();
        wthread->id = i;
        nwork_pool.push_back(wthread);
        QThreadEx* tr = new QThreadEx();
        wthread->moveToThread(tr);

        
        wthread->connect(tr, SIGNAL(started()), SLOT(generate_impressions()));
        tr->connect(wthread, SIGNAL(finish()), SLOT(quit()));
        connect(wthread, SIGNAL(finish()), SLOT(child_finished()));
        if (is_GUI)
            Dialog->connect(wthread, SIGNAL(update(int)), SLOT(set_values(int)));
        else
            console_progress->connect(wthread, SIGNAL(update(int)), SLOT(set_values(int)));

        wthread->Dialog = Dialog;
        thread_pool.push_back(tr);
        tr->start();

    }

    while (!is_GUI && !thread_pool.empty())
    {
        QApplication::processEvents();
#ifdef WIN32
        _sleep(1000);
#else
        sleep(1);
#endif
    }
}

/*
    Deletes the memory allocated for filters and noise blobs.
  */
void threaded_app::delete_filter() {
    int i, k;
    int freq_ind, orient_ind;
    max_filter_size = 0;
    int filter_size;

    for (freq_ind = 0; freq_ind < distnct_f; freq_ind++) {
        for (orient_ind = 0; orient_ind < distnct_o; orient_ind++) {

            filter_size = filter_size_2Dmat[freq_ind][orient_ind];

            for (k = 0; k < filter_size; k++)
                delete(filterbank_4Dmat[freq_ind][orient_ind][k]);

            delete(filterbank_4Dmat[freq_ind][orient_ind]);
        }
    }

    for (i = 0; i < distnct_f; i++)
        delete(filterbank_4Dmat[i]);

    delete(filterbank_4Dmat);
}




