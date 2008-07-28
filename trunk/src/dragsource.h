#ifndef DRAGSOURCE_H
#define DRAGSOURCE_H

#include <QDirModel>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QAction>
#include <QFlag>

#include <QHeaderView>
#include <QFileIconProvider>
#include <QMessageBox>

#include <KFileTreeView>
#include <KDirLister>
#include <KDirModel>
#include <KFileItem>
#include <KMimeType>
#include <KUrl>
#include <KIcon>

#include "rar.h"

class dragSource : public KFileTreeView
{
Q_OBJECT

public:
  dragSource (QWidget *parent = 0);
  ~dragSource();
  ///QDir::Filters filters();
  bool hiddenShown();
  KDirModel *modelInUse();

public slots:
  //virtual void currentFile(QModelIndex);
  virtual void showHiddenFiles(bool);
  virtual void dirUp();
  virtual void goHome();
  virtual void enterDir(QModelIndex);

protected:
  virtual void mouseMoveEvent(QMouseEvent *event);
  
private:
  KDirModel *mod;

  KDirLister *lister;

  QString filePath;

  QModelIndex indexForIcon;

  bool hiddens;

signals:
  void currentUrlChanged(KUrl);
};

#endif
