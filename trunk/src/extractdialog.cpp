#include "extractdialog.h"

QString rarToExtract;
QStringList itemsToExtract;
rarProcess *rarprocess;
QStringList params;

extractDialog::extractDialog (QString archiver, QString archive, QStringList files, QStringList additionalParameters, QWidget* parent, Qt::WFlags fl ) : KDialog ( parent, fl ), Ui::Dialog()
{
  globalArchiver = archiver;
  params = additionalParameters;
  parentWidget = parent;
  setupUi (this);
  KAction *actionShow_hidden_files = new KAction(treeView);
  actionShow_hidden_files -> setText(i18n("Show hidden files"));
  actionShow_hidden_files -> setCheckable(true);
  treeView -> addAction ( actionShow_hidden_files );
  treeView -> setColumnHidden (1, true );
  treeView -> setColumnHidden (2, true );
  treeView -> setColumnHidden (3, true );
  treeView -> setColumnHidden (4, true );
  treeView -> setColumnHidden (5, true );
  treeView -> setColumnHidden (6, true );
  treeView -> header() -> setResizeMode ( 0, QHeaderView::ResizeToContents );
  treeView -> resizeColumnToContents (0);
  treeView -> setDirOnlyMode(true);
  treeView -> setEditTriggers(QAbstractItemView::NoEditTriggers);
  treeView -> setCurrentUrl(KUrl(QDir::homePath()));
  show();
  completerBox();
  rarToExtract = archive;
  itemsToExtract = files;
  enableButtonOk(false);
  enableButtonCancel(false);
  showButton(KDialog::Ok, false);
  showButton(KDialog::Cancel, false);
  kdialogbuttonbox -> button(QDialogButtonBox::Ok) -> setIcon(KIcon("dialog-ok"));
  kdialogbuttonbox -> button(QDialogButtonBox::Cancel) -> setIcon(KIcon("dialog-cancel"));
  KGuiItem guiItem(i18n("Defaults"), "edit-undo");
  buttonDefault -> setGuiItem(guiItem);
  KGuiItem guiItem2(i18n("New Directory"), "folder-new");
  newdirButton -> setGuiItem(guiItem2);
  KGuiItem guiItem3(i18n("Save Settings"), "document-save");
  buttonSave -> setGuiItem(guiItem3);
  //loadSettings();
  //connect ( kdialogbuttonbox, SIGNAL (accepted()), this, SLOT (extraction()));
  connect ( kdialogbuttonbox, SIGNAL (rejected()), this, SLOT (closeDialog()));
  connect ( newdirButton, SIGNAL (clicked()), this, SLOT (createNewFolder()));
  connect ( buttonDefault, SIGNAL (clicked()), this, SLOT (defaultSettings()));
  connect ( buttonSave, SIGNAL (clicked()), this, SLOT (saveSettings()));
  connect ( actionShow_hidden_files, SIGNAL (toggled(bool)), this, SLOT (hiddenFiles(bool)));
  connect ( treeView, SIGNAL (currentChanged (KUrl)), this, SLOT (updateCombo(KUrl)));
  connect ( khistorycombobox, SIGNAL(returnPressed(QString)), this, SLOT(updateTreeViewSelection(QString)));
  connect ( khistorycombobox, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateTreeViewSelection(QString)));
  radioAbsolutepath -> setEnabled(false);

}

extractDialog::~extractDialog()
{
  qDeleteAll(children());
}

void extractDialog::updateCombo(KUrl url)
{
  khistorycombobox -> setCurrentItem(url.pathOrUrl(), true);
}

void extractDialog::createNewFolder()
{
  bool ok;
  QString folderName = QInputDialog::getText(this, i18n("New Folder"), i18n("Enter a name for the new folder"),  KLineEdit::Normal, i18n("New Folder"),&ok);
  if(ok && !folderName.isEmpty())
  {
    QString currentPath = treeView -> currentUrl().pathOrUrl();
    if(!currentPath.endsWith(QDir::separator())) currentPath.append(QDir::separator());
    if(QDir(currentPath).mkdir(folderName)) treeView -> setCurrentUrl(KUrl(currentPath+folderName));
  }
}

void extractDialog::updateTreeViewSelection(QString path)
{
  KUrl url(path);
  if(url.isLocalFile())
    treeView -> setCurrentUrl(url); 
}

void extractDialog::hiddenFiles(bool status)
{
  if (status == true) 
  {
    //puts ("true");
    KUrl current = treeView -> currentUrl();
    treeView -> setShowHiddenFiles(true);
    treeView -> setCurrentUrl(current);
  }
  else
  {
    //puts ("false");
    KUrl current = treeView -> currentUrl();
    treeView -> setShowHiddenFiles(false);
    treeView -> setCurrentUrl(current);
  }
}

