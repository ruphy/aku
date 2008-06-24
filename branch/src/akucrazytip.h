#ifndef AKUCRAZYTIP_H
#define AKUCRAZYTIP_H

#include <QSize>
#include <QLabel>
#include <QPaintEvent>
#include <KPushButton>
#include <KAction>
#include <QTimer>

class akuCrazyTip : public QWidget
{
  Q_OBJECT
   public:
    akuCrazyTip(QWidget *parent = 0);
    ~akuCrazyTip();

   virtual QSize sizeHint() const;
   KAction* actionTip();

   public slots:
    virtual void show();
    virtual void setTip(const QString&);
   protected slots:
    virtual void gradualShow();
    virtual void gradualHide();
    virtual void startHide();
   private:
    class privateTip;
    privateTip *d;  

};

class akuCrazyTip::privateTip 
{
 public:
  privateTip() : size(QSize(800,22)),
                 count(0),
                 box(0), close(0), actionTip(0),
                 closeTimer(0){}
 
  QSize size;
  int count;
  QWidget *box;
  QLabel *toolTip;
  KPushButton *close;
  KAction *actionTip;
  QTimer *closeTimer;
};

#endif
