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
#include <KUrl>
#include <KDateTime>
#include <KLocale>

#include "akuratiowidget.h"

class rar
{
  public:
    rar();
    ~rar();

    int parse (QTreeWidget *, QString bf, akuRatioWidget *);
    void simpleParse ( QTreeWidget *, QString bf );

    QStringList getArchiveDetails();
    QStringList getFileList(QString);
    QStringList getAttributeList(QString);
    //gets the content of a folder in the archive or returns the sinle file
    QStringList getListFromPath(QString, QString);

    static QString getSingleFileSize(QString, QString);
    static KDateTime getSingleFileModificationTime(QString, QString); 

  private:
    QString ftext; //memorizzo il nome del file
    QString getSingleFileAttributes(QString, QString);

    QStringList mimeTypes;
    QStringList cache; //cache idea by Francesco Grieco
    QStringList backwardPathsCache; //cache test per la ramificazione
    QStringList archiveDetails;

};

#endif
