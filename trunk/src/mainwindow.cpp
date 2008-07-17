#include "mainwindow.h"

MainWindow::MainWindow (QWidget* parent, Qt::WFlags flags): KXmlGuiWindow (parent, flags)
{
  baseWindowWidget = new KVBox(this);
  splitter = new QSplitter(this);
  table = new akuMainTable(baseWindowWidget);
  splitter -> addWidget(baseWindowWidget);
  setCentralWidget(splitter);
  
  // a tip widget to show info
  tip = new akuTip(baseWindowWidget);
  tip->setVisible(false);
  tip->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  //adding metaWidget to splitter
  metaWidget = new akuMetaWidget(this);
  splitter -> addWidget(metaWidget);
  metaWidget -> setVisible(false);

  filespassprotected = false;
  setupSearchBar();
  setupStatusBar();
  setupActions();
  setupPopupMenu();
  setupConnections();
  cmdLineOptions();
  setupGUI (QSize(650,460));
  setupDocks();

  dockComment -> setVisible(false);
  buttonComment -> setVisible(false);

  
}

MainWindow::~MainWindow()
{
  recentFilesAction->saveEntries( KGlobal::config()->group( "Recent Files" ));
  for (int i = 0; i < tmpFiles.size(); i++)
    QFile::remove(tmpFiles[i]);
}

void MainWindow::setupDocks()
{
  dockComment = new QDockWidget(this);
  dockComment -> setObjectName("dockComment");
  dockComment -> setVisible(false);
  QLabel *commentTitle = new QLabel(i18n("Comment"),dockComment);
  commentTitle -> setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  QWidget *baseComment = new QWidget(dockComment);
  editComment = new KTextEdit(baseComment);
  editComment -> setReadOnly(true);
  editComment -> setTextInteractionFlags (Qt::TextSelectableByMouse);
  QGridLayout *commentLayout = new QGridLayout(baseComment);
  commentLayout -> addWidget(editComment, 1, 1);
  dockComment -> setWidget(baseComment);
  this -> addDockWidget ( Qt::LeftDockWidgetArea, dockComment );   
  // commentEdit -> setReadOnly(true);
  dockComment -> setFloating(true);
  buttonComment = dockComment -> toggleViewAction();
  buttonComment -> setText(i18n("Show Comment"));
  actionCollection() -> addAction("comment", buttonComment);
  
}

void MainWindow::setupStatusBar()
{
  archiveInfo = new QLabel(this); 
  archiveInfo ->  setFont(KGlobalSettings::generalFont());
  statusBar() -> addPermanentWidget(archiveInfo, 0);
  infoExtra = new QLabel(this);
  infoExtra -> setMaximumSize(22,18);
  infoExtrabis = new QLabel(this);
  infoExtrabis -> setMaximumSize(22,18);
  ratioBar = new akuRatioWidget(0,this);
  ratioBar -> setMinimumSize(120,0);
  ratioBar -> setVisible(false);
  statusWidget = new QWidget(this);
  QGridLayout *statusLayout = new QGridLayout(statusWidget);
  statusLayout -> addWidget(archiveInfo,1,1);
  statusLayout -> addWidget(ratioBar,1,2);
  statusLayout -> addWidget(infoExtra,1,3);
  statusLayout -> addWidget(infoExtrabis,1,4);
  infoExtrabis -> setVisible(false);
  statusBar() -> addPermanentWidget(statusWidget);
  
}

void MainWindow::setupSearchBar()
{
  searchWidget = new akuSearchWidget(table, baseWindowWidget);
  searchWidget -> setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  searchWidget -> setVisible (false);	
  buttonFind = searchWidget -> getCloseAction();
}


