#include "akumetawidget.h"
#include <QVBoxLayout>

akuMetaWidget::akuMetaWidget (QWidget *parent) : QWidget (parent)
{

  baseScrollWidget = new KVBox(this);
 // scrollArea = new QScrollArea(this);
 // scrollArea -> setBackgroundRole(QPalette::Dark);
 // scrollArea->setFrameShape(QFrame::NoFrame);
 // scrollArea->setFrameShadow(QFrame::Plain);
  iconMap = new QLabel(baseScrollWidget);

  iconMap -> setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  iconMap->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  metaName = new QLabel(baseScrollWidget);
  metaName -> setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  metaName -> setWordWrap(true);
  metaName -> setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  metaSize = new QLabel(baseScrollWidget);
  metaSize -> setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  metaSize -> setWordWrap(true);
  metaSize -> setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  dtime = new QLabel(baseScrollWidget);
  dtime->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  dtime->setWordWrap(true);
  dtime->setSizePolicy(QSizePolicy::Minimum , QSizePolicy::Minimum);
  metaMime = new QLabel(baseScrollWidget);
  metaMime -> setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  metaMime -> setWordWrap(true);
  metaMime -> setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  QFrame *line = new QFrame(baseScrollWidget);
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  w_ratio = new QWidget(baseScrollWidget);
  QHBoxLayout *ratioL = new QHBoxLayout(w_ratio);
  ratio = new akuRatioWidget(0, w_ratio);
  w_ratio->setVisible(false);
  ratio->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  QLabel *label = new QLabel("<b>"+i18n("Ratio:")+"</b>", w_ratio);
  label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  label->setAlignment( Qt::AlignRight | Qt::AlignVCenter);
  ratioL->addWidget(label);
  ratioL->addWidget(ratio);
  QWidget *spacer = new QWidget(baseScrollWidget);
  spacer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

 // scrollArea -> setWidget(baseScrollWidget);
 // scrollArea -> setWidgetResizable(true);

  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->addWidget(baseScrollWidget);

  setMinimumSize(150,0); //TODO: do not hardcode sizes
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
  final.fill(Qt::transparent);
  QPainter decor(&final);
  decor.setRenderHint(QPainter::Antialiasing);
  decor.setPen(palette.color(QPalette::Shadow));
  decor.setBrush(palette.color(QPalette::Shadow));
  decor.drawRoundRect(QRect(2.9,2.3,finalRect.width()-6, finalRect.height()-6),3,3);
  //decor.fillRect(QRect(0,3,finalRect.width(), finalRect.height()-3), QBrush(palette.color(QPalette::Shadow)));
  decor.drawPixmap(QRect(0,0,rect.width(), rect.height()), pixPrev);
  
  setMimeIcon(final.scaled(QSize(192,192), Qt::KeepAspectRatio));
}

void akuMetaWidget::setDateTime(const QDateTime &dt)
{
 QString niceDate;
 niceDate += QString().setNum(dt.date().day())
          += QString(" ")
          += QDate::longMonthName(dt.date().month())
          += QString(" ")
          += QString().setNum(dt.date().year()).right(2)
          += QString(" ")
          += dt.time().toString(Qt::LocaleDate);

 dtime->setVisible(true);
 dtime -> setText("<b>"+i18n("Modified:")+"</b> "+niceDate);
}
void akuMetaWidget::clear()
{
 metaName->clear();
 metaSize->clear();
 metaMime->clear();
 w_ratio->setVisible(false);
}

void akuMetaWidget::setMimeIcon(QPixmap iconPixmap)
{
  iconMap -> setPixmap (iconPixmap);
}

void akuMetaWidget::setMime(QString mime)
{
  metaMime->setText(QString());
  if(!mime.isEmpty()) metaMime -> setText("<b>"+i18n("Type: ")+"</b>"+mime);
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
    w_ratio->setVisible(false);
    dtime->setVisible(false);
  }

}

void akuMetaWidget::handleItemSelections(QList<QTreeWidgetItem*> list)
{
  int listSize = list.size();
  //icons =  192 / listSize;
  if(listSize>1){
    w_ratio->setVisible(false);
    dtime->setVisible(false);
  }
  QPalette palette;
  QPixmap view(128,128);
  view.fill(Qt::transparent);
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
      view.fill(Qt::transparent);
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

 //  TODO: see if this multiple-ratio-view is needed and nice
 //
 // for(int x=0; x<list.size(); x++){                                           
 //   akuRatioWidget *r = new akuRatioWidget(list[x]->text(3).remove("%").toFloat(), baseScrollWidget);
 //   r->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
 //  }
}


void akuMetaWidget::setFileSize(QString size)
{
  metaSize -> setText(size);
}

void akuMetaWidget::setRatio(float r)
{
 w_ratio->setVisible(true);
 ratio->setRatio(r);
}
