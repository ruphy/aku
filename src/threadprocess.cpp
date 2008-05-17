#include "threadprocess.h"

threadProcess::threadProcess(QObject * parent) : QThread(parent), d(new threadProcessPrivate)
{
 connect(d->process, SIGNAL(readyReadStandardOutput()), this, SIGNAL(readyReadStandardOutput()));
 connect(d->process, SIGNAL(readyReadStandardError()), this, SIGNAL(readyReadStandardError()));
}

threadProcess::~threadProcess()
{}

void threadProcess::run()
{
  d->process -> start(d->program, d->params);
}

void threadProcess::start(QString prog, QStringList par, QThread::Priority prior)
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
  d->process->terminate();
}

void threadProcess::pauseProcess()
{
  QProcess pauseProc;
  pauseProc.start("kill", QStringList() << "-s"<<"SIGSTOP" << QString().setNum(d->process->pid()));
  pauseProc.waitForFinished();
}

void threadProcess::continueProcess()
{
  QProcess continueProc;
  continueProc.start("kill", QStringList() << "-s"<<"SIGCONT" << QString().setNum(d->process->pid()));
  continueProc.waitForFinished();
}
