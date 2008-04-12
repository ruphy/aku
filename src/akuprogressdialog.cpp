#include "akuprogressdialog.h"

akuProgressDialog::akuProgressDialog(QWidget* parent, int maximum) : QDialog(parent)
{
  setWindowTitle(i18n("Processing..."));
  isPaused = false;
  setWindowModality(Qt::WindowModal);
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
  

  QGridLayout *fileInfoLayout = new QGridLayout();
  fileInfoLayout -> addWidget(currentFileIcon,1,1);
  fileInfoLayout -> addWidget(currentFileName,1,2);

  QGridLayout *fileDestLayout = new QGridLayout();
  fileDestLayout -> addWidget(folderdownloadIcon,1,1);
  fileDestLayout -> addWidget(archiveName,1,2);

  currentFileSize = new QLabel(i18n("Size: "), this);
  currentFileSize -> setAlignment(Qt::AlignRight);
  fileProgress = new QProgressBar(this);
  overallProgress = new QProgressBar(this);
  fileProgress -> setMaximumSize(6556669, 20);
  overallProgress -> setMaximumSize(6556669, 20);
  progressBox = new QGroupBox(i18n("Processing..."),this);
  //progressBox = new QGroupBox(this);
  
  QGridLayout *baseLayout = new QGridLayout(this);
  baseLayout -> addWidget(progressBox,1,1);
  
  QFrame *singleFileFrame = new QFrame(this);
  QFrame *overallFrame = new QFrame(this);
  singleFileFrame -> setFrameStyle(QFrame::StyledPanel);
  overallFrame -> setFrameStyle(QFrame::StyledPanel);
  
  QGridLayout *singleFileLayout = new QGridLayout(singleFileFrame);
  singleFileLayout -> addLayout(fileInfoLayout,1,1);
  singleFileLayout -> addWidget(currentFileSize,2,1);
  singleFileLayout -> addWidget(fileProgress,3,1);
  
  QGridLayout *overallLayout = new QGridLayout(overallFrame);
  overallLayout -> addLayout(fileDestLayout,1,1);
  overallLayout -> addWidget(overallProgress,2,1);
  
  KPushButton *cancel = new KPushButton(i18n("Cancel"), this);
  cancel-> setIcon(KIcon("dialog-cancel"));
  pause = new KPushButton(i18n("Pause"), this);
  pause-> setIcon(KIcon("edit-clear-history"));
  
  QGridLayout *inBoxLayout = new QGridLayout(progressBox);
  inBoxLayout -> addWidget(singleFileFrame,1,1);
  inBoxLayout -> addWidget(overallFrame,2,1);
  
  QGridLayout *buttonsLayout = new QGridLayout();
  buttonsLayout -> addWidget(cancel,1,1);
  buttonsLayout -> addWidget(pause,1,2);
  baseLayout -> addLayout(buttonsLayout,3,1);
  
  fileProgress -> setValue(0);
  increment = maximum / 100;
  if (maximum == 0) maximum = 100;
  overallProgress -> setMaximum(maximum);
  fileProgress -> setMaximum(99);
  connect(cancel, SIGNAL(clicked()), this, SLOT(cancelPressed()));
  connect(pause, SIGNAL(clicked()), this, SLOT(pauseClicked()));
}

akuProgressDialog::~akuProgressDialog()
{
  qDeleteAll(children());
}

int akuProgressDialog::currentFileProgressValue()
{
  return fileProgress -> value();
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
  if (isPaused == false)
  {
    isPaused = true;
    pause -> setText(i18n("Continue"));
    pause-> setIcon(KIcon("legalmoves"));
    emit paused();
  }
  else
  {
    isPaused = false;
    pause -> setText(i18n("Pause"));
    pause-> setIcon(KIcon("edit-clear-history"));
    emit continued();
  }
}

void akuProgressDialog::cancel()
{
  reject();
}

void akuProgressDialog::setArchiveName(QString name)
{
  archiveName -> setText(name);
}

void akuProgressDialog::setCurrentFileName(QString name)
{
  currentFileName -> setText(name);
  QString mime = KMimeType::iconNameForUrl(KUrl(name));
  KIcon icon(mime);
  QPixmap pixmap = icon.pixmap(64,64);
  currentFileIcon -> setPixmap(pixmap);
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
  show();
}

void akuProgressDialog::setCurrentFileProgressToMaximum()
{
  fileProgress -> setValue(fileProgress -> maximum());
}

void akuProgressDialog::closeEvent(QCloseEvent *event)
{
  cancelPressed();
  event -> accept();
}
