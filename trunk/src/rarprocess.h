#ifndef RARPROCESS_H
#define RARPROCESS_H

#include <QWidget>

#include <KPasswordDialog>
#include <KPushButton>
#include <KMessageBox>
#include <KTemporaryFile>

#include <KDebug>

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
    void passwordCanceled();
    void tempFiles(QString);
    void activeInterface(bool);

  public slots:
    virtual void start(QString = "");
    virtual void showProgress();
    virtual void getError();
    virtual void handleCancel();
    virtual void miniGetError();
    virtual void miniGetOutput();
  
  private:
    QWidget *parentWidget;
   
    QString archiver;
    QString archivename;
    QString destination;
    QString stdoutput;
    QString globalTOC;
    QString archivePassword;
    QString filePassword;

    QStringList options;
    QStringList files;
    QStringList fileswithpassword;
    QStringList listpfiles;
  
    QTimer *processTimer;

    KTemporaryFile tmpPassFile;

    KPasswordDialog *dlg;

    bool noproblem;
    bool headercrypted;
    bool toall;
    bool passwordAsked;
    bool rememberPassword;
    //bool hasPasswordParameter;

    int totalFileCount;

    QByteArray streamerror;
    QByteArray rawoutput;

    akuErrorDialog *errorDialog;

    akuProgressDialog *rarprogressdialog;

    threadProcess *thread;
    threadProcess *minithread;

  protected slots:
    virtual void initProcess();
    virtual void giveOutput(int, QProcess::ExitStatus);
    virtual void handleProcess();
    virtual void handlePaused();
    virtual void handleContinued();
    virtual void handlePasswordedFiles(bool incorrectPassword = false);
    virtual void setPassword(const QString&, bool); 
    virtual void nextPasswordedFile();
    virtual void miniGiveOutput();
};
#endif
