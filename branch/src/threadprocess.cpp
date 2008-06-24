#include "threadprocess.h"

threadProcess::threadProcess(QObject * parent) : QThread(parent), d(new threadProcessPrivate)
{
 connect(d->process, SIGNAL(readyReadStandardOutput()), this, SIGNAL(readyReadStandardOutput()));
 connect(d->process, SIGNAL(readyReadStandardError()), this, SIGNAL(readyReadStandardError()));
 connect(d->process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SIGNAL(finished(int, QProcess::ExitStatus))); 
}

threadProcess::~threadProcess()
{}

void threadProcess::run()
{
  d->process -> start(d->program, d->params);
}

void threadProcess::start(QString prog, QStringList par, QThread::Priority)
{
 d->program = prog;
 d->params = par;
 run();
}

bool threadProcess::waitForFinished( int msecs)
{
 return d->process -> waitForFinished(msecs);
}

QByteArray threadProcess::readAllStandardOutput()
{
 return d->process -> readAllStandardOutput();
}

QByteArray threadProcess::readAllStandardError()
{
 return d->process -> readAllStandardError();
}

QProcess::ProcessState threadProcess::state()
{
 return d->process -> state();
}

QProcess* threadProcess::proc()
{
 return d->process;
}

void threadProcess::killProcess()
{
  d->process->kill();
}

void threadProcess::pauseProcess()
{
#ifdef Q_WS_X11
  QProcess pauseProc;
  pauseProc.start("kill", QStringList() << "-s"<<"SIGSTOP" << QString().setNum(d->process->pid()));
  pauseProc.waitForFinished();
#endif
}

void threadProcess::continueProcess()
{
#ifdef Q_WS_X11
  QProcess continueProc;
  continueProc.start("kill", QStringList() << "-s"<<"SIGCONT" << QString().setNum(d->process->pid()));
  continueProc.waitForFinished();
#endif
}
