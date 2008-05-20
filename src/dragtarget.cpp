
#include "dragtarget.h"
#include <QMessageBox>
#include "rar.h"

QAction *newFolder;
QAction *delItem;
QMenu *menu;

dragTarget::dragTarget (dragSource *source, QWidget *parent ) : QTreeWidget ( parent )
{
  //let's initialize the dropIndicator
  indicator = new QWidget(viewport());
  indicator -> setAutoFillBackground(true);
  QPalette pale = indicator -> palette();
  pale.setColor(QPalette::Base, Qt::blue);
  indicator -> setPalette(pale);
  hideIndicator();
  //let's initialize some pointers...
  sourceView = source;
  scrollUpTimer = new QTimer();
  scrollDownTimer = new QTimer();

  indicatorToRestore = NULL;
  //setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
  //...and some list settings
  subLevel = false;
  setAcceptDrops ( true );
  setDragEnabled ( true );
  setAutoScroll(true);
  setDragDropMode ( QAbstractItemView::DragDrop );
  setColumnCount ( 4 );
  setHeaderLabels ( QStringList() <<tr ( "Item" ) <<tr ( "Path" ) <<tr ( "MimeType" ) <<tr ( "Size" ) );
  header() -> moveSection ( 3,1 );
  setColumnWidth ( 0,170 );
  setDropIndicatorShown ( true );
  //setAnimated ( true );

  newFolder = new QAction ( tr ( "New Folder" ), this );
  newFolder -> setIcon ( KIcon ( "folder-new" ) );
  //newFolder -> setShortcut(QKeySequence(tr("Ctrl+A")));
  //this -> addAction(newFolder);
  delItem = new QAction ( tr ( "Delete Item" ), this );
  delItem -> setIcon ( KIcon ( "edit-delete" ) );
  //delItem -> setShortcut(QKeySequence(tr("Del"))); //TODO check!
  //this -> addAction(delItem);
  //this-> setContextMenuPolicy(Qt::DefaultContextMenu); //se non imposto defaultcontextmenu non posso reimplementare la gestione del contextmenu

  //creo il menu
  menu = new QMenu ( tr ( "Options" ),this );
  menu -> addAction ( newFolder );
  menu -> addAction ( delItem );
  connect ( newFolder, SIGNAL ( triggered() ), this, SLOT ( createNewFolder() ) );
  connect ( delItem, SIGNAL ( triggered() ), this, SLOT ( deleteItem() ) );
  delItem -> setEnabled ( false );
  setItemsExpandable ( true );
  setSelectionMode(QAbstractItemView::ExtendedSelection);
  //setSelectionMode(QAbstractItemView::ExtendedSelection); //inizialmente viene gestita solo la selezione singola

}

dragTarget::~dragTarget() 
{
}

void dragTarget::hideIndicator()
{
  indicator -> hide();
}

void dragTarget::showIndicator(int y )
{
  indicator -> setGeometry(0, y, width(), 2);
  indicator -> show();
}

void dragTarget::dragEnterEvent ( QDragEnterEvent *event ) //everything starts from here
{
  if ( event -> mouseButtons() == Qt::LeftButton )
  {
    if ( event -> mimeData() -> hasFormat ( "aku/newarchive" ) || event -> mimeData() -> hasFormat ( "aku/newarchive-fromHere" ) )
      event -> acceptProposedAction();
  }
  else event -> ignore();
}

void dragTarget::recursiveAppend ( QTreeWidgetItem *source, QTreeWidgetItem *destination )
{
  QTreeWidgetItem *tmp = destination;
  for ( int i = 0; i < source -> childCount(); i++ )
  {
    tmp = ramifica ( source ->child ( i ) -> text ( 1 ) + source ->child ( i ) -> text ( 0 ) + "!*mimetosend:" + source ->child ( i ) -> text ( 2 ), destination );
    recursiveAppend ( source -> child ( i ), tmp );
  }
}

void dragTarget::mouseReleaseEvent(QMouseEvent *e)
{
  puts("mouseReleaseEvent");
  if(indicatorToRestore != NULL)
  {
    delete indicatorToRestore; 
    indicatorToRestore = NULL;
  } 
  hideIndicator();
  e -> accept();
}

