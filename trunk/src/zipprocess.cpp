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

  else if (options[0] == "-d") {
    thread -> start(archiver, QStringList() << options << archivename << files);
  }
 
  else if (options[0] == "-q") {
    thread -> start(archiver, QStringList() << options << archivename << files << "-d" << destination);
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
 // invalid option(s) used with -d; ignored --> warning relativo alla cancellazione di directory
   if (streamerror.contains("invalid option(s) used with -d; ignored")) { 
     streamerror.remove(0, (streamerror.indexOf("\n") + 1));
     noproblem = true;
   } 
 // stringa relativa all'eliminazione di tutti i file dell'archivio
   if (streamerror.contains("zip warning: zip file empty")) {
     streamerror.remove(0, (streamerror.indexOf("\n") + 1));
     noproblem = true;
   } 

   if (!streamerror.isEmpty()) {
     puts("problem! (ZIP)");
     noproblem = false;
     showError(streamerror);
   }
 }
 emit processCompleted(noproblem); //check the bool 
}

void zipProcess::showError(QByteArray streamerror)
{
  if (!streamerror.isEmpty()) {
    QByteArray error = streamerror;
    QByteArray original = QString("Permission denied").toAscii();
    QByteArray translated = QString("<b>" + i18n("Permission denied") + "</b><br>").toAscii();
    error.replace(original, translated);
    original = QString("zip I/O error:").toAscii();
    translated = QString("<b><font color=red>" + i18n("zip I/O error:") + "</b></font>").toAscii();
    error.replace(original, translated);
    original = QString("caution:").toAscii();
    translated = QString("<b>" + i18n("caution:") + "</b>").toAscii();
    original = QString("zip error:").toAscii();
    translated = QString("<b><font color=red>" + i18n("zip error:") + "</font").toAscii();
    error.replace(original, translated);
    original = QString("Nothing to do!").toAscii();
    translated = QString("<b>" + i18n("Nothing to do!") + "</b>").toAscii();
    error.replace(original, translated);
    original = QString("Could not create output file").toAscii();
    translated = QString("<b>" + i18n("Could not create output file") + "</b><br>").toAscii();
    error.replace(original, translated);
    original = QString("zip warning:").toAscii();
    translated = QString("<b><font color=red>" + i18n("zip warning:") + "</b></font>").toAscii();
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
