#include "mainwindow.h"
#include <KCmdLineArgs>

QString archiver = "rar";
QString bf; //buffer
QString rarout; //rar output
QString namex; //variabile globale, contiene il path dell'archivio corrente
QProgressDialog *pdial;
QProcess *proc;

QAction *commentAction = 0;
QDockWidget *commentDock = 0;
QTextEdit *commentEdit = 0;
QSplitter *splitter;

enum { MaxRecentFiles = 5 };

MainWindow::MainWindow ( QWidget* parent, Qt::WFlags fl ): KXmlGuiWindow ( parent, fl )
{
  baseWindowWidget = new QWidget(this);
  baseWindowLayout = new QGridLayout(baseWindowWidget);
  splitter = new QSplitter(this);
  
  rarList = new akuTreeWidget(splitter);
  splitter -> addWidget(rarList);

  //adding metaWidget to splitter
  metaWidget = new akuMetaWidget(this);
  splitter -> addWidget(metaWidget);
  metaWidget -> setVisible(false);

  baseWindowLayout -> addWidget(splitter,1,1);
  setCentralWidget(baseWindowWidget);
 
  setHelpMenuEnabled(false);
  setupCommentView();
  setupSearchUI(); //generiamo la searchWidget
  setupActions();
  commentDock -> setVisible(false); //brutto
  commentAction -> setVisible(false); //brutto
  menuBar() -> addMenu(helpMenu(QString(), false));
  setPopUpMenus();
  setConnections();
  setupStatusBar();
  
  toolDelete -> setEnabled ( false );
  rarList -> setSortingEnabled ( false );

  fromNewArchive = false;
  setRecentActions();
  enableActions(false);
  toolDelete -> setEnabled (false );
  renameAction -> setEnabled ( false );
  waitDialog = new akuWaitDialog(this);
  showStatusInfo(false);
  puts("parsing");
  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
  if(args->isSet("extracthere")){
    // code to extract the archive
    setVisible(false);
    QDir herepath(args->arg(0));
    KUrl url = herepath.absolutePath();
    rarProcessHandler *pHand = new rarProcessHandler(this, archiver, QStringList()<<"x",args->arg(0), QStringList(), url.directory() );
    connect(pHand, SIGNAL(processCompleted(bool)), this, SLOT(closeAll(bool)));
    pHand->start();
  }
  else if(args->isSet("extractto")){
    // code to extract the archive
    setVisible(false);
    KUrl url = KFileDialog::getExistingDirectoryUrl(KUrl(QDir().homePath()),  this, i18n("Extract to"));
    puts(url.pathOrUrl().toAscii());
    if(!url.isEmpty()){
     rarProcessHandler *pHand = new rarProcessHandler(this, archiver, QStringList()<<"x",args->arg(0), QStringList(), url.path() );
     connect(pHand, SIGNAL(processCompleted(bool)), this, SLOT(closeAll(bool)));
     pHand->start();
    } else{
     puts("uscendo");
     kapp->quit(); //FIXME does not work!
     }
  }
  else{
    for(int i=0; i < args -> count(); i++)  raropen(args -> arg(i));     
   }
  puts("created");
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeAll(bool)
{
 kapp->quit();
}
void MainWindow::checkRarExe()
{
 QString check = KStandardDirs::findExe("rar");
 if(check.isEmpty())
 {
   QString secondCheck = KStandardDirs::findExe("unrar");
   if(!secondCheck.isEmpty())
   {
     archiver = "unrar";
     enableActions(false);
     toolView -> setEnabled(true);
     toolExtract -> setEnabled(true);
     actionInfo -> setEnabled(true);
     toolNew -> setEnabled(false);
     toolNew -> setToolTip(i18n("rar executable not found, archive creation is disabled"));
   }
   else
   {
     enableActions(false);
     toolNew -> setEnabled(false);
     KMessageBox::information(this, i18n("Rar or Unrar not found. Any of the useful functions of aKu are enabled"), i18n("Rar or Unrar not found"));
   }
  }
  else archiver = "rar";
}

void MainWindow::enableActions(bool enable)
{
  toolView -> setEnabled (enable);
  toolExtract -> setEnabled(enable);
  actionInfo -> setEnabled ( enable );
  if(enable == true && archiver == "unrar") enable = false;
  actionAddFolder -> setEnabled( enable );
  actionAddComment -> setEnabled(enable);
  actionLock -> setEnabled(enable );
  actionFind -> setEnabled(enable);
  actionAddFile -> setEnabled(enable);
  actionEncryptArchive -> setEnabled(enable);
  actionAddFilePwd -> setEnabled(enable);
  actionAddFolderPwd -> setEnabled(enable);
}

void MainWindow::setupStatusBar()
{
  archiveInfo = new QLabel(this);  
  statusBar() -> addPermanentWidget(archiveInfo, 0);
  infoLock = new QLabel(this);
  QFont font = archiveInfo -> font();
  font.setPointSize(9);
  archiveInfo -> setFont(font);
  infoLock -> setMaximumSize(22,18);
  ratioBar = new akuRatioWidget(0,this);
  ratioBar -> setMinimumSize(120,0);
  statusWidget = new QWidget(this);
  QGridLayout *statusLayout = new QGridLayout(statusWidget);
  statusLayout -> addWidget(archiveInfo,1,1);
  statusLayout -> addWidget(ratioBar,1,2);
  statusLayout -> addWidget(infoLock,1,3);
  statusBar() -> addPermanentWidget(statusWidget);
}

void MainWindow::setupSearchUI()
{
  searchWidget = new akuSearchWidget(rarList, this);
  searchWidget -> setMaximumSize(655555, 35);
  baseWindowLayout->addWidget( searchWidget, 2, 1 );
  searchWidget -> setVisible ( false );	
  actionFind = searchWidget -> getCloseAction();
}

void MainWindow::showStatusInfo(bool show)
{
  statusWidget -> setVisible(show);
}

void MainWindow::setPopUpMenus()
{
  selectAll = new KAction ( i18n( "Select All" ), rarList );
  selectAll -> setIcon(KIcon("edit-select-all"));
  rarList -> addAction ( selectAll );
  rarList -> setContextMenuPolicy ( Qt::ActionsContextMenu );
  invertSelection = new KAction ( i18n( "Invert Selection" ), rarList );
  rarList -> addAction ( invertSelection );
  QAction *separator = new QAction ( rarList );
  separator -> setSeparator ( true );
  rarList -> addAction ( separator );
  renameAction = new KAction ( i18n( "&Rename" ), rarList );
  renameAction -> setIcon(KIcon("edit-rename"));
  rarList -> addAction ( renameAction );
  renameAction -> setShortcut ( QKeySequence ( "F2" ) );
  toolDelete -> setShortcut ( QKeySequence ( "Del" ) );
  rarList -> addAction ( toolDelete );
  rarList -> addAction( toolView );
  KAction *separator2 =new KAction(this);
  separator2 -> setSeparator(true);
  rarList -> addAction(separator2);
  rarList -> addAction(actionAddFile);
  rarList -> addAction(actionAddFolder);
}


void MainWindow::setConnections()
{
  connect ( rarList, SIGNAL ( itemSelectionChanged() ), this, SLOT ( singleExtractInit() ) );
  connect ( toolExtract, SIGNAL ( triggered() ), this, SLOT ( singleExtract() ) );
  connect ( selectAll, SIGNAL ( triggered() ), rarList, SLOT ( selectAll() ) );
  connect ( invertSelection, SIGNAL ( triggered() ), this, SLOT ( setSelectionInverted() ) );
  connect ( actionInfo, SIGNAL ( toggled ( bool ) ), this, SLOT ( setInformation ( bool ) ) );
  connect ( toolView, SIGNAL ( triggered() ), this, SLOT ( embeddedViewer() ) );
  connect ( renameAction, SIGNAL ( triggered() ), this, SLOT ( renameItem() ) );
  connect ( toolDelete, SIGNAL ( triggered() ), this, SLOT ( rarDelete() ) );
  connect ( rarList, SIGNAL(itemDoubleClicked ( QTreeWidgetItem *, int )), this, SLOT(openItemUrl(QTreeWidgetItem *, int)));
  connect ( actionLock, SIGNAL ( triggered() ), this, SLOT ( lockArchive() ) );
  connect ( actionAddComment, SIGNAL(triggered()), this, SLOT(addComment()));
  connect ( actionAddFolder, SIGNAL(triggered()), this, SLOT(addFolder()));
  connect ( actionAddFile, SIGNAL(triggered()), this, SLOT(addFile()));
  connect ( actionEncryptArchive, SIGNAL(triggered()), this, SLOT(encryptArchive()));
  connect ( actionAddFilePwd, SIGNAL(triggered()), this, SLOT(addFilePwd()));
  connect ( actionAddFolderPwd, SIGNAL(triggered()), this, SLOT(addFolderPwd()));
}

void MainWindow::debugging()
{
  //akuPartViewer *viewer = new akuPartViewer(this);
  //viewer->view("~/Development/aku2/src/mainwindow.cpp");
}

void MainWindow::addFilePwd()
{
  addFile(true);
}

void MainWindow::addFolderPwd()
{
  addFolder(true);
}

void MainWindow::setRecentActions()
{
  KConfig config;
  config.sync();
  QStringList actionList = KConfigGroup(&config ,"KFileDialog Settings").readEntry("Recent Files" , QStringList());
  if ( !actionList.isEmpty())
  {
    recents -> clear(); //clear actions
    for ( int i = 0; i < actionList.size(); i++ )  recents -> addUrl(KUrl(actionList[i])); //and set new ones
  }
}


void MainWindow::addFolder( bool pwd)
{ 
  KUrl folderUrl;
  if(rarList -> selectedItems().size() == 1)
  {
    if( rarList -> selectedItems().first() -> text(1) == "")
    {
      if(rarList -> selectedItems().first() -> parent() !=NULL)
        folderUrl = KFileDialog::getExistingDirectoryUrl(KUrl(QDir().homePath()),  this, i18n("Add folder under ")+ rarList -> selectedItems().first() -> text(0));
      else
        folderUrl = KFileDialog::getExistingDirectoryUrl(KUrl(QDir().homePath()),  this, i18n("Add folder"));
    }
    else
      if( rarList -> selectedItems().first()->parent() != NULL)
        folderUrl = KFileDialog::getExistingDirectoryUrl(KUrl(QDir().homePath()),  this, i18n("Add folder under ")+ rarList -> selectedItems().first()->parent() -> text(0));
      else
        folderUrl = KFileDialog::getExistingDirectoryUrl(KUrl(QDir().homePath()),  this, i18n("Add folder"));
  }
  else
    folderUrl = KFileDialog::getExistingDirectoryUrl(KUrl(QDir().homePath()),  this, i18n("Add folder"));


  if(!folderUrl.isEmpty())
  {
    //let's check if the user asked for password
    QString pwdToSet;
    if(pwd)
    {
      KNewPasswordDialog pwDialog(this);
      pwDialog.setPrompt(i18n("Enter a password"));
      if(pwDialog.exec()) pwdToSet = pwDialog.password();
    }

    rarProcessHandler *addingProc;
    if(rarList -> selectedItems().size() == 1 )
    {
      QString parentFolder;
      if(rarList -> selectedItems()[0] -> text(1) == "")
      {
        parentFolder = rebuildFullPath(rarList -> selectedItems()[0]);
        if(pwdToSet.isEmpty()) 
          addingProc = new rarProcessHandler(this, "rar", QStringList() << "a"<<"-ep1"<<"-ap"+parentFolder,namex, QStringList()<<folderUrl.pathOrUrl());
        else
          addingProc = new rarProcessHandler(this, "rar", QStringList() << "a"<<"-ep1"<<"-p"+pwdToSet<<"-ap"+parentFolder,namex, QStringList()<<folderUrl.pathOrUrl());
        connect(addingProc, SIGNAL(processCompleted(bool)), this, SLOT(reloadArchive(bool)));
      }
    else
    {
      if ( rarList -> selectedItems()[0] -> parent() != NULL)
      {
        parentFolder = rebuildFullPath(rarList -> selectedItems()[0] -> parent());
        if(pwdToSet.isEmpty())
          addingProc = new rarProcessHandler(this, "rar", QStringList() << "a"<<"-ep1"<<"-ap"+parentFolder,namex, QStringList()<<folderUrl.pathOrUrl());
        else
          addingProc = new rarProcessHandler(this, "rar", QStringList() << "a"<<"-ep1"<<"-p"+pwdToSet<<"-ap"+parentFolder,namex, QStringList()<<folderUrl.pathOrUrl());
        connect(addingProc, SIGNAL(processCompleted(bool)), this, SLOT(reloadArchive(bool)));
      }
      else
      {
        if(pwdToSet.isEmpty())
          addingProc = new rarProcessHandler(this, "rar", QStringList() << "a"<<"-ep1",namex, QStringList()<<folderUrl.pathOrUrl());
        else
          addingProc = new rarProcessHandler(this, "rar", QStringList() << "a"<<"-ep1"<<"-p"+pwdToSet,namex, QStringList()<<folderUrl.pathOrUrl());
        connect(addingProc, SIGNAL(processCompleted(bool)), this, SLOT(reloadArchive(bool)));
      }
    }

    }
    else
    {
      if(pwdToSet.isEmpty())
        addingProc = new rarProcessHandler(this, "rar", QStringList() << "a"<<"-ep1",namex, QStringList()<<folderUrl.pathOrUrl());
      else 
        addingProc = new rarProcessHandler(this, "rar", QStringList() << "a"<<"-ep1"<<"-p"+pwdToSet,namex, QStringList()<<folderUrl.pathOrUrl());
      connect(addingProc, SIGNAL(processCompleted(bool)), this, SLOT(reloadArchive(bool)));
    }
    addingProc -> start();
  }
}

void MainWindow::addFile(bool pwd)
{
  KUrl::List fileUrlList;
  if(rarList -> selectedItems().size() == 1)
  {
    if( rarList -> selectedItems().first() -> text(1) == "")
    {
      if(rarList -> selectedItems().first() -> parent() != NULL)
        fileUrlList = KFileDialog::getOpenUrls(KUrl(QDir().homePath()), QString(), this, i18n("Add file under ")+ rarList -> selectedItems().first() -> text(0));
      else
        fileUrlList = KFileDialog::getOpenUrls(KUrl(QDir().homePath()), QString(), this, i18n("Add file"));
    }
  else
    if( rarList -> selectedItems().first()->parent() != NULL)
      fileUrlList = KFileDialog::getOpenUrls(KUrl(QDir().homePath()), QString(), this, i18n("Add file under ")+ rarList -> selectedItems().first()->parent() -> text(0));
    else fileUrlList = KFileDialog::getOpenUrls(KUrl(QDir().homePath()), QString(), this, i18n("Add file"));
  }
  else
    fileUrlList = KFileDialog::getOpenUrls(KUrl(QDir().homePath()), QString(), this, i18n("Add file"));
 
  QStringList fileList;
  for(int i = 0; i < fileUrlList.size(); i++) fileList << fileUrlList[i].pathOrUrl();
  if(!fileList.isEmpty())
  {
    //let's check if the user asked for password
    QString pwdToSet;
    if(pwd)
    {
      KNewPasswordDialog pwDialog(this);
      pwDialog.setPrompt(i18n("Enter a password"));
      if (pwDialog.exec()) pwdToSet = pwDialog.password();
    }
    rarProcessHandler *addingProc;
    if(rarList -> selectedItems().size() == 1 )
    {
      QString parentFolder;
      if(rarList -> selectedItems()[0] -> text(1) == "")
      {
        parentFolder = rebuildFullPath(rarList -> selectedItems()[0]);
        if(pwdToSet.isEmpty())
          addingProc = new rarProcessHandler(this, "rar", QStringList() << "a"<<"-ep1"<<"-ap"+parentFolder,namex, QStringList()<<fileList);
        else 
          addingProc = new rarProcessHandler(this, "rar", QStringList() << "a"<<"-ep1"<<"-p"+pwdToSet<<"-ap"+parentFolder,namex, QStringList()<<fileList);
        connect(addingProc, SIGNAL(processCompleted(bool)), this, SLOT(reloadArchive(bool)));
      }
      else
      {
        if ( rarList -> selectedItems()[0] -> parent() != NULL)
        {
          parentFolder = rebuildFullPath(rarList -> selectedItems()[0] -> parent());
          if(pwdToSet.isEmpty())
            addingProc = new rarProcessHandler(this, "rar", QStringList() << "a"<<"-ep1"<<"-ap"+parentFolder,namex, QStringList()<<fileList);
          else 
            addingProc = new rarProcessHandler(this, "rar", QStringList() << "a"<<"-ep1"<<"-p"+pwdToSet<<"-ap"+parentFolder,namex, QStringList()<<fileList);
          connect(addingProc, SIGNAL(processCompleted(bool)), this, SLOT(reloadArchive(bool)));
        }
        else
        {
          if(pwdToSet.isEmpty())
            addingProc = new rarProcessHandler(this, "rar", QStringList() << "a"<<"-ep1",namex, QStringList()<<fileList);
          else 
            addingProc = new rarProcessHandler(this, "rar", QStringList() << "a"<<"-ep1"<<"-p"+pwdToSet,namex, QStringList()<<fileList);
          connect(addingProc, SIGNAL(processCompleted(bool)), this, SLOT(reloadArchive(bool)));
        }
      }
  
    }
    else
    {
      if(pwdToSet.isEmpty())
        addingProc = new rarProcessHandler(this, "rar", QStringList() << "a"<<"-ep1",namex, QStringList()<<fileList);
      else 
        addingProc = new rarProcessHandler(this, "rar", QStringList() << "a"<<"-ep1"<<"-p"+pwdToSet,namex, QStringList()<<fileList);
      connect(addingProc, SIGNAL(processCompleted(bool)), this, SLOT(reloadArchive(bool)));
    }
    addingProc -> start();
  }
}

void MainWindow::reloadArchive(bool correctly)
{
  if (correctly)
  {
    if(!namex.isEmpty()) raropen(namex, false);
    else if(fromNewArchive) closeNewArchiveGUI(true);
  }
}

void MainWindow::openRecentFile(KUrl url)
{
  raropen(url.pathOrUrl());
}

void MainWindow::openDialog()
{
  QString fname = KFileDialog::getOpenFileName(KUrl(QDir().homePath()), QString("RAR Archive *.rar"), this, i18n("Open rar archive"));
  if ( !fname.isEmpty() )  raropen ( fname );
  setRecentActions();
}

void MainWindow::raropen ( QString filename, bool restrictions )
{
  globalRestrictions = restrictions;
  searchWidget -> searchLineEdit() ->clear();
  bf = "";
  namex = filename; //rendo globale il nome dell'archivio
  
  newProcHandler = new rarProcessHandler(this, archiver, QStringList() << "v" , namex );
  connect(newProcHandler, SIGNAL(outputReady(QString, bool)), this, SLOT(parseAndShow(QString, bool)));

  newProcHandler -> start();
  
}

void MainWindow::parseAndShow(QString rarout, bool crypted)
{
  disconnect(newProcHandler, SIGNAL(outputReady(QString, bool)), this, SLOT(parseAndShow(QString, bool)));
  if(crypted)
    globalArchivePassword = newProcHandler -> getArchivePassword();
  else 
    globalArchivePassword.clear();

  if(!rarout.isEmpty())
  { 
    setCursor(Qt::WaitCursor);
    rarList -> clear(); //ripulisco la lista
    //inizio il processo per il parsing dell'output
    rar nrar;
    nrar.parse ( rarList, rarout, ratioBar ); //metaWidget -> ratioBar() 
    QStringList archiDetails = nrar.getArchiveDetails();
    showStatusInfo(true);
    archiveInfo -> setText(archiDetails[0] + " " + i18n("file(s)") + "  " + i18n("Size: ") + archiDetails[1] + "  " + i18n("Packed: ") + archiDetails[2] + "  ");
    metaWidget ->clear();
    QPixmap mimeIcon = KIcon("application-x-rar").pixmap(128,128);
    metaWidget -> setMimeIcon( mimeIcon );
    //mi occupo di settare tutte le opzioni che riguardano l'interfaccia
    enableActions(true);
    this -> setWindowTitle ( i18n( "aKu - " ) + KUrl(namex).pathOrUrl() );
    setFolderIcons();
    rarList -> setSortingEnabled ( true );
   // rarList -> sortItems ( 1, Qt::AscendingOrder );
    rarList -> header() -> setResizeMode ( 0, QHeaderView::ResizeToContents );
    rarList -> header() -> setResizeMode(9, QHeaderView::ResizeToContents);
    if(globalRestrictions) handleRestrictions(namex, rarout);
    if ( fromNewArchive == true )   //ripristiniamo la gui se proveniamo da un new archive
    {
      closeNewArchiveGUI(false);
      fromNewArchive = false;
    }
    rarList -> resizeColumnToContents(10);
    showStatusInfo(true);
    setCursor(QCursor());
  }
}
void MainWindow::handleRestrictions(QString nomeFile, QString archList)
{ 
  // here we parse rar listing to find out comments and lock info
  // to know whether we have comments or not we can simply use the QString
  // received from rarProcessHandler::outputReady()
  // But to know if lock is present we need to call rar with vt params.
  // rarProcessHandler is asyncronous and heavier than QProcess for this simple operation
  // even if it handles better rar errors. However QProcess seems better for this fast call
  // TODO: check if really QProcess is enough!

  QProcess *p = new QProcess();
  if(!globalArchivePassword.isEmpty())
   p->start(archiver, QStringList() << "vt" << "-p"+globalArchivePassword << nomeFile);
  else
   p->start(archiver, QStringList() << "vt" << nomeFile);
  p->waitForFinished();
  
  QString lockcheck = p->readAllStandardOutput();
  delete p;
  if (lockcheck.contains("Lock is present\n"))
  {
    lockcheck.clear();
    QString toolTip(i18n("<b><font color=red>Locked archive</u></b></font><p><i>Any attempt to change the archive will be ignored</i>"));
    QPixmap lockIcon = KIcon("object-locked").pixmap(22,18);
    infoLock -> setPixmap(lockIcon);
    infoLock -> setToolTip(toolTip); 
  }
  else
  {
    infoLock -> clear();
  }

 if (archList.contains("Comment:"))//se troviamo la stringa Comment: vuol dire che c'è un commento, quindi lo estraiamo
  {
    int target = archList.indexOf("Comment:");
    archList.remove(0, target);
    archList.remove("Comment:");
    archList.remove(archList.indexOf("Pathname"), archList.length());

    commentEdit -> setText(archList);
    commentDock -> setVisible(true);
    commentAction -> setVisible(true);
    commentDock -> setGeometry(x() - 200, y(), 200,200);
  }
  else
  {
    commentDock -> setVisible(false);
    commentAction -> setVisible(false);
  }
  if(!globalArchivePassword.isEmpty())
  {
    infoLock -> setPixmap(KIcon("dialog-password").pixmap(22,18));
    infoLock -> setToolTip(i18n("This archive has a header password protection.<br>File data," 
                                "file names, sizes, attributes, comments are encrypted.<br> Without a password it is"
                                "impossible to view even the list of files in archive.")); 
  }
  if(infoLock ->pixmap()==NULL)
  {
    infoLock ->setPixmap(KIcon("dialog-ok-apply").pixmap(22,18));
    QString toolTip(i18n("This archive has no global restrictions"));
    infoLock -> setToolTip(toolTip);
  }
}

void MainWindow::setupCommentView()
{
  commentDock = new QDockWidget(this);
  commentDock -> setObjectName("commentDock");
  QLabel *commentTitle = new QLabel(i18n("Comment"),commentDock);
  commentTitle -> setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  QWidget *baseComment = new QWidget(commentDock);
  commentEdit = new QTextEdit(baseComment);
  QGridLayout *commentLayout = new QGridLayout(baseComment);
  commentLayout -> addWidget(commentEdit, 1, 1);
  commentDock -> setWidget(baseComment);
  this -> addDockWidget ( Qt::LeftDockWidgetArea, commentDock );
    
  commentEdit -> setReadOnly(true);
  commentDock -> setFloating(true);
  commentDock -> setVisible(false);
  commentAction = commentDock -> toggleViewAction();
  commentAction -> setVisible(false);
  commentAction -> setEnabled(true);
  commentAction -> setText(i18n("Show Comment"));
  actionCollection() -> addAction("comment", commentAction);
}

void MainWindow::insertComment(QString newcomment)
{ 
  KTemporaryFile temptxt;
  if (temptxt.open())
  {
    temptxt.write(newcomment.toUtf8());
    temptxt.waitForBytesWritten(-1);
    temptxt.flush();
    rarProcessHandler process(this, "rar", QStringList() << "c" << "-z" + temptxt.fileName(), namex);
    process.start();
  }
  temptxt.close();
  raropen(namex); 
}

void MainWindow::addComment()
{ 
  akuComment *testdialog = new akuComment(this);
  connect (testdialog, SIGNAL (okClicked(QString)), this, SLOT(insertComment(QString)));
  testdialog -> exec();
}

void MainWindow::lockArchive()
{
  // 5 = Continue
  // 2 = Cancel
  if (KMessageBox::warningContinueCancel(this, i18n("Locking disables archive modifications"), i18n("Lock archive")) == 5)
  {
    rarProcessHandler *proce;
    if(!globalArchivePassword.isEmpty())
      proce = new rarProcessHandler(this, archiver, QStringList() << "k"<< "-p"+globalArchivePassword, namex);
    else
      proce = new rarProcessHandler(this, archiver, QStringList() << "k", namex);
    proce -> start();
    raropen(namex);
  }
}

void MainWindow::encryptArchive()
{
  KNewPasswordDialog pwDlg(this);
  pwDlg.setPrompt(i18n("Enter a password. NOTE: this password will encrypt both header and files"));
  if(pwDlg.exec())
  {
    if(!pwDlg.password().isEmpty())
    {
      rarProcessHandler *encProc = new rarProcessHandler(this, archiver, QStringList() << "ch" <<"-hp"+pwDlg.password(), namex);
      connect(encProc, SIGNAL(processCompleted(bool)), this, SLOT(reloadArchive(bool)));
      encProc -> start();
    }
  }
}

void MainWindow::singleExtractInit()
{
  QList<QTreeWidgetItem*> checkUnselected = rarList -> selectedItems();
  if ( checkUnselected.size() != 0 )
  {
    if(archiver == "rar")
    {
      renameAction -> setEnabled ( true );
      toolDelete -> setEnabled ( true );
      toolDelete -> setEnabled ( true );
    }
    if ( checkUnselected.size() == 1 )
    {
      getMetaInfo ( checkUnselected[0] );
      if(checkUnselected[0] -> text(9).contains("image") && metaWidget -> isVisible() && checkUnselected[0] -> icon(10).isNull())
      {
      //if the file is an image we make a preview
        QString itemPath = rebuildFullPath(checkUnselected[0]);
        QProcess toPreview;
        toPreview.start(archiver, QStringList()<<"p"<<"-inul"<<namex<<itemPath);
        toPreview.waitForFinished();
        toPreview.terminate();
        QByteArray preview = toPreview.readAllStandardOutput();
        metaWidget -> setPreview(preview);
      }
    }
    else
    {
      if ( checkUnselected.size() == 0 ) metaWidget -> setFileName ( i18n( "No items selected" ) );
      else
      {
        int folders=0;
        int files = 0;
        for(int i = 0; i < checkUnselected.size(); i++)
        {
          if(checkUnselected[i] -> text(1).isEmpty()) folders ++;
          else files++;
        }
        metaWidget -> setFileName ( QString().setNum ( folders ) +i18n( " folder(s)\n" ) + QString().setNum( files) + i18n(" file(s)"));
        metaWidget -> handleItemSelections(checkUnselected); //testing multiple files selections
      }
      sumSelectedItemsSize();
    }
  }
  else
  {
    renameAction -> setEnabled ( false );
    toolDelete -> setEnabled ( false );
    toolDelete -> setEnabled ( false );
  }
}

void MainWindow::singleExtract()
{
  QStringList pathToExtract;

  QList<QTreeWidgetItem*> selectedToExtract = rarList -> selectedItems();
  QStringList listToExtract;

  if(selectedToExtract.size() != 0)
  {
    for ( int i=0; i < selectedToExtract.size(); i++ )
    {
      QTreeWidgetItem * tmp;
      QStringList pathlist; // file da estrarre dall'archivio
      //pathlist << toextract -> text(0);
      pathlist << ( selectedToExtract[i] ) -> text ( 0 );
      //tmp = toextract -> parent();
      tmp = ( selectedToExtract[i] ) -> parent();
      while ( tmp != NULL )
      {
        pathlist << tmp -> text ( 0 );
        tmp = tmp -> parent();
      }
      QString path;
      for ( int i = pathlist.size()-1; i>=0; i-- )
      {
        path.append ( pathlist[i] );
        if ( i!=0 ) path.append ( QDir().separator() );
      }
      pathToExtract << path;
      //QMessageBox::information(this, "Debug", path);
    }
    extractDialog *exD;
    if(globalArchivePassword.isEmpty()) 
    {
      exD = new extractDialog ( archiver, namex, pathToExtract,QStringList(), this );
      connect (exD, SIGNAL(cantDelete()), this, SLOT(cantDeleteMessage()));
      connect (exD, SIGNAL(closeArchive()), this, SLOT(closeDeletedArchive()));
    }
    else
    {
      exD = new extractDialog ( archiver, namex, pathToExtract,QStringList()<<"-p"+globalArchivePassword, this );
      connect (exD, SIGNAL(cantDelete()), this, SLOT(cantDeleteMessage()));
      connect (exD, SIGNAL(closeArchive()), this, SLOT(closeDeletedArchive()));
    }
  }
  else
  {
    extractDialog *exD;
    if(globalArchivePassword.isEmpty()) 
    {
      exD = new extractDialog (archiver,  namex,QStringList(), QStringList(), this );
      connect (exD, SIGNAL(cantDelete()), this, SLOT(cantDeleteMessage()));
      connect (exD, SIGNAL(closeArchive()), this, SLOT(closeDeletedArchive()));
    }
    else
    {
      exD = new extractDialog (archiver,  namex,QStringList(), QStringList() << "-p"+globalArchivePassword, this );
      connect (exD, SIGNAL(cantDelete()), this, SLOT(cantDeleteMessage()));
      connect (exD, SIGNAL(closeArchive()), this, SLOT(closeDeletedArchive()));
    }
  }
}

void MainWindow::setSelectionInverted()
{
  QList<QTreeWidgetItem*> itemsToUnselect = rarList -> selectedItems();
  rarList -> selectAll();
  for ( int u = 0; u < itemsToUnselect.size(); u++ )
  {
    rarList -> setItemSelected ( itemsToUnselect[u], false );
    QTreeWidgetItem *tmp = ( itemsToUnselect[u] )->parent();
    while ( tmp != NULL )   //questo while evita di selezionare i padri degli elementi da lasciare unselected
    {
      rarList -> setItemSelected ( tmp, false );
      tmp = tmp -> parent();
    }
  }
}

void MainWindow::setInformation ( bool visible )
{
  metaWidget -> setVisible ( visible );
  if(visible) singleExtractInit();
}

void MainWindow::setFolderIcons()
{
  for ( int i = 0; i < rarList -> topLevelItemCount(); i++ )
    if ( ( rarList -> topLevelItem ( i ) ) -> text ( 2 ).isEmpty())
    {
      ( rarList -> topLevelItem ( i ) ) -> setIcon ( 0,KIcon ( "inode-directory" ) );
      recursiveFolderIcons ( rarList -> topLevelItem ( i ) );
    }

}

void MainWindow::recursiveFolderIcons ( QTreeWidgetItem *checkParent )
{
  for ( int i = 0; i < checkParent -> childCount(); i++ )
    if ( ( checkParent -> child ( i ) ) -> text ( 2 ).isEmpty() )
    {
      ( checkParent -> child ( i ) ) -> setIcon ( 0, KIcon ( "inode-directory" ) );
      recursiveFolderIcons ( checkParent -> child ( i ) );
    }
  
}

void MainWindow::getMetaInfo ( QTreeWidgetItem *item ) //setta le informazioni della metabar
{
  if(!item -> text(1).isEmpty()){
    metaWidget -> setFileName ( item -> text ( 0 ) );
    metaWidget -> setFileSize ( item -> text ( 1 ) );
    metaWidget->setRatio(item->text(3).remove("%").toFloat());
  }else
   metaWidget -> setFileName ( item -> text ( 0 ), true );

}

QStringList MainWindow::rebuildPathForNew ( dragTarget *listForNew ) //ricostruisce i percorsi da aggiungere all'archivio chiamando la funzione ricorsiva
{
  QStringList result;
  for ( int i = 0; i < listForNew -> topLevelItemCount(); i++ )
    result << recursiveRebuildForNew ( listForNew -> topLevelItem ( i ) );
  return result;
}

QStringList MainWindow::recursiveRebuildForNew ( QTreeWidgetItem* item )
{
  if ( item -> childCount() != 0 )
  {
    QString tempCheck = item -> child ( 0 ) -> text ( 1 ); //contiene il percorso dell'elemento
    QStringList subFolders = QDir ( item -> text ( 1 ) + item -> text ( 0 ) ).entryList ( QStringList() << "*.*",QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files ); 
    QStringList endList;
    for ( int j = 0; j < item -> childCount(); j++ )
      endList << recursiveRebuildForNew ( item -> child ( j ) );

    return endList;
  }
  else
  {
    if ( item -> parent() != NULL )   //aggiungo un ap se ha una padre
    {
      QTreeWidgetItem *tmp = item -> parent();
      QStringList toReturn;
      while ( tmp!=NULL ) //ricostruiamo i parent all'indietro
      {
        toReturn << tmp -> text ( 0 );
        tmp = tmp -> parent();
      }
      QString parentPath; //ricostruiamo il percorso parent
      for ( int i = toReturn.size()-1; i >= 0 ; i-- )
      {
        parentPath = parentPath + toReturn[i];
        if ( i != 0 ) parentPath.append ( QDir().separator() );
      }
      // se è un cartella del tipo *User created Folder* allora non passo nessun path di origine
      if ( item -> text ( 1 ).indexOf ( QDir().separator() ) != -1 )
      {
        return QStringList() << "-ap"+parentPath << item -> text ( 1 ) + item -> text ( 0 ); //altrimenti restituisco direttamente il percorso
      }
      else
      {
        return QStringList() << "-ap"+parentPath <<item -> text ( 0 ); //altrimenti restituisco direttamente il percorso
      }
    }
    else //altrimenti no
    {
      //puts ( "Percorso ricostruito: "+QString ( item -> text ( 1 ) + item -> text ( 0 ) ).toAscii() );
      if(!item->text(3).isEmpty()) return QStringList() << item -> text ( 1 ) + item -> text ( 0 );
      else return QStringList();
    }
  }
}
 
void MainWindow::newArchive()
{

  filesToAdd = rebuildPathForNew ( targetList ); //ricostruiamo i path di tutti gli elementi da aggiungere
  archive = KFileDialog::getSaveFileName ( KUrl(QDir().homePath()),i18n( "RAR Archive *.rar" ),this, i18n( "Archive Name" ) );
  if ( archive != "" ) //solo se è stato scelto un nomefile eseguiamo le operazioni
  {
    targetList -> setEnabled ( false );
    password = compressionWidget -> getPassword();
    compressionLevel = compressionWidget -> getCompressionLevel();
   
    if(compressionWidget -> isSplitRequested()) // TODO:highly unstable!!! check
    { 
      double splitSize;
      splitSize = compressionWidget -> getSplitSize();
      newProcHandler = new rarProcessHandler(this, "rar", QStringList()<<"a"<<"-ep1"<<"-m"+QString().setNum(compressionLevel)<<"-p"+password<<"-v"+QString().setNum(splitSize), archive, filesToAdd);
      connect(newProcHandler, SIGNAL(processCompleted(bool)), this, SLOT(closeNewArchiveGUI(bool)));
      connect(newProcHandler, SIGNAL(processCanceled()), this, SLOT(closeRar()));
    }
    else
    {
      newProcHandler = new rarProcessHandler(this, "rar", QStringList()<<"a"<<"-ep1"<<"-m"+QString().setNum(compressionLevel)<<"-p"+password, archive, filesToAdd);
      connect(newProcHandler, SIGNAL(processCompleted(bool)), this, SLOT(closeNewArchiveGUI(bool)));
      connect(newProcHandler, SIGNAL(processCanceled()), this, SLOT(closeRar()));
    }

    newProcHandler -> start();
  }
}

void MainWindow::closeNewArchiveGUI(bool correctly)
{
  rarList -> setVisible(true);
  //menubar -> setEnabled ( true );
  delete widgetForList;
  delete sourceDock;
  //toolDelete -> setEnabled ( true );
  toolNew -> setEnabled(true);
  enableActions(true);
  fromNewArchive = false;
  delete dockOption;
  if(actionInfo->isChecked()) setInformation(true);
  if(correctly)
  {
    QFile fileCheck(archive);
    if (fileCheck.exists())  raropen ( archive ); //carichiamo l'archivio appena creato
    else enableActions(false);
    correctly = false;
  }
    
}

void MainWindow::closeRar() //closes rar creation
{
  targetList -> setEnabled ( true );
  QDockWidget *creationResults = new QDockWidget ( i18n( "Correctly added files" ), this );
  creationResults -> setFloating ( true );
  this -> addDockWidget ( Qt::LeftDockWidgetArea,creationResults );
  creationResults -> setGeometry ( creationResults -> x(), creationResults -> y(), 400,600 );
  QWidget *baseWidget = new QWidget ( creationResults );
  QTreeWidget *results = new QTreeWidget ( baseWidget );
  results -> setColumnCount ( 10 );
  results -> setHeaderLabels ( QStringList() <<i18n( "File Name" ) <<i18n( "Size" ) <<i18n( "Packed Size" ) <<i18n( "Ratio" ) <<i18n( "Date" ) <<i18n( "Time" ) <<i18n( "Attributes" ) <<i18n( "CRC" ) <<i18n( "Method" ) <<i18n( "Version" ) );
  QGridLayout *layout = new QGridLayout ( baseWidget );
  layout -> addWidget ( results, 1,1 );
  creationResults -> setWidget ( baseWidget );
  if ( !archive.endsWith ( ".rar" ) ) archive.append ( ".rar" ); //può succedere che chi sceglie il nome del file non vi "appenda" l'estensione... lo facciamo noi
  if ( QFile ( archive ).exists() == true ) //se la creazione è stata interrotta mostriamo i file aggiunti finora
  {
    QProcess rarResults;
    rarResults.start ( archiver, QStringList() <<"v"<<archive );
    rarResults.waitForFinished();
    QString buffer = rarResults.readAllStandardOutput();
    rar().simpleParse ( results, buffer );
  }
  else
  {
    delete creationResults;
    KMessageBox::information ( this,i18n( "No archive file was created" ), i18n( "New Archive" )  );
  }

}

void MainWindow::setupForNew()
{
  fromNewArchive = true;
  rarList -> clear();
  rarList -> setVisible ( false );
  setInformation ( false );
  toolDelete -> setEnabled ( false );
  toolNew -> setEnabled ( false );
  enableActions(false);
  commentDock -> setVisible(false);
  commentAction -> setVisible(false);
  showStatusInfo(false);

  // the source of file dragging
  sourceDock = new QDockWidget ( this );
  sourceDock -> setGeometry ( x() + width() + 20,y(),400,600 );
  sourceDock -> setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
  QWidget *baseWidget = new QWidget ( sourceDock );

  // dragSource widget
  sourceView = new akuDragSourceWidget(baseWidget);
  QGridLayout *layout = new QGridLayout ( baseWidget );
  layout -> addWidget ( sourceView,0,0,1,1 );
  sourceDock -> setWidget ( baseWidget );

  // dragTarget treewidget
  widgetForList = new QWidget ( this );
  QGridLayout *destLayout = new QGridLayout ( widgetForList );
  targetList = new dragTarget ( sourceView -> sourceViewInUse(), widgetForList );
  destLayout -> addWidget ( targetList, 1,1 );
  splitter -> addWidget ( widgetForList );

  // dockOptions
  dockOption = new QDockWidget(i18n("New archive"),this);
  dockOption -> setAllowedAreas(Qt::NoDockWidgetArea);
  compressionWidget = new akuCompressionWidget(this);
  dockOption -> setWidget(compressionWidget);
  this -> addDockWidget ( Qt::LeftDockWidgetArea, dockOption );
  dockOption -> setFloating(true);
  dockOption -> setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
  dockOption -> setGeometry(x() - 305, y(), 305,358);
 

  this -> addDockWidget ( Qt::RightDockWidgetArea,sourceDock );

  //a useful tooltip to explain what to do
  QString tip;
  tip = i18n("<b>New Archive:</b>\njust drag and drop files and folders you wish to add to the new archive");
  new akuToolTip(tip, this);


  connect( compressionWidget, SIGNAL(creationCalled()), this, SLOT(newArchive()));
  connect(compressionWidget, SIGNAL(canceled()), this, SLOT(reloadArchive()));
  akuShadeAnimation *animation = new akuShadeAnimation(sourceDock); //let's animate sourceDock
  animation -> start();
  
}

void MainWindow::embeddedViewer()
{
  if ( rarList -> selectedItems().size() == 1 )
  {
    QTreeWidgetItem *toView = rarList -> selectedItems() [0];
    if ( toView -> text ( 1 ) != "" && toView -> icon(10).isNull())
    {
      QString path = rebuildFullPath(toView);
      QProcess view;
      view.start ( archiver, QStringList() << "p" << "-inul" << namex << path << QDir::tempPath() );
      view.waitForFinished();
      view.terminate();

      QByteArray buffer = view.readAllStandardOutput(); //leggo il file temporaneo e memorizzo in buffer
      akuViewer *embedded = new akuViewer ( this ); //chiamiamo il viewer e passiamogli il testo
      embedded -> setAttribute(Qt::WA_DeleteOnClose);
      embedded -> setData(buffer);
      embedded -> setFileName ( toView -> text ( 0 ) );
      buffer.clear();
      embedded -> setGeometry ( this -> x() +200, this -> y() +200, 600,600 ); // centriamo e ingrandiamo a sufficienza
      embedded -> show();
    }
  }
  else  statusBar() -> showMessage ( i18n( "Only one item at time can be viewed" ), 2000 );
}

void MainWindow::find ( bool find )
{

  if ( find == true )
  {
    searchWidget -> setVisible ( true );
    searchWidget -> searchLineEdit() -> setFocus ( Qt::OtherFocusReason );                                                               
  }
  else  searchWidget -> setVisible ( false );
}

void MainWindow::renameItem()
{
  QList<QTreeWidgetItem*> selectedItems = rarList -> selectedItems();
  if ( selectedItems.size() > 1 ) statusBar() -> showMessage ( i18n( "Only one item at time can be renamed" ), 2000 );
  else
  {
    //tempForRename stores item brothers to avoid same naming under same parent
    if ( selectedItems[0] -> parent() != NULL )
      for ( int i = 0; i < selectedItems[0] -> parent() -> childCount(); i++ )
        tempForRename << selectedItems[0] -> parent() -> child ( i ) -> text ( 0 );
    else 
      for ( int i = 0; i < rarList -> topLevelItemCount(); i++ )
        tempForRename << rarList -> topLevelItem ( i ) -> text ( 0 );
    oldItemName = selectedItems[0]-> text ( 0 );
    //oldItemPath = rebuildPath ( selectedItems[0] );
    oldItemPath = rebuildFullPath(selectedItems[0]);
    selectedItems[0] -> setFlags ( Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    rarList -> editItem ( selectedItems[0], 0 );
    connect ( rarList, SIGNAL ( itemChanged ( QTreeWidgetItem *, int ) ), this, SLOT ( rarRename ( QTreeWidgetItem* , int ) ) );
  }
}

QString MainWindow::rebuildPath ( QTreeWidgetItem *toRebuild ) //missing item name.. see rebuildFullPath for complete string
{
  QTreeWidgetItem *tmp; //elemento temporaneo
  QStringList pathlist; //lista delle cartelle di percorso
  tmp = toRebuild -> parent();
  while ( tmp != NULL )
  {
    pathlist << tmp -> text ( 0 );
    tmp = tmp -> parent();
  }
  QString path;
  for ( int i = pathlist.size()-1; i>=0; i-- )
  {
    path.append ( pathlist[i] );
    if ( i!=0 ) path.append ( QDir().separator() );
  }
  return path;
}

QString MainWindow::rebuildFullPath(QTreeWidgetItem *toRebuild)
{
  QString rebuilded = rebuildPath(toRebuild);
  if(!rebuilded.isEmpty()) return rebuilded + QDir().separator()+toRebuild -> text(0);
  else return toRebuild -> text(0);
}

void MainWindow::rarRename ( QTreeWidgetItem *current, int )
{
  currentToRename = current;
  disconnect ( rarList, SIGNAL ( itemChanged ( QTreeWidgetItem *, int ) ), this, SLOT ( rarRename ( QTreeWidgetItem*, int ) ) );
  rarProcessHandler *renameProcess;
  if(oldItemName != current -> text(0) && !tempForRename.contains(current->text(0)))
  {
    puts("rename accepted");
    waitDialog -> setLabelText(i18n("Renaming ") + oldItemName+"...");
    waitDialog -> exec();
    if(!globalArchivePassword.isEmpty())
    { 
      renameProcess = new rarProcessHandler(this, "rar", QStringList() << "rn"<<"-p"+globalArchivePassword, namex , QStringList() << oldItemPath << rebuildFullPath( current ));
      connect(renameProcess, SIGNAL(processCompleted(bool)), this, SLOT(completeRename(bool)));
    }
    else
    {
      renameProcess = new rarProcessHandler(this, "rar", QStringList() << "rn", namex , QStringList() << oldItemPath << rebuildFullPath( current ));
      connect(renameProcess, SIGNAL(processCompleted(bool)), this, SLOT(completeRename(bool)));
    }
    renameProcess -> start();
  }
  else
  {
    current -> setText(0,oldItemName);
    statusBar() -> showMessage(i18n("Rename denied"), 2000);
  }
  
}

void MainWindow::completeRename(bool ok)
{
  waitDialog -> quit();
  if(!ok) currentToRename -> setText(0, oldItemName);
  tempForRename.clear();
}

void MainWindow::rarDelete()
{
  //assicuriamoci di deselezionare tutti i figli dei padri selezionati inquanto saranno sicuramente rimossi
  QList<QTreeWidgetItem*> selectedItems = rarList -> selectedItems();
  for ( int i = 0; i < selectedItems.size(); i++ )
    if ( selectedItems[i] -> childCount() != 0 ) //se ha dei figli li deselezioniamo
     for ( int j=0; j<selectedItems[i] -> childCount(); j++ )
       rarList -> setItemSelected ( selectedItems[i] -> child ( j ), false );
  
  selectedItems = rarList -> selectedItems(); //ricarichiamo la lista degli elementi selezionati
  //recuperiamo i percorsi dalla lista
  QStringList itemsToDelete;
  QStringList topLevel; //solo i nomi dei file

  //memorizziamo i nomi dei file di toplevel
  for ( int j = 0; j < rarList -> topLevelItemCount(); j++ )
    topLevel << rarList -> topLevelItem ( j ) -> text ( 0 );

  deleteAll = false;

  //ricostruiamo i percorsi dei file da eliminare
  for ( int i = 0; i < selectedItems.size(); i++ )
  {
    QString path = rebuildPath ( selectedItems[i] );
    if ( !path.isEmpty() )
      itemsToDelete << path + QDir::separator() + selectedItems[i] -> text ( 0 );
    else
      itemsToDelete << selectedItems[i] -> text ( 0 );
  }
  if ( KMessageBox::questionYesNo ( this,i18n( "Are you sure you want to delete the selected file(s)?" ), i18n( "Deleting items" )) == KMessageBox::Yes )
  {
    if ( topLevel == itemsToDelete )   //se gli elementi da eliminare coincidono con tutti i toplevel allora stiamo eliminando l'archivio intero
    {
      KMessageBox::information ( this,  i18n( "You chose to remove every item from this archive. The entire archive file will be removed." ), i18n( "Deleting items" ) );
      deleteAll = true;
    }
    waitDialog -> setLabelText(i18n("Deleting file(s)..."));
    waitDialog -> exec();
    rarProcessHandler *deleteItem;
    deleteItem = new rarProcessHandler(this, "rar", QStringList() << "d" <<"-p"+globalArchivePassword, namex , itemsToDelete );
    connect(deleteItem, SIGNAL(processCompleted(bool)), this, SLOT(completeDelete(bool)));
   
    deleteItem -> start();
  }
}

void MainWindow::completeDelete(bool ok)
{
  waitDialog -> quit();
  if(ok)
  {
    if ( deleteAll == false ) reloadArchive();
    else
    {
      rarList -> clear();
      this -> setWindowTitle ( i18n( "aKu" ) );
      enableActions(false);
      showStatusInfo(false);
      statusBar() -> showMessage(i18n("Archive deleted"),2000);
    }
  }
}

void MainWindow::expandTopLevelItems()
{
  for ( int i = 0; i < rarList -> topLevelItemCount(); i++ )
    rarList -> setItemExpanded ( rarList -> topLevelItem ( i ), true );
}

void MainWindow::sumSelectedItemsSize()
{
  QStringList sizes;
  QList<QTreeWidgetItem*> selectedItems = rarList -> selectedItems();
  QList<double> rawSizeSum;
  for ( int i = 0; i < selectedItems.size(); i++ )
  {
    //acquisisco le dimensioni con le opportune conversioni
    if ( selectedItems[i] -> text ( 1 ) != "" )
    {
      sizes = ( selectedItems[i] -> text ( 1 ) ).split ( " " );
      if ( sizes[1] == "KiB" ) rawSizeSum << ( sizes[0].toFloat() * 1024 );
      else if ( sizes[1] ==  "MiB" ) rawSizeSum << ( sizes[0].toFloat() * 1048576 );
      else if ( sizes[1] == "GiB" ) rawSizeSum << ( sizes[0].toFloat() * 1073741824 );
      else rawSizeSum << sizes[0].toFloat();
    }
  }

  double result = 0;
  for ( int j=0; j < rawSizeSum.size(); j++ )
  {
    result = result + rawSizeSum[j];
  }
  metaWidget -> setFileSize( KLocale("").formatByteSize(result));
}

void MainWindow::setupActions()
{
  toolExtract = new KAction ( this);
  toolExtract -> setIcon ( KIcon ( "archive-extract.png" ) ); 
  toolExtract -> setText ( i18n( "Extract" ) );
  actionCollection() -> addAction ( "extract", toolExtract );
  toolDelete = new KAction ( this );
  toolDelete -> setIcon ( KIcon ( "archive-remove.png" ) );
  toolDelete -> setText ( i18n( "Remove" ) );
  actionCollection() -> addAction ( "delete", toolDelete );
  toolView = new KAction ( this );
  toolView -> setIcon ( KIcon ( "document-preview-archive.png" ) );
  toolView -> setText ( i18n( "Preview" ) );
  actionCollection() -> addAction ("preview", toolView );
  actionInfo = new KAction(this);
  actionInfo -> setIcon(KIcon("help-about"));
  actionInfo -> setText(i18n("Information"));
  actionInfo -> setCheckable(true);
  actionCollection() -> addAction("info", actionInfo);
  toolNew = KStandardAction::openNew(this, SLOT(setupForNew()), actionCollection());
  actionCollection()->addAction( "file_new",toolNew );
  actionCollection()->addAction( KStandardAction::Open, "file_open", this, SLOT(openDialog()) );
  recents = KStandardAction::openRecent(this, SLOT(openRecentFile(KUrl)), actionCollection());
  //recents -> setIcon(KIcon("document-open-recent")); //new oxygen icon
  KStandardAction::quit(kapp, SLOT(quit()), actionCollection());
  actionFind -> setShortcut(Qt::CTRL + Qt::Key_F);
  actionCollection() -> addAction("find", actionFind);
  KMenu *tool = new KMenu(i18n("Tools"), menuBar());
  actionCollection() -> addAction("tools", tool->menuAction());
  actionLock = new KAction(this);
  actionLock -> setText(i18n("Lock Archive"));
  actionLock -> setIcon(KIcon("document-encrypt"));
  actionCollection() -> addAction("lock", actionLock);
  KMenu *commands = new KMenu(i18n("Commands"), menuBar());
  actionCollection() -> addAction("commands", commands->menuAction());
  actionAddComment = new KAction(this);
  actionAddComment -> setText(i18n("Add Archive Comment"));
  actionAddComment -> setIcon(KIcon("kontact-notes"));
  actionCollection() -> addAction("add_comment", actionAddComment);
  actionAddFolder = new KAction(this);
  actionAddFolder -> setText(i18n("Add Folder"));
  actionAddFolder -> setIcon(KIcon("archive-insert-directory"));
  actionCollection() -> addAction("add_folder", actionAddFolder);
  actionAddFile = new KAction(this);
  actionAddFile -> setText(i18n("Add File"));
  actionAddFile -> setIcon(KIcon("archive-insert"));
  actionCollection() -> addAction("add_file", actionAddFile);
  actionEncryptArchive = new KAction(this);
  actionEncryptArchive -> setText(i18n("Encrypt this archive"));
  actionEncryptArchive -> setIcon(KIcon("dialog-password"));
  actionCollection() -> addAction("encrypt_archive", actionEncryptArchive);
  actionAddFilePwd = new KAction(this);
  actionAddFilePwd -> setText(i18n("Add file with password"));
  actionAddFilePwd -> setIcon(KIcon("archive-insert"));
  actionCollection() -> addAction("add_file_pwd", actionAddFilePwd);
  actionAddFolderPwd = new KAction(this);
  actionAddFolderPwd -> setText(i18n("Add folder with password"));
  actionAddFolderPwd -> setIcon(KIcon("archive-insert-directory"));
  actionCollection() -> addAction("add_folder_pwd", actionAddFolderPwd);
  // DEBUG
  KAction *debug = new KAction(i18n("Debug"), this);
  connect(debug, SIGNAL(triggered()), this, SLOT(debugging()));
  actionCollection() -> addAction("debug", debug);
  setupGUI (QSize(650,460));
  checkRarExe();
}

void MainWindow::cantDeleteMessage()
{
  statusBar() -> showMessage(i18n("Can't delete the archive file"),2000);
}

void MainWindow::closeDeletedArchive()
{
  rarList -> clear();
  this -> setWindowTitle ( i18n( "aKu" ) );
  enableActions(false);
  archiveInfo -> clear();
  infoLock-> clear();
  statusBar() -> showMessage(i18n("Archive deleted"));
}

void MainWindow::openItemUrl(QTreeWidgetItem *toOpen, int) //apriamo l'elemento con la relativa applicazione associata
{
  if(!toOpen -> text(1).isEmpty())
  {
    QString tempPath = QDir().tempPath();
    if(!tempPath.endsWith(QDir().separator())) tempPath.append(QDir().separator()); //controlliamo che la stringa termini con un separator
    QString fileToExtract= rebuildFullPath(toOpen);
    rarProcessHandler *procHandler;
    QFile tempFile(tempPath+fileToExtract);
    if(!tempFile.exists())
    {
      if(!globalArchivePassword.isEmpty()) 
        procHandler = new rarProcessHandler(this, "rar", QStringList() << "x"<<"-p"+globalArchivePassword,namex ,QStringList() << fileToExtract ,tempPath); //estraiamo il file nella cartella temporanea
      else 
        procHandler = new rarProcessHandler(this, "rar", QStringList() << "x",namex ,QStringList() << fileToExtract ,tempPath); //estraiamo il file nella cartella temporanea
      procHandler -> start();
    }
    QString forUrl;
    forUrl = tempPath + fileToExtract;
    KUrl url(tempPath + fileToExtract);
    QDesktopServices::openUrl(url);
  }

}
