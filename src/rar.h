#ifndef RAR_H
#define RAR_H
#include <QTreeWidget>
#include <QStatusBar>
#include <QLabel>
#include <QTextEdit>
#include <QProgressBar>
#include <QHeaderView>
#include <QMessageBox>
#include <QStringList>
#include <QDirModel>
#include <QDir>
#include <QPalette>
#include <QDateTime>

#include <KIcon>
#include <KFileItem>
#include <KMimeType>
#include <KSharedPtr>
#include <KUrl>
#include <KDateTime>
#include <KLocale>
#include "akuratiowidget.h"

class rar
{
  public:
  rar();
  ~rar();

  int parse ( QTreeWidget *, QString bf, akuRatioWidget * );
  void simpleParse ( QTreeWidget *, QString bf );
  QStringList getArchiveDetails();
  QStringList getFileList(QString);
  QStringList getAttributeList(QString);
  QString getSingleFileSize(QString, QString);
  KDateTime getSingleFileModificationTime(QString, QString);
  QStringList getListFromPath(QString, QString); //gets the content of a folder in the archive or returns the sinle file

 private:
  QString ftext; //memorizzo il nome del file
  QStringList mimeTypes;
  QStringList cache; //cache idea by Francesco Grieco
  QStringList backwardPathsCache; //cache test per la ramificazione
  QStringList archiveDetails;
  QString getSingleFileAttributes(QString, QString);

};

#endif
