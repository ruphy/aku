#include "akumaintable.h"

akuMainTable::akuMainTable(QWidget *parent) : QTreeWidget(parent)
{
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setDragDropMode(QAbstractItemView::NoDragDrop);
  setAlternatingRowColors(true);
  setSelectionMode(QAbstractItemView::ExtendedSelection);
  setAutoExpandDelay(-1);
  setRootIsDecorated(true);
  setUniformRowHeights(false);
  //setSortingEnabled(true);
  setAnimated(true);
  setAllColumnsShowFocus(true);
  headerItem()->setText(0, i18n("File Name"));
  headerItem()->setText(1, i18n("Size"));
  headerItem()->setText(2, i18n("Packed Size"));
  headerItem()->setText(3, i18n("Ratio"));
  headerItem()->setText(4, i18n("Modified"));
  headerItem()->setText(5, i18n("Attributes"));
  headerItem()->setText(6, i18n("CRC"));
  headerItem()->setText(7, i18n("Method"));
  headerItem()->setText(8, i18n("Version"));
  headerItem()->setText(9, i18n("MIME Type"));
  headerItem()->setText(10,"");
  headerItem()->setIcon(10, KIcon("document-properties"));
  headerItem()->setToolTip(10, i18n("Status: file information"));
  header() -> moveSection(10, 0);
  header() -> setResizeMode(10, QHeaderView::ResizeToContents);
  headerItem()->setText(11, i18n("Owner/Group"));
  header() -> moveSection(11, 9);
  //header() -> setResizeMode(4, QHeaderView::ResizeToContents);
  setupActions();
}

akuMainTable::~akuMainTable()
{
}

void akuMainTable::setupActions()
{
  mPopup = new KMenu(header());
  mPopup -> addTitle(i18n("Columns"));
  header() -> setContextMenuPolicy (Qt::CustomContextMenu);
  headerSize = new KAction (i18n("Size"), header());
  mPopup -> addAction(headerSize);
  headerSize -> setCheckable(true);
  headerSize -> setChecked(true);
  headerPackedSize = new KAction(i18n ("Packed Size"), header());
  mPopup -> addAction(headerPackedSize);
  headerPackedSize -> setCheckable(true);
  headerPackedSize -> setChecked(true);
  headerRatio = new KAction (i18n("Ratio"), header());
  mPopup -> addAction (headerRatio);
  headerRatio -> setCheckable (true);
  headerRatio -> setChecked(true);
  headerModified = new KAction(i18n ("Modified"), header());
  mPopup -> addAction(headerModified);
  headerModified -> setCheckable( true );
  headerModified -> setChecked(true);
  headerAttributes = new KAction ( i18n( "Attributes" ), header());
  mPopup -> addAction (headerAttributes);
  headerAttributes -> setCheckable (true);
  headerAttributes -> setChecked(true);
  headerCRC = new KAction(i18n("CRC"), header());
  mPopup -> addAction(headerCRC);
  headerCRC -> setCheckable(true);
  headerCRC -> setChecked(true);
  headerMethod = new KAction (i18n( "Method" ), header());
  mPopup -> addAction ( headerMethod );
  headerMethod -> setCheckable ( true );
  headerMethod -> setChecked(true);
  headerVersion = new KAction ( i18n( "Version" ),  header() );
  mPopup -> addAction ( headerVersion );
  headerVersion -> setCheckable ( true );
  headerVersion -> setChecked(true);
  headerOG = new KAction(i18n("Owner/Group"),  header());
  mPopup -> addAction(headerOG);
  headerOG -> setCheckable(true);
  headerOG -> setChecked(true);
  headerMIME = new KAction(i18n("MIME Type"),  header());
  mPopup -> addAction(headerMIME);
  headerMIME -> setCheckable(true);
  headerMIME -> setChecked(true);
  connect(header(), SIGNAL(customContextMenuRequested (QPoint)), this, SLOT(headerCustomContextMenu(QPoint)));
  connect(headerRatio, SIGNAL(toggled(bool)), this, SLOT(showratio(bool)));
  connect(headerAttributes, SIGNAL(toggled(bool)), this, SLOT(showattributes(bool)));
  connect(headerMethod, SIGNAL(toggled(bool)), this, SLOT(showmethod(bool)));
  connect(headerVersion, SIGNAL(toggled(bool)), this, SLOT(showversion(bool)));
  connect(headerModified, SIGNAL(toggled(bool)), this, SLOT( showmodified(bool)));
  connect(headerCRC, SIGNAL(toggled(bool)), this, SLOT(showcrc(bool)));
  connect(headerSize, SIGNAL(toggled(bool)), this, SLOT(showsize(bool)));
  connect(headerPackedSize,SIGNAL(toggled(bool)), this, SLOT(showpackedsize(bool)));
  connect(headerOG,SIGNAL(toggled(bool)), this, SLOT(showog(bool)));
  connect(headerMIME,SIGNAL(toggled(bool)), this, SLOT(showmime(bool)));
  // opzioni di default
  headerRatio -> setChecked(false);
  headerPackedSize -> setChecked(false);
  headerAttributes -> setChecked(false);
  headerCRC -> setChecked(false);
  headerMethod -> setChecked(false);
  headerVersion -> setChecked(false);
  headerMIME -> setChecked(false);
  headerOG -> setChecked(false);
}