void extractDialog::completerBox()
{
  KUrlCompletion *comp = new KUrlCompletion(KUrlCompletion::DirCompletion);
  khistorycombobox -> setCompletionObject(comp);
  khistorycombobox -> setAutoDeleteCompletionObject(true);
  khistorycombobox -> setCompletionMode(KGlobalSettings::CompletionPopupAuto);
}

void extractDialog::checkPath()
{
  if (!QDir().exists(khistorycombobox -> currentText())) 
    QDir().mkpath(khistorycombobox -> currentText());
}

// void extractDialog::extraction()
// {
//   checkPath();
// 
//   QString command;
//   QString updatemode;
//   QString overwritemode;
// 
//   // update mode
//   switch (comboUpdateBox -> currentIndex())
//   {
//     case 1:
//     // extract and update files
//     // Update files. May be used with archive extraction or creation.
//     // the switch '-u' is used with the commands 'x' or 'e', then files
//     // not present on the disk and files newer than their copies on the
//     // disk would extracted from the archive.
//       updatemode = "-u";
//       break;
//     case 2:
//     // fresh existing files only
//     // Freshen files. May be used with archive extraction or creation.
//     // the switch '-f' is used with the commands 'x' or 'e', then only
//     // old files would be replaced with new versions extracted from the
//     // archive.
//       updatemode = "-f";
//       break;
//     default:
//       // extract and replace files
//       updatemode = "";
//   }
//   
//   // overwrite mode
//   switch (comboOverwriteBox -> currentIndex())
//   {
//     case 1:
//       // overwrite without prompt
//       overwritemode = "-o+";
//       break;
//     case 2:
//       // skip existing files 
//       overwritemode = "-o-";
//       break;
//     case 3:
//       // rename automatically
//       overwritemode = "-or";
//       break;
//     default:
//       // ask before overwrite
//       overwritemode = "";
//   }
// 
//   QStringList switches;
//   if (!updatemode.isEmpty()) switches << updatemode;
//   else if (!overwritemode.isEmpty()) switches << overwritemode;
// 
//   // Keep Broken File
//   // RAR, by default, deletes files with CRC errors after
//   // extraction. The switch -kb specifies that files with
//   // CRC errors should not be deleted.
//   if (checkKeepBroken -> isChecked()) switches << "-kb";
// 
//   // RadioFilePath "rar e" or "rar x"
//   // non considero per il momento la terza opzione "absolute path"
//   // checkRadioFilePath
//   // 1. Extract full paths -->>          x
//   // 2. Do not extract full paths -->>   e
//   // 3. Extract absolute paths -->>      ?
//   if (radioFullpath -> isChecked()) command = "x";
//   else command = "e";
//   
//   if (checkTimeLast -> isChecked()) switches << "-tsa";
//   if (checkTimeCreation -> isChecked()) switches << "-tsc";
//   if (checkTimeModification -> isChecked()) switches << "-tsm ";
//   // -cl     Convert file names to lower case
//   if (checkLowerCase-> isChecked()) switches << "-cl";
//   // -av-    Disable authenticity verification checking or adding.
//   if (!checkAuthenticity-> isChecked()) switches << "-av-";
// 
//   puts("extracting with params:"+ command.toAscii() + " "+ params.join(" ").toAscii() + " " +switches.join(" ").toAscii());
//   if ( !itemsToExtract.isEmpty() )
//   {
//     if (params.isEmpty()) rarprocess = new rarProcess(parentWidget,globalArchiver, QStringList() << command << switches, rarToExtract , itemsToExtract , khistorycombobox -> currentText());
//     else rarprocess = new rarProcess(parentWidget,globalArchiver, QStringList()<< command << params << switches, rarToExtract , itemsToExtract , khistorycombobox -> currentText());
//     if(checkOpenDestination ->isChecked()) connect(rarprocess, SIGNAL(processCompleted(bool)), this, SLOT(openDestinationPath(bool)));
//     if(radioDeleteAlways ->isChecked()) connect(rarprocess, SIGNAL(processCompleted(bool)), this, SLOT(deleteArchive(bool)));
//     if(radioDeleteAsk ->isChecked()) connect(rarprocess, SIGNAL(processCompleted(bool)), this, SLOT(deleteArchiveAsk(bool)));
//   }
//   else
//   {
//     
//     if(params.isEmpty()) rarprocess = new rarProcess(parentWidget, globalArchiver, QStringList() << command << switches,rarToExtract ,QStringList(), khistorycombobox -> currentText() );
//     else rarprocess = new rarProcess(parentWidget, globalArchiver, QStringList() << command << params << switches,rarToExtract ,QStringList(), khistorycombobox -> currentText() );
//     if(checkOpenDestination ->isChecked()) connect(rarprocess, SIGNAL(processCompleted(bool)), this, SLOT(openDestinationPath(bool)));
//     if(radioDeleteAlways ->isChecked()) connect(rarprocess, SIGNAL(processCompleted(bool)), this, SLOT(deleteArchive(bool)));
//     if(radioDeleteAsk ->isChecked()) connect(rarprocess, SIGNAL(processCompleted(bool)), this, SLOT(deleteArchiveAsk(bool)));
//     
//   }
//   rarprocess -> start();
//   khistorycombobox -> addToHistory(khistorycombobox -> currentText());
// 
//   KConfig config;
//   QStringList list;
//   KConfigGroup options(&config, "HistoryList");
//   list = khistorycombobox -> historyItems();
//   options.writeEntry("History", list);
// 
//   accept();
//   //ext.waitForFinished();
// }

