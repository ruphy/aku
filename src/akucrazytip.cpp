#include "akucrazytip.h"
#include <QPalette>
#include <QTimer>
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
 w->setBackgroundRole(QPalette::HighlightedText);

 d->toolTip = new QLabel(w);
 d->toolTip->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);

 QLabel *icon = new QLabel(w);
 icon->setPixmap(KIconLoader::global()->loadIcon("help-about", KIconLoader::Small));
 icon->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Ignored);

 d->close = new KPushButton(KIcon("dialog-close"), i18n("Close"), w);
 d->close->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Ignored); 

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
 setVisible(true);
 QTimer *t = new QTimer();
 connect(t, SIGNAL(timeout()), this, SLOT(gradualShow()));
 t->start(10); 
 QTimer *closeTimer = new QTimer();
 connect(closeTimer, SIGNAL(timeout()), this, SLOT(startHide()));
 closeTimer->start(5000);
}

void akuCrazyTip::gradualShow()
{
 if(d->count <=100){
  d->box->resize(d->box->size().width(), (d->count/100.0)*height());
  puts(QString().setNum(size().height()).toAscii());
  d->count++;
 }else{
  d->box->resize(d->box->size().width(), height());
  disconnect(sender(), 0, this, 0);
  connect(d->close, SIGNAL(clicked()), this, SLOT(startHide()));
 // qobject_cast<QTimer*>(sender())->stop();
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
  setVisible(false);
 }
}

QSize akuCrazyTip::sizeHint() const
{
 return d->size;
}

