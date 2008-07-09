#ifndef TARPROCESS_H
#define TARPROCESS_H

#include <QWidget>

#include "threadprocess.h"
#include "akuerrordialog.h"

class tarProcess : public QObject
{
  Q_OBJECT

  public: 
    tarProcess(QWidget* parent = 0, QString = "", QStringList = QStringList(), QString = "", QStringList = QStringList(), QString = "");
    ~tarProcess();
   
    void showError(QByteArray);
   
    QString standardOutput();
 
  private:
    QWidget *parentWidget;
   
    QString archiver;
    QString archivename;
    QString destination;
    QString stdoutput;

    QStringList options;
    QStringList files;
    QStringList filesToHandle;

    bool noproblem;
    bool headercrypted;  
   
    QByteArray streamerror;
    QByteArray rawoutput;

    akuErrorDialog *errorDialog;

    threadProcess *thread;

  signals:
    void processCompleted(bool);
    void processCanceled();
    void outputReady(QString, bool);
 
  public slots:
    virtual void start();
    virtual void showProgress();
    virtual void getError();
 
   protected slots:
     virtual void initProcess();
     virtual void giveOutput(int, QProcess::ExitStatus);

};
#endif
