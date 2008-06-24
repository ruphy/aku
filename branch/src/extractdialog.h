
#ifndef EXTRACTDIALOG_H
#define EXTRACTDIALOG_H

#include <QModelIndex>
#include <QDesktopServices>
#include <KAction>
#include <KDialog>
#include <KLocale>
#include <KIcon>
#include <KGuiItem>
#include <KPushButton>
#include <KMessageBox>
#include <QProgressDialog>
#include "rarprocesshandler.h"
#include "ui_extracto.h"
#include <QHeaderView>
#include <KUrlCompletion>
#include <QInputDialog>
#include <QDir>
#include <KLineEdit>

class extractDialog : public KDialog, private Ui::Dialog
{
  Q_OBJECT

public:
  extractDialog ( QString, QString, QStringList = QStringList ( ), QStringList = QStringList(), QWidget* parent = 0, Qt::WFlags fl = 0 );
  ~extractDialog();

public slots:
  virtual void completerBox();
  virtual void updateTreeViewSelection(QString);
  virtual void hiddenFiles(bool);
  virtual void extraction();
  virtual void closeDialog();
  virtual void defaultSettings();
  virtual void loadSettings();
  virtual void saveSettings();
  virtual void updateCombo(KUrl);
  virtual void openDestinationPath(bool);
  virtual void deleteArchive(bool);
  virtual void deleteArchiveAsk(bool);
  virtual void createNewFolder();
  virtual void checkPath();

private:
  QWidget *parentWidget;
  QString globalArchiver;

signals:
  void closeArchive();
  void cantDelete();

};

#endif
