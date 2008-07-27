#ifndef AKUDRAGSOURCEWIDGET_H
#define AKUDRAGSOURCEWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QToolButton>
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>

#include <KLocale>
#include <KIcon>
#include <KUrl>
#include <KIcon>
#include <KVBox>

#include <KMimeType>

#include "dragsource.h"

class akuDragSourceWidget : public QWidget
{

Q_OBJECT

public:
  akuDragSourceWidget(QWidget *parent = 0);
  ~akuDragSourceWidget();
  dragSource * sourceViewInUse();

private:
  QToolButton *upDirButton;
  QToolButton *homeButton;
  QCheckBox *hiddenFiles;
  dragSource *sourceView;
  QLabel *currentDirectory;
  QLabel *icon;

protected slots:
  virtual void updateCurrentDirInfo(KUrl);

};

#endif
