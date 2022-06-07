#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QProgressBar>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <vector>
#include <QMutex>
#include <QWaitCondition>

class dialog : public QDialog
{
    Q_OBJECT
public:
    explicit dialog(QWidget* parent = 0);

    void lockMutex() {
        mutex.lock();
    }

    void unloMutex() {
        mutex.unlock();
    }

    void waitForCondition()
    {
        wakeup_condition.wait(&mutex);
    }

public slots:
    void set_values(int y);
    void setValue(int progress);
    void setLabelText(const QString& text);
    void canceled();
    void paused();
    void setMaximum(int maximum);
    bool wasCanceled();
    bool reset();
    bool wasPaused();

signals:
    void pausedS();
    void resumedS();
    void exitedS();

private:
    void closeEvent(QCloseEvent* event);
    bool cancelStatus;
    bool pauseStatus;
    QLabel* label;
    QPushButton* pause, * cancel;
    QProgressBar* progressBar;
    QHBoxLayout* vb2;
    QVBoxLayout* toplayout;
    QMutex mutex;
    QWaitCondition wakeup_condition;
};

class console_progressDialog : public QObject
{
    Q_OBJECT
public:
    console_progressDialog();
    void console(const char* text);
public slots:
    void set_values(int y);
private:
    char progress[53];
};

#endif
