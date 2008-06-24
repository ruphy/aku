#include "akutip.h"

akuTip::akuTip(QWidget *parent) : QWidget(parent), d(new privateTip)
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
  d->actionTip -> setIcon(KIcon("edit-redo"));
  d->actionTip -> setText(i18n("Last tip"));
  d->actionTip -> setEnabled(false);
}

akuTip::~akuTip()
{
}

KAction* akuTip::actionTip()
{
  return d->actionTip;
}

void akuTip::setTip(const QString& tip)
{
  d->toolTip->setText(tip);
  d->actionTip->setEnabled(true);
}

void akuTip::show()
{
  if(!isVisible()) {
    setVisible(true);
    QTimer *timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(gradualShow()));
    timer->start(10); 
    d->closeTimer = new QTimer();
    connect(d->closeTimer, SIGNAL(timeout()), this, SLOT(startHide()));
    d->closeTimer->start(6000);
  }
  else {
   setVisible(true);
   d->box->resize(d->box->size().width(), height());
   d->closeTimer->stop();
   d->closeTimer->start(6000);
   connect(d->closeTimer, SIGNAL(timeout()), this, SLOT(startHide()));
   //disconnect(d->closeTimer,0,this,0);
   //setVisible(true);
  }
}

void akuTip::gradualShow()
{
  if(d->count <=100) {
    d->box->resize(d->box->size().width(), (d->count/100.0)*height());
    // puts(QString().setNum(size().height()).toAscii());
    d->count++;
  }
  else {
    d->box->resize(d->box->size().width(), height());
    disconnect(sender(), 0, this, 0);
    connect(d->close, SIGNAL(clicked()), this, SLOT(startHide()));
    delete sender();
  }
}

void akuTip::startHide()
{
  disconnect(sender(),0,this,0);
  QTimer *t = new QTimer();
  connect(t, SIGNAL(timeout()), this, SLOT(gradualHide()));
  t->start(10);
}

void akuTip::gradualHide()
{
  if(d->count >= 0) {
    d->box->resize(d->box->size().width(), (d->count/100.0)*height());
    d->count--;
  }
  else {
    disconnect(sender(), 0, this, 0);
    delete sender();
    setVisible(false);
  }
}

QSize akuTip::sizeHint() const
{
  return d->size;
}

