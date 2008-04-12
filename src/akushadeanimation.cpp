#include "akushadeanimation.h"

akuShadeAnimation::akuShadeAnimation(QWidget *parent) : QObject(parent)
{
  shots = 0;
  widgetToAnimate = parent;
}

akuShadeAnimation::~akuShadeAnimation()
{
}

void akuShadeAnimation::start()
{
  animationTimer = new QTimer();
  animationTimer -> setInterval(10);
  widgetToAnimate -> setWindowOpacity(0.0);
  animationTimer -> start();
  connect(animationTimer, SIGNAL(timeout()), this, SLOT(animateShade()));  
}

void akuShadeAnimation::animateShade()
{
  if(shots < 100) //animation will last 1 second
  {
    widgetToAnimate -> setWindowOpacity(shots*0.01);
  }
  else
  {
    animationTimer -> stop();
    widgetToAnimate -> setWindowOpacity(1);
  }
  shots++;
}
