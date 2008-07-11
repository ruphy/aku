#include "akuratiowidget.h"


akuRatioWidget::akuRatioWidget(float ratio, QWidget *parent) : QWidget(parent)
{
  if (ratio > 100) {
    ratio = 100;
  }
  
  lunghezza = ratio;
}

akuRatioWidget::~akuRatioWidget()
{
}

void akuRatioWidget::setRatio(float newRatio)
{
  //if (lunghezza != newRatio) {
  //  lunghezza = newRatio;
  //  repaint();
  //}
  lunghezza = newRatio;
  repaint();
}

void akuRatioWidget::paintEvent(QPaintEvent *)
{
  kDebug() << "AKURATIO -> INIZIO";
  QPainter painter(this);
  QColor color;
  QColor edgeColor;

  kDebug() << "AKURATIO -> RIGA 31";
  // Align to grid
  painter.translate(0.5, 0.5);
  painter.setRenderHint(QPainter::Antialiasing);

  color = QPalette().color(backgroundRole()).darker();
  edgeColor = color.darker();

  int x0 = 12;
  int y0 = 4;
  int x1 = width()-25;
  int y1 = height()-9;

  kDebug() << "AKURATIO -> RIGA 44";

  QLinearGradient grad(QPoint(0, y0), QPoint(0, y1));
  grad.setColorAt(0, color.lighter());
  grad.setColorAt(1, color);
  painter.setBrush(grad);

  kDebug() << "AKURATIO -> RIGA 51";
  
  QPen pen;
  pen.setColor(edgeColor);
  pen.setWidth(1);
  painter.setPen(pen);
  // draw outside frame
  painter.drawPath(drawRoundedRect(QRect(x0, y0, x1, y1), 3));

  kDebug() << "AKURATIO -> RIGA 60"; 

  // with shadow
  painter.setOpacity(0.25);
  painter.drawLine(x0, y0+1, x1+11, y0+1);
  painter.setOpacity(1);

  edgeColor.setHsv(200,150,255);
 // color.setHsv(lunghezza*1.2, 255,220);
  color = QPalette().color(QPalette::Highlight);

  kDebug() << "AKURATIO -> RIGA 71";
 
  painter.save();
  painter.translate(1, 1);
  painter.setOpacity(0.6);
  grad.setColorAt(0, color);
  grad.setColorAt(0.4, color);
  grad.setColorAt(0.4, color.darker());
  grad.setColorAt(0.75, color.darker());
  QColor glossyReflection = color.lighter();
  glossyReflection.setAlpha(150);
  grad.setColorAt(1, glossyReflection);
  painter.setBrush(grad);
  pen.setColor(edgeColor);
  painter.setPen(pen);
  painter.drawPath(drawRoundedRect(QRect(x0, y0, (((lunghezza+2)/100)*x1)-2, y1-2), 2));
  painter.restore();

  kDebug() << "AKURATIO -> RIGA 89";

  // draw percentage
  painter.setFont(KGlobalSettings::smallestReadableFont());
  painter.save();
  painter.setPen(QPalette().color(QPalette::Text));
  painter.translate(1, 1);
  painter.drawText(rect(), Qt::AlignCenter, QString().setNum(lunghezza)+"%");

  kDebug() << "AKURATIO -> RIGA 98";

  painter.restore();
  painter.setPen(QPalette().color(QPalette::BrightText));
  painter.drawText(rect(), Qt::AlignCenter, QString().setNum(lunghezza)+"%");
  kDebug() << "AKURATIO -> FINE";
}

//stolen from plasma codebase
QPainterPath akuRatioWidget::drawRoundedRect(const QRectF& rect, qreal radius)
{
    QPainterPath path(QPointF(rect.left(), rect.top() + radius));
    path.quadTo(rect.left(), rect.top(), rect.left() + radius, rect.top());         // Top left corner
    path.lineTo(rect.right() - radius, rect.top());                                 // Top side
    path.quadTo(rect.right(), rect.top(), rect.right(), rect.top() + radius);       // Top right corner
    path.lineTo(rect.right(), rect.bottom() - radius);                              // Right side
    path.quadTo(rect.right(), rect.bottom(), rect.right() - radius, rect.bottom()); // Bottom right corner
    path.lineTo(rect.left() + radius, rect.bottom());                               // Bottom side
    path.quadTo(rect.left(), rect.bottom(), rect.left(), rect.bottom() - radius);   // Bottom left corner
    path.closeSubpath();

    return path;
}


QSize akuRatioWidget::sizeHint() const
{
 return QSize(180, 24);
}
