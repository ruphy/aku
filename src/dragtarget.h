#ifndef DRAGTARGET_H
#define DRAGTARGET_H
#include "dragsource.h"

#include <QTreeWidget>
#include <QDragEnterEvent>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QList>
#include <QAction>
#include <QInputDialog>
#include <QDir>
#include <QMenu>
#include <QDirModel>
#include <QDir>
#include <QTimer>
#include <QPainter>
#include <QFlags>
#include <QUrl>
#include <QDesktopServices>
#include <QLabel>
#include <QPalette>
#include <QHeaderView>
#include <KIcon>
#include <KMimeType>
#include <KUrl>
#include <KFileItem>
#include <KLocale>
class dragTarget : public QTreeWidget
{
Q_OBJECT

public:
  dragTarget ( dragSource *,QWidget *parent = 0 );
  ~dragTarget();

public slots:
  virtual void createNewFolder();
  virtual void deleteItem();

protected:
  virtual void dragEnterEvent ( QDragEnterEvent *event );
  virtual void dropEvent ( QDropEvent *event );
  virtual void dragMoveEvent ( QDragMoveEvent *event );
  virtual QTreeWidgetItem* ramifica ( QString , QTreeWidgetItem *);
  virtual QTreeWidgetItem* ramifica (QString, QTreeWidget *); //overloaded function provided for adding toplevel items
  virtual void mouseMoveEvent ( QMouseEvent *event );
  virtual void focusOutEvent ( QFocusEvent *event );
  virtual void recursiveAppend ( QTreeWidgetItem *source, QTreeWidgetItem *destination );
  virtual void contextMenuEvent ( QContextMenuEvent * event );
  virtual void retrieveChildren ( QTreeWidgetItem *rootItem ); //ricostruiamo i figli dalla directory di partenza
  virtual void recursivePopulate ( QTreeWidgetItem *folder ); //funzione ricorsiva per completare tutti i figli
  virtual void mouseReleaseEvent ( QMouseEvent *event );
  virtual void showIndicator(int);
  virtual void hideIndicator();

protected slots:
  virtual void scrollDown();
  virtual void scrollUp();

private:
  bool subLevel;
  QTreeWidgetItem *padre;
  QString mimeCheck;
  QTreeWidgetItem *lastItemAdded;
  QTreeWidgetItem *lastItemClicked; //per sapere la destinazione delle azioni dei contextmenu
  bool hasSameParent ( QTreeWidgetItem*, QString );
  QTimer *scrollDownTimer;
  QTimer *scrollUpTimer;
  QTreeWidgetItem *tempScroll;
  QTreeWidgetItem *backgroundToRestore;
  QTreeWidgetItem *indicatorToRestore;
  QTreeWidgetItem *currentItem;
  QList<QTreeWidgetItem*> fadesToRestore;
  dragSource *sourceView;
  QWidget *indicator;
};

#endif
