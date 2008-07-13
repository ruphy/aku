#include "akuaddfiledialog.h"

akuAddFileDialog::akuAddFileDialog(QWidget *parent, bool headerpass) : KFileDialog(KUrl(QDir::homePath()), "*.*", parent)
{
    setOperationMode(KFileDialog::Other);
    setMode(KFile::Files);
    setModal(true);
    // Here we set the custom widget up (this is used for adding files crypting them with password)
    KVBox *layout = new KVBox();
    layout -> setSpacing(8);
    addWithPassword = new QCheckBox(i18n("Add file(s) with a password"), layout);
    label = new QLabel(layout);
    label -> setText("<font color=red>" + i18n("Password inserted") + "</font>");
    label -> setVisible(false);
    static_cast<KFileWidget*>( fileWidget())->setCustomWidget(layout);
    setInitialSize(QSize(700, 420));
    password.clear();

    connect(addWithPassword, SIGNAL(stateChanged(int)), this, SLOT(checkPassword(int)));
    connect(okButton(), SIGNAL(clicked()), this, SLOT(slotOk()));
    connect(cancelButton(), SIGNAL(clicked()), this, SLOT(slotCancel()));
    connect(this, SIGNAL(accepted()), this, SLOT(slotDialogAccepted()));

    if (headerpass) {
      addWithPassword -> setEnabled(false);
      label -> setText("<font color=red>" + i18n("The archive is already header password protected") + "</font>");
      label -> setVisible(true);
    }
}

akuAddFileDialog::~akuAddFileDialog()
{
}

void akuAddFileDialog::checkPassword(int state)
{
  // 0 = Unchecked
  // 2 = Checked
  if (state == 2) {
    KNewPasswordDialog *dlg = new KNewPasswordDialog(this);
    dlg -> setModal(true);
    dlg -> setAllowEmptyPasswords(false);
    dlg -> setPrompt("<b>" + i18n("Enter a password") + "</b>");
    connect(dlg, SIGNAL(newPassword(const QString&)), this, SLOT(setPassword(const QString&)));
    connect(dlg, SIGNAL(rejected()), this, SLOT(noPassword()));
    dlg->show();
  } 
  else{ 
    password.clear();
    label -> setVisible(false);
  }
}


void akuAddFileDialog::setPassword(const QString& newpassword)
{  
  password = newpassword;
  label -> setVisible(true);
}

void akuAddFileDialog::noPassword()
{
  addWithPassword -> setChecked(false);
  password.clear();
  label -> setVisible(false);
}


void akuAddFileDialog::slotDialogAccepted() 
{

    kDebug()<<selectedFiles();
    emit destination(QStringList(selectedFiles()), password); 
}