void MainWindow::setupActions()
{
  buttonNew = KStandardAction::openNew(this, SLOT(createNewArchive()), actionCollection());
  actionCollection()->addAction("file_new",buttonNew );
  buttonOpen = KStandardAction::open(this, SLOT(openDialog()), actionCollection());
  actionCollection()->addAction("file_open", buttonOpen );
  KStandardAction::quit(this, SLOT(quit()), actionCollection());
  buttonInfo = new KAction(this);
  buttonInfo -> setIcon(KIcon("help-about"));
  buttonInfo -> setText(i18n("Information"));
  buttonInfo -> setCheckable(true);
  buttonInfo -> setEnabled(false);
  buttonInfo -> setShortcut(Qt::CTRL + Qt::Key_I);
  actionCollection() -> addAction("info", buttonInfo);

  recentFilesAction = KStandardAction::openRecent( this, SLOT(openUrl(const KUrl&)), actionCollection());
  actionCollection()->addAction("file_open_recent", recentFilesAction );
  recentFilesAction->loadEntries( KGlobal::config()->group("Recent Files"));
  connect(recentFilesAction, SIGNAL(triggered()), this, SLOT(openDialog()));

  KMenu *quickextractMenu = new KMenu(this);
  quickextractMenu -> addTitle(KIcon("archive-extract"), i18n("Quick extracto to"));

  KConfig config;
  extractGroup = new QActionGroup(this);
  extractGroup -> setExclusive(false);

  QStringList actionList = KConfigGroup(&config, "Extraction dialog").readEntry("destination dirs", QStringList());
  QAction *actionHome = quickextractMenu -> addAction(KIcon("user-home"), QDir().homePath());
  actionHome -> setData(QVariant(QDir().homePath()));
  QAction *actionDesktop = quickextractMenu -> addAction(KIcon("user-desktop"), KGlobalSettings::desktopPath());
  actionDesktop -> setData(QVariant(KGlobalSettings::desktopPath()));
  extractGroup -> addAction(actionHome);
  extractGroup -> addAction(actionDesktop);
  if (!actionList.isEmpty()) {
    for (int i = 0; i < actionList.size(); i++) {
      // se il percorso esiste altrimenti non ha senso
      if (QDir(actionList[i]).exists() && !actionList[i].isEmpty()) {
        if ((actionList[i] != QDir().homePath()) && (actionList[i] != KGlobalSettings::desktopPath())) {
          QAction *tmpAction = quickextractMenu -> addAction(KIcon("folder-blue"), actionList[i]);
          tmpAction -> setData(QVariant(actionList[i]));
          extractGroup -> addAction(tmpAction); //and set new ones
        }
      }
    }
  }

  connect(extractGroup, SIGNAL(triggered(QAction*)), this, SLOT(extractoToPreferred(QAction*)));
  //kDebug() << group -> actions();

  buttonExtract = (quickextractMenu -> menuAction());
  buttonExtract -> setIcon(KIcon("archive-extract.png")); 
  buttonExtract -> setText(i18n("Extract"));
  buttonExtract -> setEnabled(false);
  actionCollection() -> addAction("extract", buttonExtract);  

  KAction *lasttip = tip->actionTip();
  actionCollection() -> addAction("lasttip", lasttip);
  connect(lasttip, SIGNAL(triggered()),tip, SLOT(show()));
  lasttip -> setShortcut(Qt::CTRL + Qt::Key_T);
  buttonFind -> setShortcut(Qt::CTRL + Qt::Key_F);
  actionCollection() -> addAction("find", buttonFind);
  buttonDelete = new KAction(this);
  buttonDelete -> setIcon(KIcon("archive-remove.png"));
  buttonDelete -> setText(i18n("Remove"));
  buttonDelete -> setEnabled(false);
  actionCollection() -> addAction("delete", buttonDelete);
  buttonDelete -> setShortcut (QKeySequence("Del"));
  buttonView = new KAction(this);
  buttonView -> setIcon(KIcon("document-preview-archive"));
  buttonView -> setText(i18n("Preview"));
  buttonView -> setEnabled(false);
  buttonView -> setShortcut(QKeySequence ("F11"));
  actionCollection() -> addAction("preview", buttonView);
  
  //KMenu *tool = new KMenu(i18n("Tools"), menuBar());
  //actionCollection() -> addAction("tools", tool->menuAction());
  buttonLock = new KAction(this);
  buttonLock -> setText(i18n("Lock Archive"));
  buttonLock -> setIcon(KIcon("document-encrypt"));
  actionCollection() -> addAction("lock", buttonLock);
  buttonLock -> setEnabled(false);
  //KMenu *commands = new KMenu(i18n("Commands"), menuBar());
  //actionCollection() -> addAction("commands", commands->menuAction());
  buttonAddComment = new KAction(this);
  buttonAddComment -> setText(i18n("Add Archive Comment"));
  buttonAddComment -> setIcon(KIcon("view-pim-notes"));
  buttonAddComment -> setEnabled(false);
  actionCollection() -> addAction("add_comment", buttonAddComment);
  buttonAddDir = new KAction(this);
  buttonAddDir -> setText(i18n("Add Directory"));
  buttonAddDir -> setIcon(KIcon("archive-insert-directory"));
  buttonAddDir -> setEnabled(false);
  actionCollection() -> addAction("add_dir", buttonAddDir);
  buttonAddFile = new KAction(this);
  buttonAddFile -> setText(i18n("Add File(s)"));
  buttonAddFile -> setIcon(KIcon("archive-insert"));
  buttonAddFile -> setEnabled(false);
  actionCollection() -> addAction("add_file", buttonAddFile);
  /*buttonEncryptArchive = new KAction(this);
  buttonEncryptArchive -> setText(i18n("Encrypt this archive"));
  buttonEncryptArchive -> setIcon(KIcon("dialog-password"));
  actionCollection() -> addAction("encrypt_archive", buttonEncryptArchive);
  */
}

void MainWindow::cmdLineOptions()
{
//    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
//    if (args -> isSet("extractto")) { 
//      quickExtract *dirextract = new quickExtract(args -> arg(0), "quickExtract", this);
//      connect(dirextract, SIGNAL(processCompleted(bool)), this, SLOT(quit()));
//      dirextract -> show();
//    }
// 
//    else {
//      for (int i=0; i < args -> count(); i++) openUrl(args -> url(i));
//    }
//    args -> clear();
}

void MainWindow::setupConnections()
{
  connect (buttonAddFile, SIGNAL(triggered()), this, SLOT(addFile()));
  connect (buttonInfo, SIGNAL (toggled (bool)), this, SLOT(viewInformation(bool)));
  connect (buttonView, SIGNAL (triggered()), this, SLOT (embeddedViewer()));
  connect (buttonAddComment, SIGNAL(triggered()), this, SLOT(addComment()));
  connect (buttonDelete, SIGNAL(triggered()), this, SLOT(deleteFile()));
  connect (buttonAddDir, SIGNAL(triggered()), this, SLOT(addDir()));
  connect (table, SIGNAL(itemSelectionChanged()), this, SLOT(metaBar()));
  connect (table, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(openItemUrl(QTreeWidgetItem *, int)));
  connect (buttonExtract, SIGNAL(triggered()), this, SLOT(extractArchive()));
  connect (buttonLock, SIGNAL (triggered()), this, SLOT(lockArchive()));
  connect (popSelectall, SIGNAL(triggered()), table, SLOT(selectAll()));
  connect (popInvertselection, SIGNAL(triggered()), this, SLOT(selectionInverted()));
  connect (popRename, SIGNAL (triggered()), this, SLOT (renameItem()));
  connect (metaWidget, SIGNAL (tempFiles(QString)), this, SLOT (collectTempFiles(QString)));
}

void MainWindow::enableActions(bool enable)
{
  buttonInfo -> setEnabled(enable);
  buttonOpen -> setEnabled(enable);
  buttonNew -> setEnabled(enable);
  buttonView -> setEnabled(enable);
  buttonExtract -> setEnabled(enable);
  buttonAddComment -> setEnabled(enable);
  buttonAddFile -> setEnabled(enable);
  buttonAddDir -> setEnabled(enable);
  buttonDelete -> setEnabled(enable);
  popRename -> setEnabled(enable);
  buttonLock -> setEnabled(enable);
  if (enable) {
    setCursor(QCursor());
    table -> setSelectionMode(QAbstractItemView::ExtendedSelection);
  }
  else {
    setCursor(Qt::WaitCursor);
    table -> setSelectionMode(QAbstractItemView::NoSelection);
  }

  if ((mimetype -> name() == "application/x-compressed-tar") || (mimetype -> name() == "application/x-bzip-compressed-tar")) buttonDelete -> setEnabled(false);
  if ((mimetype -> name() == "application/x-compressed-tar") || (mimetype -> name() == "application/x-bzip-compressed-tar") || (mimetype -> name() == "application/x-tar")) popRename -> setEnabled(false);
}

void MainWindow::setupPopupMenu()
{
  popSelectall = new KAction (i18n("Select All"), table);
  popSelectall -> setIcon(KIcon("edit-select-all"));
  table -> addAction (popSelectall);
  table -> setContextMenuPolicy (Qt::ActionsContextMenu);
  popInvertselection = new KAction (i18n("Invert Selection"), table);
  table -> addAction (popInvertselection);
  separator = new KAction(this);
  separator -> setSeparator (true);
  table -> addAction (separator);
  popRename = new KAction (i18n("Rename"), table);
  popRename -> setIcon(KIcon("edit-rename"));
  popRename -> setEnabled(false);
  table -> addAction (popRename);
  popRename -> setShortcut (QKeySequence ("F2"));
  table -> addAction (buttonDelete);
  table -> addAction(buttonView);
  separator2 =new KAction(this); 
  separator2 -> setSeparator(true);
  table -> addAction(separator2);
  table -> addAction(buttonAddFile);
  table -> addAction(buttonAddDir);
}

