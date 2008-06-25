#ifndef RARPROCESS_H
#define RARPROCESS_H

#include <QWidget>

#include <KPasswordDialog>
#include <KMessageBox>

#include "threadprocess.h"
#include "akuerrordialog.h"
#include "overwritedialog.h"
#include "akuprogressdialog.h"

class rarProcess : public QObject
{
  Q_OBJECT

  public: 
    rarProcess(QWidget* parent = 0, QString = "", QStringList = QStringList(), QString = "", QStringList = QStringList(), QString = "");
    ~rarProcess();

    void showError(QByteArray);
  
    QString getArchivePassword();
    QString standardOutput();

  signals:
    void processCompleted(bool);
    void processCanceled();
    void outputReady(QString, bool);

  public slots:
    virtual void start();
    virtual void showProgress();
    virtual void getError();
    virtual void handleCancel();
  
  private:
    QWidget *parentWidget;
   
    QString archiver;
    QString archivename;
    QString destination;
    QString stdoutput;
    QString globalTOC;
    QString archivePassword;

    QStringList options;
    QStringList files;
  
    QTimer *processTimer;

    bool noproblem;
    bool headercrypted;
    bool toall;
    bool passwordAsked;
    bool hasPasswordParameter;

    int totalFileCount;

    QByteArray streamerror;
    QByteArray rawoutput;

    akuErrorDialog *errorDialog;

    akuProgressDialog *rarprogressdialog;

    threadProcess *thread;

  protected slots:
    virtual void initProcess();
    virtual void giveOutput(int, QProcess::ExitStatus);
    //virtual void handleProcess();
    virtual void handlePaused();
    //virtual void handleContinued();
};
#endif