void dragTarget::dropEvent ( QDropEvent *event )
{
  //-------ripristiniamo a 0 gli indicatori--------------//
  puts("dropEvent deleting indicator");
  if(indicatorToRestore != NULL)
  {
    delete indicatorToRestore;
    indicatorToRestore = NULL;
  } //cancelliamo l'indicatore
  //--------------------------------------------------------//
  puts("indicatore cancellato");
  hideIndicator();
  puts("nascondo indicatore");
  if ( subLevel == false )  
  {
  //evito la duplicazione di uno stesso elemento sotto uno stesso ramo
    puts("inizio a ramificare");
    QTreeWidgetItem *ultimo;
    if(event -> mimeData()->hasFormat("aku/newarchive"))
      ultimo = ramifica ( QString().fromAscii ( event -> mimeData() -> data ( "aku/newarchive" ) ),this); 
    else
      ultimo = ramifica ( QString().fromAscii ( event -> mimeData() -> data ( "aku/newarchive-fromHere" ) ),this);
    bool allow = false;
    for ( int i = 0; i < topLevelItemCount(); i++ )
    {
      if ( topLevelItem ( i ) -> text ( 1 ) + topLevelItem ( i ) -> text ( 0 ) == ultimo -> text ( 1 ) + ultimo -> text ( 0 ) && topLevelItem ( i ) != ultimo )   //se esiste già nego la possibilità di allocare l'elemento
      {
        delete ultimo;
        allow = false;
        break;
      }
      else allow = true;
    }
    if ( allow == true )
    {
      //tramite la funzione retrieveChildren appendiamo anche tutti i figli della directory
      if(event -> mimeData() -> hasFormat("aku/newarchive-fromHere"))
      {
        //se selectedItems e' diverso da 0 sicuramente stiamo trascinando un elemento di questa stessa lista, quindi devo procedere al recupero di tutti i suoi figli prima di eliminarlo
        for(int i = 0; i < currentItem -> childCount(); i++)
        {
          //mi tocca simulare un mime per poter trascinare anche i figli
          QTreeWidgetItem *item = ramifica ( currentItem-> child ( i ) -> text ( 1 ) + currentItem -> child ( i ) -> text ( 0 ) + "!*mimetosend:" + currentItem -> child ( i ) -> text ( 2 ), ultimo);
          recursiveAppend ( currentItem -> child ( i ), item ); //passo come destinazione l'item appena creato
        }
        delete currentItem;
        event -> acceptProposedAction();
      }
      else retrieveChildren ( ultimo ); //altrimenti recuperiamo i figli dalle directory, perchè significa che il trascinamento avviene dall'esterno
      event -> acceptProposedAction();
    }
    else
    {
      event -> ignore();
      return;
    }

  }
  else
  {
    //succede esattamente la stessa cosa di quello precedente, solo che con la ramificazione
    QList<QTreeWidgetItem*> selectedItems = this -> selectedItems();
    QTreeWidgetItem *ultimo;
    if(event -> mimeData()->hasFormat("aku/newarchive"))
      ultimo = ramifica ( QString().fromAscii ( event -> mimeData() -> data ( "aku/newarchive" ) ), padre);
    else ultimo = ramifica ( QString().fromAscii ( event -> mimeData() -> data ( "aku/newarchive-fromHere" ) ), padre);
    bool allow = false;
    for ( int i = 0; i < padre -> childCount(); i++ )
    {
      if ( padre -> child ( i ) -> text ( 1 ) + padre -> child ( i ) -> text ( 0 ) == ultimo -> text ( 1 ) + ultimo -> text ( 0 ) && padre -> child ( i ) != ultimo )   //se esiste già
      {
        delete ultimo;
        allow = false;
        break;
      }
      else allow = true;
    }
    if ( allow == true )
    {
      padre -> setExpanded ( true );
      if ( selectedItems.size() != 0 )   //se stiamo trascinando un elemento della stessa lista ci riportiamo i figli
      {
        for ( int i = 0; i < selectedItems[0] -> childCount(); i++ )
        {
          //mi tocca simulare un mime per poter trascinare anche i figli
          QTreeWidgetItem *item = ramifica ( selectedItems[0] -> child ( i ) -> text ( 1 ) + selectedItems[0] -> child ( i ) -> text ( 0 ) + "!*mimetosend:" + selectedItems[0] -> child ( i ) -> text ( 2 ), ultimo );
          recursiveAppend ( selectedItems[0] -> child ( i ), item );
        }
        delete selectedItems[0];
      }
      else retrieveChildren ( ultimo ); //altrimenti recuperiamo i figli dalle directory
      event -> acceptProposedAction();
    }
    else
    {
      event -> ignore();
      return;
    }

  }
}

void dragTarget::scrollDown()
{
  disconnect(scrollDownTimer, SIGNAL(timeout()), this, SLOT(scrollDown()));
  puts("scrollDown chiamato");
  this -> scrollToItem(tempScroll);
  tempScroll = itemBelow(tempScroll);
  connect(scrollDownTimer, SIGNAL(timeout()), this, SLOT(scrollDown()));
  if(tempScroll == NULL)
  {
    disconnect(scrollDownTimer, SIGNAL(timeout()), this, SLOT(scrollDown()));
    scrollDownTimer -> stop();
  }
}

