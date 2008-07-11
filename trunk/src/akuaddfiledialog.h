#ifndef AKUADDFILEDIALOG_H
#define AKUADDFILEDIALOG_H

#include <QSplitter>
#include <QCheckBox>

#include <KDialog>

#include <KDebug>

#include <kfilewidget.h>

#include "rarprocess.h"

class akuAddFileDialog : public KDialog
{
  Q_OBJECT

public:
  akuAddFileDialog (QWidget* parent = 0);
  ~akuAddFileDialog();

};
#endif
