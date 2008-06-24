#include "akucrazytip.h"
#include <QPalette>
#include <KVBox>
#include <KLocale>
#include <QPainter>
#include <QBrush>
#include <QHBoxLayout>
#include <KIconLoader>
#include <QPixmap>
#include <KIcon>

akuCrazyTip::akuCrazyTip(QWidget *parent) : QWidget(parent),
                                            d(new privateTip)
{
 d->box = new KVBox(this);
 
 QWidget *w = new QWidget(d->box);
 QHBoxLayout *layout = new QHBoxLayout(w);
 w->setAutoFillBackground(true);
 QPalette p = w->palette();
 p.setColor(QPalette::Window, Qt::white);
 w->setPalette(p);

 d->toolTip = new QLabel(w);
 d->toolTip->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
 QPalette pt = d->toolTip->palette();
 pt.setColor(QPalette::WindowText, Qt::black);
 d->toolTip->setPalette(pt);
 
 QLabel *icon = new QLabel(w);
 icon->setPixmap(KIconLoader::global()->loadIcon("help-about", KIconLoader::Small));
 icon->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

 d->close = new KPushButton(KIcon("dialog-close"), i18n("Close"), w);

 layout->addWidget(icon);
 layout->addWidget(d->toolTip);
 layout->addWidget(d->close);

 QHBoxLayout *baseLayout = new QHBoxLayout(this);
 baseLayout->addWidget(d->box);

 d->actionTip = new KAction(this);
 d->actionTip -> setIcon(KIcon("view-refresh"));
 d->actionTip -> setText(i18n("Last tip"));
 d->actionTip -> setEnabled(false);
}

akuCrazyTip::~akuCrazyTip()
{
}

KAction* akuCrazyTip::actionTip()
{
  return d->actionTip;
}

void akuCrazyTip::setTip(const QString& tip)
{
 d->toolTip->setText(tip);
 d->actionTip->setEnabled(true);
}

void akuCrazyTip::show()
{
 if(!isVisible())
 {
  setVisible(true);
  QTimer *t = new QTimer();
  connect(t, SIGNAL(timeout()), this, SLOT(gradualShow()));
  t->start(10); 
  d->closeTimer = new QTimer();
  connect(d->closeTimer, SIGNAL(timeout()), this, SLOT(startHide()));
  d->closeTimer->start(6000);
 }else{
  disconnect(d->closeTimer,0,this,0);
  setVisible(true);
 }
}

void akuCrazyTip::gradualShow()
{
 if(d->count <=100){
  d->box->resize(d->box->size().width(), (d->count/100.0)*height());
 // puts(QString().setNum(size().height()).toAscii());
  d->count++;
 }else{
  d->box->resize(d->box->size().width(), height());
  disconnect(sender(), 0, this, 0);
  connect(d->close, SIGNAL(clicked()), this, SLOT(startHide()));
  delete sender();
 }
}

void akuCrazyTip::startHide()
{
 disconnect(sender(),0,this,0);
 QTimer *t = new QTimer();
 connect(t, SIGNAL(timeout()), this, SLOT(gradualHide()));
 t->start(10);
}

void akuCrazyTip::gradualHide()
{
 if(d->count >= 0){
  d->box->resize(d->box->size().width(), (d->count/100.0)*height());
  d->count--;
 }else{
  disconnect(sender(), 0, this, 0);
  delete sender();
  setVisible(false);
 }
}

QSize akuCrazyTip::sizeHint() const
{
 return d->size;
}

