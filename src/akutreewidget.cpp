#include "akutreewidget.h"

akuTreeWidget::akuTreeWidget(QWidget *parent) : QTreeWidget(parent)
{
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setDragDropMode(QAbstractItemView::NoDragDrop);
  setAlternatingRowColors(true);
  setSelectionMode(QAbstractItemView::ExtendedSelection);
  setAutoExpandDelay(-1);
  setRootIsDecorated(true);
  setUniformRowHeights(false);
  setSortingEnabled(true);
  setAnimated(true);
  setAllColumnsShowFocus(true);
  headerItem()->setText(0, i18n("File Name", 0));
  headerItem()->setText(1, i18n("Size", 0));
  headerItem()->setText(2, i18n("Packed Size", 0));
  headerItem()->setText(3, i18n("Ratio", 0));
  headerItem()->setText(4, i18n("Modified", 0));
  headerItem()->setText(5, i18n("Attributes", 0));
  headerItem()->setText(6, i18n("CRC", 0));
  headerItem()->setText(7, i18n("Method", 0));
  headerItem()->setText(8, i18n("Version", 0));
  headerItem()->setText(9, i18n("MIME Type", 0));
  headerItem()->setText(10,"");
  headerItem()->setIcon(10,KIcon("document-properties"));
  headerItem()->setToolTip(10, i18n("Status: a key is shown if the file is crypted"));
  header() -> moveSection(10,0);
  header() -> setResizeMode(10,QHeaderView::ResizeToContents);
  header() -> setResizeMode(4,QHeaderView::ResizeToContents);
  setupActions();
}

akuTreeWidget::~akuTreeWidget()
{
}

void akuTreeWidget::setupActions()
{
  mPopup = new KMenu(header());
  mPopup -> addTitle(i18n("Columns"));
  header() -> setContextMenuPolicy ( Qt::CustomContextMenu );
  headerSize = new KAction (i18n( "Size"), header() );
  (mPopup) -> addAction(headerSize);
  headerSize -> setCheckable( true);
  headerSize -> setChecked(true);
  headerPackedSize = new KAction(i18n ("Packed Size"), header() );
  (mPopup) -> addAction(headerPackedSize);
  headerPackedSize -> setCheckable( true );
  headerPackedSize -> setChecked(true);
  headerRatio = new KAction ( i18n( "Ratio" ), header() );
  (mPopup) -> addAction ( headerRatio );
  headerRatio -> setCheckable ( true );
  headerRatio -> setChecked(true);
  headerModified = new KAction(i18n ("Modified"), header() );
  (mPopup) -> addAction(headerModified);
  headerModified -> setCheckable( true );
  headerModified -> setChecked(true);
  headerAttributes = new KAction ( i18n( "Attributes" ), header() );
  (mPopup) -> addAction ( headerAttributes );
  headerAttributes -> setCheckable ( true );
  headerAttributes -> setChecked(true);
  headerCRC = new KAction(i18n("CRC"), header());
  (mPopup) -> addAction(headerCRC);
  headerCRC -> setCheckable(true);
  headerCRC -> setChecked(true);
  headerMethod = new KAction ( i18n( "Method" ), header() );
  (mPopup) -> addAction ( headerMethod );
  headerMethod -> setCheckable ( true );
  headerMethod -> setChecked(true);
  headerVersion = new KAction ( i18n( "Version" ),  header() );
  (mPopup) -> addAction ( headerVersion );
  headerVersion -> setCheckable ( true );
  headerVersion -> setChecked(true);
  headerMIME = new KAction(i18n("MIME Type"),  header());
  (mPopup) -> addAction(headerMIME);
  headerMIME -> setCheckable(true);
  headerMIME -> setChecked(true);
  connect(  header(), SIGNAL(customContextMenuRequested (QPoint)), this, SLOT(headerCustomContextMenu(QPoint)));
  connect ( headerRatio, SIGNAL ( toggled(bool) ), this, SLOT ( showratio(bool) ) );
  connect ( headerAttributes, SIGNAL ( toggled(bool) ), this, SLOT ( showattributes(bool) ) );
  connect ( headerMethod, SIGNAL ( toggled(bool) ), this, SLOT ( showmethod(bool) ) );
  connect ( headerVersion, SIGNAL (toggled(bool)), this, SLOT ( showversion(bool) ) );
  connect( headerModified, SIGNAL( toggled(bool)), this, SLOT( showmodified(bool) ));
  connect( headerCRC, SIGNAL( toggled(bool)), this, SLOT( showcrc(bool)));
  connect(headerSize, SIGNAL(toggled(bool)), this, SLOT(showsize(bool)));
  connect(headerPackedSize, SIGNAL(toggled(bool)), this, SLOT(showpackedsize(bool)));
  connect(headerMIME, SIGNAL(toggled(bool)), this, SLOT(showmime(bool)));
  headerAttributes -> setChecked(false);
  headerCRC -> setChecked(false);
  headerMethod -> setChecked(false);
  headerVersion -> setChecked(false);
  headerMIME -> setChecked(false);
}

void akuTreeWidget::headerCustomContextMenu(QPoint)
{
  mPopup ->  exec(QCursor::pos());
}

void akuTreeWidget::showratio(bool hide)
{
  setColumnHidden ( 3, !hide );
}

void akuTreeWidget::showattributes(bool hide)
{
  setColumnHidden ( 5, !hide );
}

void akuTreeWidget::showmethod(bool hide)
{
  setColumnHidden ( 7, !hide );
}

void akuTreeWidget::showversion(bool hide)
{
  setColumnHidden ( 8, !hide );
}

void akuTreeWidget::showsize(bool hide)
{
  setColumnHidden(1, !hide);
}

void akuTreeWidget::showpackedsize(bool hide)
{
  setColumnHidden(2, !hide);
}

void akuTreeWidget::showmodified(bool hide)
{
  setColumnHidden(4, !hide);
}

void akuTreeWidget::showmime(bool hide)
{
  setColumnHidden(9, !hide);
}

void akuTreeWidget::showcrc(bool hide)
{
  setColumnHidden(6, !hide);
}
