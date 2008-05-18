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
    errorDock = new QWidget();
    errorEdit = new QTextEdit(errorDock);
    errorEdit -> setReadOnly(true);
    QGridLayout *layout = new QGridLayout(errorDock);
    layout -> addWidget(errorEdit,1,1);
    toAll = false;
    totalFileCount = 0; //this var will increment until getting the same value of filesToHandle.size()
    process = archiver;
    params = parameters;
    hasPasswordParameter = false;
    for(int i = 0; i < params.size(); i++)
      if(params[i].isEmpty() || params[i] == "-p") params.removeAt(i);

    QString str;
    foreach(str, params)
    {
      hasPasswordParameter = str.contains("-p");
      if(hasPasswordParameter)
        break;
    }
    if(!hasPasswordParameter && params[0] != "a" && params[0] != "ch") params << "-p-"; //this is to handle password later
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
    initProcess();
    showError(rarProcError);

  }
}


void rarProcessHandler::initProcess()
{
  puts("initProcess..");
  rarProc = new threadProcess(this);
  //setUp firstTime connections
  connect(rarProc, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
  connect(rarProc, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
  connect(rarProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(giveOutput(int, QProcess::ExitStatus)));
  //let's check how to launch the process
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


     // totalExtraction = false;
     // puts(QString("Launching process: " + process + " " + params.join(" ")+ " "+rarArchive+ " " + filesToHandle.join(" ") + " " + pathTarget).toAscii());
      
      //we need to recover archive file list
      

      rarProcProgress = new akuProgressDialog(parentWidget, filesToHandle.size());
      connect(rarProcProgress, SIGNAL(canceled()), this, SLOT(handleCancel()));
      connect(rarProcProgress, SIGNAL(paused()), this, SLOT(handlePaused()));
      connect(rarProcProgress, SIGNAL(continued()), this, SLOT(handleContinued()));
      rarProcProgress -> setArchiveName(rarArchive);

      if(fullArchive)
        rarProc -> start(process, QStringList() << params << rarArchive << pathTarget);
      else
       rarProc -> start(process, QStringList() << params << rarArchive << filesToHandle << pathTarget);
      // rarProc -> waitForFinished();
   // }
   // else
   // {
      //here we rebuild paths of the file the user wants to extract
    //  puts(QString("Launching process: " + process + " " + params.join(" ")+ " "+rarArchive+ " " + filesToHandle.join(" ") + " "+ pathTarget).toAscii());
      /*rar aids;
      //we need to recover archive file list
      puts("password parameter: "+params.last().toAscii());
      rarProc->start(process, QStringList() << "v" << params.last() << rarArchive);
      if(!hasPasswordParameter) rarProc->waitForFinished(1000); 
      else rarProc->waitForFinished(-1);
      globalTOC = rarProc->readAllStandardOutput();
      QStringList tempFileList;
      for(int i = 0; i < filesToHandle.size(); i++) tempFileList << aids.getListFromPath(filesToHandle[i], globalTOC); //we get the archive file list here from paths specified
      filesToHandle = tempFileList;

      rarProcProgress = new akuProgressDialog(parentWidget, filesToHandle.size());
      connect(rarProcProgress, SIGNAL(canceled()), this, SLOT(handleCancel()));
      connect(rarProcProgress, SIGNAL(paused()), this, SLOT(handlePaused()));
      connect(rarProcProgress, SIGNAL(continued()), this, SLOT(handleContinued()));
      rarProcProgress -> setArchiveName(rarArchive);
            //here we start a timer to handle the extraction avoiding the GUI to freeze
       processTimer = new QTimer();
       connect(processTimer, SIGNAL(timeout()), this, SLOT(handleProcess()));
       processTimer -> start(1);
       puts("started timer");*/
   // }
 }
  else if(params[0] == "a")
  {
    puts(QString("NewArchive Launching process: " + process + " " + params.join(" ")+ " "+rarArchive+ " " + filesToHandle.join(" ") + " "+ pathTarget).toAscii());
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
    connect(rarProc, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
    connect(rarProc, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
    rarProc->start(process, QStringList() << params << rarArchive);
   // if(!hasPasswordParameter) rarProc->waitForFinished(1000); 
   // else rarProc->waitForFinished(-1);
   // rarProc->waitForFinished();
  }
  else if(params[0] == "rn")
  {
    puts(QString("Launching process: " + process + " " + params.join(" ")+ " "+rarArchive+ " " + filesToHandle.join(" ") + " "+pathTarget).toAscii());
    connect(rarProc, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
    connect(rarProc, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
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
    connect(rarProc, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
    connect(rarProc, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
    rarProc->start(process, QStringList() << params << rarArchive << filesToHandle);
    rarProc->waitForFinished(1000);
  }
  else if(params[0] == "d" )
  {
    if(filesToHandle.isEmpty())
    {
      puts(QString("Launching process: " + process + " " + params.join(" ")+ " "+rarArchive + " "+filesToHandle.join(" ")+pathTarget).toAscii());
      connect(rarProc, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
      connect(rarProc, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
      rarProc->start(process, QStringList() << params << rarArchive);
      if(!hasPasswordParameter) rarProc->waitForFinished(1000); 
      else rarProc->waitForFinished(-1);
      if (rarProcError.isEmpty() == false) done = false;
      else done = true;
    }
    else
    {
      puts(QString("Launching process: " + process + " " + params.join(" ")+ " "+rarArchive + " "+filesToHandle.join(" ")).toAscii());
      connect(rarProc, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
      connect(rarProc, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
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
      connect(rarProc, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
      connect(rarProc, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
      rarProc->start(process, QStringList() << params << rarArchive);
      rarProc->waitForFinished(-1);
      if (rarProcError.isEmpty() == false) done = false;
      else done = true;
    }
    else
    {
      puts(QString("Launching process: " + process + " " + params.join(" ")+ " "+rarArchive + " "+filesToHandle.join(" ")).toAscii());
      connect(rarProc, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
      connect(rarProc, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
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
    connect(rarProc, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
    connect(rarProc, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
    rarProc->start(process, QStringList() << params << rarArchive);
    rarProc->waitForFinished(-1);
  }
  puts("initProcess terminata");

}

bool rarProcessHandler::completedCorrectly()
{
  return done;
}

void rarProcessHandler::handleProcess()
{
  connect(rarProc, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
  connect(rarProc, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
  puts("extracting single file");
  puts("totalFileCount = "+ QString().setNum(totalFileCount).toAscii());
  if(totalFileCount < filesToHandle.size() && rarProc -> state() == QProcess::NotRunning) //if the extraction is complete we can start the next
  {
    puts("extracting: "+ filesToHandle[totalFileCount].toAscii());
    QStringList toDebug;
    toDebug << params << rarArchive  << filesToHandle[totalFileCount] << pathTarget;
    puts(process.toAscii() +" " + toDebug.join(" ").toAscii());
    if(params[0] == "x" || params[0] == "e")
    {
      if(pathTarget != "") rarProc->start(process, QStringList() << params << rarArchive  << filesToHandle[totalFileCount] << pathTarget);
      else rarProc->start(process, QStringList() << params << rarArchive << filesToHandle[totalFileCount]);
      if(!hasPasswordParameter) rarProc -> waitForFinished(3000); 
      else rarProc -> waitForFinished(-1);
      rarProcProgress -> setCurrentFileName(filesToHandle[totalFileCount]);
      rarProcProgress -> incrementOverall();
      QString size = rar().getSingleFileSize(globalTOC, filesToHandle[totalFileCount]);
      rarProcProgress -> setCurrentFileSize(size);
    }
    else
    {
      rarProc -> waitForFinished(-1);
      if ( filesToHandle[totalFileCount].indexOf ( "-ap" ) == -1 )
      {
        rarProc->start(process, QStringList() << params << rarArchive << filesToHandle[totalFileCount]); 
        rarProcProgress -> setCurrentFileName(filesToHandle[totalFileCount]);
        rarProcProgress -> incrementOverall();
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
    }
    if(!passwordAsked) totalFileCount ++;
    else passwordAsked = false; //patch for one-file-extraction with password protection
  }
  else
  {
    puts("stopping timer");
    puts("totalFileCount =" +QString().setNum(totalFileCount).toAscii() + " filesToHandle.size() = "+QString().setNum(filesToHandle.size()).toAscii());
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
  //questa funzione acquisisce l'errore e in base alla sua entità lo tratta in maniera specifica
  puts("called");
  QByteArray temp = rarProc -> readAllStandardError();
  puts("temp = "+temp);
  if(QString().fromAscii(temp).indexOf("already") != -1) //se riceviamo una richiesta di sovrascrittura la catturiamo
  {
    puts("primo if");
    //rarProcProgress -> setValue ( rarProcProgress -> maximum()); //annulliamo la barra, perchè sicuramente ha finito l'estrazione precedente
    QString targetFile = QString().fromAscii(temp); //estraiamo il nome del file da sovrascrivere
    int forParsing = targetFile.indexOf("already"); //basandoci sull'occorrenza della stringa "already"
    targetFile.remove(forParsing-1, targetFile.length());
    targetFile.remove("\n");
    
    overwriteDialog *oDialog = new overwriteDialog(rarProc -> proc(), parentWidget); //chiamiamo l'overwrite dialog
    if(toAll == false)
    {
      QFileInfo details(targetFile); //generiamo le informazioni sul file
      oDialog -> setDestinationDetails(details.filePath());

      QString currentExtraction = filesToHandle[totalFileCount];
      QString size = rar().getSingleFileSize(globalTOC, filesToHandle[totalFileCount]);
      oDialog -> setSourceDetails(currentExtraction, rar().getSingleFileModificationTime(globalTOC, filesToHandle[totalFileCount]), size);
      rarProcProgress -> hide();
      if( oDialog -> exec() == QDialog::Rejected) handleCancel();
      else
      {
      //here we handle the "toAll option"
        if(oDialog -> isToAllChecked() == true) toAll = true;
        rarProcProgress -> show();
      }
    }

    else
    {
      if(oDialog -> yesToAllChecked() == true) oDialog -> yesOverwrite(); //if was toAll checked then we answer the same for next times
      else oDialog -> noOverwrite();                  
    }
                
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
    pwD.setPrompt(i18n("This archive is header-password-protected, enter the password"));
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
      puts("initProcess called");
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
  puts("fine di getError");  
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
    errorDock -> setGeometry(parentWidget -> x() , parentWidget -> y(), 300,200);
    errorDock -> setWindowTitle(i18n("An error occurred"));
    if(errorDock -> isVisible() != true) errorDock -> show();
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
    errorEdit -> append(error);
  }
}

QString rarProcessHandler::standardOutput()
{
  //stdOutput.remove("UNRAR 3.71 beta 1 freeware      Copyright (c) 1993-2007 Alexander Roshal");
  if(stdOutput.indexOf("Pathname/Comment") == -1 ) stdOutput.clear();
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
     puts(filesToHandle[totalFileCount].toAscii());
     rarProcProgress -> setCurrentFileName(filesToHandle[totalFileCount]);
     QString size = rar::getSingleFileSize(globalTOC, filesToHandle[totalFileCount]);
     rarProcProgress -> setCurrentFileSize(size);
     totalFileCount++;
   }

  }
  if(QString(gotOutput).contains("OK")){  // && !QString(gotOutput).contains("All OK")
     rarProcProgress -> incrementOverall();
 //   if(totalFileCount < filesToHandle.size()) totalFileCount++;
  }
  if ( QString(gotOutput).contains ( "%" )  )
  {
    percentuale = QString(gotOutput).split ( " ", QString::SkipEmptyParts );
    if ( percentuale.size() == 2 )
    {
      percentuale[1].remove ( "%" );
      int perc = percentuale[1].toInt();
      perc++;
      int increment = 100 / filesToHandle.size();
      perc = perc - increment*totalFileCount;
      rarProcProgress -> setCurrentFileProgress(perc*filesToHandle.size());
    }
  }
  if ( QString(gotOutput).contains("All OK"))  rarProcProgress -> accept(); 

}

void rarProcessHandler::giveOutput(int, QProcess::ExitStatus)
{
 emit outputReady(standardOutput(), crypted);
}

void rarProcessHandler::handleCancel()
{
  rarProc -> killProcess();
  rarProcProgress -> cancel();
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

