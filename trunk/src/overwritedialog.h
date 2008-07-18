#ifndef OVERWRITEDIALOG_H
#define OVERWRITEDIALOG_H

#include <QProcess>
#include <QPixmap>
#include <QFileInfo>
#include <QAction>

#include <KDialog>
#include <KIcon>
#include <KUrl>
#include <KFileItem>
#include <KDateTime>
#include <KMenu>

#include <KDebug>

#include "ui_overwriteui.h"
#include "rar.h"

class overwriteDialog : public KDialog, private Ui::OverwriteDialog
{
  Q_OBJECT

public:
  overwriteDialog(QProcess* ,QWidget* parent = 0, Qt::WFlags fl = 0 );
  ~overwriteDialog();

  enum clickedButton { Yes, No, YesToAll, NoToAll, Cancel};

  bool isYes();

public slots:
  virtual void renameAction();
  virtual void yesOverwrite();
  virtual void noOverwrite();
  virtual void cancelOverwrite();
  virtual void renameOverwrite(QTableWidgetItem * item);
  virtual void setDestinationDetails(QString); //path, lastmodified time, size 
  virtual void setSourceDetails(QString, KDateTime, QString);
  virtual void disableTableEditing();

protected:

protected slots:
  virtual void setDestinationIcon(QString);
  //virtual void setDestinationInfo(); //name of the file and whatever you wish to show in the message
  virtual void setSourceIcon(QString);
  //virtual void setSourceInfo(QString);

private:
  bool yesToAll;
  bool noToAll;
  bool yes;

  QString destinationPath;
  QString destinationName;
  QString destinationFilePath;

  QTableWidgetItem *itemPath;

  QAction *yesApplyAll;
  QAction *noApplyAll;


};

#endif
