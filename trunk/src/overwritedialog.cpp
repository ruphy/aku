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
  yesButton-> setIcon(KIcon("dialog-ok-apply"));
  noButton-> setIcon(KIcon("dialog-cancel"));
  cancelButton -> setIcon(KIcon("dialog-close"));
  renameButton -> setIcon(KIcon("edit-rename"));
  overwriteProcess = proc;

  KMenu *okMenu = new KMenu(this);
  KMenu *noMenu = new KMenu(this);
  noMenu -> addTitle(KIcon("edit-redo"), i18n("Keep for the next file(s)"));
  okMenu -> addTitle(KIcon("edit-redo"), i18n("Keep for the next file(s)"));
  yesApplyAll = okMenu -> addAction(i18n("Apply to all"));
  noApplyAll = noMenu -> addAction(i18n("Apply to all"));
  
  noButton -> setDelayedMenu(noMenu);
  yesApplyAll -> setCheckable(true);
  yesButton -> setDelayedMenu(okMenu);
  noApplyAll -> setCheckable(true);

  //destDetails -> header() -> setVisible(false);
  destDetails -> horizontalHeader() -> setVisible(false);
  destDetails -> verticalHeader() -> setVisible(false);
  sourceDetails -> horizontalHeader() -> setVisible(false);
  sourceDetails -> verticalHeader() -> setVisible(false);
  //destDetails -> setReadOnly(true);
  //sourceDetails -> setReadOnly(true);
  destDetails -> setFocusPolicy(Qt::NoFocus);
  sourceDetails -> setFocusPolicy(Qt::NoFocus);
  destDetails -> setContextMenuPolicy(Qt::NoContextMenu);
  sourceDetails -> setContextMenuPolicy(Qt::NoContextMenu);

  destDetails -> setAlternatingRowColors(true);
  sourceDetails -> setAlternatingRowColors(true);
  
  itemPath = new QTableWidgetItem(i18n("Filepath"));
  QFont font;
  font.setItalic(true);
  itemPath -> setFont(font);
  destDetails -> setItem(0, 0, itemPath);
  itemPath = new QTableWidgetItem(i18n("Owner"));
  itemPath -> setFont(font);
  destDetails -> setItem(1, 0, itemPath);
  itemPath = new QTableWidgetItem(i18n("Modified on"));
  itemPath -> setFont(font);
  destDetails -> setItem(2, 0, itemPath);
  itemPath = new QTableWidgetItem(i18n("Size"));
  itemPath -> setFont(font);
  destDetails -> setItem(3, 0, itemPath);
 
  itemPath = new QTableWidgetItem(i18n("Archive path"));
  itemPath -> setFont(font);
  sourceDetails -> setItem(0, 0, itemPath);
  itemPath = new QTableWidgetItem(i18n("Modified on"));
  itemPath -> setFont(font);
  sourceDetails -> setItem(1, 0, itemPath);
  itemPath = new QTableWidgetItem(i18n("Size"));
  itemPath -> setFont(font);
  sourceDetails -> setItem(2, 0, itemPath);
  
  sourceDetails -> resizeColumnToContents(0);
  sourceDetails -> resizeRowsToContents();
  destDetails -> resizeColumnToContents(0);
  destDetails -> resizeRowsToContents();
  

  connect(renameButton, SIGNAL(clicked()), this, SLOT(renameAction()));
  connect(yesButton, SIGNAL(clicked()),this, SLOT(yesOverwrite()));
  connect(noButton, SIGNAL(clicked()), this, SLOT(noOverwrite()));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelOverwrite()));
  connect(sourceDetails, SIGNAL(itemSelectionChanged()), this, SLOT(disableTableEditing())); 
}

// destination -->> hard disk file
// source -->> rar file

overwriteDialog::~overwriteDialog()
{
}

