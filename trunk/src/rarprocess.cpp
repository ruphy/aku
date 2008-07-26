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
    rememberPassword = false;
    //hasPasswordParameter = false;

    totalFileCount = 0;   //this var will increment until getting the same value of files.size()
  
    rarprogressdialog = NULL;
    processTimer = NULL;

    errorDialog = new akuErrorDialog();
}

rarProcess::~rarProcess()
{
  qDeleteAll(children());
}

void rarProcess::start(QString passwordPassed)
{
  if (passwordPassed.isEmpty()) {
    foreach (QString str, options) {
      if (str.startsWith("-p"))
        archivePassword = str.remove("-p");
    }
  }
  else archivePassword = passwordPassed;

  if (archivePassword.isEmpty() && options[0] == "v")
    options << "-p-"; 
  // per gestire estrazione rapida (konqueror menu e quickextract) ... non il normale main -> extract to -> ok
    
  initProcess();
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
    disconnect(thread, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(giveOutput(int, QProcess::ExitStatus)));
    rar aids;
    QStringList opt;
    opt << "v";
   
    if (!archivePassword.isEmpty())
      opt << "-p" + archivePassword;
        
    thread -> start(archiver, opt << archivename);
    thread -> waitForFinished();
    globalTOC = standardOutput();
     
    // files password protected
    if (archivePassword.isEmpty()) {
      listpfiles = aids.getFilePasswordedList(globalTOC);
      options << "-p-"; 
    }
    else listpfiles.clear();
    //kDebug() << listpfiles;
    
    bool fullArchive;
    if (files.isEmpty()) {   //should we extract the entire archive?
      fullArchive = true;
      files = aids.getFileList(globalTOC);
      // se si deve estrarre l'intero archivio, la lista dei file con password da gestire
      // è proprio la listpfiles;
      if (!listpfiles.isEmpty()) {
        fileswithpassword = listpfiles;
      }
    }
    // se non devo estrarre tutto l'archivio devo comunque avere un fileswithpassword da gestire
    else {
      fullArchive = false;
      if (!listpfiles.isEmpty()) {
        foreach(QString str, files) {
          if (listpfiles.contains(str))
            fileswithpassword << str;
        }
      }
    }
   
    rarprogressdialog = new akuProgressDialog(parentWidget, files.size());
    connect(rarprogressdialog, SIGNAL(canceled()), this, SLOT(handleCancel()));
    connect(rarprogressdialog, SIGNAL(paused()), this, SLOT(handlePaused()));
    connect(rarprogressdialog, SIGNAL(continued()), this, SLOT(handleContinued()));
    rarprogressdialog -> setArchiveName(archivename);

    connect(thread, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(giveOutput(int, QProcess::ExitStatus)));
 
    if (!listpfiles.isEmpty()) {
       // creo un file di testo contenente i nomi di tutti i file protetti da password per gestirli separatamente
      tmpPassFile.setAutoRemove(false);
      if (tmpPassFile.open() && (!listpfiles.isEmpty())) {
        foreach (QString str, listpfiles) {
          tmpPassFile.write(("*" + str + "\n").toUtf8());
          tmpPassFile.waitForBytesWritten(-1);
          tmpPassFile.flush();
        }
      }
      options << "-x@" + tmpPassFile.fileName();
      emit tempFiles(tmpPassFile.fileName());

    }
    else fileswithpassword.clear();  // questa QStringList contiene i nomi dei file con password da gestire
        
    if (fullArchive) {
        thread -> start(archiver, QStringList() << options << archivename << destination);
        kDebug() << "full archive";
    }
    else {       
      thread -> start(archiver, QStringList() << options << archivename << files << destination);
      kDebug() << "not full archive";
    }
    tmpPassFile.close();
    kDebug() << options << archivename << destination;
  }

  else if (options[0] == "a") {
    disconnect(thread, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(giveOutput(int, QProcess::ExitStatus)));
    rarprogressdialog = new akuProgressDialog(parentWidget, files.size());
    int total = 0;
    for (int i = 0; i < files.size(); i++)
      if (files[i].indexOf("-ap") == -1) total++; 
    rarprogressdialog -> setMaximum(total);

    connect(rarprogressdialog, SIGNAL(canceled()), this, SLOT(handleCancel()));
    connect(rarprogressdialog, SIGNAL(paused()), this, SLOT(handlePaused()));
    connect(rarprogressdialog, SIGNAL(continued()), this, SLOT(handleContinued()));
    rarprogressdialog -> setArchiveName(archivename);

    //here we start a timer to handle the extraction avoiding the GUI to freeze
    processTimer = new QTimer();
    connect(processTimer, SIGNAL(timeout()), this, SLOT(handleProcess()));
    processTimer -> start(1);
  }

  // opzioni passate a rar: v oppure vt
  else if (options[0] == "v" || options[0] == "vt" ) {  //common fast calls to rar
    thread->start(archiver, QStringList() << options << archivename);
    //thread -> waitForFinished();
  }
 
  else if(options[0] == "rn") {
    thread -> start(archiver, QStringList() << options << archivename << files);
    thread -> waitForFinished();
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

  //kDebug() << "initProcess terminata";
}

