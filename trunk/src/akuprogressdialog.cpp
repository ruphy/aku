#include "akuprogressdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

akuProgressDialog::akuProgressDialog(QWidget* parent, int maximum) : QDialog(parent)
{
  tTip = new akuProgressTooltip();
  tTip -> resize(200,200);
  setWindowModality(Qt::WindowModal);
  parentW = parent;
  setWindowTitle(i18n("Processing..."));
  isPaused = false;
  //setWindowModality(Qt::WindowModal);
  setMaximumSize ( 500,350 );
  setMinimumSize ( 500,350 );
  //setGeometry(parent -> x() + parent -> width() / 4, parent -> y(), 300,300);
  folderdownloadIcon = new QLabel(this);
  folderdownloadIcon -> setMinimumSize(64,64);
  folderdownloadIcon -> setMaximumSize(64,64);
  QPixmap icon = KIcon("folder-downloads").pixmap(64,64);
  folderdownloadIcon -> setPixmap(icon);
  currentFileIcon = new QLabel(this);
  currentFileIcon -> setMinimumSize(64,64);
  currentFileIcon -> setMaximumSize(64,64);
  currentFileName = new KTextEdit(this);
  currentFileName -> setReadOnly(true);
  archiveName = new KTextEdit(this);
  archiveName -> setReadOnly(true);
  archiveName -> setFocusPolicy(Qt::NoFocus);
  currentFileName -> setFocusPolicy(Qt::NoFocus);
  archiveName -> setContextMenuPolicy(Qt::NoContextMenu);
  currentFileName -> setContextMenuPolicy(Qt::NoContextMenu);
  //archiveName -> setTextInteractionFlags(Qt::NoTextInteraction);
  //currentFileName -> setTextInteractionFlags(Qt::NoTextInteraction);
  

  // QGridLayout *fileInfoLayout = new QGridLayout();
  QHBoxLayout *fileInfoLayout = new QHBoxLayout();
  fileInfoLayout -> addWidget(currentFileIcon);
  fileInfoLayout -> addWidget(currentFileName);

  // QGridLayout *fileDestLayout = new QGridLayout();
  QHBoxLayout *fileDestLayout = new QHBoxLayout();
  fileDestLayout -> addWidget(folderdownloadIcon);
  fileDestLayout -> addWidget(archiveName);

  currentFileSize = new QLabel(i18n("Size: "), this);
  currentFileSize -> setAlignment(Qt::AlignRight);
  fileProgress = new QProgressBar(this);
  overallProgress = new QProgressBar(this);
  fileProgress -> setMaximumSize(6556669, 20);
  overallProgress -> setMaximumSize(6556669, 20);
  progressBox = new QGroupBox(i18n("Processing..."),this);
  //progressBox = new QGroupBox(this);
  
 // QGridLayout *baseLayout = new QGridLayout(this);
  QVBoxLayout *baseLayout = new QVBoxLayout(this);
  QToolButton *tButton = new QToolButton(this);
  tButton -> setToolTip(i18n("Send aKu to tray"));
  tButton -> setIcon(KIcon("arrow-down-double"));
  tButton -> setAutoRaise(true);
  baseLayout -> addWidget(progressBox);
  
  QFrame *singleFileFrame = new QFrame(this);
  QFrame *overallFrame = new QFrame(this);
  singleFileFrame -> setFrameStyle(QFrame::StyledPanel);
  overallFrame -> setFrameStyle(QFrame::StyledPanel);
  
 // QGridLayout *singleFileLayout = new QGridLayout(singleFileFrame);
  QVBoxLayout *singleFileLayout = new QVBoxLayout(singleFileFrame);
  singleFileLayout -> addLayout(fileInfoLayout);
  singleFileLayout -> addWidget(currentFileSize);
  singleFileLayout -> addWidget(fileProgress);
  
 // QGridLayout *overallLayout = new QGridLayout(overallFrame);
  QVBoxLayout *overallLayout = new QVBoxLayout(overallFrame);
  overallLayout -> addLayout(fileDestLayout);
  overallLayout -> addWidget(overallProgress);
  
  KPushButton *cancel = new KPushButton(i18n("Cancel"), this);
  cancel-> setIcon(KIcon("dialog-cancel"));
  pause = new KPushButton(i18n("Pause"), this);
  pause-> setIcon(KIcon("edit-clear-history"));
  
 // QGridLayout *inBoxLayout = new QGridLayout(progressBox);
  QVBoxLayout *inBoxLayout = new QVBoxLayout(progressBox);
  inBoxLayout -> addWidget(singleFileFrame);
  inBoxLayout -> addWidget(overallFrame);
  
 // QGridLayout *buttonsLayout = new QGridLayout();
  QHBoxLayout *buttonsLayout = new QHBoxLayout();
  buttonsLayout -> addWidget(cancel);
  buttonsLayout -> addWidget(pause);
  buttonsLayout -> addWidget(tButton);
  baseLayout -> addLayout(buttonsLayout);
  
  fileProgress -> setValue(0);
  increment = maximum / 100;
  if (maximum == 0) maximum = 100;
  overallProgress -> setMaximum(maximum);
  fileProgress -> setMaximum(99);
  
  tray = new KSystemTrayIcon(KIcon("folder-downloads"), parent);

  helpFilter *filter = new helpFilter(tTip,this);
  tray->installEventFilter(filter);
  actionPause = new KAction(KIcon("edit-clear-history"), i18n("Pause"), this);
  tray->contextMenu()->addAction(actionPause);
  connect(actionPause, SIGNAL(triggered()), this, SLOT(pauseClicked()));
  connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(showInfoOnTray(QSystemTrayIcon::ActivationReason)));
  connect(cancel, SIGNAL(clicked()), this, SLOT(cancelPressed()));
  connect(pause, SIGNAL(clicked()), this, SLOT(pauseClicked()));
  connect(tButton, SIGNAL(clicked()), this, SLOT(sendToTray()));
  connect(fileProgress, SIGNAL(valueChanged(int)), this, SLOT(updateTooltip(int)));
  connect(overallProgress, SIGNAL(valueChanged(int)), this, SLOT(updateTooltip(int)));
  connect(this, SIGNAL(accepted()), this, SLOT(clearEventualTrayIcon()));
}

