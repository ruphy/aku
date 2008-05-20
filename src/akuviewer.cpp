
#include "akuviewer.h"


#include <QString>

akuViewer::akuViewer ( QWidget *parent, Qt::WFlags fl ) : QDialog ( parent, fl )
{
  setModal ( true );
  

  //Setting up file information widget
  QWidget *fileWidget = new QWidget(this);
  fileName = new QLabel(fileWidget);
  fileIcon = new QLabel(fileWidget);
  fileIcon -> setMaximumSize(65,68);
  fileMime = new QLabel(fileWidget);
  fileWidget -> setMaximumSize(654999, 68);
  QGridLayout *fileInfoLayout = new QGridLayout();
  fileInfoLayout -> addWidget(fileName,1,1);
  fileInfoLayout -> addWidget(fileMime,2,1);
  QGridLayout *fileLayout = new QGridLayout(fileWidget);
  fileLayout -> addWidget(fileIcon,1,1);
  fileLayout -> addLayout(fileInfoLayout,1,2);
  layout = new QGridLayout ( this );

  layout -> addWidget(fileWidget,1,1);

  setWindowTitle (i18n ( "aku Embedded Viewer" ) );
 
}

akuViewer::~akuViewer()
{
  qDeleteAll(children());
}

void akuViewer::setupTextView()
{
  viewer = new KTextEdit ( this );
  viewer -> enableFindReplace(true);
  fontCombo = new QFontComboBox(); //per scegliere il font
  sizeCombo = new QComboBox(); //per la dimensione del font
  sizeCombo -> setMaximumSize ( 90,80 );
  searchLine = new QLineEdit ( this );
  searchLine -> setMaximumSize ( 65000, 90 );
  QLabel *search = new QLabel (i18n ( "Search:" ), this );
  search->setMaximumSize ( search->text().length() *10, 90 ); //impostiamo la lunghezza della label
  QPushButton *searchBackward = new QPushButton (  this );
  QPushButton *searchForward = new QPushButton ( this );
  searchBackward -> setIcon(KIcon("go-up"));
  searchForward -> setIcon(KIcon("go-down"));
  searchBackward -> setMaximumSize ( 50,90 );
  searchForward -> setMaximumSize ( 50,90 );
  searchForward -> setShortcut(Qt::Key_F3);
  comboLayout = new QGridLayout();
  searchLayout = new QGridLayout();
  
  searchLayout -> addWidget ( searchLine,1,2 );
  searchLayout -> addWidget ( search,1,1 );
  searchLayout -> addWidget ( searchBackward, 1,3 );
  searchLayout -> addWidget ( searchForward, 1, 4 );
  
  comboLayout -> addWidget ( fontCombo, 1, 1 ); 
  comboLayout -> addWidget ( sizeCombo, 1, 2 ); 
  layout -> addLayout ( comboLayout, 2,1 );
  layout -> addWidget ( viewer,3,1 ); 
  layout -> addLayout ( searchLayout,4,1 );
  connect ( fontCombo, SIGNAL ( currentFontChanged ( QFont ) ), this, SLOT ( setTextFont ( QFont ) ) );
  connect ( sizeCombo, SIGNAL ( currentIndexChanged ( QString ) ), this, SLOT ( setTextSize ( QString ) ) );
  connect ( searchLine, SIGNAL ( textChanged ( QString ) ), this, SLOT ( startTimer ( QString ) ) );
  connect ( searchLine, SIGNAL ( returnPressed() ), this, SLOT ( searchForward() ) );
  connect ( searchBackward, SIGNAL ( clicked() ), this, SLOT ( searchBackward() ) );
  connect ( searchForward, SIGNAL ( clicked() ), this, SLOT ( searchForward() ) );
  //viewer -> setWordWrapMode(QTextOption::WordWrap); 
  viewer -> setAcceptRichText ( true );
  viewer -> setReadOnly ( true );
  initSizeCombo(); 
}

void akuViewer::setFileName( QString title )
{
  QFont font = fileName -> font();
  font.setBold(true);
  fileName -> setFont(font);
  fileName -> setText(title);
  KMimeType::Ptr mimePtr = KMimeType::findByUrl(KUrl(title));
  fileMime -> setText(mimePtr -> comment());
  KIcon icon(mimePtr -> iconName());
  fileIcon -> setPixmap(icon.pixmap(64,64));
}

void akuViewer::setData(QByteArray data)
{
  KMimeType::Ptr mimePtr = KMimeType::findByContent(data);
  if(mimePtr -> name().contains("image"))
  {
    QLabel *imageViewer = new QLabel(this);
    
    imageViewer->setScaledContents(true);
    QPixmap imageBuffer;
    imageBuffer.loadFromData(data);
    QSize realSize = imageBuffer.size();
    layout -> addWidget(imageViewer,2,1);
    imageViewer -> setPixmap(imageBuffer);
    imageViewer -> setFrameShape(QFrame::StyledPanel);
    imageViewer->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    resize(realSize);
  }
  else setText(QString::fromUtf8(data));
}