void rarProcess::handleCancel()
{
  thread -> killProcess();
  rarprogressdialog -> cancel();
  if (options[0] == "a") processTimer -> stop();
  emit processCompleted(false);
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

  if (QString(gotOutput).contains("OK") && !QString(gotOutput).contains("All OK") && (options[0]=="x" || options[0]=="e")) {
    rarprogressdialog->setCurrentFileProgressToMaximum();
    rarprogressdialog->setCurrentFileProgress(0);
    rarprogressdialog->incrementOverall();
  }

  if (QString(gotOutput).contains("%")) {
    percentuale = QString(gotOutput).split (" ", QString::SkipEmptyParts);
    if (percentuale.size() == 2)  {
      percentuale[1].remove ( "%" );
      // kDebug() << files.size();
      if (options[0] != "a") rarprogressdialog->setCurrentFileProgress(rarprogressdialog->currentFileProgressValue() + (100 / files.size()));
      else rarprogressdialog->setCurrentFileProgress(percentuale[1].toInt());
    }
  }

  // "All OK" in caso di estrazione completata. "Total errors:" in caso di estrazione completata ma in
  // presenza di file protetti da password
  if ((QString(gotOutput).contains("All OK")) || (QString(gotOutput).contains("Total errors:"))) {
    rarprogressdialog -> accept(); 
  }
}

void rarProcess::giveOutput(int exit, QProcess::ExitStatus)
{ 
  delete rarprogressdialog;
  if (fileswithpassword.isEmpty()) {
    emit outputReady(standardOutput(), headercrypted);
    if (streamerror.isEmpty()) {
      noproblem = true;
    }
    else {
      noproblem = false;
      showError(streamerror);
    }
    emit processCompleted(noproblem); //check the bool
  }
  else {
    options.removeLast();
    // elimino l'ultima stringa di options che sicuramente è "-x@nomefiletemp"
    handlePasswordedFiles();
  }
}

// processCompleted in caso di archivi con file protetti
void rarProcess::miniGiveOutput() {
  kDebug() << "MINIGIVEOUTPUT";
  kDebug() << streamerror;
  if (streamerror.isEmpty()) {
    noproblem = true;
  }
  else {
    noproblem = false;
    showError(streamerror);
  }
  emit processCompleted(noproblem); //check the bool
}

