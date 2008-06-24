#include "zipprocess.h"

zipProcess::zipProcess(QWidget *parent, QString ziparchiver, QStringList zipoptions, QString ziparchivename, QStringList zipfiles, QString zipdestination) : QObject(parent)
{
  parentWidget = parent;
  archiver = ziparchiver;
  options = zipoptions;
  archivename = ziparchivename;
  files = zipfiles;
  destination = zipdestination;
  noproblem = false;
  headercrypted = false;
}

zipProcess::~zipProcess()
{
  qDeleteAll(children());
}

void zipProcess::start()
{
  initProcess();
}

void zipProcess::initProcess()
{
  puts("Avvio del threadProcess (ZIP)...");
  thread = new threadProcess(this);
  // setUp firstTime connections
  //connect(thread, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
  connect(thread, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
  connect(thread, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(giveOutput(int, QProcess::ExitStatus)));

  if(options[0] == "-Zl") 
  { 
   // connect(rarProc, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
   // connect(rarProc, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
    thread->start(archiver, QStringList() << options << archivename);
    //puts(("Process: " + archiver + " " + options.join(" ")+ " " + archivename).toAscii());
  }
  puts("initProcess (ZIP) terminato");
}

void zipProcess::showProgress() 
{
 
  QByteArray gotOutput = thread -> readAllStandardOutput();
  stdoutput.append(gotOutput);
  rawoutput.append(gotOutput);
}

void zipProcess::giveOutput(int, QProcess::ExitStatus)
{
 puts("process terminated (ZIP)");
 emit outputReady(stdoutput, headercrypted);
 if (streamerror.isEmpty()) {
    puts("no problem");
    noproblem = true;
 }
 else {
   puts("problem! (ZIP)");
   noproblem = false;
   showError(streamerror);
 }
 emit processCompleted(noproblem); //check the bool 
}

void zipProcess::showError(QByteArray streamerror)
{
  QByteArray error = streamerror;
  errorDialog->setError(error);
  errorDialog->show();
}
