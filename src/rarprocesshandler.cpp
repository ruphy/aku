#include "rarprocesshandler.h"
#include "rar.h"

rarProcessHandler::rarProcessHandler( QWidget *parent,QString archiver, QStringList parameters, QString archive, QStringList files, QString destination ) : QObject(parent)
{ 
  if(parent != 0)
  {      
    totalExtraction = false;
    passwordAsked = false;
    passwordRequests = 0;
    crypted = false;
    rarProcProgress = NULL;
    processTimer = NULL;
    done = false;
    errorDialog = new akuErrorDialog();
    toAll = false;
    totalFileCount = 0; //this var will increment until getting the same value of filesToHandle.size()
    process = archiver;
    params = parameters;
    hasPasswordParameter = false;
    rarArchive = archive;
    filesToHandle= files;
    pathTarget = destination;
    parentWidget = parent;
    globalArchiver = archiver;
  }

}

rarProcessHandler::~rarProcessHandler()
{
  qDeleteAll(children());
}

void rarProcessHandler::start()
{
  if(globalArchiver != "")
  {
     for(int i = 0; i < params.size(); i++){
      if(params[i].isEmpty() || params[i] == "-p") params.removeAt(i);
     }
     QString str;
     foreach(str, params){
       hasPasswordParameter = str.contains("-p");
       if(hasPasswordParameter)
        break;
     }
     if(!hasPasswordParameter && params[0] != "a" && params[0] != "ch") params << "-p-"; //this is to handle password later
    
    initProcess();

  }
}


