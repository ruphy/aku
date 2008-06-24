#ifndef AKUPROGRESSTOOLTIP
#define AKUPROGRESSTOOLTIP

#include <QLabel>

#include "akuratiowidget.h"

class akuProgressTooltip : public QWidget
{
  Q_OBJECT

   public:
     akuProgressTooltip(QWidget *parent = 0);
     ~akuProgressTooltip();

     QSize sizeHint() const;

   public slots:
     virtual void setCurrentFileName(QString); 
     virtual void setCurrentFileProgress(int);
     virtual void setArchiveName(QString);
     virtual void setArchiveProgress(int);

   private:
     QLabel *currentFileName;
     QLabel *archiveName;

     akuRatioWidget *currentProgress;
     akuRatioWidget *overallProgress;
};

#endif