void MainWindow::extractoToPreferred(QAction *action)
{
  QString extractWhere; 
  extractWhere = (action -> data()).toString();
  if (compressor == "rar") {
    QStringList options;
    options << "x";
    if (!archivePassword.isEmpty()) options << "-p" + archivePassword;
    rarProcess *process = new rarProcess(this, "rar", options, archive, table -> filesToExtract(), extractWhere );
    connect(process, SIGNAL(processCompleted(bool)), this, SLOT(extractionCompleted(bool)));
    process -> start();
  }
}

void MainWindow::openDialog()
{
  KUrl url = KFileDialog::getOpenUrl(KUrl("kfiledialog:///AkuOpenDir"), i18n("*.rar *.zip *.bz2 *.gz *.tar|All supported types\n*.rar|Rar archives\n*.zip|Zip archives\n*.bz2|Tar archives (bzip)\n*.gz|Tar archives (gzip)\n*.tar|Tar archives\n*.*|All files"), this);
  if (!url.isEmpty())
    openUrl(url);
}

void MainWindow::openUrl(const KUrl& url)
{
  if (!KFileItem(KFileItem::Unknown, KFileItem::Unknown, url).isReadable()) return;
  mimetype = KMimeType::findByUrl(url);
  searchWidget -> searchLineEdit() -> clear();

  if (mimetype -> name() == "application/x-rar") {
    infoExtrabis -> setVisible(false);
    compressor = "rar";
    enableActions(false);
    statusBar()->showMessage(i18n("Loading archive..."));
    QStringList options;
    options << "v";
    if (!archivePassword.isEmpty() && archive == url.pathOrUrl())
      options << "-p" + archivePassword;
    else archivePassword.clear();
    
    rarprocess = new rarProcess(this, "rar", options, url.pathOrUrl());
    connect(rarprocess, SIGNAL(outputReady(QString, bool)), this, SLOT(buildRarTable(QString, bool)));
    rarprocess -> start();
    metaWidget -> clear();
    metaWidget -> setFileName("application/x-rar");
    QPixmap mimeIcon = KIcon("application-x-rar").pixmap(128,128);
    metaWidget -> setMimeIcon( mimeIcon );
    metaWidget -> setMime("");
  }

  else if (mimetype -> name() == "application/zip") {
    infoExtrabis -> setVisible(false);
    compressor = "zip";
    enableActions(false);
    statusBar()->showMessage(i18n("Loading archive..."));
    zipprocess = new zipProcess(this, "unzip", QStringList() << "-Zl", url.pathOrUrl());
    connect(zipprocess, SIGNAL(outputReady(QString, bool)), this, SLOT(buildZipTable(QString, bool)));
    zipprocess -> start();
    metaWidget -> clear();
    metaWidget -> setFileName("application/zip");
    QPixmap mimeIcon = KIcon("application-zip").pixmap(128,128);
    metaWidget -> setMimeIcon( mimeIcon );
    metaWidget -> setMime("");
  }

  else if ((mimetype -> name() == "application/x-bzip-compressed-tar") || (mimetype -> name() == "application/x-compressed-tar") || (mimetype -> name() == "application/x-tar")) { 
    infoExtrabis -> setVisible(false);
    compressor = "tar";
    enableActions(false);
    statusBar()->showMessage(i18n("Loading archive..."));
    tarprocess = new tarProcess(this, "tar", QStringList() << "-tvf", url.pathOrUrl());
    connect(tarprocess, SIGNAL(outputReady(QString, bool)), this, SLOT(buildTarTable(QString)));
    tarprocess -> start();
    metaWidget -> clear();
    metaWidget -> setFileName(mimetype -> name());
    QPixmap mimeIcon = KIcon("application-x-bzip").pixmap(128,128);
    metaWidget -> setMimeIcon( mimeIcon );
    metaWidget -> setMime("");     
  }

  else compressor.clear();
  // costruisco la tabella
  if (!compressor.isEmpty()) {
    archive = url.pathOrUrl();
    ratioBar -> setVisible(true);
    recentFilesAction -> addUrl(url);
  }
  else {
    tip->setTip(i18n("The file is not a supported archive") + " (" + (mimetype -> comment()) + ")");
    tip->show();
  }
}

void MainWindow::buildZipTable(QString zipoutput, bool crypted)
{
   disconnect(zipprocess, SIGNAL(outputReady(QString, bool)), this, SLOT(buildZipTable(QString, bool)));
   if (!zipoutput.isEmpty())
   { 
     table -> clear(); //ripulisco la lista
     table -> setFormat("zip");
     zip nzip;

     bool someprotection = nzip.parse(table, zipoutput, ratioBar);
     // non credo ci siano archivi zip con header password protected
     archivePassword.clear();
  
     if (someprotection) filespassprotected = true;
     else filespassprotected = false;

     QStringList archivedetails = nzip.getArchiveDetails();
     archiveInfo -> setText(archivedetails[0] + " " + "<b>" + i18n("file(s)") + "  " + i18n("size:") + "</b> " + archivedetails[1] + " <b> " + i18n("packed:") + "</b> " + archivedetails[2] + "  "); 

     table -> header() -> setResizeMode(0, QHeaderView::ResizeToContents);
     table -> header() -> setResizeMode(3, QHeaderView::ResizeToContents);
     table -> header() -> setResizeMode(4, QHeaderView::ResizeToContents);
     table -> header() -> setResizeMode(7, QHeaderView::ResizeToContents);
     table -> header() -> setResizeMode(8, QHeaderView::ResizeToContents);
     table -> sortItems ( 0, Qt::AscendingOrder );
     table -> setFolderIcons();
     table -> expandTopLevelItems();
     setCaption(archive);
     handleAdvancedZip(archive);
   }
   enableActions(true);
   statusBar()->clearMessage();
   buttonLock -> setEnabled(false);
   buttonAddDir -> setEnabled(false);
   buttonAddFile -> setEnabled(false);
}

