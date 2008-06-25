#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSplitter>
#include <QLabel>
#include <QGridLayout>
#include <QProcess>
#include <QDesktopServices>

#include <KApplication>
#include <KXmlGuiWindow>
#include <KVBox>
#include <KStatusBar>
#include <KActionCollection>
#include <KMenuBar>
#include <KFileDialog>
#include <KUrl>
#include <KRecentFilesAction>
#include <KMimeType>
#include <KGlobalSettings>
#include <KDebug>

#include "akumaintable.h"
#include "akutip.h"
#include "akusearchwidget.h"
#include "rarprocess.h"
#include "zipprocess.h"
#include "tarprocess.h"
#include "rar.h"
#include "zip.h"
#include "tar.h"
#include "akumetawidget.h"
#include "akuviewer.h"
#include "extractdialog.h"

class MainWindow : public KXmlGuiWindow
{
 Q_OBJECT

public:

  MainWindow (QWidget* parent = 0, Qt::WFlags flags = 0);
  ~MainWindow();

public slots:
  virtual void openDialog();
  virtual void setupConnections();
  virtual void enableActions(bool);
  virtual void openUrl(const KUrl& url);
  virtual void setFolderIcons();
  virtual void recursiveFolderIcons(QTreeWidgetItem*);
  virtual void getMetaInfo(QTreeWidgetItem*);
  virtual void metaBar();
  virtual void expandTopLevelItems();
  virtual void viewInformation(bool);
  virtual void sumSelectedItemsSize();
  virtual void embeddedViewer();
  virtual void openItemUrl(QTreeWidgetItem *, int );
  virtual void extractArchive();

private:
  QWidget *baseWindowWidget;
  QWidget *statusWidget;
  
  QSplitter *splitter;
  
  akuMainTable *table;
 
  akuSearchWidget *searchWidget;
  
  QLabel *infoArchive;
  QLabel *infoExtra;
  QLabel *archiveInfo;  
 
  QAction *buttonExtract;

  KAction *buttonNew;
  KAction *buttonDelete;
  KAction *buttonOpen;
  KAction *buttonInfo;
  KAction *buttonFind;
  KAction *buttonExit;
  KAction *buttonView;
  KAction *separator;

  QString archivePassword;

  KRecentFilesAction *recentFilesAction;

  akuTip *tip;

  rarProcess *rarprocess;
  zipProcess *zipprocess;
  tarProcess *tarprocess;

  akuRatioWidget *ratioBar;

  akuMetaWidget *metaWidget;

private slots:
  void quit();

protected:
  QString rebuildPath(QTreeWidgetItem*);
  QString rebuildFullPath(QTreeWidgetItem*);

  void setupSearchBar();

protected slots:
  virtual void setupStatusBar();
  virtual void buildRarTable(QString, bool headercrypted = false);
  virtual void buildZipTable(QString, bool headercrypted = false);
  virtual void buildTarTable(QString);
  virtual void setupActions();
  virtual void handleAdvancedRar(QString, QString, bool);

};

#endif
