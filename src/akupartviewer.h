#ifndef AKUPARTVIEWER_H
#define AKUPARTVIEWER_H

#include <KDialog>
#include <KParts/ReadOnlyPart>

class akuPartViewer : public KDialog
{
  Q_OBJECT

  public:
       akuPartViewer(QWidget *parent = 0);
       ~akuPartViewer();

       bool view (const QString& filename );
   private:
       KParts::ReadOnlyPart *m_part;
       QWidget *m_widget;
};
#endif