void dragTarget::scrollUp()
{
  disconnect(scrollUpTimer, SIGNAL(timeout()), this, SLOT(scrollUp()));
  puts("scrollUp chiamato");
  this -> scrollToItem(tempScroll);
  tempScroll = itemAbove(tempScroll);
  connect(scrollUpTimer, SIGNAL(timeout()), this, SLOT(scrollUp()));
  if(tempScroll == NULL)
  {
   disconnect(scrollUpTimer, SIGNAL(timeout()), this, SLOT(scrollUp()));
   scrollUpTimer -> stop();
  }
}

void dragTarget::dragMoveEvent ( QDragMoveEvent *event )
{
  if ( event -> mouseButtons() == Qt::LeftButton )
  {
    //eseguiamo lo scrollDown se ci troviamo alla fine della lista
    if(event -> pos().y() >viewport() -> height() - 50 && event -> pos().y() <viewport() -> height()) //se ci troviamo alla fine della lista iniziamo le operazioni di scroll automatico
    { 
      tempScroll = itemAt(event -> pos());
      connect(scrollDownTimer, SIGNAL(timeout()), this, SLOT(scrollDown()));
      scrollDownTimer -> start(100);
    }
    else
    {
      disconnect(scrollDownTimer, SIGNAL(timeout()), this, SLOT(scrollDown()));
      scrollDownTimer -> stop();
    }
    //eseguiamo lo scrollUp se ci troviamo all'inizio della lista
    if(event -> pos().y() < viewport() -> y() + 20 && event -> pos().y() > 0) //se ci troviamo all'inizio della lista iniziamo le operazioni di scroll automatico
    { 
      tempScroll = itemAt(event -> pos());
      connect(scrollUpTimer, SIGNAL(timeout()), this, SLOT(scrollUp()));
      scrollUpTimer -> start(100);
    }
    else
    {
      disconnect(scrollUpTimer, SIGNAL(timeout()), this, SLOT(scrollUp()));
      scrollUpTimer -> stop();
    }
    if(itemAt(event -> pos()) != NULL)
    {
      QTreeWidgetItem *current = itemAt(event -> pos() );
      showIndicator(visualItemRect(current).bottom());
      if(indicatorToRestore != NULL && itemAt(event -> pos()) != indicatorToRestore)
      {
        delete indicatorToRestore; 
        indicatorToRestore = NULL;
      } 
    }
    else
    {
      if(indicatorToRestore != NULL) delete indicatorToRestore;
      indicatorToRestore = new QTreeWidgetItem(this);
      scrollToItem(indicatorToRestore);
      showIndicator(visualItemRect(topLevelItem(topLevelItemCount()-1)).bottom());
    }
  //------------------------------------------iniziano le operazioni di allocazione-----------------------------------------------------------------------------//

    QList<QTreeWidgetItem*> selectedItems = this -> selectedItems(); //mi serve per accertarmi che l'utente non voglia trascinare un file sul file stesso

    if ( selectedItems.size() == 0 )   //se non ci sono elementi selezionati
    {
      //*******************************************************************//
      if ( itemAt ( event -> pos() ) != NULL && itemAt ( event -> pos() ) -> text ( 2 ) == "inode/directory" )   //se voglio trascinare su un altro elemento che è una cartella, acconsento, TODO ma solo se la cartella non contiene già un elemento uguale
      {
        subLevel = true;
        padre = itemAt ( event -> pos() );
        event -> acceptProposedAction();
      }
      else
      {
        if ( itemAt ( event -> pos() ) != NULL && itemAt ( event -> pos() ) -> text ( 2 ) !="inode/directory" )
        {
          if(itemAt( event -> pos() ) -> text(0) == "")
          { //possiamo trascinare sull'indicatore
            subLevel = false;
            event -> acceptProposedAction();
          }
          else 
            event -> ignore();//se voglio trascinare su un elemento che non è una cartella, nego
        }
        else  //se voglio semplicemente trascinare un nuovo elemento acconsento
        {
          subLevel = false;
          event -> acceptProposedAction();
        }
      }
    //*************************************************************//
    } //fine dell'if selectedItems.size()

    else  //se ci sono elementi selezionati mi accerto di non trascinare su me stesso e di non trascinarmi in un mio figlio
    {
      if ( itemAt ( event -> pos() ) != NULL && itemAt ( event -> pos() ) -> text ( 2 ) == "inode/directory" && itemAt ( event -> pos() ) != selectedItems[0] )   //se voglio trascinare su un elemento che è una cartella e non è l'elemento stesso
      {
        QString nomeFile; //recupero il nome file dell'elemento per accertarmi di non trascinarmi in un mio figlio
        //QStringList formati = event -> mimeData() -> formats();
        if(event -> mimeData()->hasFormat("aku/newarchive"))
          nomeFile = event -> mimeData()->data ( "aku/newarchive" );
        else nomeFile = event -> mimeData()->data("aku/newarchive-fromHere");
        QStringList path = nomeFile.split ( QDir().separator() );
        int target = path[path.size()-1].indexOf ( "!*mimetosend:" );
        path[path.size()-1].remove ( target, path[path.size()-1].length() );
        QString temp = path[path.size()-1]; //nome del parent (directory)
        bool checkHasSameParent = hasSameParent ( itemAt ( event -> pos() ), temp ); //con questo evito di trascinare un parent in un suo figlio
        if ( checkHasSameParent == false )
        {
          subLevel = true;
          padre = itemAt ( event -> pos() );
          event -> acceptProposedAction();
        }
        else
        {
          event -> ignore();
          ///return;
        }
        /****************************************************************************************/
        // devo controllare di non trascinare su uno stesso parent.. perchè è inutile e rende instabile tutto :-D
        bool allowAllocation = true;
        for ( int i = 0; i < itemAt ( event -> pos() ) -> childCount(); i++ ) //se l'elemento di destinazione ha tra i suoi figli uno con lo stesso nome di quello che sto trascinando allora nego il trascinamento
        {
          if ( itemAt ( event -> pos() ) -> child ( i )->text ( 0 ) == temp ) allowAllocation = false; //se non ha un child con lo stesso nome permettiamo l'allocazione
        }
        if ( allowAllocation == true )
        {
          subLevel = true;
          padre = itemAt ( event -> pos() );
          event -> acceptProposedAction();
        }
        else
        {
        event -> ignore(); 
        ///return;
        }
      /****************************************************************************************/
      }
      else  //altrimenti
      {
        if ( itemAt ( event -> pos() ) == NULL )   //se il cursore non si trova su nessun elemento vuol dire che posso allocare nella lista
        {
          subLevel = false;
          event -> acceptProposedAction();
        }
        else
        {
          if(itemAt( event -> pos() ) -> text(0) == "")
          { //possiamo trascinare sull'indicatore
            subLevel = false;
            event -> acceptProposedAction();
          }
          else
          {
            event -> ignore(); // altrimenti ignoro
            ///return;
          }
        }

      }
    }
  }

  else  //se non è iniziato il trascinamento col tasto sinistro chiudo
  {
    event -> ignore();
    return;
   }

}


