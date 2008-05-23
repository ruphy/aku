#ifndef AKUPARTVIEWER_H
#define AKUPARTVIEWER_H
// this code and what comes with akupartviewer.cpp is widely taken from arkviewer. Thanks to the ark team
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
