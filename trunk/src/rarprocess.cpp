#include "rarprocess.h"

rarProcess::rarProcess(QWidget *parent, QString rararchiver, QStringList raroptions, QString rararchivename, QStringList rarfiles, QString rardestination) : QObject(parent)
{
    parentWidget = parent;
    archiver = rararchiver;
    options = raroptions;
    archivename = rararchivename;
    files = rarfiles;
    destination = rardestination;
  
    noproblem = false;
    headercrypted = false;
    toall = false;
    passwordAsked = false;
    hasPasswordParameter = false;

    totalFileCount = 0;   //this var will increment until getting the same value of files.size()
  
    rarprogressdialog = NULL;
    processTimer = NULL;

    errorDialog = new akuErrorDialog();
}

rarProcess::~rarProcess()
{
  qDeleteAll(children());
}

void rarProcess::start()
{
  if(!archiver.isEmpty()) {
     for (int i = 0; i < options.size(); i++) {
      if (options[i].isEmpty() || options[i] == "-p") options.removeAt(i);
     }

     QString str;
     foreach(str, options) {
       hasPasswordParameter = str.contains("-p");
       if(hasPasswordParameter)
        break;
     }

     if(!hasPasswordParameter && options[0] != "a" && options[0] != "ch") options << "-p-"; //this is to handle password later
  
  initProcess();
  }
}

