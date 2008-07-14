#ifndef AKUMETAWIDGET_H
#define AKUMETAWIDGET_H

#include "akuratiowidget.h"

#include <QLabel>
#include <QFrame>
#include <QPixmap>
#include <QFont>
#include <QGroupBox>
#include <QList>
#include <QTreeWidgetItem>
#include <QPainter>
#include <QDateTime>
#include <QVBoxLayout>
#include <QBuffer>
#include <QFile>

#include <KIcon>
#include <KMimeType>
#include <KLocale>
#include <KVBox>
#include <KAction>

#include <phonon/mediaobject.h>
#include <phonon/audiooutput.h>


class akuMetaWidget : public QWidget
{

Q_OBJECT

public:
  akuMetaWidget(QWidget *parent = 0);
  ~akuMetaWidget();

public slots:
  virtual void setMimeIcon(QPixmap);
  virtual void setFileName(QString, bool folder = false);
  virtual void setFileSize(QString);
  virtual void clear();
  virtual void setMime(QString);
  virtual void setPreview(QByteArray);
  virtual void setAudio(QByteArray);
  virtual void setAudioControl(bool);
  virtual void handleItemSelections(QList<QTreeWidgetItem*>);
  virtual void setRatio(float);
  virtual void setDateTime(const QDateTime&);
  virtual void setDateTxt(QString);

private:
  QLabel *iconMap;
  QLabel *metaName;
  QLabel *metaSize;
  QLabel *metaMime;
  QLabel *archiveName;
  QLabel *archiveDetails;
  QLabel *notificationIcon;
  QLabel *dtime;

 // QScrollArea *scrollArea;

  akuRatioWidget *ratio;

  KVBox *baseScrollWidget;

  KHBox *hbox;

  QWidget *w_ratio;

  Phonon::MediaObject *mediaObject;
  Phonon::AudioOutput *audioOutput;

  KAction *playAction;
  KAction *pauseAction;
  KAction *stopAction;

  private slots:
    void stateChanged(Phonon::State newState, Phonon::State oldState);

  protected slots:
    virtual void setupPhonon();


};
#endif