QTreeWidgetItem* dragTarget::ramifica ( QString path, QTreeWidgetItem *parent )
{
  /*se il cursore si trova sopra un elemento esistente appendiamo sotto...*/
  KIcon icon;
  //e' necessario gestire la stringa di controllo per il mime che arriva dal mime di tipo aku/newarchive
  //rimuovo la stringa di controllo dalla stringa che servir� per ricostruire il path e il nome file, e scrivo il mimeType nella colonna appropriata
  QString toPath = "/"; //su windows impostare C:\/
  int target = path.indexOf ( "!*mimetosend:" );
  mimeCheck = path;
  mimeCheck.remove ( 0,target );
  mimeCheck.remove ( "!*mimetosend:" );
  puts("mimeCheck ricavato: "+mimeCheck.toAscii());
  icon = KIcon(KMimeType::mimeType(mimeCheck) -> iconName());///KIcon(mimeCheck);
  path.remove ( target, path.length() );
  //************************************************************************************************************************************//
  if ( path.indexOf ( tr ( "*User created Folder*" ) ) == -1 )  //distinguo le cartelle normali da quelle create dall'utente (* iniziale)
  {
    QStringList folders = path.split ( QDir().separator(), QString::SkipEmptyParts );
    lastItemAdded = new QTreeWidgetItem ( parent );
    lastItemAdded -> setText ( 0, folders[folders.size()-1] );
    for ( int i=0; i<folders.size() - 1; i++ )
    {
      toPath.append ( folders[i]+QDir().separator() );
    }
  }
  else
  {
    //imposto gli attributi della cartella creata dall'utente
    toPath = tr ( "*User created Folder*" ); //questo va nel path
    path.remove ( target, path.length() );
    path.remove ( tr ( "*User created Folder*" ) );

    lastItemAdded = new QTreeWidgetItem ( parent );
    lastItemAdded -> setText ( 0, path ); //nome cartella
    mimeCheck = "inode/directory"; //mimetype
    icon = KIcon("inode-directory");
  }
  lastItemAdded -> setText ( 1, toPath );

  //scrivo le informazioni sul mime
  lastItemAdded -> setText ( 2, mimeCheck );
  lastItemAdded -> setIcon ( 0,icon );
   //scriviamo le informazioni sulla dimensione
  if ( lastItemAdded -> text ( 2 ) != "inode/directory" )   //se non è una cartella
  {
    QFile sizeInfo ( lastItemAdded -> text ( 1 ) + lastItemAdded -> text ( 0 ) );
    lastItemAdded -> setText ( 3, KLocale("").formatByteSize(sizeInfo.size()) ); //usiamo la funzione della classe rar pe la conversione della dimensione
    lastItemAdded -> setTextAlignment ( 3, Qt::AlignRight | Qt::AlignVCenter );
  }

  header() -> setResizeMode ( QHeaderView::ResizeToContents );
  return lastItemAdded;

}