void rarProcess::handlePasswordedFiles(bool incorrectPassword)
{
  dlg = new KPasswordDialog(parentWidget, KPasswordDialog::ShowKeepPassword, KDialog::User1);
  KPushButton *buttonSkip = dlg -> button(KDialog::User1);
  buttonSkip -> setText(i18n("Skip this file"));
  buttonSkip -> setIcon(KIcon("arrow-right-double"));

  connect(buttonSkip, SIGNAL(clicked()), this, SLOT(nextPasswordedFile()));
  connect(dlg, SIGNAL(gotPassword(const QString& , bool)), this, SLOT(setPassword(const QString &, bool)));
  //connect(dlg, SIGNAL(rejected()), this, SLOT(cancelPasswordRequest()));

  if (rememberPassword) {
      kDebug() << fileswithpassword;
      if (!fileswithpassword.isEmpty()) {
        setPassword(filePassword, true);
      }   
      else {
        miniGiveOutput();
      }
  }
  else {
    if (fileswithpassword.size() != 0) {

      if ((fileswithpassword.size() > 1) && (!incorrectPassword))
        dlg -> addCommentLine(QString(), i18n("Check") + " <i>" + i18n("remember password") + "</i> " + i18n("to use the current password for the next file(s)"));
      else if ((fileswithpassword.size() > 1) && (incorrectPassword))
        dlg -> addCommentLine(QString(), i18n("Check") + " <i>" + i18n("remember password") + "</i> " + i18n("to use the current password for the next file(s)") + "\n" + "<br><b><font color=red>" + i18n("Password incorrect") + "</b></font>");     
      else if (incorrectPassword)
        dlg -> addCommentLine(QString(), "<br><b><font color=red>" + i18n("Password incorrect") + "</font></b>");
      
      dlg -> setPrompt(i18n("The file") + " <b>" + fileswithpassword[0] + " </b><i>" + i18n("is password protected") + "<br></i>" + i18n("Enter the password:"));
      dlg -> setModal(true);
      dlg -> show();
    }
    else miniGiveOutput();
  }
    
}

void rarProcess::nextPasswordedFile()
{ 
  if (dlg -> close()) {
    streamerror.append("<b>" + fileswithpassword[0] + "</b><font color=red> " + i18n("skipped") + "</font><br>");
    fileswithpassword.removeFirst();
      if (!fileswithpassword.isEmpty())
        handlePasswordedFiles();
      else miniGiveOutput();
  }
}

void rarProcess::setPassword(const QString& newpassword, bool rPassword)
{
  if (rPassword) {
    rememberPassword = true;
    filePassword = newpassword;
  }
  
  if (!newpassword.isEmpty()) {
    // devo eliminare -p dalle opzioni
    if (options.indexOf("-p-") != -1)
      options.removeAt(options.indexOf("-p-"));
     
    minithread = new threadProcess(this);
    connect(minithread, SIGNAL(readyReadStandardError()), this, SLOT(miniGetError()));
    connect(minithread, SIGNAL(readyReadStandardOutput()), this, SLOT(miniGetOutput()));
    //parentWidget -> setCursor(Qt::WaitCursor);
    emit activeInterface(false);
    if (!fileswithpassword.isEmpty())
      minithread -> start(archiver, QStringList() << options << "-p" + newpassword << archivename << fileswithpassword[0] << destination);
  }
  else {
    if (dlg -> close()) handlePasswordedFiles(true);
  }
  
}

void rarProcess::miniGetOutput()
{
  QByteArray miniStd = minithread -> readAllStandardOutput();
  if (miniStd.contains("All OK")) {
    fileswithpassword.removeFirst();
    if (!fileswithpassword.isEmpty()) {
      dlg -> close();
      handlePasswordedFiles();
    }
    else miniGiveOutput();
    //parentWidget -> setCursor(QCursor());
    emit activeInterface(true);
  }
}

