#ifndef TAR_H
#define TAR_H

#include <QTreeWidget>
#include <QDir>
#include <QRegExp>
#include <QDateTime>

#include <KIcon>
#include <KMimeType>
#include <KLocale>

#include "akuratiowidget.h"

class tar
{
  public:
    tar();
    ~tar();

    void parse (QTreeWidget *, QString bf);
    void simpleParse ( QTreeWidget *, QString bf );
    QStringList getArchiveDetails();

  private:
    QStringList archiveDetails;
};

#endif
