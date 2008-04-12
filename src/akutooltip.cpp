#include "akutooltip.h"

akuToolTip::akuToolTip(QString tip, QWidget *parent) : QWidget()
{
  shots = 0;
  setWindowFlags(Qt::ToolTip);
  //creiamo una tooltip con una label per la tip, una barra di progressione e un tasto close
  timeBar = new QLabel(this);
  toolTip = new QLabel(tip,this);
  toolTip -> setAlignment(Qt::AlignLeft | Qt::AlignTop);
  toolTip -> setWordWrap(true);
  timeBar -> setMaximumSize(20,6554000);
  if( parent != NULL && parent -> x() > 220) setGeometry(parent -> x() - 210, parent -> y(), 200, 200);
  else setGeometry(0,0, 200,200);
  QPushButton *closeButton = new QPushButton(tr("Close"));
  closeButton -> setIcon(KIcon("dialog-close"));
  closeButton -> setMaximumSize(300, 30);
  QGridLayout *layout = new QGridLayout(this);
  QGridLayout *tipLayout = new QGridLayout();
  tipLayout -> addWidget(toolTip, 1,1);
  tipLayout -> addWidget(closeButton,2,1);  
  layout -> addWidget(timeBar, 1,1);
  layout -> addLayout(tipLayout, 1,2);
  
  tipTimer = new QTimer();
  tipTimer -> setInterval(10);
  tipTimer -> start();
  connect(tipTimer, SIGNAL(timeout()), this, SLOT(progress()));
  connect(closeButton, SIGNAL(clicked()), this, SLOT(closeTip()));
  show();

}

akuToolTip::~akuToolTip()
{
}

void akuToolTip::progress()
{
  //creiamo l'effetto di progressione della barra finchè non scompare, 5 secondi
  if( shots <= 500 ) //5 secondi 
  {
    QPixmap barMap(timeBar -> width(), toolTip -> height());
    QColor timeColor;
    timeColor.setHsv(0,245,195);
    QColor paletteColor;
    paletteColor.setHsv(60,140,240);
    barMap.fill(paletteColor);
    QPainter painter(&barMap);

    painter.setPen(timeColor);
    painter.setBrush(QBrush(paletteColor));
    painter.drawRect(0,0,11,142);

    painter.setPen(paletteColor);
    painter.setBrush(QBrush(timeColor));
    painter.drawRect(QRect(1, 140, 9, shots*0.28 - 140)); //disegnamo il rettangolo all'interno di quello di contorno
  
    timeBar -> setPixmap(barMap);
  }
  else
  {
    tipTimer -> stop();
    this -> close();
  }
  shots++;
}

void akuToolTip::closeTip()
{
  tipTimer -> stop();
  this -> close();
}
 
void akuToolTip::paintEvent(QPaintEvent*)
{
  //impostiamo i colori della tooltip
  QPainter backGround(this);
  QColor paletteColor;
  paletteColor.setHsv(60,140,240);
  backGround.setPen(paletteColor);
  backGround.setBrush(QBrush(paletteColor));
  backGround.drawRect(rect());
}

void akuToolTip::enterEvent(QEvent *)
{
  tipTimer -> stop();
}

void akuToolTip::leaveEvent(QEvent *)
{
  if(!tipTimer -> isActive()) tipTimer -> start(10);
}