void rarProcess::initProcess()
{
  //puts("Avvio del threadProcess... (RAR)");
  thread = new threadProcess(this);
  // setUp firstTime connections
  connect(thread, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
  connect(thread, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
  connect(thread, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(giveOutput(int, QProcess::ExitStatus)));

  if (options[0] == "x" || options[0] == "e") { 
    bool fullArchive;
    rar aids;
    thread -> start(archiver, QStringList() << "v" << options.last() << archivename);
    thread -> waitForFinished();
    globalTOC = standardOutput();
    if (files.isEmpty()) {   //should we extract the entire archive?
      fullArchive = true;
      files = aids.getFileList(globalTOC);
    }
    else fullArchive = false;

    rarprogressdialog = new akuProgressDialog(parentWidget, files.size());
    connect(rarprogressdialog, SIGNAL(canceled()), this, SLOT(handleCancel()));
    connect(rarprogressdialog, SIGNAL(paused()), this, SLOT(handlePaused()));
    connect(rarprogressdialog, SIGNAL(continued()), this, SLOT(handleContinued()));
    rarprogressdialog -> setArchiveName(archivename);

    if (fullArchive) thread -> start(archiver, QStringList() << options << archivename << destination);
    else thread -> start(archiver, QStringList() << options << archivename << files << destination);

  }

  else if (options[0] == "a") {
    disconnect(thread, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(giveOutput(int, QProcess::ExitStatus)));
    //puts(QString("Launching process: " + archiver + " " + params.join(" ")+ " "+archivename+ " " + files.join(" ") + " "+ destination).toAscii());
    rarprogressdialog = new akuProgressDialog(parentWidget, files.size());
    int total = 0;
    for (int i = 0; i < files.size(); i++)
      if(files[i].indexOf("-ap") == -1) total++; 
    rarprogressdialog -> setMaximum(total);

    connect(rarprogressdialog, SIGNAL(canceled()), this, SLOT(handleCancel()));
    connect(rarprogressdialog, SIGNAL(paused()), this, SLOT(handlePaused()));
    connect(rarprogressdialog, SIGNAL(continued()), this, SLOT(handleContinued()));
    rarprogressdialog -> setArchiveName(archivename);

    //here we start a timer to handle the extraction avoiding the GUI to freeze
    processTimer = new QTimer();
    connect(processTimer, SIGNAL(timeout()), this, SLOT(handleProcess()));
    processTimer -> start(1);
    //puts("started timer");
  }

  // opzioni passate a rar: v oppure vt
  else if(options[0] == "v" || options[0] == "vt" ) {  //common fast calls to rar
    thread->start(archiver, QStringList() << options << archivename);
    kDebug() << "FASE -->> unrar -v\n";
  }
 
  else if(options[0] == "rn") {
    thread -> start(archiver, QStringList() << options << archivename << files);
    if(!hasPasswordParameter) thread ->waitForFinished(); 
    else thread -> waitForFinished(-1);
  }

  else if(options.size() == 0) {
    thread -> start(archiver, QStringList() << options << archivename << files);
    thread->waitForFinished(1000);
  }

  else if (options[0] == "d" ) {
    if (files.isEmpty()) {
     thread -> start(archiver, QStringList() << options << archivename);
    }
    else {
     thread -> start(archiver, QStringList() << options << archivename << files);
    }

    if (!hasPasswordParameter) thread -> waitForFinished(); 
    else thread -> waitForFinished(-1);
  }
 
  else if(options[0] == "ch") {
    if(files.isEmpty()) {
     thread -> start(archiver, QStringList() << options << archivename);
      thread -> waitForFinished(-1);
    }
    else {
      thread -> start(archiver, QStringList() << options << archivename << files);
      thread -> waitForFinished(-1);
    }
  }

  else if(options[0] == "k" || options[0] == "c") {
    thread -> start(archiver, QStringList() << options << archivename);
    thread -> waitForFinished();
  }

  kDebug() << "initProcess terminata";
}

void rarProcess::handleCancel()
{
  thread -> killProcess();
  rarprogressdialog -> cancel();
  if (options[0] == "a") processTimer -> stop();
  emit processCanceled();
}

void rarProcess::handlePaused()
{
  thread -> pauseProcess();
}

void rarProcess::handleContinued()
{
  thread -> continueProcess();
}

QString rarProcess::standardOutput()
{
  if(!stdoutput.contains("Pathname/Comment")) stdoutput.clear();
  return stdoutput;
}

//gestiamo un progressdialog
void rarProcess::showProgress() 
{
  kDebug() << "FUNZIONE ::SHOWPROGRESS";
  QByteArray gotOutput = thread -> readAllStandardOutput();
  stdoutput.append(gotOutput);
  rawoutput.append(gotOutput);
  QStringList percentuale;
  
  if(QString(gotOutput).contains("Extracting") && !QString(gotOutput).contains("Extracting from")) {
    if(totalFileCount < files.size()) {
    // puts(files[totalFileCount].toAscii());
      rarprogressdialog -> setCurrentFileName(files[totalFileCount]);
      QString size = rar::getSingleFileSize(globalTOC, files[totalFileCount]);
      rarprogressdialog -> setCurrentFileSize(size);
      totalFileCount++;
    }
  }

  if (QString(gotOutput).contains("OK") && !QString(gotOutput).contains("All OK") && (options[0]=="x" && options[0]=="e")) {
    rarprogressdialog->setCurrentFileProgressToMaximum();
    rarprogressdialog->setCurrentFileProgress(0);
    rarprogressdialog->incrementOverall();
  }

  if (QString(gotOutput).contains("%")) {
    percentuale = QString(gotOutput).split (" ", QString::SkipEmptyParts);
    if (percentuale.size() == 2)  {
      percentuale[1].remove ( "%" );
      if(options[0]!="a") rarprogressdialog->setCurrentFileProgress(rarprogressdialog->currentFileProgressValue() + (100 / files.size()));
      else rarprogressdialog->setCurrentFileProgress(percentuale[1].toInt());
    }
  }

  if ( QString(gotOutput).contains("All OK")) {
    rarprogressdialog -> accept(); 
  }
  kDebug() << "FINE FUNZIONE ::SHOWPROGRESS";
}


void rarProcess::giveOutput(int exit, QProcess::ExitStatus)
{
 kDebug() << "::GIVEOUTPUT - process terminated";
 kDebug() << exit;
 emit outputReady(standardOutput(), headercrypted);
 if (streamerror.isEmpty()) {
    puts("no problem");
    noproblem = true;
 }
 else {
   puts("problem!");
   noproblem = false;
   showError(streamerror);
 }
 emit processCompleted(noproblem); //check the bool
 
}

void rarProcess::showError(QByteArray streamerror)
{
  if(!streamerror.isEmpty()) {
    QByteArray error = streamerror;
    QByteArray original(QString("Cannot create").toAscii());
    QByteArray translated(QString("<b>" + i18n("\nCannot create") + "</b>").toAscii());
    error.replace(original, translated);
    original = QString("ERROR:").toAscii();
    translated = QString("<b>" + i18n("ERROR:") + "</b>").toAscii();
    error.replace(original, translated);
    original = QString("Locked archive").toAscii();
    translated = QString("<font color=red>" + i18n("Locked archive") + "</font>").toAscii();
    error.replace(original, translated);
    original = QString("Cannot open").toAscii();
    translated = QString("<b>" + i18n("Cannot open") + "</b>").toAscii();
    error.replace(original, translated);
    original = QString("Cannot create").toAscii();
    translated = QString("<b>" + i18n("Cannot create") + "</b>").toAscii();
    error.replace(original, translated);
    original = QString("Cannot create directory").toAscii();
    translated = QString("<b>" + i18n("Cannot create directory") + "</b>").toAscii();
    error.replace(original, translated);
    original = QString("Permission denied").toAscii();
    translated = QString("<br><font color=red>" + i18n("Permission denied") + "<br></font>").toAscii();
    error.replace(original, translated);
    original = QString("is read-only").toAscii();
    translated = QString("<br><font color=red>" + i18n("is read-only") + "<br></font>").toAscii();
    error.replace(original, translated);
    original = QString("No such file or directory").toAscii();
    translated = QString("</i><br><font color=red>" + i18n("No such file or directory") + "<br></font>").toAscii();
    error.replace(original, translated);
    original = QString("Encrypted file:").toAscii();
    translated = QString("</i><br><font color=red>" + i18n("Encrypted file") + "<br></font>").toAscii();
    error.replace(original, translated);
    original = QString("CRC failed").toAscii();
    translated = QString("<b>" + i18n("CRC failed") + "</b>").toAscii();
    error.replace(original, translated);
    original = QString("Unexpected end of archive").toAscii();
    translated = QString("<font color=red>" + i18n("Unexpected end of archive") + "</font>").toAscii();
    error.replace(original, translated);
    errorDialog->setError(error);
    errorDialog->show();
  }
}

QString rarProcess::getArchivePassword()
{
  return archivePassword;
}

void rarProcess::getError()
{ 
  kDebug() << "FUNZIONE ::GETERROR";
  QByteArray temp = thread -> readAllStandardError();

  // the keyword "already" tells us that rar is asking for an overwrite
  // we need to save the filename:
  // the string from rar is like: /the/path/of/the/archive.rar already exists. Overwrite it?
  // so we need to erase everything from already to get the file name
  // then we can call an overwrite dialog to handle user's choice

  if(QString().fromAscii(temp).contains("already")) {
    //rarprogressdialog -> setValue ( rarprogressdialog -> maximum()); 
    QString targetFile = QString().fromAscii(temp); 
    int forParsing = targetFile.indexOf("already"); 
    targetFile.remove(forParsing-1, targetFile.length());
    targetFile.remove("\n");
    
    overwriteDialog *owDialog = new overwriteDialog(thread -> proc(), parentWidget); //chiamiamo l'overwrite dialog

      QFileInfo details(targetFile); // generating file info
      owDialog -> setDestinationDetails(details.filePath());
      QString currentExtraction = files[totalFileCount];
      QString size = rar().getSingleFileSize(globalTOC, files[totalFileCount]);
      owDialog -> setSourceDetails(currentExtraction, rar::getSingleFileModificationTime(globalTOC, files[totalFileCount]), size);
      rarprogressdialog -> hide();
      if( owDialog -> exec() == QDialog::Rejected ) handleCancel();
      else {
      // here we handle the "toAll option"
        if(!owDialog->isYes()) totalFileCount++;
        rarprogressdialog -> show();
      }
  
   delete owDialog;             
  }

  else if(QString().fromAscii(temp).contains("password incorrect ?")) { 
  // here we handle a header-password-protected archive
    headercrypted = true; //the archive is crypted;
    totalFileCount = 0;

    disconnect(thread, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
    disconnect(thread, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
    disconnect(thread, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(giveOutput(int, QProcess::ExitStatus)));

    thread -> killProcess();

    //if(rarprogressdialog != NULL) rarprogressdialog -> cancel();
    //if(processTimer != 0) processTimer -> stop();
 
    KPasswordDialog pwD(parentWidget);
    pwD.setPrompt(i18n("This archive is <b>header password protected</b>.<br>Enter the password:"));
    QPixmap pixmap = KIcon("dialog-password").pixmap(64,64);
    pwD.setPixmap(pixmap);
    
    if(!pwD.exec()) { 
      emit outputReady(QString(""), false);
      return;
    }
    
    QString password = pwD.password();
  
    if (!password.isEmpty()) {
      options << "-p" + password;
      archivePassword = password;
      stdoutput.clear();
      passwordAsked = true;
    }
    initProcess();
  }

  else {  //altrimenti lasciamo che l'errore sia gestito da showError
    if(QString().fromAscii(temp).indexOf("already") == -1 || QString().fromAscii(temp).indexOf("password") == -1) streamerror.append(temp);
  } 
}
