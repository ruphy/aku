#include "akuaddfiledialog.h"

akuAddFileDialog::akuAddFileDialog(QWidget *parent) : KDialog(parent)
{
   QSplitter *mainlayout = new QSplitter(this);
   setMainWidget(mainlayout);
   fw = new KFileWidget(KUrl(QDir::homePath()), mainlayout);
   fw -> setMode(KFile::Files);

   KVBox *layout = new KVBox();
   layout -> setSpacing(8);
   addwithpassword = new QCheckBox(i18n("Add file(s) with a password"), layout);
   label = new QLabel(layout);
   label -> setText("<font color=red>" + i18n("Password inserted") + "</font>");
   label -> setVisible(false);
   fw -> setCustomWidget(layout);
   setInitialSize(QSize(700, 420));

   connect(addwithpassword, SIGNAL(stateChanged(int)), this, SLOT(checkPassword(int)));
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
    connect(dlg, SIGNAL(rejected()), this, SLOT(slotCancel()));
    dlg->show();
  } 
  else 
    password.clear();
    label -> setVisible(false);
}

void akuAddFileDialog::setPassword(const QString& newpassword)
{  
  password = newpassword;
  label -> setVisible(true);
}

void akuAddFileDialog::slotCancel()
{
  addwithpassword -> setChecked(false);
  password.clear();
  label -> setVisible(false);
}

void akuAddFileDialog::slotButtonClicked(int button) 
{
  if (button == KDialog::Ok) {
    QStringList test;
    test << fw -> selectedFiles();
    kDebug() << test;
    QStringList lista = fw -> selectedFiles();
    kDebug() << fw -> selectedFiles();
    emit destination(KUrl::List(fw -> selectedUrls()), QString()); 
    accept();
  }

  else if (button == KDialog::Cancel) reject();
}

