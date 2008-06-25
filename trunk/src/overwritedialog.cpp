#include "overwritedialog.h"

QProcess *overwriteProcess;

overwriteDialog::overwriteDialog(QProcess *proc, QWidget* parent, Qt::WFlags fl)
: KDialog( parent, fl ), Ui::OverwriteDialog()
{
  yesToAll = false;
  noToAll = false;
  enableButtonOk(false);
  enableButtonCancel(false);
  showButton(KDialog::Ok, false);
  showButton(KDialog::Cancel, false);
  setupUi(this);
  setWindowTitle(i18n("Confirm file replace"));
  yesButton-> setIcon(KIcon("dialog-ok"));
  noButton-> setIcon(KIcon("list-remove"));
  toallButton-> setIcon(KIcon("arrow-right"));
  cancelButton -> setIcon(KIcon("dialog-cancel"));
  renameButton -> setIcon(KIcon("edit-rename"));
  overwriteProcess = proc;
  
  destDetails -> setReadOnly(true);
  sourceDetails -> setReadOnly(true);
  destDetails -> setFocusPolicy(Qt::NoFocus);
  sourceDetails -> setFocusPolicy(Qt::NoFocus);
  destDetails -> setContextMenuPolicy(Qt::NoContextMenu);
  sourceDetails -> setContextMenuPolicy(Qt::NoContextMenu);

  connect(renameButton, SIGNAL(clicked()), this, SLOT(renameAction()));
  connect(yesButton, SIGNAL(clicked()),this, SLOT(yesOverwrite()));
  connect(noButton, SIGNAL(clicked()), this, SLOT(noOverwrite()));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelOverwrite()));
 
}

// destination -->> hard disk file
// source -->> rar file

overwriteDialog::~overwriteDialog()
{
}

void overwriteDialog::yesOverwrite()
{
  if(toallButton -> isChecked() == true)
  {
    overwriteProcess -> write("a\n");
    yesToAll = true;
  }
  else overwriteProcess -> write("y\n");
  yes = true;
  this -> accept();
}

void overwriteDialog::noOverwrite()
{
  if(toallButton -> isChecked() == true)
  {
    overwriteProcess -> write("e\n");
    noToAll = true;
  }
  else overwriteProcess -> write("n\n");
  yes = false;
  this -> accept();
}
 
void overwriteDialog::cancelOverwrite()
{
  overwriteProcess -> write("q\n");
  this -> reject();
}

void overwriteDialog::renameAction()
{
  klineedit -> setEnabled(true);
  klineedit -> setFocus(Qt::MouseFocusReason);
  connect(klineedit, SIGNAL(returnPressed()), this, SLOT(renameOverwrite()));
}

void overwriteDialog::renameOverwrite()
{ 
  toallButton -> setChecked(false);
  if(klineedit -> text() != "")
  {
    overwriteProcess -> write("r\n");
    overwriteProcess -> write(klineedit -> text().toAscii()+"\n");
    this -> accept();
  }
}

//void overwriteDialog::setDestinationInfo()
//{
  //destFile -> setText("<b>" + QUrl().fromLocalFile(destinationFilePath).toString() + destinationName + "</b>");
  //destFile -> setText("<br><b><font size=2><a href=\""+QUrl().fromLocalFile(destinationFilePath).toString()+"\">"+destinationName+"</a></font></b></br>\n"+tr("\nalready exists.\nDo you wish to overwrite it?"));
//}

void overwriteDialog::setDestinationDetails(QString filePath)
{
  KFileItem fileItem(KFileItem::Unknown, KFileItem::Unknown, KUrl(filePath));
  //destinationPath = fileItem.url().directory();
  //destinationName = fileItem.url().fileName();
  destinationFilePath = fileItem.url().path();
  //setDestinationInfo();
  QString size = KIO::convertSize(fileItem.size());
  destDetails -> setText("<i>" + i18n("Filepath") + ": </i>" + "<a style='text-decoration: none' href=\"" + KUrl().fromLocalFile(destinationFilePath).toString() + "\">" + destinationFilePath + "</a><br><i>" + i18n("Owner")+ ": </i>" + fileItem.user() + "<br><i>"+ i18n("Modified on ") + "</i>" + fileItem.time(KFileItem::ModificationTime).toString(KDateTime::LocalDate)+ "<br><i>" + i18n("Size") + ": </i>" + size);
  //destDetails -> setToolTip("<i>" + i18n("Filepath") + ": </i>" + destinationFilePath);
  QString mime =  KMimeType::iconNameForUrl(KUrl(filePath));
  setDestinationIcon(mime);
}
 
void overwriteDialog::setDestinationIcon(QString source)
{
  QPixmap icon = KIcon(source).pixmap(50,50);
  destIconLabel -> setPixmap(icon);
  //<a href=\""+QUrl().fromLocalFile(destinationFilePath).toString()+"\">
}
 
// void overwriteDialog::setSourceInfo(QString info)
// {
//   // destFile is the path of the existing file in the archive
//   destFile -> setText(info);
// }
 
void overwriteDialog::setSourceIcon(QString source)
{
  QPixmap icon = KIcon(source).pixmap(50,50);
  sourceIconLabel -> setPixmap(icon);
}
 
void overwriteDialog::setSourceDetails(QString filePath, KDateTime lastModifiedTime, QString Size)
{
  KUrl url(filePath);
  sourceDetails -> setText("<i>" + i18n("Archive path") + ": </i>" + url.path() + "<br><i>" + i18n("Modified on ") + "</i>" + lastModifiedTime.toString(KDateTime::LocalDate)+ "<br><i>" + i18n("Size") + ": </i>" + Size);
  //setSourceInfo(url.fileName());
  //setSourceInfo(url.path());
  setSourceIcon(KMimeType::iconNameForUrl(url));
}

bool overwriteDialog::isToAllChecked()
{
  return toallButton -> isChecked();
}
 
bool overwriteDialog::yesToAllChecked()
{
  return yesToAll;
}
 
bool overwriteDialog::noToAllChecked()
{
  return noToAll;
}

bool overwriteDialog::isYes()
{
 return yes;
}
