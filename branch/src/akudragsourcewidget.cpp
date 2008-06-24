#include "akudragsourcewidget.h"

akuDragSourceWidget::akuDragSourceWidget(QWidget *parent) : QWidget(parent)
{
  hiddenFiles = new QCheckBox(i18n("Show hidden files"), this);
  upDirButton = new QToolButton(this);
  upDirButton -> setIcon(KIcon("go-up"));
  upDirButton -> setAttribute(Qt::WA_AlwaysShowToolTips);
  upDirButton -> setToolTip(i18n("Parent Folder"));
  upDirButton -> setAutoRaise(true);
  currentDirectory = new QLabel(this);
  icon = new QLabel(this);
  icon -> setMaximumSize(16,16);
  sourceView = new dragSource(this);
  QGridLayout *optLayout = new QGridLayout();
  optLayout -> addWidget(icon, 1,1);
  optLayout -> addWidget(currentDirectory,1,2);
  optLayout -> addWidget(upDirButton, 2,1);
  optLayout -> addWidget(hiddenFiles,2,2);
  QGridLayout *layout = new QGridLayout(this);
  layout -> addLayout(optLayout,1,1);
  layout -> addWidget(sourceView,2,1);
  connect(hiddenFiles, SIGNAL(toggled(bool)), sourceView, SLOT(showHiddenFiles(bool)));
  connect(upDirButton, SIGNAL(clicked()), sourceView, SLOT(dirUp()));
  connect(sourceView, SIGNAL(currentUrlChanged(KUrl)), this, SLOT(updateCurrentDirInfo(KUrl)));
  KUrl current = sourceView -> rootUrl();
  currentDirectory -> setText(current.fileName());
  QPixmap pixmap = KIcon(KMimeType::iconNameForUrl(current)).pixmap(16,16);
  icon -> setPixmap(pixmap);
}

akuDragSourceWidget::~akuDragSourceWidget()
{
}

dragSource * akuDragSourceWidget::sourceViewInUse()
{
  return sourceView;
}

void akuDragSourceWidget::updateCurrentDirInfo(KUrl current)
{
  currentDirectory -> setText(current.fileName());
  QPixmap pixmap = KIcon(KMimeType::iconNameForUrl(current)).pixmap(16,16);
  icon -> setPixmap(pixmap);
}