QTreeWidgetItem* dragTarget::ramifica ( QString path, QTreeWidget *parent ) //overloaded function to work with toplevels
{
  puts("ramifico");
  /*se il cursore si trova sopra un elemento esistente appendiamo sotto...*/
  KIcon icon;
  //e' necessario gestire la stringa di controllo per il mime che arriva dal mime di tipo aku/newarchive
  //rimuovo la stringa di controllo dalla stringa che servir� per ricostruire il path e il nome file, e scrivo il mimeType nella colonna appropriata
  QString toPath = "/"; //su windows impostare C:\/
  int target = path.indexOf ( "!*mimetosend:" );
  mimeCheck = path;
  mimeCheck.remove ( 0,target );
  mimeCheck.remove ( "!*mimetosend:" );
  icon = KIcon(KMimeType::mimeType(mimeCheck) -> iconName());///KIcon(mimeCheck);
  path.remove ( target, path.length() );
  //************************************************************************************************************************************//
  if ( path.indexOf ( tr ( "*User created Folder*" ) ) == -1 )  //distinguo le cartelle normali da quelle create dall'utente (* iniziale)
  {
    QStringList folders = path.split ( QDir().separator(), QString::SkipEmptyParts );
    lastItemAdded = new QTreeWidgetItem ( parent );
    lastItemAdded -> setText ( 0, folders[folders.size()-1] );
    for ( int i=0; i<folders.size() - 1; i++ )
    {
      toPath.append ( folders[i]+QDir().separator() );
    }
  }
  else
  {
    //imposto gli attributi della cartella creata dall'utente
    toPath = tr ( "*User created Folder*" ); //questo va nel path
    path.remove ( target, path.length() );
    path.remove ( tr ( "*User created Folder*" ) );
    lastItemAdded = new QTreeWidgetItem ( parent );
    lastItemAdded -> setText ( 0, path ); //nome cartella
    mimeCheck =  "inode/directory" ; //mimetype
    icon = KIcon("inode-directory");
  }
  lastItemAdded -> setText ( 1, toPath );

  //scrivo le informazioni sul mime
  lastItemAdded -> setText ( 2, mimeCheck );
  lastItemAdded -> setIcon ( 0,icon );

  //scriviamo le informazioni sulla dimensione
  if ( lastItemAdded -> text ( 2 ) != "inode/directory" )   //se non è una cartella
  {
    QFile sizeInfo ( lastItemAdded -> text ( 1 ) + lastItemAdded -> text ( 0 ) );
    lastItemAdded -> setText ( 3, KLocale("").formatByteSize(sizeInfo.size()) ); //usiamo la funzione della classe rar per la conversione della dimensione
    lastItemAdded -> setTextAlignment ( 3, Qt::AlignRight | Qt::AlignVCenter );
  }

  header() -> setResizeMode ( QHeaderView::ResizeToContents );
  return lastItemAdded;
  
}

void dragTarget::mouseMoveEvent ( QMouseEvent *event ) //gestisce il trascinamento all'interno della lista stessa
{
  if ( event -> buttons() == Qt::LeftButton && itemAt ( event -> pos() ) != NULL ) //se è iniziato un trascinamento con il tasto destro inizio a creare il mime
  {
    QTreeWidgetItem *current = itemAt(event -> pos());
    showIndicator(visualItemRect(current).bottom());
    if( this -> selectedItems().size() != 0 )
    {
      int size = this -> selectedItems().size();
      QList<QTreeWidgetItem*> selectedItems = this -> selectedItems();
      for(int i = 0; i < size; i++) this -> setItemSelected(selectedItems[i], false); //deseleziona tutti gli altri elementi... evita un crash //NON SUPPORTIAMO ANCORA IL TRASCINAMENTO MULTIPLO
    }
    QDrag *drag = new QDrag ( this );
    QMimeData * mimeData = new QMimeData;
    //QList<QTreeWidgetItem*> selectedItems = this -> selectedItems();
    QTreeWidgetItem *toDrag = itemAt ( event -> pos() );
    currentItem = toDrag;
    this -> setItemSelected ( toDrag, true );
    drag -> setPixmap ( ( toDrag -> icon ( 0 ) ).pixmap ( 32,32 ) ); //recupero l'icona
    mimeData -> setImageData ( ( toDrag -> icon ( 0 ) ).pixmap ( 32,32 ) );
    QString filePath = toDrag -> text ( 1 ) + toDrag -> text ( 0 ); //ricostruisco il percorso
    filePath.append ( "!*mimetosend:"+toDrag -> text ( 2 ) );
    QByteArray mimeToSend = filePath.toAscii();
    mimeData -> setData ( "aku/newarchive-fromHere",mimeToSend );
    drag -> setMimeData ( mimeData );
    drag -> exec ( Qt::MoveAction );
  }
  else
  {
    hideIndicator();
    event -> ignore(); 
    return;
  }
}

