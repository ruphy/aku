#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//AKU INCLUDES
#include "extractdialog.h"
#include "rar.h"
#include "akuviewer.h"
#include "akusearchline.h"
#include "rarprocesshandler.h"
#include "overwritedialog.h"
#include "akutooltip.h"
#include "akushadeanimation.h"
#include "akucompressionwidget.h"
#include "akuratiowidget.h"
#include "akuprogressdialog.h"
#include "dragsource.h"
#include "dragtarget.h"
#include "akumetawidget.h"
#include "akusearchwidget.h"
#include "akudragsourcewidget.h"
#include "akucomment.h"
#include "akuwaitdialog.h"
#include "akutreewidget.h"
#include "akupartviewer.h"
#include "akuerrordialog.h"
#include "akucrazytip.h"

//QT4 INCLUDES
#include <QStringList>
#include <QHeaderView>
#include <QLabel>
#include <QDockWidget>
#include <QDesktopServices>
#include <QSplitter>
#include <QLineEdit>
#include <QTreeWidget>
#include <QPoint>
#include <QFont>
#include <QVBoxLayout>

//KDE4 INCLUDES
#include <KApplication>
#include <KFileDialog>
#include <KAction>
#include <KLocale>
#include <KStandardAction>
#include <KStatusBar>
#include <KActionCollection>
#include <KRecentFilesAction>
#include <KIcon>
#include <KUrl>
#include <KXmlGuiWindow>
#include <KProcess>
#include <KConfig>
#include <KConfigGroup>
#include <KMessageBox>
#include <KTemporaryFile>
#include <KMenuBar>
#include <knewpassworddialog.h>
#include <KStandardDirs>
//#include "ui_mainak4u.h"

class MainWindow : public KXmlGuiWindow
{
 Q_OBJECT

public:

  MainWindow ( QWidget* parent = 0, Qt::WFlags fl = 0 );
  ~MainWindow();
 
  void raropen ( QString, bool restrictions = true );
  
public slots:

  virtual void openDialog();
  virtual void debugging();
  virtual void singleExtractInit();
  virtual void singleExtract();
  virtual void setPopUpMenus();
  virtual void setConnections();
  virtual void setSelectionInverted();
  virtual void setInformation ( bool );
  virtual void setFolderIcons();
  virtual void recursiveFolderIcons ( QTreeWidgetItem* );
  virtual void getMetaInfo ( QTreeWidgetItem* );
  virtual void newArchive();
  virtual void embeddedViewer();
  virtual void find ( bool );
  virtual void renameItem();
  virtual void rarRename ( QTreeWidgetItem*, int );
  virtual void rarDelete();
  virtual void expandTopLevelItems();
  virtual void sumSelectedItemsSize();
  virtual void setupForNew();
  virtual void closeRar();
  virtual void closeNewArchiveGUI(bool);
  virtual void openRecentFile(KUrl);
  virtual void openItemUrl(QTreeWidgetItem *, int );
  virtual void insertComment(QString);
  virtual void addFolder(bool pwd = false);
  virtual void addFile(bool pwd = false);
  virtual void reloadArchive(bool ok = true);
  virtual void enableActions(bool);
  virtual void cantDeleteMessage();
  virtual void closeDeletedArchive();
  
private:
  QByteArray newArchiveError;
  QTreeWidget *rarList;

  KAction *selectAll;
  KAction *invertSelection;
  KAction *debug;
  KAction *renameAction;
  akuSearchWidget *searchWidget;
  //rename vars----//
  QStringList tempForRename;
  QString oldItemName;
  QString oldItemPath;
  //---------------//

  //new Archive vars-----------------------//
  dragSource *sourceList;
  akuDragSourceWidget *sourceView;
  dragTarget *targetList;
  QWidget *widgetForList;
  QDockWidget *sourceDock;
  QProcess *newArch;
  QDockWidget *dockOption;
  int compressionLevel;
  QString password;
  akuCompressionWidget *compressionWidget;
  QStringList filesToAdd; //lista dei file per il nuovo archivio
  QString archive;
  bool fromNewArchive; //questa variabile ci dice semplicemente se il new archive è in corso o meno
  rarProcessHandler *newProcHandler;
  //------------------------------//
  //--recentFile section---//

  //-----------------------//
  //------toolBar and actions-------//
  KAction *toolNew;
  KAction *toolExtract;
  KAction *toolDelete;
  KAction *toolOpen;
  KAction *actionExit;
  KAction *toolView;
  KAction *separator;
  KAction *actionInfo;
  KAction *actionFind;
  KAction *actionLock;
  KAction *actionAddComment;
  KAction *actionAddFolder;
  KAction *actionAddFile;
  KAction *actionEncryptArchive;
  KAction *actionAddFilePwd;
  KAction *actionAddFolderPwd;
  //-------------//
  KRecentFilesAction *recents;
// QSplitter *splitter; 
  akuMetaWidget *metaWidget;
  QWidget *baseWindowWidget;
  QVBoxLayout *baseWindowLayout;
  
  QLabel *archiveInfo;
  QLabel *infoLock;
  QString globalArchivePassword;
  akuWaitDialog *waitDialog;
  QTreeWidgetItem *currentToRename;
  bool deleteAll;
  akuRatioWidget *ratioBar;
  QWidget *statusWidget;
  bool globalRestrictions;
  akuCrazyTip *tip;

protected:
  void setupSearchUI();
  QString rebuildPath ( QTreeWidgetItem* );
  QString rebuildFullPath(QTreeWidgetItem*);
  QStringList rebuildPathForNew ( dragTarget* );
  QStringList recursiveRebuildForNew ( QTreeWidgetItem* );
  void setupActions();
  void setRecentActions();

protected slots:
  virtual void lockArchive();
  virtual void addComment();
  virtual void handleRestrictions(QString, QString);
  virtual void setupStatusBar();
  virtual void setupCommentView();
  virtual void completeRename(bool);
  virtual void completeDelete(bool);
  virtual void encryptArchive();
  virtual void addFilePwd();
  virtual void addFolderPwd();
  virtual void checkRarExe();
  virtual void showStatusInfo(bool);
  virtual void parseAndShow(QString, bool crypted=false);
  virtual void closeAll(bool); //this is used after handling some cmdline options
};


#endif
