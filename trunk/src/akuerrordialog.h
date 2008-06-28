#ifndef AKUERRORDIALOG_H
#define AKUERRORDIALOG_H

#include <QHBoxLayout>
#include <QLabel>

#include <KDialog>
#include <KTextEdit>
#include <KVBox>
#include <KIconLoader>
#include <KLocale>

class akuErrorDialog : public KDialog
{
 Q_OBJECT

  public:
   akuErrorDialog(QWidget *parent=0);
   ~akuErrorDialog();

  public slots:
   virtual void setError(QString);

  private:
   KTextEdit *textedit;
};

#endif
