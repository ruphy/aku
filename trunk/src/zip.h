#ifndef ZIP_H
#define ZIP_H

#include <QTreeWidget>
#include <QDir>
#include <QDateTime>
#include <QRegExp>

#include <KIcon>
#include <KMimeType>
#include <KLocale>

#include "akuratiowidget.h"

class zip
{
  public:
  zip();
  ~zip();

  int parse (QTreeWidget *, QString bf, akuRatioWidget *);
  void simpleParse ( QTreeWidget *, QString bf );

};

#endif