void dragTarget::createNewFolder()
{

  if ( lastItemClicked != NULL ) //se è selezionato un elemento creo la cartella come figlia di questo elemento (il controllo che questo elemento non sia un file viene effettuato dalla funzione che abilita i context menu
  {
    //----------tramite questo controllo appendiamo un indice al nome consigliato se ci sono già delle cartelle chiamate New Folder-------//
    int count = 0;
    for ( int i = 0; i < lastItemClicked -> childCount(); i++ )
    {
      if ( ( lastItemClicked -> child ( i ) -> text ( 0 ) ).indexOf ( tr ( "New Folder" ) ) != -1 ) count++;
    }
    QString folderCount;
    QString appendCount;
    if ( count > 0 ) folderCount.append ( "_"+appendCount.setNum ( count ) );
    else folderCount = "";
    //---------------------------------------------------------------------------------------------------------------//
    QTreeWidgetItem *folder = new QTreeWidgetItem ( lastItemClicked );
    QString name;
    bool allow = false;
    while ( allow == false )
    {
      name = QInputDialog::getText ( this, tr ( "Folder Name" ), tr ( "Give a name for the new Folder" ), QLineEdit::Normal, tr ( "New Folder" ) +folderCount );
      allow = true;
      for ( int i = 0; i < lastItemClicked -> childCount(); i++ ) //tramite questo ciclo controlliamo che non ci sia già una cartella con lo stesso nome
      {
        if ( lastItemClicked -> child ( i ) -> text ( 0 ) == name )
        {
          allow = false;
          break;
        }
      }
    }
    folder -> setText ( 0, name );
    folder -> setText ( 2, "inode/directory" );
    folder -> setText ( 1, tr ( "*User created Folder*" ) );
    folder -> setIcon ( 0, KIcon ( "inode-directory" ) );
    lastItemClicked -> setExpanded ( true );

  }
  else
  {
    //----------tramite questo controllo appendiamo un indice al nome consigliato se ci sono già delle cartelle chiamate New Folder-------//
    int count = 0;
    for ( int i = 0; i < topLevelItemCount(); i++ )
    {
      if ( ( topLevelItem ( i ) -> text ( 0 ) ).indexOf ( tr ( "New Folder" ) ) != -1 ) count++;
    }
    QString folderCount;
    QString appendCount;
    if ( count > 0 ) folderCount.append ( "_"+appendCount.setNum ( count ) ); //suggeriamo un nome New Folder aggiungendo un numero di distinzione
    else folderCount = "";
    //---------------------------------------------------------------------------------------------------------------//

    QString name;
    bool allow = false;
    while ( allow == false )
    {
      name = QInputDialog::getText ( this, tr ( "Folder Name" ), tr ( "Give a name for the new Folder" ), QLineEdit::Normal, tr ( "New Folder" ) +folderCount );
      allow = true;
      if ( name == "" )
      {
        allow = false;
        break;
      }
      for ( int i = 0; i < topLevelItemCount(); i++ ) //tramite questo ciclo controlliamo che non ci sia già una cartella con lo stesso nome
      {
        if ( topLevelItem ( i ) -> text ( 0 ) == name )
        {
          allow = false;
          break;
        }
      }
    }
    if ( allow != false )
    {
      QTreeWidgetItem *folder = new QTreeWidgetItem ( this );
      folder -> setText ( 0, name );
      folder -> setText ( 2, "inode/directory" );
      folder -> setText ( 1, tr ( "*User created Folder*" ) );
      folder -> setIcon ( 0, KIcon ( "inode-directory" ) );
    }
  }

}

void dragTarget::deleteItem()
{
  int size = this -> selectedItems().size();
  QList<QTreeWidgetItem*> selectedItems = this -> selectedItems();
  for(int i = 0; i < size ; i++) delete selectedItems[i];
}



