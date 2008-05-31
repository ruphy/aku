
#include "akuviewer.h"


#include <QString>
#include <QHBoxLayout>
#include <KVBox>
#include <KHBox>
#include <QScrollArea>
#include <QSlider>

akuViewer::akuViewer ( QWidget *parent, Qt::WFlags fl ) : KDialog ( parent, fl )
{
  setModal ( true );
  setButtons(Close);
  blayout = new KVBox(this);

  QWidget *fileWidget = new QWidget(blayout);
  fileIcon = new QLabel(fileWidget);

  fileIcon->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
  KVBox *fileInfoLayout = new KVBox();
  fileName = new QLabel(fileInfoLayout);
  fileMime = new QLabel(fileInfoLayout);

  QHBoxLayout *fileLayout = new QHBoxLayout(fileWidget);
  fileLayout -> addWidget(fileIcon);
  fileLayout -> addWidget(fileInfoLayout);
  setMainWidget(blayout);


  setWindowTitle (i18n ( "aku Embedded Viewer" ) );
 
}

akuViewer::~akuViewer()
{
  qDeleteAll(children());
}

void akuViewer::setupTextView()
{
  QWidget *comboW = new QWidget(blayout);
  viewer = new KTextEdit ( blayout );
  QWidget *searchW = new QWidget(blayout); 
  viewer -> enableFindReplace(true);
  fontCombo = new KFontComboBox();
  fontCombo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
  sizeCombo = new QComboBox(); 
  sizeCombo -> setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum );
  searchLine = new QLineEdit ( this );
  QLabel *search = new QLabel (i18n ( "Search:" ), this );
  search->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  QPushButton *searchBackward = new QPushButton (  this );
  QPushButton *searchForward = new QPushButton ( this );
  searchBackward -> setIcon(KIcon("go-up"));
  searchForward -> setIcon(KIcon("go-down"));
  searchBackward->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  searchForward->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  searchForward -> setShortcut(Qt::Key_F3);

  QHBoxLayout *comboLayout = new QHBoxLayout(comboW);
  QHBoxLayout *searchLayout = new QHBoxLayout(searchW);
  
  searchLayout -> addWidget ( search );
  searchLayout -> addWidget ( searchLine);
  searchLayout -> addWidget ( searchBackward );
  searchLayout -> addWidget ( searchForward );
  
  comboLayout -> addWidget ( fontCombo ); 
  comboLayout -> addWidget ( sizeCombo ); 
  connect ( fontCombo, SIGNAL ( currentFontChanged ( QFont ) ), this, SLOT ( setTextFont ( QFont ) ) );
  connect ( sizeCombo, SIGNAL ( currentIndexChanged ( QString ) ), this, SLOT ( setTextSize ( QString ) ) );
  connect ( searchLine, SIGNAL ( textChanged ( QString ) ), this, SLOT ( startTimer ( QString ) ) );
  connect ( searchLine, SIGNAL ( returnPressed() ), this, SLOT ( searchForward() ) );
  connect ( searchBackward, SIGNAL ( clicked() ), this, SLOT ( searchBackward() ) );
  connect ( searchForward, SIGNAL ( clicked() ), this, SLOT ( searchForward() ) );
  // viewer -> setWordWrapMode(QTextOption::WordWrap); 
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

void akuViewer::setData(const QByteArray &data)
{
  KMimeType::Ptr mimePtr = KMimeType::findByContent(data);
  if(mimePtr -> name().contains("image"))
  {
    QScrollArea *sArea = new QScrollArea(blayout);
    imageViewer = new QLabel(sArea);
    sArea->setBackgroundRole(QPalette::Dark);
    imageViewer->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    sArea->setWidget(imageViewer);
    sArea->setWidgetResizable(true);
    imageBuffer.loadFromData(data);
    imageViewer -> setPixmap(imageBuffer);
    QWidget *resW = new QWidget(blayout);
    am = new QLabel(resW);
    QSlider *slider = new QSlider(Qt::Horizontal, resW);
    slider->setRange(0,100);
    slider->setTracking(true);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(resizePreview(int)));
    slider->setValue(50);
    QHBoxLayout *hLay = new QHBoxLayout(resW);
    hLay->addWidget(am);
    hLay->addWidget(slider);
  }
  else setText(QString::fromUtf8(data));
}

void akuViewer::resizePreview(int s)
{
  am->setText(QString("%1\%").arg(s));
  float amount = s/100.0;
  imageViewer->setPixmap(imageBuffer.scaled(imageBuffer.size()*amount, Qt::KeepAspectRatio));
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

void akuViewer::searchBackward()
{
  viewer -> find ( searchLine -> text(),QTextDocument::FindBackward );
}
