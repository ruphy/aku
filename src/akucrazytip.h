#ifndef AKUCRAZYTIP_H
#define AKUCRAZYTIP_H

#include <QWidget>
#include <QSize>
#include <QLabel>
#include <QPaintEvent>
#include <KPushButton>

class akuCrazyTip : public QWidget
{
  Q_OBJECT
   public:
    akuCrazyTip(QWidget *parent = 0);
    ~akuCrazyTip();

   virtual QSize sizeHint() const;
   

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
  privateTip() : size(QSize(800,30)),
                 count(0),
                 box(0), close(0){}
  QSize size;
  int count;
  QWidget *box;
  QLabel *toolTip;
  KPushButton *close;
};

#endif
