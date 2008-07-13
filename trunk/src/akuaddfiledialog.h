#ifndef AKUADDFILEDIALOG_H
#define AKUADDFILEDIALOG_H

#include <QSplitter>
#include <QCheckBox>

#include <KFileDialog>
#include <KVBox>

#include <KDebug>

#include <kfilewidget.h>
#include <knewpassworddialog.h>

#include "rarprocess.h"

class akuAddFileDialog : public KFileDialog
{
  Q_OBJECT

  public:
    akuAddFileDialog (QWidget* parent = 0, bool headerpass = false);
    ~akuAddFileDialog();

  public slots:
    virtual void checkPassword(int);
    virtual void noPassword();
    virtual void setPassword(const QString&);

  private: 
    QLabel *label;

    QString password;

    QCheckBox *addWithPassword;


  protected slots:
    virtual void slotDialogAccepted();

  signals:
    void destination(QStringList, QString);

};
#endif