void dragTarget::focusOutEvent ( QFocusEvent *event )
{
  if ( event -> reason() == Qt::ActiveWindowFocusReason )   //quando perde il focus a causa di un cambio di finestra deseleziono gli elementi
  {
    QList<QTreeWidgetItem *> selectedItems = this -> selectedItems();
    if ( selectedItems.size() != 0 ) this -> setItemSelected ( selectedItems[0], false );
  }
}

void dragTarget::contextMenuEvent ( QContextMenuEvent * event ) //ci assicuriamo che il contextMenu NewFolder non sia disponibile su un elemento che non è una cartella
{
  QList<QTreeWidgetItem*> toUnselect = this -> selectedItems();
  if ( toUnselect.size() != 0 && itemAt ( event -> pos() ) == NULL ) this -> setItemSelected ( toUnselect[0], false ); //deselezioniamo gli elementi in modo che il contextmenu si riferisca solo alla posizione del mouse
  lastItemClicked = NULL; //azzero il puntatore
  if ( itemAt ( event -> pos() ) != NULL )
  {
    if ( itemAt ( event -> pos() ) -> text ( 2 ) != "inode/directory")
    {
      newFolder -> setEnabled ( false );
    }
    else
    {
      //if (newFolder -> isEnabled() == false)
      newFolder -> setEnabled ( true );
    }

  }
  else
    newFolder -> setEnabled ( true );
  //controllo anche di trovarmi su un elemento per abilitare il context menu Delete
  if ( itemAt ( event -> pos() ) != NULL ) delItem -> setEnabled ( true );
  else delItem -> setEnabled ( false );
  //visualizzo il menu alla posizione dove ho cliccato
  menu -> popup ( this -> mapToGlobal ( event -> pos() ) );
  lastItemClicked = itemAt ( event -> pos() ); //memorizzo l'elemento dove è stato lanciato il contextmenu
}

void dragTarget::retrieveChildren ( QTreeWidgetItem *rootItem ) //si preoccupa di allocare tutti i figli a partire dall'elemento passato
{
  QString rootPath = rootItem -> text ( 1 ) + rootItem -> text ( 0 ); //percorso completo del file
  QDir rootDir ( rootPath );
  //scansione separata, una per le directory e l'altra per i figli
  QStringList rootEntries;
  ///QDir::Filters filtri = sourceView -> filters();
  /**if(filtri.testFlag(QDir::Hidden) == true) rootEntries = rootDir.entryList (QDir::NoDotAndDotDot | QDir::AllDirs| QDir::Hidden);
  else rootEntries = rootDir.entryList ( QDir::NoDotAndDotDot |  QDir::AllDirs); //elenchiamo le cartelle figlie**/
  if(sourceView -> hiddenShown() == true)
    rootEntries = rootDir.entryList (QDir::NoDotAndDotDot | QDir::AllDirs| QDir::Hidden);
  else
    rootEntries = rootDir.entryList ( QDir::NoDotAndDotDot |  QDir::AllDirs); //elenchiamo le cartelle figlie
  for ( int i = 0; i < rootEntries.size(); i++ )
  {
    QTreeWidgetItem *child = new QTreeWidgetItem ( rootItem );
    child -> setText ( 0, rootEntries[i] );
    child -> setText ( 1, rootPath +"/" );
    child -> setText ( 2, "inode/directory" );
    child -> setIcon ( 0, KIcon ( "inode-directory" ) );
    recursivePopulate ( child );
  }
  /**if(filtri.testFlag(QDir::Hidden) == true) rootEntries = rootDir.entryList ( QDir::NoDotAndDotDot | QDir::Files |  QDir::Hidden);
  else rootEntries = rootDir.entryList (QDir::NoDotAndDotDot | QDir::Files); //elenchiamo le cartelle figlie**/
  if(sourceView ->hiddenShown() == true)
    rootEntries = rootDir.entryList ( QDir::NoDotAndDotDot | QDir::Files |  QDir::Hidden);
  else
    rootEntries = rootDir.entryList (QDir::NoDotAndDotDot | QDir::Files); //elenchiamo le cartelle figlie
  for ( int i = 0; i < rootEntries.size(); i++ )
  {
    QTreeWidgetItem *child = new QTreeWidgetItem ( rootItem );
    child -> setText ( 0, rootEntries[i] );
    child -> setText ( 1, rootPath +"/" );
    //scriviamo le informazioni sulla dimensione
    QFile sizeInfo ( child -> text ( 1 ) + child -> text ( 0 ) );
    child -> setText ( 3, KLocale("").formatByteSize(sizeInfo.size()) ); //usiamo la funzione della classe rar per la conversione della dimensione
    child -> setTextAlignment ( 3, Qt::AlignRight | Qt::AlignVCenter );
    //-----------------------------------------------------//
    //-------troviamo e impostiamo l'icona grazie al mime ricavato-----//
    //speziamo il nome del file per ricavare il mime
    //----------------------------------------------------------------------//
    QString mime = KMimeType::findByUrl(KUrl(rootEntries[i])) -> iconName();///fileItem.iconName();
    child -> setIcon ( 0, KIcon ( mime ) ); //se sono riuscito a ricavare il mime setto l'icona
    child -> setText ( 2, KMimeType::findByUrl(KUrl(rootEntries[i])) -> name() ); //scrivo il mime nella colonna appropriata
  }

}

