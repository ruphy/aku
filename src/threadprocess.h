#ifndef THREADPROCESS_H
#define THREADPROCESS_H

#include <QThread>
#include <QProcess>

class threadProcess : public QThread
{
 Q_OBJECT
  public:
     threadProcess(QObject *parent = 0);
     ~threadProcess();
    QByteArray readAllStandardOutput();
    QByteArray readAllStandardError();
    void run();
    bool waitForFinished(int msec = 30000);
    QProcess::ProcessState state();
    QProcess* proc();
    void pauseProcess();
    void continueProcess();
  private:
    class threadProcessPrivate;
    threadProcessPrivate *d;
    
  public slots:
    void start(QString, QStringList, QThread::Priority = QThread::InheritPriority);
    void killProcess();
  signals:
   void readyReadStandardError(); 
   void readyReadStandardOutput();

};

class threadProcess::threadProcessPrivate 
{
 public:
  threadProcessPrivate() : process( new QProcess){}; 
  QProcess *process;
  QString program;
  QStringList params;
};

#endif

