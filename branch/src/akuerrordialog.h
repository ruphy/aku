#ifndef AKUERRORDIALOG_H
#define AKUERRORDIALOG_H
#include <KDialog>
#include <KTextEdit>

class akuErrorDialog : public KDialog
{
 Q_OBJECT
  public:
   akuErrorDialog(QWidget *parent=0);
   ~akuErrorDialog();

  public slots:
   virtual void setError(QString);

  private:
   KTextEdit *t;
};
#endif