void MainWindow::buildTarTable(QString taroutput)
{  
   disconnect(tarprocess, SIGNAL(outputReady(QString, bool)), this, SLOT(buildTarTable(QString)));
   if (!taroutput.isEmpty()) {
     kDebug() << mimetype -> name();
     table -> setFormat("tar");
     table -> clear();
     tar ntar;
     ntar.parse(table, taroutput);
     // non credo ci siano archivi tar con header password protected
     archivePassword.clear();

     QStringList archivedetails = ntar.getArchiveDetails();
   
     KFileItem file(KFileItem::Unknown, KFileItem::Unknown, KUrl(archive));
     float ratio (100 - ((100.0 * file.size()) / archivedetails[1].toULongLong()));
     if (ratio < 0) ratio = 0.0;
     ratioBar -> setRatio(int(ratio + 0.5f)); 
   
     archiveInfo -> setText(archivedetails[0] + " " + "<b>" + i18n("file(s)") + "  " + i18n("size:") + "</b> " + KLocale(archivedetails[1]).formatByteSize(archivedetails[1].toULong()) + " <b> " + i18n("packed:") + "</b> " + KLocale(QString().setNum(file.size())).formatByteSize((QString().setNum(file.size())).toULong()) + "  ");   
 
     table -> header() -> setResizeMode(0, QHeaderView::ResizeToContents);
     table -> header() -> setResizeMode(4, QHeaderView::ResizeToContents);
     table -> header() -> setResizeMode(11, QHeaderView::ResizeToContents);
     table -> expandTopLevelItems();
     table -> sortItems ( 0, Qt::AscendingOrder );
     table -> setFolderIcons();
     setCaption(archive);
  
   }
   enableActions(true);
   statusBar()->clearMessage();
   buttonLock -> setEnabled(false);
   popRename -> setEnabled(false);
   buttonAddDir -> setEnabled(false);
   buttonAddFile -> setEnabled(false);
   // gzip e bzip non supportano il delete
   if ((mimetype -> name() == "application/x-compressed-tar") || (mimetype -> name() == "application/x-bzip-compressed-tar")) buttonDelete -> setEnabled(false);
}

void MainWindow::buildRarTable(QString raroutput, bool headercrypted)
{
  kDebug() << "buildRARTable";
  disconnect(rarprocess, SIGNAL(outputReady(QString, bool)), this, SLOT(buildRarTable(QString, bool)));
  if (!raroutput.isEmpty())   { 
    infoExtra -> clear();

    table -> setFormat("rar");
    table -> clear(); //ripulisco la lista

    rar nrar;
    bool someprotection = nrar.parse (table, raroutput, ratioBar);  //metaWidget -> ratioBar() 

    QStringList archivedetails = nrar.getArchiveDetails();
    archiveInfo -> setText(archivedetails[0] + " " + "<b>" + i18n("file(s)") + "  " + i18n("size:") + "</b> " + archivedetails[1] + " <b> " + i18n("packed:") + "</b> " + archivedetails[2] + "  ");
  
    if (!archivePassword.isEmpty()) headercrypted = false;
    else archivePassword = rarprocess -> getArchivePassword();

    if ((someprotection) && (archivePassword.isEmpty()))
      filespassprotected = true;
    else filespassprotected = false;

    table -> header() -> setResizeMode(0, QHeaderView::ResizeToContents); // filename
    table -> header() -> setResizeMode(3, QHeaderView::ResizeToContents); // ratio
    table -> header() -> setResizeMode(4, QHeaderView::ResizeToContents); // modified
    table -> header() -> setResizeMode(7, QHeaderView::ResizeToContents); // method
    table -> header() -> setResizeMode(8, QHeaderView::ResizeToContents); // version
    table -> sortItems (0, Qt::AscendingOrder );

    table -> expandTopLevelItems();
    table -> setFolderIcons();
    enableActions(true);
    statusBar()->clearMessage();
    setCaption(archive);
    handleAdvancedRar(archive, raroutput);
  }
  else {
    enableActions(true);
    statusBar()->clearMessage();
  }
}

void MainWindow::handleAdvancedZip(QString filename)
{  
  QProcess *process = new QProcess();
  QStringList options;
  options << "-z";
  process->start("unzip", options << filename);  
  process->waitForFinished();
  
  QString extrainfo = process -> readAllStandardOutput();
  delete process;

  extrainfo.remove(0, extrainfo.indexOf("\n") + 1);
  if (!extrainfo.isEmpty()) {
    editComment -> setText(extrainfo);
    dockComment -> setGeometry(x() - 300, y(), 300, 200);
    dockComment -> setVisible(true);
    buttonComment -> setVisible(true);
  }
  else {
    dockComment -> setVisible(false);
    editComment -> clear();
    buttonComment -> setVisible(false);
  }

  if (filespassprotected) {
    infoExtra -> setPixmap(KIcon("dialog-ok").pixmap(22,18));
    QString toolTip(i18n("This archive has one or more <b>password protected files</b>"));
    infoExtra -> setToolTip(toolTip);
  }
  else {
    infoExtra -> setPixmap(KIcon("dialog-ok-apply").pixmap(22,18));
    QString toolTip(i18n("This archive has no global restrictions"));
    infoExtra -> setToolTip(toolTip);
  }
}