void overwriteDialog::disableTableEditing()
{
  sourceDetails -> setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void overwriteDialog::yesOverwrite()
{
  if(yesApplyAll -> isChecked()) {
    overwriteProcess -> write("a\n");
    yesToAll = true;
  }
  else overwriteProcess -> write("y\n");
  yes = true;
  this -> accept();
}

void overwriteDialog::noOverwrite()
{
  if(noApplyAll -> isChecked()) {
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
  sourceDetails -> setEditTriggers(QAbstractItemView::AllEditTriggers);
  itemPath = sourceDetails -> item(0, 1);
  sourceDetails -> setCurrentItem(itemPath);
  sourceDetails -> editItem(itemPath);
  connect(sourceDetails, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(renameOverwrite(QTableWidgetItem * )));
}

void overwriteDialog::renameOverwrite(QTableWidgetItem * item)
{ 
  //toallButton -> setChecked(false);
  itemPath = sourceDetails -> item(0, 1);
  //disconnect(sourceDetails, SIGNAL (itemChanged(QTreeWidgetItem *)), this, SLOT (renameOverwrite(QTreeWidgetItem *)));
  if (!(itemPath -> text()).isEmpty()) {
  //if (!((klineedit -> text()).isEmpty())) {
    overwriteProcess -> write("r\n");
    overwriteProcess -> write((itemPath -> text()).toAscii()+"\n");
    this -> accept();
  }
}

void overwriteDialog::setDestinationDetails(QString filePath)
{
  KFileItem fileItem(KFileItem::Unknown, KFileItem::Unknown, KUrl(filePath));
 
  destinationFilePath = fileItem.url().path(); 
  QString size = KIO::convertSize(fileItem.size());
  
  itemPath = new QTableWidgetItem(destinationFilePath);
  QFont font;
  font.setBold(true);
  itemPath -> setFont(font);
  destDetails -> setItem(0, 1, itemPath);
  itemPath = new QTableWidgetItem(fileItem.user());
  destDetails -> setItem(1, 1, itemPath);
  itemPath = new QTableWidgetItem(fileItem.time(KFileItem::ModificationTime).toString(KDateTime::LocalDate));
  destDetails -> setItem(2, 1, itemPath);
  itemPath = new QTableWidgetItem(size);
  destDetails -> setItem(3, 1, itemPath);
  
  destDetails -> resizeColumnToContents(1);
  //destDetails -> setText("<i>" + i18n("Filepath") + ": </i><b>" + destinationFilePath + "</b><br><i>" + i18n("Owner")+ ": </i>" + fileItem.user() + "<br><i>"+ i18n("Modified on ") + "</i>" + fileItem.time(KFileItem::ModificationTime).toString(KDateTime::LocalDate)+ "<br><i>" + i18n("Size") + ": </i>" + size);
  
  QString mime =  KMimeType::iconNameForUrl(KUrl(filePath));
  setDestinationIcon(mime);
}
 
void overwriteDialog::setDestinationIcon(QString source)
{
  QPixmap icon = KIcon(source).pixmap(60,60);
  destIconLabel -> setPixmap(icon);
  
}
 
void overwriteDialog::setSourceIcon(QString source)
{
  QPixmap icon = KIcon(source).pixmap(60,60);
  sourceIconLabel -> setPixmap(icon);
}
 
void overwriteDialog::setSourceDetails(QString filePath, KDateTime lastModifiedTime, QString Size)
{
  KUrl url(filePath);

  itemPath = new QTableWidgetItem(url.path());
  sourceDetails -> setItem(0, 1, itemPath);
  itemPath = new QTableWidgetItem(lastModifiedTime.toString(KDateTime::LocalDate));
  sourceDetails -> setItem(1, 1, itemPath);
  itemPath = new QTableWidgetItem(Size);
  sourceDetails -> setItem(2, 1, itemPath);

  sourceDetails -> resizeColumnToContents(1);
  
  //sourceDetails -> setText("<i>" + i18n("Archive path") + ": </i>" + url.path() + "<br><i>" + i18n("Modified on ") + "</i>" + lastModifiedTime.toString(KDateTime::LocalDate)+ "<br><i>" + i18n("Size") + ": </i>" + Size);
  setSourceIcon(KMimeType::iconNameForUrl(url));
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
