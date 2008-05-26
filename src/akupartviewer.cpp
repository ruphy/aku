#include "akupartviewer.h"
#include <KVBox>
#include <KUrl>
#include <KMimeType>
#include <KIconLoader>
#include <KParts/ComponentFactory>
#include <QLabel>
#include <QHBoxLayout>

akuPartViewer::akuPartViewer(QWidget *parent) : KDialog(parent), m_part( 0 )
{
  setButtons( Close );
  m_widget = new KVBox( this );
  
  setMainWidget ( m_widget );
}

akuPartViewer::~akuPartViewer()
{}

bool akuPartViewer::view(const QString &filename, const QByteArray &data)
{
  KUrl u(filename);
  KMimeType::Ptr mimetype = KMimeType::findByUrl( u, 0, false); // the file is not phisical
  QSize size(400,500);
  setInitialSize(size);
  setCaption(u.fileName());
  QWidget *top_w = new QWidget (m_widget);
  QHBoxLayout *topLayout = new QHBoxLayout(top_w);
  QLabel *icon = new QLabel(top_w);
  topLayout->addWidget(icon);
  icon->setPixmap(KIconLoader::global()->loadMimeTypeIcon(mimetype->iconName(), KIconLoader::Desktop ) );
  icon->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Minimum );
  KVBox *details = new KVBox(top_w);
  topLayout -> addWidget(details);
  QLabel *fileName = new QLabel(KUrl(filename).fileName(), details);
  QFont font;
  font.setBold(true);
  fileName->setFont(font);
  QLabel *fileType = new QLabel(mimetype->comment(), details);
  m_part = KParts::ComponentFactory::createPartInstanceFromQuery<KParts::ReadOnlyPart>( mimetype->name(), QString(), m_widget, this );

  if (m_part){
     // m_part->openUrl(u);
     m_part->openStream(mimetype->name(), u);
     m_part->writeStream(data);
     exec();
     return true;
  }
   else return false;
}