void MainWindow::handleAdvancedRar(QString filename, QString raroutput)
{
  // here we parse rar listing to find out comments and lock info
  // to know whether we have comments or not we can simply use the QString
  // received from rarProcess::outputReady()
  // But to know if lock is present we need to call rar with vt params.
  // rarProcessHandler is asyncronous and heavier than QProcess for this simple operation
  // even if it handles better rar errors. However QProcess seems better for this fast call

  QProcess *process = new QProcess();
  QStringList options;
  options << "vt";
  if(!archivePassword.isEmpty()) options << "-p" + archivePassword;

  process->start(compressor, options << filename);
  
  process->waitForFinished();
  
  QString extrainfo = process -> readAllStandardOutput();
  delete process;
  
  bool locked = false;
 
  if (extrainfo.contains("Lock is present\n")) {
    extrainfo.clear();
    QString toolTip(i18n("<b><font color=red>Locked archive</u></b></font><p><i>Any attempt to change the archive will be ignored</i>"));
    QPixmap lockIcon = KIcon("object-locked").pixmap(22,18);
    locked = true;
    infoExtra -> setPixmap(lockIcon);
    infoExtra -> setToolTip(toolTip); 

    buttonAddComment -> setEnabled(false);
    buttonLock -> setEnabled(false);
    popRename -> setEnabled(false);
    buttonDelete -> setEnabled(false);
    buttonAddFile -> setEnabled(false);
    buttonAddDir -> setEnabled(false);

    //actionEncryptArchive -> setEnabled(false);
    //actionAddFilePwd -> setEnabled(false);
    //actionAddFolderPwd -> setEnabled(false);
    //actionAddFolder -> setEnabled(false);
  }

  if (raroutput.contains("Comment:")) {
  //se troviamo la stringa Comment: vuol dire che c'è un commento, quindi lo estraiamo
    int target = raroutput.indexOf("Comment:");
    raroutput.remove(0, target);
    raroutput.remove("Comment:");
    raroutput.remove(raroutput.indexOf("Pathname"), raroutput.length());

    editComment -> setText(raroutput);
    dockComment -> setGeometry(x() - 300, y(), 300, 200);
    dockComment -> setVisible(true);
    buttonComment -> setVisible(true);
  }
  else {
    dockComment -> setVisible(false);
    editComment -> clear();
    buttonComment -> setVisible(false);
  }

  if (!archivePassword.isEmpty()) {
    infoExtra -> setPixmap(KIcon("dialog-password").pixmap(22,18));
    infoExtra -> setToolTip(i18n("This archive has a <b>header password</b> protection.<br>File data, " 
                                "file names, sizes, attributes, comments are encrypted.<br> Without a password it is "
                                "impossible to view even the list of files in archive.")); 
    if (locked) {
      QString toolTip(i18n("<b><font color=red>Locked archive</u></b></font><p><i>Any attempt to change the archive will be ignored</i>"));
      QPixmap lockIcon = KIcon("object-locked").pixmap(22,18);
      infoExtrabis -> setPixmap(lockIcon);
      infoExtrabis -> setToolTip(toolTip);
      infoExtrabis -> setVisible(true);
    }
  }
  else if (filespassprotected) {
    infoExtra -> setPixmap(KIcon("dialog-ok").pixmap(22,18));
    QString toolTip(i18n("This archive has one or more <b>password protected files</b>"));
    infoExtra -> setToolTip(toolTip);
    if (locked) {
      QString toolTip(i18n("<b><font color=red>Locked archive</u></b></font><p><i>Any attempt to change the archive will be ignored</i>"));
      QPixmap lockIcon = KIcon("object-locked").pixmap(22,18);
      infoExtrabis -> setPixmap(lockIcon);
      infoExtrabis -> setToolTip(toolTip);
      infoExtrabis -> setVisible(true);
    }
  }

  if (infoExtra -> pixmap() == NULL) {
    infoExtra -> setPixmap(KIcon("dialog-ok-apply").pixmap(22,18));
    QString toolTip(i18n("This archive has no global restrictions"));
    infoExtra -> setToolTip(toolTip);
  }
}

void MainWindow::viewInformation (bool visible)
{
  metaWidget -> setVisible(visible);
  if (visible) metaBar();
  else metaWidget -> setAudioControl(false);
}

void MainWindow::metaBar()
{
  QList<QTreeWidgetItem*> checkSelected = table -> selectedItems();
  if (checkSelected.size() != 0) {
    if (checkSelected.size() == 1) {
      getMetaInfo(checkSelected[0]);
      if ((checkSelected[0] -> text(9).contains("image")) && metaWidget -> isVisible() && (!(!checkSelected[0] -> icon(10).isNull() && filespassprotected))) {
        metaWidget -> setAudioControl(false);
        //if the file is an image we make a preview
        QString itemPath = table -> rebuildFullPath(checkSelected[0]);
        QProcess imagepreview;
        if (compressor == "rar") {
          QStringList options;
          options << "p" << "-inul";
          if (!archivePassword.isEmpty()) options << "-p" + archivePassword;
          imagepreview.start(compressor, options << archive << itemPath);
        }
        if (compressor == "zip") imagepreview.start("unzip", QStringList() << "-p" << "-qq" << archive << itemPath);
        if (compressor == "tar") imagepreview.start(compressor, QStringList()<< "-xOf" << archive <<itemPath);
        imagepreview.waitForFinished();
        imagepreview.terminate();
        QByteArray preview = imagepreview.readAllStandardOutput();
        metaWidget -> setPreview(preview);
      }

      else if ((checkSelected[0] -> text(9).contains("audio")) && metaWidget -> isVisible() && (!(!checkSelected[0] -> icon(10).isNull() && filespassprotected))) {
        metaWidget -> setAudioControl(true);
        QString itemPath = table -> rebuildFullPath(checkSelected[0]);
        QProcess audiopreview;
        if (compressor == "rar") {
          QStringList options;
          options << "p" << "-inul";
          if (!archivePassword.isEmpty()) options << "-p" + archivePassword;
          audiopreview.start(compressor, options << archive << itemPath);
        }
        if (compressor == "zip") audiopreview.start("unzip", QStringList() << "-p" << "-qq" << archive << itemPath);
        if (compressor == "tar") audiopreview.start(compressor, QStringList()<< "-xOf" << archive <<itemPath);
        audiopreview.waitForFinished(1000);
        //audiopreview.terminate();
        QByteArray preview = audiopreview.readAllStandardOutput();
        //QIODevice preview = audiopreview
        metaWidget -> setAudio(preview);
      }
      else metaWidget -> setAudioControl(false);
    }
    else {
      int folders = 0;
      int files = 0;
      for (int i = 0; i < checkSelected.size(); i++) {
        if (checkSelected[i] -> text(1).isEmpty()) folders ++;
        else files++;
      }
      metaWidget -> setFileName (QString().setNum (folders) + " " +i18n("folder(s)") + "\n" + QString().setNum(files) + " " + i18n("file(s)"));
      metaWidget -> handleItemSelections(checkSelected); //testing multiple files selections
      sumSelectedItemsSize();
    }
  }
  //else metaWidget -> setFileName(i18n("No item(s) selected"));
}

void MainWindow::sumSelectedItemsSize()
{
  QStringList sizes;
  QList<QTreeWidgetItem*> selectedItems = table -> selectedItems();
  QList<double> rawSizeSum;
  for ( int i = 0; i < selectedItems.size(); i++ ) { 
     //acquisisco le dimensioni con le opportune conversioni
    if ( selectedItems[i] -> text ( 1 ) != "" ) {
      sizes = ( selectedItems[i] -> text ( 1 ) ).split ( " " );
      if ( sizes[1] == "KiB" ) rawSizeSum << ( sizes[0].toFloat() * 1024 );
      else if ( sizes[1] ==  "MiB" ) rawSizeSum << ( sizes[0].toFloat() * 1048576 );
      else if ( sizes[1] == "GiB" ) rawSizeSum << ( sizes[0].toFloat() * 1073741824 );
      else rawSizeSum << sizes[0].toFloat();
    }  
  }
  double result = 0;
  for ( int j=0; j < rawSizeSum.size(); j++ ) {
    result = result + rawSizeSum[j];
  }
  metaWidget -> setFileSize( KLocale("").formatByteSize(result));
}

