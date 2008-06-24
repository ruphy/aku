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

  int parse (QTreeWidget *, QString bf, akuRatioWidget *);
  void simpleParse ( QTreeWidget *, QString bf );

};

#endif
