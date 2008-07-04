#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSplitter>
#include <QLabel>
#include <QGridLayout>
#include <QProcess>
#include <QDesktopServices>
#include <QDockWidget>

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
#include <KTemporaryFile>
#include <KCmdLineArgs>
#include <KProcess>

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
#include "akucomment.h"
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
  virtual void getMetaInfo(QTreeWidgetItem*);
  virtual void metaBar();
  virtual void viewInformation(bool);
  virtual void sumSelectedItemsSize();
  virtual void embeddedViewer();
  virtual void openItemUrl(QTreeWidgetItem *, int);
  virtual void extractArchive();
  virtual void operationCompleted(bool);
  virtual void insertComment(QString);
  virtual void setupPopupMenu();
  virtual void selectionInverted();
  virtual void renameItem();
  virtual void renameProcess(QTreeWidgetItem*, int);
  virtual void addFile(bool pwd = false);

private:
  QWidget *baseWindowWidget;
  QWidget *statusWidget;
  
  QSplitter *splitter;
  
  akuMainTable *table;
 
  akuSearchWidget *searchWidget;
  
  QLabel *infoArchive;
  QLabel *infoExtra;
  QLabel *infoExtrabis;
  QLabel *archiveInfo;  
 
  QAction *buttonExtract;
  QAction *buttonComment;

  KAction *buttonNew;
  KAction *buttonDelete;
  KAction *buttonOpen;
  KAction *buttonInfo;
  KAction *buttonFind;
  KAction *buttonExit;
  KAction *buttonView;
  KAction *buttonAddComment;
  KAction *buttonLock;
  KAction *buttonAddFile;
  
  KAction *separator;
  KAction *popSelectall;
  KAction *popInvertselection;
  KAction *popRename;
  
  QStringList tempForRename;

  QString oldItemName;
  QString oldItemPath;
  QString archivePassword;
  QString archive; // nome dell'archivio in uso
  QString compressor; 
  // stringa contenente il tipo di compressione
  // 1. rar 
  // 2. zip
  // 3. tar
 
  bool filespassprotected;

  KRecentFilesAction *recentFilesAction;

  akuTip *tip;

  rarProcess *rarprocess;
  zipProcess *zipprocess;
  tarProcess *tarprocess;

  akuRatioWidget *ratioBar;

  akuMetaWidget *metaWidget;

  QDockWidget *dockComment;

  KTextEdit *editComment;

  QTreeWidgetItem *currentToRename;

private slots:
  void quit();

protected:
  void setupSearchBar();
  //void cmdlineOptions();

protected slots:
  virtual void setupStatusBar();
  virtual void buildRarTable(QString, bool headercrypted = false);
  virtual void buildZipTable(QString, bool headercrypted = false);
  virtual void buildTarTable(QString);
  virtual void setupActions();
  virtual void setupDocks();
  virtual void handleAdvancedRar(QString, QString);
  virtual void handleAdvancedZip(QString);
  virtual void addComment();
  virtual void renameCompleted(bool);
  virtual void lockArchive();
  //virtual void test();
 
};

#endif