void rarProcessHandler::initProcess()
{
  puts("initProcess..");
  rarProc = new threadProcess(this);
  // setUp firstTime connections
  connect(rarProc, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
  connect(rarProc, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
  connect(rarProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(giveOutput(int, QProcess::ExitStatus)));
  // let's check how to launch the process
  if(params[0] == "x" || params[0] == "e")
  { 

      bool fullArchive;

      rar aids;
      rarProc->start(process, QStringList() << "v" << params.last() << rarArchive);
      rarProc->waitForFinished();
      globalTOC = standardOutput();
      if(filesToHandle.isEmpty()) //should we extract the entire archive?
        {
         fullArchive = true;
         filesToHandle = aids.getFileList(globalTOC);
       } else
        fullArchive = false;

      rarProcProgress = new akuProgressDialog(parentWidget, filesToHandle.size());
      connect(rarProcProgress, SIGNAL(canceled()), this, SLOT(handleCancel()));
      connect(rarProcProgress, SIGNAL(paused()), this, SLOT(handlePaused()));
      connect(rarProcProgress, SIGNAL(continued()), this, SLOT(handleContinued()));
      rarProcProgress -> setArchiveName(rarArchive);

      if(fullArchive)
        rarProc -> start(process, QStringList() << params << rarArchive << pathTarget);
      else
       rarProc -> start(process, QStringList() << params << rarArchive << filesToHandle << pathTarget);

 }
  else if(params[0] == "a")
  {
    disconnect(rarProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(giveOutput(int, QProcess::ExitStatus)));
    puts(QString("Launching process: " + process + " " + params.join(" ")+ " "+rarArchive+ " " + filesToHandle.join(" ") + " "+ pathTarget).toAscii());
    rarProcProgress = new akuProgressDialog(parentWidget, filesToHandle.size());
    int total = 0;
    for(int i = 0; i < filesToHandle.size(); i++) if(filesToHandle[i].indexOf("-ap") == -1) total++;
    rarProcProgress -> setMaximum(total);
    connect(rarProcProgress, SIGNAL(canceled()), this, SLOT(handleCancel()));
    connect(rarProcProgress, SIGNAL(paused()), this, SLOT(handlePaused()));
    connect(rarProcProgress, SIGNAL(continued()), this, SLOT(handleContinued()));
    rarProcProgress -> setArchiveName(rarArchive);

    //here we start a timer to handle the extraction avoiding the GUI to freeze
    processTimer = new QTimer();
    connect(processTimer, SIGNAL(timeout()), this, SLOT(handleProcess()));
    processTimer -> start(1);
    puts("started timer");
  }
  else if(params[0] == "v" || params[0] == "vt" ) //common fast calls to rar
  {
    puts(QString("Launching process: " + process + " " + params.join(" ")+ " "+rarArchive+ " " + filesToHandle.join(" ") + " "+pathTarget).toAscii());
   // connect(rarProc, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
   // connect(rarProc, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
    rarProc->start(process, QStringList() << params << rarArchive);
  }
  else if(params[0] == "rn")
  {
    puts(QString("Launching process: " + process + " " + params.join(" ")+ " "+rarArchive+ " " + filesToHandle.join(" ") + " "+pathTarget).toAscii());
   // connect(rarProc, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
   // connect(rarProc, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
    rarProc->start(process, QStringList() << params << rarArchive << filesToHandle);
    if(!hasPasswordParameter) rarProc->waitForFinished(1000); 
    else rarProc->waitForFinished(-1);
    if (rarProcError.isEmpty() == false) done = false;
    else done = true;
    puts("emitting");
    emit processCompleted(done);
  }
  else if(params.size() == 0)
  {
   // connect(rarProc, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
   // connect(rarProc, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
    rarProc->start(process, QStringList() << params << rarArchive << filesToHandle);
    rarProc->waitForFinished(1000);
  }
  else if(params[0] == "d" )
  {
    if(filesToHandle.isEmpty())
    {
      puts(QString("Launching process: " + process + " " + params.join(" ")+ " "+rarArchive + " "+filesToHandle.join(" ")+pathTarget).toAscii());
     // connect(rarProc, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
     // connect(rarProc, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
      rarProc->start(process, QStringList() << params << rarArchive);
      if(!hasPasswordParameter) rarProc->waitForFinished(1000); 
      else rarProc->waitForFinished(-1);
      if (rarProcError.isEmpty() == false) done = false;
      else done = true;
    }
    else
    {
      puts(QString("Launching process: " + process + " " + params.join(" ")+ " "+rarArchive + " "+filesToHandle.join(" ")).toAscii());
     // connect(rarProc, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
     // connect(rarProc, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
      rarProc->start(process, QStringList() << params << rarArchive << filesToHandle);
      if(!hasPasswordParameter) rarProc->waitForFinished(1000); 
      else rarProc->waitForFinished(-1);
      if (rarProcError.isEmpty() == false) done = false;
      else done = true;
    }
    puts("emitting..");
    emit processCompleted(done);
  }
  else if(params[0] == "ch")
  {
    if(filesToHandle.isEmpty())
    {
      puts(QString("Launching process: " + process + " " + params.join(" ")+ " "+rarArchive + " "+filesToHandle.join(" ")+pathTarget).toAscii());
     // connect(rarProc, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
     // connect(rarProc, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
      rarProc->start(process, QStringList() << params << rarArchive);
      rarProc->waitForFinished(-1);
      if (rarProcError.isEmpty() == false) done = false;
      else done = true;
    }
    else
    {
      puts(QString("Launching process: " + process + " " + params.join(" ")+ " "+rarArchive + " "+filesToHandle.join(" ")).toAscii());
     // connect(rarProc, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
     // connect(rarProc, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
      rarProc->start(process, QStringList() << params << rarArchive << filesToHandle);
      rarProc->waitForFinished(-1);
      if (rarProcError.isEmpty() == false) done = false;
      else done = true;
    }
    puts("emitting..");
    emit processCompleted(done);
  }
  else if(params[0] == "k" || params[0] == "c")
  {
    puts(QString("Launching process: " + process + " " + params.join(" ")+ " "+rarArchive+ " " + filesToHandle.join(" ") + " "+pathTarget).toAscii());
   // connect(rarProc, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
   // connect(rarProc, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
    rarProc->start(process, QStringList() << params << rarArchive);
    rarProc->waitForFinished();
  }
  puts("initProcess terminata");

}

bool rarProcessHandler::completedCorrectly()
{
  return done;
}

void rarProcessHandler::handleProcess()
{
  puts("handling adding");
  connect(rarProc, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
  connect(rarProc, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
 // puts("extracting single file");
  puts("totalFileCount = "+ QString().setNum(totalFileCount).toAscii());
  if(totalFileCount < filesToHandle.size() && rarProc -> state() == QProcess::NotRunning) //if the extraction is complete we can start the next
  {
   // puts("extracting: "+ filesToHandle[totalFileCount].toAscii());
    QStringList toDebug;
    toDebug << params << rarArchive  << filesToHandle[totalFileCount] << pathTarget;
    puts(process.toAscii() +" " + toDebug.join(" ").toAscii());
  
      rarProc -> waitForFinished(-1);
      if ( filesToHandle[totalFileCount].indexOf ( "-ap" ) == -1 )
      {
        rarProc->start(process, QStringList() << params << rarArchive << filesToHandle[totalFileCount]); 
        rarProcProgress -> setCurrentFileName(filesToHandle[totalFileCount]);
        QFileInfo fileInfo(filesToHandle[totalFileCount]);
        QString size = KLocale("").formatByteSize(fileInfo.size());
        rarProcProgress -> setCurrentFileSize(size);
      }
      else
      {
        rarProc->start(process, QStringList() << params << filesToHandle[totalFileCount]<< rarArchive << filesToHandle[totalFileCount+1]); 
        rarProcProgress -> setCurrentFileName(filesToHandle[totalFileCount+1]);
        rarProcProgress -> incrementOverall();
        QFileInfo fileInfo(filesToHandle[totalFileCount+1]);
        QString size = KLocale("").formatByteSize(fileInfo.size());
        rarProcProgress -> setCurrentFileSize(size);
        totalFileCount++;
      }
   // }
    if(!passwordAsked) totalFileCount ++;
    else passwordAsked = false; //patch for one-file-extraction with password protection
  }
  else
  {
    if(totalFileCount == filesToHandle.size() && rarProc -> state() == QProcess::NotRunning)
    {
      processTimer -> stop();
      rarProcProgress -> setCurrentFileProgressToMaximum();
      rarProcProgress -> accept();
      if (rarProcError.isEmpty() == false) done = false;
      else done = true;
      emit processCompleted(done);
      //this section is asyncronous so we got to call showError here also..
      showError(rarProcError);

    }
  }

}

void rarProcessHandler::getError()
{ 

  QByteArray temp = rarProc -> readAllStandardError();

  // the keyword "already" tells us that rar is asking for an overwrite
  // we need to save the filename:
  // the string from rar is like: /the/path/of/the/archive.rar already exists. Overwrite it?
  // so we need to erase everything from already to get the file name
  // then we can call an overwrite dialog to handle user's choice

  if(QString().fromAscii(temp).contains("already")) 
  {
    
    //rarProcProgress -> setValue ( rarProcProgress -> maximum()); 
    QString targetFile = QString().fromAscii(temp); 
    int forParsing = targetFile.indexOf("already"); 
    targetFile.remove(forParsing-1, targetFile.length());
    targetFile.remove("\n");
    
    overwriteDialog *oDialog = new overwriteDialog(rarProc -> proc(), parentWidget); //chiamiamo l'overwrite dialog
    if(toAll == false)
    {
      QFileInfo details(targetFile); // generating file info
      oDialog -> setDestinationDetails(details.filePath());
      QString currentExtraction = filesToHandle[totalFileCount];
      QString size = rar().getSingleFileSize(globalTOC, filesToHandle[totalFileCount]);
      oDialog -> setSourceDetails(currentExtraction, rar::getSingleFileModificationTime(globalTOC, filesToHandle[totalFileCount]), size);
      rarProcProgress -> hide();
      if( oDialog -> exec() == QDialog::Rejected ) handleCancel();
      else
      {
      // here we handle the "toAll option"
        if(oDialog -> isToAllChecked() == true) toAll = true;
        rarProcProgress -> show();
      }
    }

    else
    {
      if(oDialog -> yesToAllChecked() == true) oDialog -> yesOverwrite(); //if was toAll checked then we answer the same for next times
      else oDialog -> noOverwrite();                  
    }
   delete oDialog; 
   totalFileCount++;             
  }
  else if(QString().fromAscii(temp).contains("password incorrect ?")) // here we handle a header-password-protected archive
  {
    crypted = true; //the archive is crypted;
    totalFileCount = 0;

    disconnect(rarProc, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
    disconnect(rarProc, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
    disconnect(rarProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(giveOutput(int, QProcess::ExitStatus)));
    rarProc->killProcess();
    if(rarProcProgress != NULL) rarProcProgress -> cancel();
    if(processTimer != 0) processTimer -> stop();
    KPasswordDialog pwD(parentWidget);
    pwD.setPrompt(i18n("This archive is <b>header password protected</b>.<br>Enter the password:"));
    QPixmap pixmap = KIcon("dialog-password").pixmap(64,64);
    pwD.setPixmap(pixmap);
    if(passwordRequests <3)
    {
      if(!pwD.exec()) return;
      else passwordRequests++;
      QString password = pwD.password();
      params.removeLast();
      params << "-p"+password;
      archivePassword = password;
      stdOutput.clear();
      passwordAsked = true;
     // puts("initProcess called");
      initProcess();
    }
    else
    {
      KMessageBox::information(parentWidget, i18n("Wrong password"), i18n("Wrong password"));
      return;
    }

  }
  else //altrimenti lasciamo che l'errore sia gestito da showError
  {
    if(QString().fromAscii(temp).indexOf("already") == -1 || QString().fromAscii(temp).indexOf("password") == -1) rarProcError.append(temp);
  } 
}

bool rarProcessHandler::isCrypted()
{
  return crypted;
}
 
QString rarProcessHandler::getArchivePassword()
{
  return archivePassword;
}

void rarProcessHandler::showError(QByteArray errore)
{

  if(!errore.isEmpty())
  {
    QByteArray error = errore;
    QByteArray original(QString("Cannot create").toAscii());
    QByteArray translated(QString("<b>" + tr("\nCannot create") + "</b>").toAscii());
    error.replace(original, translated);
    original = QString("ERROR:").toAscii();
    translated = QString("<b>" + tr("ERROR:") + "</b>").toAscii();
    error.replace(original, translated);
    original = QString("Locked archive").toAscii();
    translated = QString("<font color=red>" + tr("Locked archive") + "</font>").toAscii();
    error.replace(original, translated);
    original = QString("Cannot open").toAscii();
    translated = QString("<b>" + tr("Cannot open") + "</b>").toAscii();
    error.replace(original, translated);
    original = QString("Cannot create").toAscii();
    translated = QString("<b>" + tr("Cannot create") + "</b>").toAscii();
    error.replace(original, translated);
    original = QString("Cannot create directory").toAscii();
    translated = QString("<b>" + tr("Cannot create directory") + "</b>").toAscii();
    error.replace(original, translated);
    original = QString("Permission denied").toAscii();
    translated = QString("<br><font color=red>" + tr("Permission denied") + "<br></font>").toAscii();
    error.replace(original, translated);
    original = QString("is read-only").toAscii();
    translated = QString("<br><font color=red>" + tr("is read-only") + "<br></font>").toAscii();
    error.replace(original, translated);
    original = QString("No such file or directory").toAscii();
    translated = QString("</i><br><font color=red>" + tr("No such file or directory") + "<br></font>").toAscii();
    error.replace(original, translated);
    original = QString("Encrypted file:").toAscii();
    translated = QString("</i><br><font color=red>" + tr("Encrypted file") + "<br></font>").toAscii();
    error.replace(original, translated);
    original = QString("CRC failed").toAscii();
    translated = QString("<b>" + tr("CRC failed") + "</b>").toAscii();
    error.replace(original, translated);
    errorDialog->setError(error);
    errorDialog->show();
  }
}

QString rarProcessHandler::standardOutput()
{
  if(!stdOutput.contains("Pathname/Comment")) stdOutput.clear();
  return stdOutput;
}

QByteArray rarProcessHandler::rawStandardOutput()
{
  return rawOutput;
}

void rarProcessHandler::showProgress() //gestiamo un progressdialog
{
 
  QByteArray gotOutput = rarProc -> readAllStandardOutput();
  stdOutput.append(gotOutput);
  rawOutput.append(gotOutput);
  QStringList percentuale;
  
  if(QString(gotOutput).contains("Extracting") && !QString(gotOutput).contains("Extracting from"))
   {
    if(totalFileCount < filesToHandle.size()){
    // puts(filesToHandle[totalFileCount].toAscii());
     rarProcProgress -> setCurrentFileName(filesToHandle[totalFileCount]);
     QString size = rar::getSingleFileSize(globalTOC, filesToHandle[totalFileCount]);
     rarProcProgress -> setCurrentFileSize(size);
     totalFileCount++;
   }

  }
  if(QString(gotOutput).contains("OK") && !QString(gotOutput).contains("All OK") && params[0]!="a")
   {
    rarProcProgress->setCurrentFileProgressToMaximum();
    rarProcProgress->setCurrentFileProgress(0);
    rarProcProgress->incrementOverall();
   }
 //    rarProcProgress -> incrementOverall();

  if ( QString(gotOutput).contains ( "%" )  )
  {
    percentuale = QString(gotOutput).split ( " ", QString::SkipEmptyParts );
    if ( percentuale.size() == 2 )
    {
      percentuale[1].remove ( "%" );
      if(params[0]!="a") rarProcProgress->setCurrentFileProgress(rarProcProgress->currentFileProgressValue() + (100 / filesToHandle.size()));
        else rarProcProgress->setCurrentFileProgress(percentuale[1].toInt());
    //  rarProcProgress->setOverallProgress(percentuale[1].toInt());
    }
  }
  if ( QString(gotOutput).contains("All OK"))  rarProcProgress -> accept(); 

}

void rarProcessHandler::giveOutput(int, QProcess::ExitStatus)
{
 puts("process terminated");
 emit outputReady(standardOutput(), crypted);
 emit processCompleted(true); //check the bool
 showError(rarProcError);

}

void rarProcessHandler::handleCancel()
{
  rarProc -> killProcess();
  rarProcProgress -> cancel();
  if(params[0] == "a") processTimer->stop();
  emit processCanceled();
}

void rarProcessHandler::handlePaused()
{
  rarProc->pauseProcess();
}

void rarProcessHandler::handleContinued()
{
  rarProc -> continueProcess();
}

