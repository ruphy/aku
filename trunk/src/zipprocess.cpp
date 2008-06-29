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
 
  errorDialog = new akuErrorDialog();
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
  connect(thread, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
  connect(thread, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
  connect(thread, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(giveOutput(int, QProcess::ExitStatus)));
  
  if(options[0] == "-Zl") 
  { 
    thread -> start(archiver, QStringList() << options << archivename);
  }
  else if (options[0] == "-z") {
    thread -> start(archiver, QStringList() << options[0] << archivename );
    QString text = options[1];
    kDebug() << text;
    text.append("\n.\n");
    kDebug() << text;
    thread -> write(text.toUtf8());
    thread -> waitForFinished();
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
  if (!streamerror.isEmpty()) {
    QByteArray error = streamerror;
    QByteArray original = QString("Permission denied").toAscii();
    QByteArray translated = QString("<b><font color=red>" + i18n("Permission denied") + "</b></font>").toAscii();
    error.replace(original, translated);
    original = QString("zip I/O error:").toAscii();
    translated = QString("<b>" + i18n("zip I/O error:") + "</b>").toAscii();
    error.replace(original, translated);
    original = QString("zip error:").toAscii();
    translated = QString("<b>" + i18n("zip error:") + "</b>").toAscii();
    error.replace(original, translated);
    original = QString("Could not create output file").toAscii();
    translated = QString("<b><font color=red>" + i18n("Could not create output file") + "</b></font>").toAscii();
    error.replace(original, translated);

  
    errorDialog -> setError(error);
    errorDialog -> show();
  }
}

void zipProcess::getError()
{ 
  QByteArray temp = thread -> readAllStandardError();
  streamerror.append(temp);
}
