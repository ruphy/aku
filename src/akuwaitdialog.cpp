#include "akuwaitdialog.h"

akuWaitDialog::akuWaitDialog(QWidget *parent) : KProgressDialog(parent, i18n("Processing Archive"))
{
  progressBar() -> setMaximumSize(654999, 20);
  progressBar() -> setMaximum(0);
  progressBar() -> setMinimum(0);
  progressBar() -> setValue(0);
  setMinimumSize(320, 100);
  setMaximumSize(320, 100);
  setModal(true);
  setAutoClose(false);
  setAutoReset(false);
  setAllowCancel(false);
}

akuWaitDialog::~akuWaitDialog()
{
  qDeleteAll(children());
}

void akuWaitDialog::exec()
{
  show();
  cursorTimer = new QTimer();
  connect(cursorTimer, SIGNAL(timeout()), this, SLOT(moveCursor()));
  cursorTimer -> start(500);
}

void akuWaitDialog::moveCursor()
{
  progressBar() -> setValue(progressBar() -> value() + 10);
  setAllowCancel(false);
}

void akuWaitDialog::quit()
{
  cursorTimer -> stop();
  disconnect(cursorTimer, SIGNAL(timeout()), this, SLOT(moveCursor()));
  close();
}

void akuWaitDialog::reject()
{
  quit();
}
