#include "akuprogresstooltip.h"
#include <QGridLayout>

akuProgressTooltip::akuProgressTooltip(QWidget *parent) : QWidget(parent)
{
 setWindowFlags(Qt::ToolTip);
 currentFileName = new QLabel(this);
 archiveName = new QLabel(this);
 currentFileName->setAlignment(Qt::AlignHCenter);
 archiveName->setAlignment(Qt::AlignHCenter);
 currentProgress = new akuRatioWidget(0,this);
 overallProgress = new akuRatioWidget(0,this);
 QGridLayout *layout = new QGridLayout(this);
 layout->addWidget(currentFileName,1,1);
 layout->addWidget(currentProgress,2,1);
 layout->addWidget(archiveName,3,1);
 layout->addWidget(overallProgress,4,1);
}

akuProgressTooltip::~akuProgressTooltip()
{}

void akuProgressTooltip::setCurrentFileName(QString name)
{
 currentFileName->setText(name);
}

void akuProgressTooltip::setArchiveName(QString name)
{
 archiveName->setText(name);
}

void akuProgressTooltip::setCurrentFileProgress(int prog)
{
 currentProgress->setRatio(prog);
}

void akuProgressTooltip::setArchiveProgress(int prog)
{
 overallProgress->setRatio(prog);
}

QSize sizeHint()
{
 return QSize(100,100);
}
