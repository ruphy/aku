#ifndef AKUPROGRESSDIALOG_H
#define AKUPROGRESSDIALOG_H
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <KPushButton>
#include <QWidget>
#include <QDialog>
#include <QFrame>
#include <QProgressBar>
#include <QCloseEvent>
#include <QPixmap>
#include <QStringList>
#include <KIcon>
#include <KMimeType>
#include <KLocale>
#include <KTextEdit>

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
  virtual void setCurrentFileProgressToMaximum();
  virtual void cancel();
  virtual void incrementOverall();
  virtual void pauseClicked();
  virtual void setMaximum(int);

protected slots:
  virtual void cancelPressed();

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

protected:
  virtual void closeEvent(QCloseEvent*);
};

#endif