akuProgressDialog::~akuProgressDialog()
{
  qDeleteAll(children());
}

int akuProgressDialog::currentFileProgressValue()
{
  return fileProgress -> value();
}

void akuProgressDialog::clearEventualTrayIcon()
{
  if(tray->isVisible())
  { 
   tray->hide();
   parentW->setVisible(!parentW->isVisible());
  }
}
void akuProgressDialog::setMaximum(int max)
{
  overallProgress -> setMaximum(max);
}

void akuProgressDialog::cancelPressed()
{
  emit canceled();
}

void akuProgressDialog::pauseClicked()
{
  if (!isPaused)
  {
    isPaused = true;
    pause -> setText(i18n("Continue"));
    pause-> setIcon(KIcon("media-playback-start"));
    emit paused();
  }
  else
  {
    isPaused = false;
    pause -> setText(i18n("Pause"));
    pause-> setIcon(KIcon("edit-clear-history"));
    emit continued();
  }
    actionPause->setIcon(pause->icon());
    actionPause->setText(pause->text());
}

void akuProgressDialog::cancel()
{
  reject();
}

void akuProgressDialog::setArchiveName(QString name)
{
  archiveName -> setText(name);
  tTip->setArchiveName(name);
}

void akuProgressDialog::setCurrentFileName(QString name)
{
  currentFileName -> setText(name);
  QString mime = KMimeType::iconNameForUrl(KUrl(name));
  KIcon icon(mime);
  QPixmap pixmap = icon.pixmap(64,64);
  currentFileIcon -> setPixmap(pixmap);
  tTip->setCurrentFileName(name);
}

void akuProgressDialog::setCurrentFileSize(QString size)
{
  currentFileSize -> setText(i18n("Size: ")+size);
}

void akuProgressDialog::incrementOverall()
{
  overallProgress -> setValue(overallProgress -> value() + 1);
  if( overallProgress -> value() == 100) accept();
}

void akuProgressDialog::setCurrentFileProgress(int progress)
{
  fileProgress -> setValue( progress );
  if(!tray->isVisible()) show();
}

void akuProgressDialog::setOverallProgress(int p)
{
 overallProgress->setValue(p);
}

void akuProgressDialog::setCurrentFileProgressToMaximum()
{
  fileProgress -> setValue(fileProgress -> maximum());
}

void akuProgressDialog::closeEvent(QCloseEvent *event)
{
  tray->hide();
  cancelPressed();
  event -> accept();
}

void akuProgressDialog::sendToTray()
{
 tray -> show();
 setVisible(false);
 parentW->setVisible(false);
}

void akuProgressDialog::showInfoOnTray(QSystemTrayIcon::ActivationReason reason)
{
 if(reason == QSystemTrayIcon::Trigger)
 {
  setVisible(!isVisible());
  tray->hide();
 }
}

void akuProgressDialog::updateTooltip(int)
{
 tTip->setCurrentFileProgress(fileProgress->value());
 int overall = (100*overallProgress->value())/overallProgress->maximum();
 tTip->setArchiveProgress(overall);
}

