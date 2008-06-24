#ifndef AKUWAITINGDIALOG_H
#define AKUWAITINGDIALOG_H

#include <KProgressDialog>
#include <KLocale>
#include <QTimer>
class akuWaitDialog : public KProgressDialog
{

Q_OBJECT

public:
  akuWaitDialog(QWidget* parent = 0);
  ~akuWaitDialog();
  void reject();

public slots:
  virtual void quit();
  virtual void exec();

protected slots:
  virtual void moveCursor();

private:
  QTimer *cursorTimer;

};
#endif