void rarProcess::miniGetError()
{
  QByteArray miniStdError = minithread -> readAllStandardError();

  if (miniStdError.contains("password incorrect ?")) {
       dlg -> close();
       rememberPassword = false;
       handlePasswordedFiles(true);
  }

  else if (miniStdError.contains("already exists. Overwrite it")) {
    kDebug() << "Sono in overwrite";
    rar aids;
    threadProcess *thread = new threadProcess(this);
    thread -> start(archiver, QStringList() << "v" << archivename);
    thread -> waitForFinished();
    
    QString toc = standardOutput();
  
    QString targetFile = QString().fromAscii(miniStdError); 
    int forParsing = targetFile.indexOf("already exists. Overwrite it"); 
    targetFile.remove(forParsing - 1, targetFile.length());
    targetFile.remove("\n");
    
    overwriteDialog *owDialog = new overwriteDialog(minithread -> proc(), parentWidget); //chiamiamo l'overwrite dialog
    QFileInfo details(targetFile); // generating file info
    owDialog -> setDestinationDetails(details.filePath());
    QString currentExtraction = fileswithpassword[0];
    QString size = rar().getSingleFileSize(toc, fileswithpassword[0]);
    owDialog -> setSourceDetails(currentExtraction, rar::getSingleFileModificationTime(toc, fileswithpassword[0]), size);

    if (owDialog -> exec() == QDialog::Rejected) {
      minithread -> killProcess();
      emit processCompleted(false);
    }
    else {
    // here we handle the "toAll option"
      // se è selezionata l'opzione di overwrite "Yes all" basta aggiungere un overwrite sempre alle opzioni di rar
      if (owDialog -> yesToAllChecked()) options << "-o+";
      else if (owDialog -> noToAllChecked()) options << "-o-";
      else if (!owDialog -> isYes()) {
        fileswithpassword.removeFirst();
        handlePasswordedFiles();
      }
    }    

    delete owDialog;             
  }

  else if (!miniStdError.contains("already exists. Overwrite it") && !miniStdError.contains("password incorrect ?") && !miniStdError.contains("[Y]es, [N]o, [A]ll") && !miniStdError.isEmpty()) {
      streamerror.append(miniStdError);
      kDebug() << miniStdError;
      fileswithpassword.removeFirst();
      handlePasswordedFiles();
  }

  emit activeInterface(true);
}

