#ifndef AKUPROGRESSDIALOG_H
#define AKUPROGRESSDIALOG_H

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMenu>
#include <QDialog>
#include <QFrame>
#include <QProgressBar>
#include <QCloseEvent>
#include <QPixmap>
#include <QStringList>
#include <QToolButton>
#include <QHelpEvent>
#include <QTimer>

#include <KIcon>
#include <KMimeType>
#include <KLocale>
#include <KTextEdit>
#include <KSystemTrayIcon>
#include <KActionCollection>
#include <KAction>
#include <KPushButton>

#include "akuprogresstooltip.h"

class akuProgressDialog : public QDialog
{
Q_OBJECT

public:
  akuProgressDialog( QWidget *parent=0, int = 0);
  ~akuProgressDialog();

  int currentFileProgressValue();

public slots:
  virtual void setArchiveName(QString);
  virtual void setCurrentFileName(QString);
  virtual void setCurrentFileSize(QString);
  virtual void setCurrentFileProgress(int);
  virtual void setOverallProgress(int);
  virtual void setCurrentFileProgressToMaximum();
  virtual void cancel();
  virtual void incrementOverall();
  virtual void pauseClicked();
  virtual void setMaximum(int);


protected slots:
  virtual void cancelPressed();
  virtual void sendToTray();
  virtual void showInfoOnTray(QSystemTrayIcon::ActivationReason);
  virtual void updateTooltip(int);
  virtual void clearEventualTrayIcon();
signals:
  void canceled();
  void paused();
  void continued();

private:
  int totalFileCount;
  int increment;
  QProgressBar *fileProgress;
  QProgressBar *overallProgress;
  KTextEdit *archiveName;
  QGroupBox *progressBox;
  //QLabel *currentFileName;
  KTextEdit *currentFileName;
  QLabel *currentFileSize;
  KPushButton *pause;
  bool isPaused;
  QLabel *folderdownloadIcon;
  QLabel *currentFileIcon;
  QWidget *parentW;
  KSystemTrayIcon *tray;
  akuProgressTooltip *tTip;
  KAction *actionPause;
protected:
  virtual void closeEvent(QCloseEvent*);
};



class helpFilter : public QObject
{
 Q_OBJECT
 public:
   helpFilter(QWidget *toolTip, QObject *parent=0)
   {
    toolTipWidget = toolTip;
   }
   ~helpFilter(){}
 protected:
  bool eventFilter(QObject *obj, QEvent *event) {
    if(event->type() == QEvent::ToolTip) {
      QHelpEvent *helpEvent = static_cast<QHelpEvent*>(event);
      QPoint point = helpEvent->globalPos();
      point.setX(point.x() - toolTipWidget->width());
      point.setY(point.y() - toolTipWidget->height());
      toolTipWidget->move(point);
      toolTipWidget->show();
      QTimer *timer = new QTimer();
      connect(timer, SIGNAL(timeout()), this, SLOT(hideTooltip()));
      timer->start(3000);
      return true;
    }
    else return QObject::eventFilter(obj, event);
  }

  protected slots:
    void hideTooltip() {
      QTimer *timer = qobject_cast<QTimer*>(sender());
      if(timer) {
        timer->stop();
        disconnect(timer, SIGNAL(timeout()), this, SLOT(hideTooltip()));
        delete timer;
      }
      toolTipWidget->setVisible(false);
     }

  private:
    QWidget *toolTipWidget;
};

#endif
