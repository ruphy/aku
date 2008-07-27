#include "akudragsourcewidget.h"

akuDragSourceWidget::akuDragSourceWidget(QWidget *parent) : QWidget(parent)
{
  QVBoxLayout *layout = new QVBoxLayout(this);
  KVBox *vbox = new KVBox(this);
  vbox -> setSpacing(10);
  layout -> addWidget(vbox);
  setLayout(layout);
  KHBox *hbox1 = new KHBox(vbox);
  hbox1 -> setSpacing(10);
  icon = new QLabel(hbox1);
  icon -> setMaximumSize(16,16);
  currentDirectory = new QLabel(hbox1);

  KHBox *hbox2 = new KHBox(vbox);
  hbox2 -> setSpacing(3);
  upDirButton = new QToolButton(hbox2);
  upDirButton -> setIcon(KIcon("go-up"));
  upDirButton -> setAttribute(Qt::WA_AlwaysShowToolTips);
  upDirButton -> setToolTip(i18n("Parent Folder"));
  upDirButton -> setAutoRaise(true);  
  homeButton = new QToolButton(hbox2);
  homeButton -> setIcon(KIcon("go-home"));
  homeButton -> setAttribute(Qt::WA_AlwaysShowToolTips);
  homeButton -> setToolTip(i18n("Home Folder"));
  homeButton -> setAutoRaise(true); 
  hiddenFiles = new QCheckBox(i18n("Show hidden files"), hbox2);
  sourceView = new dragSource(vbox); 

  connect(hiddenFiles, SIGNAL(toggled(bool)), sourceView, SLOT(showHiddenFiles(bool)));
  connect(upDirButton, SIGNAL(clicked()), sourceView, SLOT(dirUp()));
  connect(homeButton, SIGNAL(clicked()), sourceView, SLOT(goHome()));
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
