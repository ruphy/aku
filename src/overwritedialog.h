#ifndef OVERWRITEDIALOG_H
#define OVERWRITEDIALOG_H

#include <QProcess>
#include <QPixmap>
#include <KDialog>
#include <QFileInfo>
#include "ui_overwriteui.h"
#include <KIcon>
#include <KUrl>
#include "rar.h"
#include <KFileItem>
#include <KDateTime>

class overwriteDialog : public KDialog, private Ui::OverwriteDialog
{
  Q_OBJECT

public:
  overwriteDialog(QProcess* ,QWidget* parent = 0, Qt::WFlags fl = 0 );
  ~overwriteDialog();

  enum clickedButton { Yes, No, YesToAll, NoToAll, Cancel};
  bool isToAllChecked();
  bool yesToAllChecked();
  bool noToAllChecked();

public slots:
  virtual void renameAction();
  virtual void yesOverwrite();
  virtual void noOverwrite();
  virtual void cancelOverwrite();
  virtual void renameOverwrite();
  virtual void setDestinationDetails(QString); //path, lastmodified time, size 
  virtual void setSourceDetails(QString, KDateTime, QString);

protected:

protected slots:
  virtual void setDestinationIcon(QString);
  //virtual void setDestinationInfo(); //name of the file and whatever you wish to show in the message
  virtual void setSourceIcon(QString);
  //virtual void setSourceInfo(QString);

private:
  bool yesToAll;
  bool noToAll;
  QString destinationPath;
  QString destinationName;
  QString destinationFilePath;

};

#endif
