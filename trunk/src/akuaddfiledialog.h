#ifndef AKUADDFILEDIALOG_H
#define AKUADDFILEDIALOG_H

#include <QSplitter>
#include <QCheckBox>

#include <KDialog>
#include <KVBox>

#include <KDebug>

#include <kfilewidget.h>
#include <knewpassworddialog.h>

#include "rarprocess.h"

class akuAddFileDialog : public KDialog
{
  Q_OBJECT

  public:
    akuAddFileDialog (QWidget* parent = 0);
    ~akuAddFileDialog();

  public slots:
    virtual void checkPassword(int);
    virtual void slotCancel();
    virtual void setPassword(const QString&);

  private: 
    QLabel *label;
    QString password;
    QCheckBox *addwithpassword;

};
#endif
