#ifndef EXTRACTDIALOG_H
#define EXTRACTDIALOG_H

#include <QModelIndex>
#include <QProgressDialog>
#include <QHeaderView>
#include <QInputDialog>
#include <QDir>

#include <KAction>
#include <KDialog>
#include <KLocale>
#include <KIcon>
#include <KGuiItem>
#include <KPushButton>
#include <KMessageBox>
#include <KUrlCompletion>
#include <KLineEdit>
#include <KRun>

#include "rarprocess.h"
#include "ui_extractdialog.h"

class extractDialog : public KDialog, private Ui::Dialog
{
  Q_OBJECT

public:
  extractDialog ( QString, QString, QList<QStringList> = QList<QStringList>(), QStringList = QStringList(), QWidget* parent = 0, Qt::WFlags fl = 0 );
  ~extractDialog();

public slots:
  virtual void completerBox();
  virtual void updateTreeViewSelection(QString);
  virtual void hiddenFiles(bool);
  virtual void extraction();
  virtual void closeDialog();
  virtual void defaultSettings();
  virtual void loadSettings();
  //virtual void saveSettings();
  virtual void updateCombo(KUrl);
  virtual void openDestinationPath(bool);
  virtual void deleteArchive(bool);
  virtual void deleteArchiveAsk(bool);
  virtual void createNewFolder();
  virtual void checkPath();

private:
  QWidget *parentWidget;
 
  QString compressor;
  QString archivename;

  QStringList options;
  QStringList files;
  QStringList tmphistory;

  KAction *showhiddenAction;

  rarProcess *rarprocess;

signals:
  void closeArchive();
  void cantDelete();
  void processDialog(bool);
  void processCompleted(bool);

};

#endif
