#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSplitter>
#include <QLabel>
#include <QGridLayout>
#include <QProcess>
#include <QDockWidget>
#include <QActionGroup>

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
#include <KRun>
#include <KStandardDirs>

#include <KDebug>

#include "akumaintable.h"
#include "akutip.h"
#include "akusearchwidget.h"
#include "akudragsourcewidget.h"
#include "rarprocess.h"
#include "zipprocess.h"
#include "tarprocess.h"
#include "rar.h"
#include "zip.h"
#include "tar.h"
#include "akumetawidget.h"
#include "akuviewer.h"
#include "akucomment.h"
#include "akuaddfiledialog.h"
#include "akucompressionwidget.h"
#include "extractdialog.h"
#include "quickextract.h"
#include "dragtarget.h"


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
  virtual void openUrl(KUrl);
  virtual void getMetaInfo(QTreeWidgetItem*);
  virtual void metaBar();
  virtual void viewInformation(bool);
  virtual void sumSelectedItemsSize();
  virtual void embeddedViewer();
  virtual void openItemUrl(QTreeWidgetItem *, int);
  virtual void extractArchive();
  virtual void operationCompleted(bool);
  virtual void extractionCompleted(bool);
  virtual void insertComment(QString);
  virtual void setupPopupMenu();
  virtual void selectionInverted();
  virtual void renameItem();
  virtual void renameProcess(QTreeWidgetItem*, int);
  virtual void addFile();
  virtual void addFileOperation(QStringList, QString);
  virtual void addDir();
  virtual void deleteFile();
  virtual void addDirOperation(KUrl);
  virtual void extractoToPreferred(QAction *action);
  virtual void collectTempFiles(QString);
  virtual void encryptArchive();
  virtual void cleanAku(QString);
  virtual void createNewArchive();
  virtual void closeNewArchive();

private:
  QWidget *baseWindowWidget;
  QWidget *statusWidget;
  QWidget *widgetForList;
  
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
  KAction *buttonAddDir;
  KAction *buttonEncrypt;
  KAction *separator;
  KAction *separator2;
  KAction *popSelectall;
  KAction *popInvertselection;
  KAction *popRename;

  QActionGroup *extractGroup;
  
  QStringList tempForRename;
  QStringList tmpFiles;

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
  bool deleteAll;

  KMimeType::Ptr mimetype;

  KRecentFilesAction *recentFilesAction;

  akuTip *tip;

  rarProcess *rarprocess;
  zipProcess *zipprocess;
  tarProcess *tarprocess;

  akuRatioWidget *ratioBar;

  akuMetaWidget *metaWidget;

  akuDragSourceWidget *sourceView;

  akuCompressionWidget *compressionWidget;

  dragTarget *targetList;

  QDockWidget *dockComment;
  QDockWidget *sourceDock;
  QDockWidget *dockOption;

  KTextEdit *editComment;

  QTreeWidgetItem *currentToRename;

private slots:
  void quit();

protected:
  void setupSearchBar();

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
  virtual void completeDelete(bool);
  //virtual void test();
 
};

#endif
