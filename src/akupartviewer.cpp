#include "akupartviewer.h"
#include <KVBox>
#include <KUrl>
#include <KMimeType>
#include <KIconLoader>
#include <KParts/ComponentFactory>
#include <QLabel>

akuPartViewer::akuPartViewer(QWidget *parent) : KDialog(parent), m_part( 0 )
{
  setButtons( Close );
  m_widget = new KVBox( this );
  
  setMainWidget ( m_widget );
}

akuPartViewer::~akuPartViewer()
{}

bool akuPartViewer::view(const QString &filename)
{
  KUrl u(filename);
  KMimeType::Ptr mimetype = KMimeType::findByUrl( u, 0, true);

  setCaption(u.fileName());
  
  m_part = KParts::ComponentFactory::createPartInstanceFromQuery<KParts::ReadOnlyPart>( mimetype->name(), QString(), m_widget, this );

  if (m_part){
     m_part->openUrl(u);
     exec();
     return true;
  }
   else return false;
}