void akuMainTable::setFormat(QString compressor) {
  if (compressor == "rar") {
    headerRatio -> setChecked(true);
    headerRatio -> setVisible(true);
    headerPackedSize -> setChecked(true);
    headerPackedSize -> setVisible(true);
    headerCRC -> setVisible(true);
    headerCRC -> setChecked(false);
    headerMethod -> setVisible(true);
    headerMethod -> setChecked(false);
    headerAttributes -> setChecked(false);
    headerAttributes -> setChecked(false);
    headerVersion -> setVisible(true);
    headerVersion -> setChecked(false);
    headerOG -> setVisible(false);
    headerOG -> setChecked(false);
    headerMIME -> setChecked(false);
  }
  else if (compressor == "zip") {
    headerPackedSize -> setChecked(true);
    headerCRC -> setVisible(false);
    headerCRC -> setChecked(false);
    headerOG -> setVisible(false);
    headerPackedSize -> setVisible(true);
    headerRatio -> setVisible(true);
    headerRatio -> setChecked(true);
    headerMethod -> setVisible(true);
    headerMethod -> setChecked(false);
    headerVersion -> setVisible(true);
    headerVersion -> setChecked(false);
    headerAttributes -> setVisible(true);
    headerAttributes -> setChecked(false);
    headerMIME -> setVisible(true);
    headerMIME -> setChecked(false);
  }
  else if (compressor == "tar") {
    headerRatio -> setVisible(false);
    headerRatio -> setChecked(false);
    headerPackedSize -> setChecked(false);
    headerPackedSize -> setVisible(false);
    headerMethod -> setVisible(false);
    headerMethod -> setChecked(false);
    headerAttributes -> setChecked(false);
    headerMIME -> setChecked(false);
    headerVersion -> setChecked(false);
    headerVersion -> setVisible(false);
    headerCRC -> setVisible(false);
    headerOG -> setChecked(true);
    headerOG -> setVisible(true);
  }
}

void akuMainTable::headerCustomContextMenu(QPoint) {
  mPopup ->  exec(QCursor::pos());
}

void akuMainTable::showratio(bool hide) {
  setColumnHidden (3, !hide );
}

void akuMainTable::showog(bool hide) {
  setColumnHidden (11, !hide );
}

void akuMainTable::showattributes(bool hide) {
  setColumnHidden (5, !hide );
}

void akuMainTable::showmethod(bool hide) {
  setColumnHidden (7, !hide );
}

void akuMainTable::showversion(bool hide) {
  setColumnHidden (8, !hide );
}

void akuMainTable::showsize(bool hide) {
  setColumnHidden(1, !hide);
}

void akuMainTable::showpackedsize(bool hide) {
  setColumnHidden(2, !hide);
}

void akuMainTable::showmodified(bool hide) {
  setColumnHidden(4, !hide);
}

void akuMainTable::showmime(bool hide) {
  setColumnHidden(9, !hide);
}

void akuMainTable::showcrc(bool hide) {
  setColumnHidden(6, !hide);
}

QString akuMainTable::rebuildFullPath(QTreeWidgetItem *toRebuild)
{
  QString rebuilded = rebuildPath(toRebuild);
  if (!rebuilded.isEmpty()) return rebuilded + QDir().separator() + toRebuild -> text(0);
  else return toRebuild -> text(0);
}

QString akuMainTable::rebuildPath (QTreeWidgetItem *toRebuild) //missing item name.. see rebuildFullPath for complete string
{
  QTreeWidgetItem *tmp; //elemento temporaneo
  QStringList pathlist; //lista delle cartelle di percorso
  tmp = toRebuild -> parent();
  while (tmp != NULL) {
    pathlist << tmp -> text (0);
    tmp = tmp -> parent();
  }
  QString path;
  for ( int i = pathlist.size()-1; i>=0; i-- )   {
    path.append (pathlist[i]);
    if ( i!=0 ) path.append (QDir().separator());
  }
  return path;
}

void akuMainTable::expandTopLevelItems()
{
  for (int i = 0; i < this -> topLevelItemCount(); i++ )
    this -> setItemExpanded (this -> topLevelItem(i), true);
}

void akuMainTable::setFolderIcons()
{
  for (int i = 0; i < this -> topLevelItemCount(); i++ )
    if ((this -> topLevelItem(i)) -> text ( 5 ).isEmpty())
    {
      (this -> topLevelItem ( i ) ) -> setIcon (0, KIcon( "inode-directory" ));
       recursiveFolderIcons (this -> topLevelItem (i));
    }
}

void akuMainTable::recursiveFolderIcons (QTreeWidgetItem *checkParent)
{
  for ( int i = 0; i < checkParent -> childCount(); i++ )
    if ( ( checkParent -> child ( i ) ) -> text ( 5 ).isEmpty() )
    {
      (checkParent -> child ( i ) ) -> setIcon ( 0, KIcon ( "inode-directory" ));
      recursiveFolderIcons (checkParent -> child ( i ));
    } 
}

QStringList akuMainTable::filesToExtract()
{
  QStringList itemsPath;
  QList<QTreeWidgetItem*> selectedToExtract = selectedItems();

  if(selectedToExtract.size() != 0) {
    for (int i = 0; i < selectedToExtract.size(); i++ ) {
      QTreeWidgetItem *tmp;
      QStringList pathlist; // file da estrarre dall'archivio
      pathlist << ( selectedToExtract[i] ) -> text ( 0 );
      tmp = ( selectedToExtract[i] ) -> parent();
      while ( tmp != NULL ) {
        pathlist << tmp -> text ( 0 );
        tmp = tmp -> parent();
      }
      QString path;
      for ( int i = pathlist.size() - 1; i >= 0; i-- ) {
        path.append ( pathlist[i] );
        if ( i!=0 ) path.append (QDir().separator( ));
      }
      itemsPath << path;
    }
  }
  else itemsPath.clear();
  return itemsPath;
}

