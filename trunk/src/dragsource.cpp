#include "dragsource.h"

dragSource::dragSource (QWidget *parent) : KFileTreeView (parent)
{
  hiddens = false;
  this -> setEnabled ( false );
  this -> setContextMenuPolicy ( Qt::ActionsContextMenu );
  setRootUrl(KUrl(QDir().homePath()));
  setEditTriggers(QAbstractItemView::NoEditTriggers);

  setSelectionMode(QAbstractItemView::SingleSelection);

  setShowHiddenFiles(false);
  resizeColumnToContents ( 0 );
  setDragEnabled ( true );
  setDragDropMode ( QAbstractItemView::DragOnly );
  this -> setEnabled ( true );
  
  connect( this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(enterDir(QModelIndex)));
  header() -> setResizeMode ( 0, QHeaderView::ResizeToContents );
}

dragSource::~dragSource()
{
}


void dragSource::enterDir(QModelIndex)
{
  KFileItem fileItem (KFileItem::Unknown, KFileItem::Unknown, currentUrl());
  if(fileItem.isDir()) {
    setRootUrl(currentUrl());
    emit currentUrlChanged(rootUrl());
  }
}

void dragSource::dirUp()
{
  KUrl url = currentUrl().upUrl();
  setRootUrl(url);
  emit currentUrlChanged(url);
}

void dragSource::goHome()
{ 
  KUrl url = QDir().homePath();
  setRootUrl(url);
  emit currentUrlChanged(url);
}

void dragSource::showHiddenFiles (bool show)
{
  setShowHiddenFiles(show);
  hiddens = show;
}

void dragSource::mouseMoveEvent (QMouseEvent *event)
{
  if (event -> buttons() == Qt::LeftButton ) {
    filePath = currentUrl().pathOrUrl();
    kDebug() << filePath.toAscii();
    QDrag *drag = new QDrag ( this );
    QMimeData * mimeData = new QMimeData;

    QFileInfo fileInfo(filePath);

    QString mimeType = KMimeType::findByPath(filePath) -> name(); 
    QString iconName = KMimeType::findByPath(filePath) -> iconName();
    KIcon icon(iconName);
    QPixmap mimeIcon = icon.pixmap(32,32); //se sono riuscito a ricavare il mime setto l'icona
    mimeData -> setImageData (QVariant(mimeIcon));
    filePath.append ( "!*mimetosend:"+mimeType ); //stringa di controllo per il mime
    drag -> setPixmap ( mimeIcon );

    /*******************************************************************************/

    QByteArray mimeToSend = filePath.toAscii();
    kDebug() << mimeToSend;
    mimeData -> setData ( "aku/newarchive", mimeToSend );
    drag -> setMimeData ( mimeData );
    drag -> exec ( Qt::CopyAction );
  }
  else return;
}

bool dragSource::hiddenShown()
{
  return hiddens;
}

KDirModel *dragSource::modelInUse()
{
  return mod;
}