void extractDialog::deleteArchive(bool ok)
{
  if (ok)
  {
    if (QFile::remove(rarToExtract)) emit closeArchive();
    else emit cantDelete();
  }
}

void extractDialog::deleteArchiveAsk(bool ok)
{
  if (ok)
  {
    // Yes = 3
    // No = 4
    if (KMessageBox::questionYesNo(this, i18n("Do you wish to delete the unpacked archive?"), i18n("Delete archive")) == 3)
    {
      if (QFile::remove(rarToExtract)) emit closeArchive();
      else emit cantDelete();
    }
  }
}


void extractDialog::openDestinationPath(bool open)
{
  if(open) QDesktopServices::openUrl(QUrl::fromLocalFile(khistorycombobox -> currentText()));
}

void extractDialog::closeDialog()
{ 
  reject();
}

void extractDialog::defaultSettings()
{
  // tab General
  comboUpdateBox -> setCurrentIndex(0);
  comboOverwriteBox -> setCurrentIndex(0);
  checkKeepBroken -> setChecked(false);
  checkOpenDestination -> setChecked(false);
  // tab Advanced
  radioFullpath -> setChecked(true);
  radioDeleteNever -> setChecked(true);
  checkTimeCreation -> setChecked(false);
  checkTimeLast -> setChecked(false);
  checkTimeModification -> setChecked(false);
  checkLowerCase -> setChecked(false);
  checkAuthenticity -> setChecked(true);
}

// void extractDialog::loadSettings()
// {
//   KConfig config;
//  
//   KConfigGroup history(&config, "HistoryList");
//   QStringList list = history.readEntry("History", QStringList());
//   khistorycombobox -> setHistoryItems(list);
// 
//   KConfigGroup options(&config, "extractOptions");
//   // tab general
//   comboUpdateBox -> setCurrentIndex(options.readEntry("update", 0));
//   comboOverwriteBox -> setCurrentIndex(options.readEntry("overwrite", 0));
//   checkKeepBroken -> setChecked(options.readEntry("broken", false));
//   checkOpenDestination -> setChecked(options.readEntry("open", false));
//   // tab advanced
//     // file paths
//   radioFullpath -> setChecked(options.readEntry("pathfull", true));
//   radioNofullpath -> setChecked(options.readEntry("pathnofull", false));
//   radioAbsolutepath -> setChecked(options.readEntry("pathabsolute", false));
//     // delete archive
//   radioDeleteNever -> setChecked(options.readEntry("deletenever", true));
//   radioDeleteAsk -> setChecked(options.readEntry("deleteask", false));
//   radioDeleteAlways -> setChecked(options.readEntry("deletealways", false));
//     // file time
//   checkTimeLast  -> setChecked(options.readEntry("timelast", false));
//   checkTimeCreation  -> setChecked(options.readEntry("timecreation", false));
//   checkTimeModification  -> setChecked(options.readEntry("timemodification", false));
//     // others
//   checkLowerCase -> setChecked(options.readEntry("lowercase", false));
//   checkAuthenticity -> setChecked(options.readEntry("authenticity", true));
// }

// void extractDialog::saveSettings()
// {
//   KConfig config;
//   KConfigGroup options(&config, "extractOptions");
//   // tab general
//   options.writeEntry("update", comboUpdateBox -> currentIndex());
//   options.writeEntry("overwrite", comboOverwriteBox -> currentIndex()); 
//   options.writeEntry("broken", checkKeepBroken -> isChecked());
//   options.writeEntry("open", checkOpenDestination -> isChecked());
//   //tab advanced
//     // file paths
//   options.writeEntry("pathfull", radioFullpath -> isChecked());
//   options.writeEntry("pathnofull", radioNofullpath -> isChecked());
//   options.writeEntry("pathabsolute", radioAbsolutepath -> isChecked());
//     // delete archive
//   options.writeEntry("deletenever", radioDeleteNever -> isChecked());
//   options.writeEntry("deleteask", radioDeleteAsk-> isChecked());
//   options.writeEntry("deletealways", radioDeleteAlways -> isChecked());
//     // file time
//   options.writeEntry("timelast", checkTimeLast -> isChecked());
//   options.writeEntry("timecreation", checkTimeCreation -> isChecked());
//   options.writeEntry("timemodification", checkTimeModification -> isChecked());
//     // others
//   options.writeEntry("lowercase", checkLowerCase-> isChecked());
//   options.writeEntry("authenticity", checkAuthenticity-> isChecked());
//   options.config() -> sync();
// }
