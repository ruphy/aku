#include "akuratiowidget.h"

akuRatioWidget::akuRatioWidget(float ratio, QWidget *parent) : QWidget(parent)
{
  if(ratio >100) ratio = 100;
    lunghezza = ratio;
}

akuRatioWidget::~akuRatioWidget()
{
}

void akuRatioWidget::setRatio(float newRatio)
{
  lunghezza = newRatio;
  repaint();
}

void akuRatioWidget::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  QColor color;
  QColor edgeColor;
  painter.setRenderHint(QPainter::Antialiasing);
  edgeColor.setHsv(200,255,255);
  color.setHsv(lunghezza*1.2, 255,220);
  painter.setOpacity(0.7);
  painter.setBrush(QBrush(color));
  painter.setPen(edgeColor);
  painter.drawRoundRect(QRect(2,4,(lunghezza/100)*width()-3, height()-9),8,8);
  if(backgroundRole() == QPalette::Dark) painter.setPen(QPalette().color(QPalette::BrightText));
   else painter.setPen(QPalette().color(QPalette::Text));
  painter.setOpacity(1);
  painter.drawText(rect(), Qt::AlignCenter, QString().setNum(lunghezza)+"%");
}

QSize akuRatioWidget::sizeHint() const
{
 return QSize(200, 22);
}