void akuViewer::setText ( QString txt )
{
  setupTextView();
  testo = txt;
  viewer -> setText ( testo );
}

void akuViewer::setTextFont ( QFont font )
{
  QFont current = viewer -> currentFont();
  font.setPointSize ( current.pointSize() ); //come size lasciamo quello impostato dall'utente
  viewer -> clear();
  viewer -> setCurrentFont ( font );
  viewer -> setText ( testo );
  initSizeCombo(); //ripristiniamo le dimensioni disponibili
}

void akuViewer::initSizeCombo()
{
  disconnect ( sizeCombo, SIGNAL ( currentIndexChanged ( QString ) ), this, SLOT ( setTextSize ( QString ) ) );
  sizeCombo -> clear();
  connect ( sizeCombo, SIGNAL ( currentIndexChanged ( QString ) ), this, SLOT ( setTextSize ( QString ) ) );
  QString comboItem;
  QFont current = viewer -> currentFont();
  int i = current.pointSize(); //acquisisco la dimensione del Font attualmentei in uso
  int end = i+32;
  int start = i - 3;
  for ( int j = start; j < end; j++ ) //inseriamo i numeri di font da 3 prima di quello impostato a 32 dopo
  {
    sizeCombo -> insertItem ( j, comboItem.setNum ( j ) );
  }
  //QMessageBox::information(this, "DEBUG", "pointSize attuale : "+comboItem.setNum(i));
  sizeCombo -> setCurrentIndex ( sizeCombo -> findText ( comboItem.setNum ( i ) ) ); //settiamo il size attuale cioè il pointSize del font attuale

}

void akuViewer::setTextSize ( QString index )
{

  QFont current = viewer -> currentFont();
  //prendo la dimensione che sta scritta
  int size = index.toInt(); //e la converto in int
  current.setPointSize ( size ); //setto la dimensione del nuovo font
  viewer -> setCurrentFont ( current );
  viewer -> setText ( testo );
}

void akuViewer::startTimer ( QString toFind )
{
  //prima di iniziare la ricerca aspettiamo che l'utente abbia finito di scrivere
  daCercare = toFind;
  timer = new QTimer ( this );
  timer -> setSingleShot ( true );
  timer -> start ( 600 );
  connect ( timer, SIGNAL ( timeout() ), this, SLOT ( find() ) );
}

void akuViewer::find()
{
 disconnect ( timer, SIGNAL ( timeout() ), this, SLOT ( find() ) );
 timer -> stop();

 searchLine -> setPalette ( QPalette() );

 // restoring text
 QTextCharFormat restore;
 QBrush brush;
 restore.setBackground ( brush ); 
 QFont fontRestore = viewer -> currentFont();
 restore.setFont ( fontRestore );
 viewer -> setExtraSelections(QList<QTextEdit::ExtraSelection>());
 viewer-> moveCursor ( QTextCursor::Start );



if ( daCercare != "" )
{
  bool found = true;
  QList<QTextEdit::ExtraSelection> selezioni;
  brush = QBrush(( QColor ( "yellow" ) )); //coloriamo lo sfondo
  restore.setBackground ( brush );
  QList<QTextEdit::ExtraSelection> exSelections;
  while ( found != false ) //cerca tutte le occorrenze della stringa
  {
    found = viewer -> find ( daCercare );
    QTextEdit::ExtraSelection tempSelection;
    tempSelection.cursor = viewer -> textCursor();
    tempSelection.format = restore;
    exSelections << tempSelection;        
  }
  viewer -> moveCursor ( QTextCursor::Start ); //impostiamo il cursore all'inizio
  found = viewer -> find ( daCercare ); //ed evidenziamo la prima parola trovata
  viewer -> ensureCursorVisible();
  if ( found == false && searchLine -> text() != "" )
  {
    QPalette palette;
    QColor alertColor;
    alertColor.setHsv ( 0, 120, 255 );
    palette.setColor ( QPalette::Active, QPalette::Base, alertColor ); //la searchline diventa rossa se non trova la parola
    searchLine -> setPalette ( palette );
  }
  viewer -> setExtraSelections(exSelections);
}

}

void akuViewer::searchForward()
{
  bool found = viewer -> find ( searchLine -> text() );
  if ( found == false ) 
    if ( searchLine -> text() != "" )
     if ( KMessageBox::questionYesNo ( this,i18n ( "No match found. Do you want to search again from the beginning?" ),  i18n ( "Find" )) ==KMessageBox::Yes )
      {
        viewer -> moveCursor ( QTextCursor::Start );
        found = viewer -> find ( searchLine -> text() );
        //alreadyQuestion = true;
      }
    
  }
}

void akuViewer::searchBackward()
{
  viewer -> find ( searchLine -> text(),QTextDocument::FindBackward );
}
