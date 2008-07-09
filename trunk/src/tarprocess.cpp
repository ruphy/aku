#include "tarprocess.h"

tarProcess::tarProcess(QWidget *parent, QString tararchiver, QStringList taroptions, QString tararchivename, QStringList tarfiles, QString tardestination) : QObject(parent)
{
  parentWidget = parent;
  archiver = tararchiver;
  options = taroptions;
  archivename = tararchivename;
  files = tarfiles;
  destination = tardestination;
  noproblem = false;
  headercrypted = false;
  errorDialog = new akuErrorDialog();
}

tarProcess::~tarProcess()
{
  qDeleteAll(children());
}

void tarProcess::start()
{
  initProcess();
}

void tarProcess::initProcess()
{
  puts("Avvio del threadProcess (TAR)...");
  thread = new threadProcess(this);
  // setUp firstTime connections
  connect(thread, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
  connect(thread, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
  connect(thread, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(giveOutput(int, QProcess::ExitStatus)));

  if(options[0] == "-tvf") 
  { 
   // connect(rarProc, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
   // connect(rarProc, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
    thread->start(archiver, QStringList() << options << archivename);
    //puts(("Process: " + archiver + " " + options.join(" ")+ " " + archivename).toAscii());
  }

  else if (options[0] == "--delete") {
    thread -> start(archiver, QStringList() << options << archivename << files);
  }

  puts("initProcess (TAR) terminato");
}

void tarProcess::showProgress() 
{
  QByteArray gotOutput = thread -> readAllStandardOutput();
  stdoutput.append(gotOutput);
  rawoutput.append(gotOutput);
}

void tarProcess::giveOutput(int, QProcess::ExitStatus)
{
 puts("process terminated (TAR)");
 emit outputReady(stdoutput, headercrypted);
 if (streamerror.isEmpty()) {
    puts("no problem");
    noproblem = true;
 }
 else {
   puts("problem! (TAR)");
   noproblem = false;
   showError(streamerror);
 }
 emit processCompleted(noproblem); //check the bool 
}

void tarProcess::showError(QByteArray streamerror)
{
  QByteArray error = streamerror;
  errorDialog->setError(error);
  errorDialog->show();
}

void tarProcess::getError()
{ 
  QByteArray temp = thread -> readAllStandardError();
  streamerror.append(temp);
}