void MainWindow::getMetaInfo (QTreeWidgetItem *item) //setta le informazioni della metabar
{
  if(!item -> text(1).isEmpty()) {
    metaWidget -> setFileName (item -> text (0));
    metaWidget -> setFileSize (item -> text (1));
    if (compressor == "rar") metaWidget-> setDateTime((item->data(4, Qt::UserRole)).toDateTime());
    else metaWidget-> setDateTxt((item -> text(4)));
    if (compressor != "tar") metaWidget -> setRatio(item->text(3).remove("%").toFloat());
  }
  else
    metaWidget -> setFileName(item -> text(0), true );
}

void MainWindow::embeddedViewer()
{
  if (table -> selectedItems().size() == 1) {
    QTreeWidgetItem *toView = table -> selectedItems()[0];
    if (!toView -> text ( 1 ).isEmpty() && !(!toView -> icon(10).isNull() && filespassprotected))  { //&& toView -> icon(10).isNull()) {
      QString path = table -> rebuildFullPath(toView);
      QProcess view;
      if (compressor == "rar") {
        QStringList options;
        options << "p" << "-inul";
        if (!archivePassword.isEmpty()) options << "-p" + archivePassword;
        view.start (compressor, options << archive << path);
      }
      if (compressor == "zip") view.start("unzip", QStringList() << "-p" << "-qq" << archive << path); //<< QDir::tempPath());
      if (compressor == "tar") view.start(compressor, QStringList() << "-xOf" << archive << path);
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
    else if (!toView -> text ( 1 ).isEmpty()) {
      tip->setTip(i18n("Cannot preview a password-protected file"));
      tip->show();
    }
  }
  else {
    tip->setTip(i18n("Only one item at time can be viewed"));
    tip->show(); 
  }
}

void MainWindow::openItemUrl(QTreeWidgetItem *toOpen, int) //apriamo l'elemento con la relativa applicazione associata
{
  if (!toOpen -> text(1).isEmpty() && !(!toOpen -> icon(10).isNull() && filespassprotected)) {
    QString tempPath = QDir().tempPath();
    if(!tempPath.endsWith(QDir().separator())) tempPath.append(QDir().separator()); //controlliamo che la stringa termini con un separator
    QString fileToExtract = table -> rebuildFullPath(toOpen);
    QFile tempFile(tempPath + fileToExtract);
    if(!tempFile.exists()) {
      if (compressor == "rar") {
        //rarProcess *rarprocess;
        QStringList options;
        options << "x" << "-o+";
        if (!archivePassword.isEmpty()) options << "-p" + archivePassword;
        QList<QStringList> list;
        list[0] << fileToExtract;
        rarprocess = new rarProcess(0, "rar", options, archive, list, tempPath); //estraiamo il file nella cartella temporanea
        rarprocess -> start();
      }
      else if (compressor == "zip") {
        //zipProcess *zipprocess;
        QStringList options;
        options << "-q" << "-o";
        zipprocess = new zipProcess(this, "unzip", options, archive, QStringList() << fileToExtract, tempPath);
        zipprocess -> start();
      }
      else if (compressor == "tar") {
        //tarProcess *tarprocess;
        QStringList options;
        options << "--overwrite" << "-xf";
        options << archive << fileToExtract;
        QProcess *tmpprocess = new QProcess();
        tmpprocess -> setWorkingDirectory(QString(tempPath));
        tmpprocess -> start("tar", options);
        tmpprocess -> waitForFinished();
      }
    }
  
    KUrl url(tempPath + fileToExtract);
    KMimeType::Ptr mimetemp;
    mimetemp = KMimeType::findByUrl(url);    
    KRun::runUrl(url, mimetemp -> name(), this, false);
    tmpFiles << tempFile.fileName();
  }

  else if (!toOpen -> text ( 1 ).isEmpty()) {
    tip->setTip(i18n("Cannot operate with a password-protected file"));
    tip->show();
  }
}

void MainWindow::extractArchive()
{
  extractDialog *exdialog;
  QList<QStringList> filesList = table -> filesToExtract();
  QStringList options;
  kDebug() << filesList;
  if (!archivePassword.isEmpty()) options << "-p" + archivePassword;
  if (!filesList.isEmpty()) exdialog = new extractDialog (compressor, archive, filesList, options, this);
  else exdialog = new extractDialog (compressor, archive, QList<QStringList>(), options, this);
  connect(exdialog, SIGNAL(processCompleted(bool)), this, SLOT(operationCompleted(bool)));
}

void MainWindow::operationCompleted(bool value)
{
  if (value) {
    tip->setTip(i18n("Operation completed"));
    tip->show();
  }
  else {
    tip->setTip(i18n("One or more errors occurred"));
    tip->show();
  }
  setCursor(QCursor());
  enableActions(true);
  openUrl(archive);
}

void MainWindow::extractionCompleted(bool value)
{
  if (value) {
    tip->setTip(i18n("Extraction complete"));
    tip->show();
  }
  else {
    tip->setTip(i18n("One or more errors occurred"));
    tip->show();
  }
  setCursor(QCursor());
  enableActions(true);
}

void MainWindow::addComment()
{ 
  dockComment -> setVisible(false);
  akuComment *dialog = new akuComment(editComment -> toPlainText(), this);
  connect (dialog, SIGNAL (okClicked(QString)), this, SLOT(insertComment(QString)));
  dialog -> exec();
}

void MainWindow::insertComment(QString newcomment)
{ 
  QStringList options;
  if (compressor == "rar") {
    KTemporaryFile temptxt;
    if (temptxt.open()) {
      temptxt.write(newcomment.toUtf8());
      temptxt.waitForBytesWritten(-1);
      temptxt.flush();
      options << "c" << "-z" + temptxt.fileName();
      if (!archivePassword.isEmpty()) options << "-p" + archivePassword;
      rarProcess *rarprocess = new rarProcess(this, compressor, options, archive);
      connect(rarprocess,SIGNAL(processCompleted(bool)), this, SLOT(operationCompleted(bool)));
      rarprocess->start();
    }
    temptxt.close();
  }
    
  else if (compressor == "zip") {
    options << "-z" << newcomment;
    zipProcess *zipprocess = new zipProcess(this, compressor, options, archive);
    connect(zipprocess,SIGNAL(processCompleted(bool)), this, SLOT(operationCompleted(bool))); 
    zipprocess -> start();
  }

}

void MainWindow::selectionInverted()
{
  QList<QTreeWidgetItem*> itemsTounselect = table -> selectedItems();
  table -> selectAll();
  for ( int u = 0; u < itemsTounselect.size(); u++ ) {
    table -> setItemSelected ( itemsTounselect[u], false );
    QTreeWidgetItem *tmp = ( itemsTounselect[u] )-> parent();
    while ( tmp != NULL ) {  //questo while evita di selezionare i padri degli elementi da lasciare unselected
      table -> setItemSelected (tmp, false);
      tmp = tmp -> parent();
    }
  }
}

void MainWindow::renameItem()
{
  QList<QTreeWidgetItem*> selectedItems = table -> selectedItems();
  if (selectedItems.size() > 1) {
    tip->setTip(i18n("Only one item at time can be renamed"));
    tip->show();
  }

  else {
    //tempForRename stores item brothers to avoid same naming under same parent
    if ( selectedItems[0] -> parent() != NULL )
      for ( int i = 0; i < selectedItems[0] -> parent() -> childCount(); i++ )
        tempForRename << selectedItems[0] -> parent() -> child ( i ) -> text ( 0 );
    else 
      for ( int i = 0; i < table -> topLevelItemCount(); i++ )
        tempForRename << table -> topLevelItem ( i ) -> text ( 0 );
    oldItemName = selectedItems[0]-> text ( 0 );
    //oldItemPath = rebuildPath ( selectedItems[0] );
    oldItemPath = table -> rebuildFullPath(selectedItems[0]);
    selectedItems[0] -> setFlags ( Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled );
    table -> editItem ( selectedItems[0], 0 );
    connect (table, SIGNAL (itemChanged (QTreeWidgetItem *, int)), this, SLOT (renameProcess(QTreeWidgetItem* , int )));
  }
}

void MainWindow::renameProcess (QTreeWidgetItem *current, int)
{
  currentToRename = current;
  disconnect(table, SIGNAL (itemChanged(QTreeWidgetItem *, int)), this, SLOT (renameProcess(QTreeWidgetItem*, int)));
  rarProcess *renameProcess;
  if (oldItemName != current -> text(0) && !tempForRename.contains(current->text(0))) {
    kDebug() << "rename accepted";
    QStringList options;
    setCursor(Qt::WaitCursor);
    if (compressor == "rar") {
      options << "rn";
      if (!archivePassword.isEmpty()) options << "-p" + archivePassword;
      QList<QStringList> list;
      list[0] << oldItemPath << table -> rebuildFullPath(current);
      renameProcess = new rarProcess(this, "rar", options, archive, list);
      connect(renameProcess, SIGNAL(processCompleted(bool)), this, SLOT(renameCompleted(bool)));
      renameProcess -> start();     
    }
    else if (compressor == "zip") {
      // zip è assurdo nel rinominare i file
      KTemporaryFile readtxt;
      KTemporaryFile writetxt;
        
      if (readtxt.open() && writetxt.open()) {
        QProcess *process = new QProcess();
        process -> setStandardOutputFile(readtxt.fileName());
        process -> start("zipnote", QStringList() << archive); 
        process -> waitForFinished();
        readtxt.flush();
        QByteArray zipstructure = readtxt.readAll();
        int start = zipstructure.indexOf(oldItemPath);
        int line = zipstructure.indexOf("\n", start);
        zipstructure.insert(line + 1, "@=" + table -> rebuildFullPath(current) + "\n");
        
        writetxt.write(zipstructure);
        writetxt.waitForBytesWritten(-1);
        writetxt.flush();
        QProcess *writeprocess = new QProcess();
        writeprocess -> setStandardInputFile(writetxt.fileName());
        writeprocess -> start("zipnote",  QStringList() << "-w" << archive);
        writeprocess -> waitForFinished();
        int exitcode = writeprocess -> exitCode();
        if (exitcode == 0) renameCompleted(true);
        else renameCompleted(false);
      }
      readtxt.close();
      writetxt.close();
    }
    setCursor(QCursor());
  }
  else {
    current -> setText(0, oldItemName);
    tip->setTip(i18n("Rename denied"));
    tip->show();
  }
}

void MainWindow::renameCompleted(bool ok)
{
  if (!ok) {
    currentToRename -> setText(0, oldItemName);
    tip->setTip(i18n("Cannot rename the file"));
    tip->show();
  }
  else {
    tip->setTip(i18n("File renamed"));
    tip->show();
  }
  tempForRename.clear();
}

void MainWindow::lockArchive()
{
  // 5 = Continue
  // 2 = Cancel
  if (KMessageBox::warningContinueCancel(this, i18n("Locking disables archive modifications"), i18n("Lock archive")) == 5) {
    QStringList options;
    options << "k";
    if(!archivePassword.isEmpty()) options << "-p" + archivePassword;
    rarprocess = new rarProcess(this, "rar", options, archive);
    connect(rarprocess,SIGNAL(processCompleted(bool)), this, SLOT(operationCompleted(bool)));
    rarprocess -> start();
  }
}

void MainWindow::quit()
{
   close();
}

void MainWindow::addFile()
{
  bool headerpass = false;
  if (!archivePassword.isEmpty()) headerpass = true;
  akuAddFileDialog *chooseFile = new akuAddFileDialog(this, headerpass);
  connect(chooseFile, SIGNAL(destination(QStringList, QString)), this, SLOT(addFileOperation(QStringList, QString)));
  if ((table -> selectedItems().size() == 1) && (table -> selectedItems().first() -> parent() !=NULL)) {
    if (table -> selectedItems().first() -> text(1) == "") 
      chooseFile -> setCaption(i18n("Add file(s) under") + " " + table -> selectedItems().first() -> text(0));
    else
      chooseFile -> setCaption(i18n("Add file(s) under") + " " + table -> selectedItems().first() -> parent() -> text(0));
  }
  else
    chooseFile -> setCaption(i18n("Add file(s)"));

  chooseFile -> show();
}

void MainWindow::addFileOperation(QStringList list, QString filesPassword)
{
  QString parentFolder = "";
  QStringList options;
  
  if (table -> selectedItems().size() == 1) {
    if(table -> selectedItems()[0] -> text(1) == "")
      parentFolder = table -> rebuildFullPath(table -> selectedItems()[0]);
    else {
      if (table -> selectedItems()[0] -> parent() != NULL)
        parentFolder = table -> rebuildFullPath(table -> selectedItems()[0] -> parent());
    }
  }
   
  kDebug() << parentFolder;  
  
  enableActions(false);
   
  if (compressor == "rar") {
    QStringList options;
    options << "a" << "-ep1";
    if (!parentFolder.isEmpty()) options << "-ap" + parentFolder;
    if (!archivePassword.isEmpty()) options << "-p" + archivePassword;
    else if (!filesPassword.isEmpty()) options << "-p" + filesPassword;
    rarProcess *process;
    // ep1           Exclude base directory from names 
    // ap<path>      Set path inside archive
    QList<QStringList> fileslist;
    fileslist[0] << list;
    process = new rarProcess(this, "rar", options, archive, fileslist);
    connect(process, SIGNAL(processCompleted(bool)), this, SLOT(operationCompleted(bool)));
    process -> start();
  }
  
}

void MainWindow::addDirOperation(KUrl url)
{
  if (!url.isEmpty()) {
    QString dir = url.pathOrUrl();
    QString parentFolder = "";
    QStringList options;

    if (table -> selectedItems().size() == 1) {
      if(table -> selectedItems()[0] -> text(1) == "")
        parentFolder = table -> rebuildFullPath(table -> selectedItems()[0]);
      else {
        if (table -> selectedItems()[0] -> parent() != NULL)
          parentFolder = table -> rebuildFullPath(table -> selectedItems()[0] -> parent());
      }
    }
 
    kDebug() << parentFolder;  
  
    enableActions(false);
   
    if (compressor == "rar") {
      QStringList options;
      options << "a" << "-ep1";
      if (!parentFolder.isEmpty()) options << "-ap" + parentFolder;
      if (!archivePassword.isEmpty()) options << "-p" + archivePassword;
      rarProcess *process;
      // ep1           Exclude base directory from names 
      // ap<path>      Set path inside archive
      QList<QStringList> list;
      list[0] << dir;
      process = new rarProcess(this, "rar", options, archive, list);
      connect(process, SIGNAL(processCompleted(bool)), this, SLOT(operationCompleted(bool)));
      process -> start();
    }
  }
}

void MainWindow::addDir()
{ 
  quickExtract *chooseDir = new quickExtract(archive, "addDir", this);
  connect(chooseDir, SIGNAL(destination(KUrl)), this, SLOT(addDirOperation(KUrl)));

  if ((table -> selectedItems().size() == 1) && (table -> selectedItems().first() -> parent() !=NULL)) {
    if (table -> selectedItems().first() -> text(1) == "") 
      chooseDir -> setCaption(i18n("Add folder under") + " " + table -> selectedItems().first() -> text(0));
    else
      chooseDir -> setCaption(i18n("Add folder under") + " " + table -> selectedItems().first() -> parent() -> text(0));
  }
  else
    chooseDir -> setCaption(i18n("Add folder"));

  chooseDir -> show();
}

void MainWindow::deleteFile()
{
  //assicuriamoci di deselezionare tutti i figli dei padri selezionati inquanto saranno sicuramente rimossi
  QList<QTreeWidgetItem*> selectedItems = table -> selectedItems();
  for (int i = 0; i < selectedItems.size(); i++)
    if (selectedItems[i] -> childCount() != 0) 
  //se ha dei figli li deselezioniamo
     for (int j = 0; j < selectedItems[i] -> childCount(); j++) 
       table -> setItemSelected ( selectedItems[i] -> child (j), false);
  
  selectedItems = table -> selectedItems(); //ricarichiamo la lista degli elementi selezionati
  //recuperiamo i percorsi dalla lista
  QStringList itemsToDelete;
  QStringList topLevel; //solo i nomi dei file

  //memorizziamo i nomi dei file di toplevel
  for (int j = 0; j < table -> topLevelItemCount(); j++)
    topLevel << table -> topLevelItem (j) -> text(0);

  deleteAll = false;

  //ricostruiamo i percorsi dei file da eliminare
  for (int i = 0; i < selectedItems.size(); i++) {
    QString path = table -> rebuildPath (selectedItems[i]);
    if (!path.isEmpty())
      itemsToDelete << path + QDir::separator() + selectedItems[i] -> text(0);
    else
      itemsToDelete << selectedItems[i] -> text(0);
  }
  if (KMessageBox::questionYesNo (this, i18n("Are you sure you want to delete the selected file(s)?"), i18n("Deleting items")) == KMessageBox::Yes ) {
    if (topLevel == itemsToDelete) {  //se gli elementi da eliminare coincidono con tutti i toplevel allora stiamo eliminando l'archivio intero
      KMessageBox::information (this, i18n("You chose to remove every item from this archive. The entire archive file will be removed."), i18n("Deleting items"));
      deleteAll = true;
    }
    enableActions(false);
    setCursor(Qt::WaitCursor);
    QStringList options;
    if (compressor == "rar") {
      rarProcess *deleteItem;
      options << "d";
      if(!archivePassword.isEmpty()) options << "-p" + archivePassword;
      QList<QStringList> list;
      list[0] << itemsToDelete;
      deleteItem = new rarProcess(this, "rar", options, archive, list);
      connect(deleteItem, SIGNAL(processCompleted(bool)), this, SLOT(completeDelete(bool)));  
      deleteItem -> start();
    }
    else if (compressor == "zip") {
      zipProcess *deleteItem;
      // -d = delete; -r = recursive directory
      options << "-d" << "-r";
      deleteItem = new zipProcess(this, "zip", options, archive, itemsToDelete);
      connect(deleteItem, SIGNAL(processCompleted(bool)), this, SLOT(completeDelete(bool)));  
      deleteItem -> start();
    }
    else if (compressor == "tar") {
      tarProcess *deleteItem;
      options << "--delete" << "-f";
      deleteItem = new tarProcess(this, "tar", options, archive, itemsToDelete);
      connect(deleteItem, SIGNAL(processCompleted(bool)), this, SLOT(completeDelete(bool)));  
      deleteItem -> start();
    }
  }
}

void MainWindow::completeDelete(bool ok)
{
  if (ok) {
    if (deleteAll == false ) operationCompleted(true);
    else {
      table -> clear();
      setCaption(QString());
      enableActions(false);
      tip->setTip(i18n("Archive deleted"));
      tip->show();
      if ((compressor == "zip") || (compressor == "tar")) QFile::remove(archive);
      
      archive.clear();
      setCursor(QCursor());
    }
  }
  else operationCompleted(false);
}

// raccoglie i file temporanei (al momento solo audio) creati da aku
void MainWindow::collectTempFiles(QString tempFileName)
{
  tmpFiles << tempFileName;
}