void rarProcess::showError(QByteArray streamerror)
{
  if(!streamerror.isEmpty()) {
    QByteArray error = streamerror;
    QByteArray original(QString("Cannot create").toAscii());
    QByteArray translated(QString("<b>\n" + i18n("Cannot create") + "</b>").toAscii());
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
    original = QString("Bad archive").toAscii();
    translated = QString("<font color=red>" + i18n("Bad archive") + "</font>").toAscii();
    error.replace(original, translated);
    original = QString("is read-only").toAscii();
    translated = QString("<br><font color=red>" + i18n("is read-only") + "<br></font>").toAscii();
    error.replace(original, translated);
    original = QString("No such file or directory").toAscii();
    translated = QString("</i><br><font color=red>" + i18n("No such file or directory") + "<br></font>").toAscii();
    error.replace(original, translated);
    original = QString("Encrypted file").toAscii();
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
  QByteArray temp = thread -> readAllStandardError();

  // the keyword "already" tells us that rar is asking for an overwrite
  // we need to save the filename:
  // the string from rar is like: /the/path/of/the/archive.rar already exists. Overwrite it?
  // so we need to erase everything from already to get the file name
  // then we can call an overwrite dialog to handle user's choice

  if(QString().fromAscii(temp).contains("already exists. Overwrite it")) {
    //rarprogressdialog -> setValue ( rarprogressdialog -> maximum()); 
    QString targetFile = QString().fromAscii(temp); 
    int forParsing = targetFile.indexOf("already exists."); 
    targetFile.remove(forParsing-1, targetFile.length());
    targetFile.remove("\n");
    
    QString currentExtraction = files[totalFileCount];
    
    overwriteDialog *owDialog = new overwriteDialog(thread -> proc(), parentWidget); //chiamiamo l'overwrite dialog

    QFileInfo details(targetFile); // generating file info
    owDialog -> setDestinationDetails(details.filePath());
    //QString currentExtraction = files[totalFileCount];
    //kDebug() << currentExtraction;
    //kDebug() << rar::getSingleFileModificationTime(globalTOC, files[totalFileCount]);
    QString size = rar().getSingleFileSize(globalTOC, files[totalFileCount]);
    owDialog -> setSourceDetails(currentExtraction, rar::getSingleFileModificationTime(globalTOC, files[totalFileCount]), size);
    rarprogressdialog -> hide(); // FIXME -> vedere come gestire questa cosa, segnata in Todo
    
    //kDebug << listpfiles;
    if( owDialog -> exec() == QDialog::Rejected ) handleCancel();
    else {
    // here we handle the "toAll option"
      if(!owDialog->isYes()) totalFileCount++;
      rarprogressdialog -> show();
    }
  
    delete owDialog;             
  }

  else if ((QString().fromAscii(temp).contains("password incorrect ?")) && (options[0] == "v")) { 
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
      emit passwordCanceled();  // necessario per l'extract To e l'extract Here
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

  else if (temp.contains("password incorrect ?") && ((options[0] == "x") || (options[0] == "e"))) { 
    // Il problema è che il testo nello standard output è asincrono
    // quindi devo gestire 1 o più righe di testo ed eventualmente estrapolare il nome del 
    // file con password.
 
    QString targetFile = QString().fromAscii(temp);

    QString filePass;

    while (targetFile.indexOf("password incorrect") != -1) {    
      int startString = targetFile.indexOf("CRC failed in") + 14;
      int endString = targetFile.indexOf("password incorrect") - 2; // -2 per parentesi e spazio vuoto
      
      filePass = targetFile.mid(startString, (endString - startString));
      
      // modo barbaro ma funzionale
      targetFile.remove(targetFile.indexOf("Encrypted file"), targetFile.indexOf("incorrect ?)"));
      kDebug() << filePass;
      fileswithpassword << filePass;
    }

    rarprogressdialog->setCurrentFileProgressToMaximum();
//     rarprogressdialog->setCurrentFileProgress(0);
//     rarprogressdialog->incrementOverall();
  }

  else {  //altrimenti lasciamo che l'errore sia gestito da showError
    if (!temp.contains("already exists. Overwrite it") && !temp.contains("password incorrect ?") && !temp.contains("[Y]es, [N]o, [A]ll")) {
      streamerror.append(temp);
    }
  } 
}

void rarProcess::handleProcess()
{
  //connect(thread, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
  //connect(thread, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));

  if (totalFileCount < files.size() && thread -> state() == QProcess::NotRunning)  {
  //if the extraction is complete we can start the next 
   // puts("extracting: "+ filesToHandle[totalFileCount].toAscii());
    QStringList toDebug;
    toDebug << options << archivename  << files[totalFileCount] << destination;
    
    kDebug() << archiver.toAscii() + " " + toDebug.join(" ").toAscii();
  
    thread -> waitForFinished(-1);

    if (files[totalFileCount].indexOf( "-ap" ) == -1 ) {
      thread -> start(archiver, QStringList() << options << archivename << files[totalFileCount]); 
      rarprogressdialog -> setCurrentFileName(files[totalFileCount]);
      QFileInfo fileInfo(files[totalFileCount]);
      QString size = KLocale("").formatByteSize(fileInfo.size());
      rarprogressdialog -> setCurrentFileSize(size);
    }
    else {
      thread -> start(archiver, QStringList() << options << files[totalFileCount] << archivename << files[totalFileCount + 1]); 
      rarprogressdialog -> setCurrentFileName(files[totalFileCount+1]);
      rarprogressdialog -> incrementOverall();
      QFileInfo fileInfo(files[totalFileCount+1]);
      QString size = KLocale(QString()).formatByteSize(fileInfo.size());
      rarprogressdialog -> setCurrentFileSize(size);
      totalFileCount++;
    }

    if(!passwordAsked) totalFileCount ++;
    else passwordAsked = false; //patch for one-file-extraction with password protection
  }

  else {
    if(totalFileCount == files.size() && thread -> state() == QProcess::NotRunning) {
      processTimer -> stop();
      rarprogressdialog -> setCurrentFileProgressToMaximum();
      rarprogressdialog -> accept();
      if (streamerror.isEmpty() == false) noproblem = false;
      else noproblem = true;
      emit processCompleted(noproblem);
      //this section is asyncronous so we got to call showError here also..
      if (!streamerror.isEmpty()) {
        showError(streamerror);
      }
    }
  }
}
