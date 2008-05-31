#ifndef RARPROCESSHANDLER_H
#define RARPROCESSHANDLER_H

#include "overwritedialog.h"
#include "akuprogressdialog.h"
#include "threadprocess.h"
#include "akuerrordialog.h"

#include <QStringList>
#include <QProcess>
#include <QByteArray>
#include <QGridLayout>
#include <QPushButton>
#include <QFileInfo>
#include <QDateTime>
#include <QUrl>
#include <QTimer>
#include <QDir>
#include <KMimeType>
#include <KLocale>
#include <KPasswordDialog>
#include <KIcon>
#include <QPixmap>
#include <KMessageBox>
class rarProcessHandler : public QObject
{
 Q_OBJECT

 public:
  rarProcessHandler(  QWidget* parent = 0,QString = "", QStringList = QStringList(), QString="", QStringList = QStringList(), QString="");
  ~rarProcessHandler();
  QString standardOutput();
  QByteArray rawStandardOutput();
  void showError(QByteArray);
  static void showError(QString);
  bool completedCorrectly();
  bool isCrypted();
  QString getArchivePassword();

 signals:
  void processCompleted(bool);
  void processCanceled();
  void outputReady(QString, bool);
 public slots:
  virtual void getError();
  virtual void showProgress();
  virtual void handleCancel();
  virtual void start();
  
 private:
  QWidget *parentWidget;
  threadProcess *rarProc;
  QByteArray rarProcError;
  akuProgressDialog *rarProcProgress;
  QString stdOutput;
  QByteArray rawOutput;
  QString process;
  QStringList params;
  QString rarArchive;
  QStringList filesToHandle;
  QStringList attributesToHandle;
  QString globalTOC;
  QString pathTarget;
  int totalFileCount;
  QTimer *processTimer;
  bool toAll;
  bool done;
  bool crypted;
  bool passwordAsked;
  QTextEdit *outPut;
  QString archivePassword;
  bool hasPasswordParameter;
  int passwordRequests;
  QString globalArchiver;
  bool totalExtraction;
  int pID;
  akuErrorDialog *errorDialog;
 protected slots:
  virtual void initProcess();
  virtual void handleProcess();
  virtual void handlePaused();
  virtual void handleContinued();
  virtual void giveOutput(int, QProcess::ExitStatus);

};
#endif
