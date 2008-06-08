#include "akucrazytip.h"
#include <QPalette>
#include <QTimer>
#include <KVBox>
#include <KLocale>
#include <QPainter>
#include <QBrush>
#include <KPushButton>
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
 w->setBackgroundRole(QPalette::BrightText);

 d->toolTip = new QLabel(w);
 d->toolTip->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

 QLabel *icon = new QLabel(w);
 icon->setPixmap(KIconLoader::global()->loadIcon("help-about", KIconLoader::Small));
 icon->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

 KPushButton *close = new KPushButton(KIcon("dialog-close"), i18n("Close"), w);
 close->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed); 

 layout->addWidget(icon);
 layout->addWidget(d->toolTip);
 layout->addWidget(close);

 QHBoxLayout *baseLayout = new QHBoxLayout(this);
 baseLayout->addWidget(d->box);
 
}

akuCrazyTip::~akuCrazyTip()
{
}

void akuCrazyTip::setTip(const QString& tip)
{
 d->toolTip->setText(tip);
}

void akuCrazyTip::show()
{
 setVisible(true);
 QTimer *t = new QTimer();
 connect(t, SIGNAL(timeout()), this, SLOT(gradualShow()));
 t->start(10); 
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
 // qobject_cast<QTimer*>(sender())->stop();
 }
}

QSize akuCrazyTip::sizeHint() const
{
 return d->size;
}

