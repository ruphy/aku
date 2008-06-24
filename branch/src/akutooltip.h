#ifndef AKUTOOLTIP_H
#define AKUTOOLTIP_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QTimer>
#include <QColor>
#include <QGridLayout>
#include <QPushButton>
#include <KIcon>
#include <QEvent>
class akuToolTip : public QWidget
{

Q_OBJECT

public:
  akuToolTip(QString, QWidget *parent = 0);
  ~akuToolTip();

public slots:
  virtual void closeTip();

protected slots:
  virtual void progress();

private:
  QLabel *timeBar;
  QLabel *toolTip;
  QTimer *tipTimer;
  int shots;
  void paintEvent(QPaintEvent*);

protected:
  virtual void enterEvent(QEvent *);
  virtual void leaveEvent(QEvent *);
};
#endif
