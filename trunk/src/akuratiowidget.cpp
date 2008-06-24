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
  color = QPalette().color(backgroundRole()).darker();
  edgeColor = color.darker();
  QLinearGradient grad(QPoint(rect().center().x(), 0), QPoint(rect().center().x(), height()));
  grad.setColorAt(0, color);
  grad.setColorAt(0.5, color.lighter());
  grad.setColorAt(1, color);
  painter.setBrush(grad);
  QPen pen;
  pen.setColor(edgeColor);
  pen.setWidthF(0.5);
  painter.setPen(pen);
  painter.drawRoundRect(QRect(12,4,width()-25, height()-9),8,8);
  edgeColor.setHsv(200,255,255);
  color.setHsv(lunghezza*1.2, 255,220);
 // painter.setOpacity(0.7);
  grad.setColorAt(0, color);
  grad.setColorAt(0.5, color);
  grad.setColorAt(0.57, color.darker());
  grad.setColorAt(0.92, color.darker());
  grad.setColorAt(1, color.lighter());
  painter.setBrush(grad);
  pen.setColor(edgeColor);
  painter.setPen(pen);
  painter.drawRoundRect(QRect(12,4,(lunghezza/100)*(width()-25), height()-9),8,8);
 // if(backgroundRole() == QPalette::Dark) painter.setPen(QPalette().color(QPalette::BrightText));
 //  else painter.setPen(QPalette().color(QPalette::Text));
  painter.setPen(QPalette().color(QPalette::BrightText));
  painter.setOpacity(1);
  painter.drawText(rect(), Qt::AlignCenter, QString().setNum(lunghezza)+"%");
}

QSize akuRatioWidget::sizeHint() const
{
 return QSize(180, 24);
}