void dragTarget::recursivePopulate ( QTreeWidgetItem *folder ) //questa funzione è molto simile a quella precedente a parte qualche differenza per la ricorsione
{
  QString folderPath = folder -> text ( 1 ) + folder -> text ( 0 );
  //QMessageBox::information(this, "DEBUG", folderPath);
  QDir folderDir ( folderPath );
  QStringList folderEntries;

  /**QDir::Filters filtri = sourceView -> filters();
  if(filtri.testFlag(QDir::Hidden) == true) folderEntries =  folderDir.entryList (QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Hidden);
  else folderEntries =  folderDir.entryList ( QDir::NoDotAndDotDot | QDir::AllDirs ); //elenchiamo le cartelle figlie**/
  if(sourceView -> hiddenShown() == true )
    folderEntries =  folderDir.entryList (QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Hidden);
  else
    folderEntries =  folderDir.entryList ( QDir::NoDotAndDotDot | QDir::AllDirs ); //elenchiamo le cartelle figlie
  //scansione delle cartelle
  QString joint = folderEntries.join ( " " );
  for ( int i = 0; i < folderEntries.size(); i++ )
  {
    QTreeWidgetItem *child = new QTreeWidgetItem ( folder );
    child -> setText ( 0, folderEntries[i] );
    child -> setText ( 1, folderPath+"/" );
    child -> setText ( 2, "inode/directory" );
    child -> setIcon ( 0, KIcon ( "inode-directory" ) );
    recursivePopulate ( child );
  }
  /**if(filtri.testFlag(QDir::Hidden) == true) folderEntries =  folderDir.entryList (  QDir::NoDotAndDotDot | QDir::Files | QDir::Hidden);
  else folderEntries =  folderDir.entryList ( QDir::NoDotAndDotDot | QDir::Files ); //elenchiamo i file figli**/
  if(sourceView -> hiddenShown() == true )
    folderEntries =  folderDir.entryList (  QDir::NoDotAndDotDot | QDir::Files | QDir::Hidden);
  else
    folderEntries =  folderDir.entryList ( QDir::NoDotAndDotDot | QDir::Files ); //elenchiamo i file figli
  for ( int i = 0; i < folderEntries.size(); i++ )
  {
    QTreeWidgetItem *child = new QTreeWidgetItem ( folder );
    child -> setText ( 0, folderEntries[i] );
    child -> setText ( 1, folderPath +"/" );
    //scriviamo le informazioni sulla dimensione
    QFile sizeInfo ( child -> text ( 1 ) + child -> text ( 0 ) );
    child -> setText ( 3, KLocale("").formatByteSize(sizeInfo.size())); //usiamo la funzione della classe rar per la conversione della dimensione
    child -> setTextAlignment ( 3, Qt::AlignRight | Qt::AlignVCenter );
    //-----------------------------------------------------//
    //-------troviamo e impostiamo l'icona grazie al mime ricavato-----//
    //speziamo il nome del file per ricavare il mime
    QString mime = KMimeType::findByUrl(KUrl(folderEntries[i])) -> iconName(); ///fileItem.iconName();
    child -> setIcon ( 0, KIcon ( mime ) ); //se sono riuscito a ricavare il mime setto l'icona
    child -> setText ( 2, KMimeType::findByUrl(KUrl(folderEntries[i])) -> name() ); //scrivo il mime nella colonna appropriata
    //----------------------------------------------------------------------//
  }
}

bool dragTarget::hasSameParent ( QTreeWidgetItem *children, QString target ) //mi assicuro che l'elemento non abbia tra i padri quello che sto trascinando (target)
{
  bool esito;
  QTreeWidgetItem *tmp = children -> parent();
  while ( tmp != NULL )
  {
   if ( tmp -> text ( 0 ) != target )
   {
     esito = false; //se è diverso diciamo che non ha lo stesso parent
   }
   else
   {
     //QMessageBox::information(this, "DEBUG", tmp -> text(0));
    esito = true;
    break;
   }
   tmp = tmp -> parent();
  }
  return esito;
}

