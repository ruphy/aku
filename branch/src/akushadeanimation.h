#ifndef AKUSHADEANIMATION_H
#define AKUSHADEANIMATION_H

#include <QObject>
#include <QWidget>
#include <QTimer>

class akuShadeAnimation : public QObject
{
Q_OBJECT

public:
  akuShadeAnimation(QWidget *parent);
  ~akuShadeAnimation();

protected slots:
  virtual void animateShade();

public slots:
  virtual void start();

private:
  QWidget *widgetToAnimate;
  QTimer *animationTimer;
  int shots;
};
#endif
