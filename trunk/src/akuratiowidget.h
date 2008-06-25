#ifndef AKURATIOWIDGET_H
#define AKURATIOWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QBrush>
#include <QColor>
#include <QRect>
#include <QFont>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QImage>
#include <QSpinBox>
#include <QString>
#include <QLinearGradient>
#include <QPen>

#include <KGlobalSettings>
#include <KDebug>


class akuRatioWidget : public QWidget
{

Q_OBJECT

public:
  akuRatioWidget(float ratio = 0.0, QWidget *parent = 0);
  ~akuRatioWidget();
  QSize sizeHint() const;
public slots:
  virtual void setRatio(float);

private:
  void paintEvent(QPaintEvent *);
  QPainterPath drawRoundedRect(const QRectF &rect, qreal radius);
  float lunghezza;
};

#endif
