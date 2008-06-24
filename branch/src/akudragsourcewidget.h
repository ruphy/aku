#ifndef AKUDRAGSOURCEWIDGET_H
#define AKUDRAGSOURCEWIDGET_H
#include "dragsource.h"
#include <QWidget>
#include <QToolButton>
#include <KIcon>
#include <QCheckBox>
#include <QGridLayout>
#include <KLocale>
#include <QLabel>
#include <KUrl>
#include <KIcon>
#include <QPixmap>
#include <KMimeType>
class akuDragSourceWidget : public QWidget
{

Q_OBJECT

public:
  akuDragSourceWidget(QWidget *parent = 0);
  ~akuDragSourceWidget();
  dragSource * sourceViewInUse();

private:
  QToolButton *upDirButton;
  QCheckBox *hiddenFiles;
  dragSource *sourceView;
  QLabel *currentDirectory;
  QLabel *icon;

protected slots:
  virtual void updateCurrentDirInfo(KUrl);

};

#endif
