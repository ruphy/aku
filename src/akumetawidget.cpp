#include "akumetawidget.h"

akuMetaWidget::akuMetaWidget (QWidget *parent) : QWidget (parent)
{

  QWidget *baseScrollWidget = new QWidget(this);
  scrollArea = new QScrollArea(this);
  scrollArea -> setBackgroundRole(QPalette::Dark);
  iconMap = new QLabel(this);
  iconMap -> setMinimumSize(0,128);
  iconMap -> setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  metaName = new QLabel(this);
  metaName -> setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  metaName -> setWordWrap(true);
  metaSize = new QLabel(this);
  metaSize -> setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  metaSize -> setWordWrap(true);
  metaMime = new QLabel(this);
  metaMime -> setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  metaMime -> setWordWrap(true);
  ratio = new akuRatioWidget(0, this);
  ratio->setVisible(false);
  QSpacerItem *spacer = new QSpacerItem(1,3, QSizePolicy::Minimum, QSizePolicy::Fixed);
  singleFileLayout = new QGridLayout();
  singleFileLayout -> addWidget(iconMap, 1,1);
  singleFileLayout -> addItem(spacer,2,1);
  singleFileLayout -> addWidget(metaName, 3,1);
  singleFileLayout -> addWidget(metaSize,4,1);
  singleFileLayout -> addWidget(metaMime,5,1);
  singleFileLayout -> addWidget(ratio,6,1);
  QSpacerItem *spacer2 = new QSpacerItem(1,10, QSizePolicy::Minimum, QSizePolicy::Expanding);
  singleFileLayout -> addItem(spacer2,7,1);
  // scrollArea -> setWidget(iconMap);
  scrollArea -> setWidgetResizable(true);



  QGridLayout *general = new QGridLayout(this);
  metaLayout = new QGridLayout(baseScrollWidget);
  metaLayout -> addLayout(singleFileLayout,1,1);
  // metaLayout -> addWidget(line,2,1);
  // metaLayout -> addLayout(archiveLayout,3,1);
  // metaLayout -> addItem(thirdSpacer,4,1);
  general -> addWidget(scrollArea,1,1);
  scrollArea -> setWidget(baseScrollWidget);
  setMinimumSize(150,0);
  setMaximumSize(280,6574777);

}

akuMetaWidget::~akuMetaWidget()
{ 
  qDeleteAll(children());
}

void akuMetaWidget::setPreview(QByteArray preview)
{
  QPixmap pixPrev;
  QPalette palette;
  pixPrev.fill();
  pixPrev.loadFromData(preview);
  QPixmap scaledMap = pixPrev.scaled(QSize(192,192), Qt::KeepAspectRatio);
  QRect rect = scaledMap.rect();
  QPixmap final(rect.width()+7, rect.height()+7);
  QRect finalRect = final.rect();
  final.fill(palette.color(QPalette::Dark));
  QPainter decor(&final);
  decor.setRenderHint(QPainter::Antialiasing);
  decor.setPen(palette.color(QPalette::Shadow));
  decor.setBrush(palette.color(QPalette::Shadow));
  decor.drawRoundRect(QRect(1.9,2.3,finalRect.width()-6, finalRect.height()-6),3,3);
  //decor.fillRect(QRect(0,3,finalRect.width(), finalRect.height()-3), QBrush(palette.color(QPalette::Shadow)));
  decor.drawPixmap(QRect(0,0,rect.width(), rect.height()), pixPrev);
  
  setMimeIcon(final.scaled(QSize(192,192), Qt::KeepAspectRatio));
}

void akuMetaWidget::clear()
{
 metaName->clear();
 metaSize->clear();
 metaMime->clear();
 ratio->setVisible(false);
}

void akuMetaWidget::setMimeIcon(QPixmap iconPixmap)
{
  iconMap -> setPixmap (iconPixmap);
}

void akuMetaWidget::setMime(QString mime)
{
  metaMime -> setText(mime);
}

void akuMetaWidget::setFileName(QString name, bool folder)
{
  QFont font;
  font.setBold(true);
  metaName -> setFont(font);
  metaName -> setText(name);
  if(folder == false)
  {
    KMimeType::Ptr mimePtr = KMimeType::findByUrl(KUrl(name));
    QPixmap pixmap = KIcon(mimePtr -> iconName()).pixmap(128,128);
    setMimeIcon(pixmap);
    setMime(mimePtr -> comment());
  }
  else
  {
    QPixmap pixmap = KIcon("inode-directory").pixmap(128,128);
    setMimeIcon(pixmap);
    setMime(i18n("Folder"));
    ratio->setVisible(false);
  }
}

void akuMetaWidget::handleItemSelections(QList<QTreeWidgetItem*> list)
{
  int listSize = list.size();
  //icons =  192 / listSize;
  if(listSize>1) ratio->setVisible(false);
  QPalette palette;
  QPixmap view(128,128);
  view.fill(palette.color(QPalette::Dark));
  int height = 128;
  QStringList iconNames;
  for(int i = 0; i < listSize; i ++)
  {
    KMimeType::Ptr mimePtr = KMimeType::findByUrl(KUrl(list[i]->text(0)));
    QString mimeIconName;
    if(list[i] -> text(1).isEmpty() ) mimeIconName = "inode-directory";
    else mimeIconName = mimePtr -> iconName();
    QPixmap icon;
    if(!iconNames.contains(mimeIconName))
    {
      iconNames << mimeIconName;
      QStringList tempIconList;
      tempIconList << iconNames;
      if(tempIconList.size() > 1 )tempIconList.removeAll("inode-directory");
      view = QPixmap(128+(15*(tempIconList.size()-1)),height);
      view.fill(palette.color(QPalette::Dark));
      for(int j = 0; j < tempIconList.size(); j++)
      {
        icon  = KIcon(tempIconList[j]).pixmap(height,height);
        QPainter painter(&view);
        painter.drawPixmap(QRect(15*j, 0, height,height), icon);
      }

    }
   }
   if(iconNames.size()== 1) 
   {
     KMimeType::Ptr mimePtr = KMimeType::findByUrl(KUrl(list[0] ->text(0)));
     if (mimePtr -> iconName() == "application-octet-stream" && list[0] -> text(1).isEmpty()) setMime(i18n("Folder"));
     else  setMime(mimePtr -> comment());
   }
   else setMime(QString());
   iconMap -> setPixmap(view);
}

void akuMetaWidget::setFileSize(QString size)
{
  metaSize -> setText(size);
}

void akuMetaWidget::setRatio(float r)
{
 ratio->setVisible(true);
 ratio->setRatio(r);
}
