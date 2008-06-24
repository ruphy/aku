
#include "akucomment.h"

akuComment::akuComment(QWidget *parent) : QDialog(parent)
{
  QString 
  setModal (true);
  textEdit = new KTextEdit(this);
  setWindowTitle(i18n("Add archive comment"));
  //enableButtonOk(false);
  //enableButtonCancel(false);
  //showButton(KDialog::Ok, false);
  //showButton(KDialog::Cancel, false);
  KDialogButtonBox *kdialogbuttonbox = new KDialogButtonBox(this);
  KGuiItem okGuiItem(i18n("Ok"), "dialog-ok");
  KGuiItem cancelGuiItem(i18n("Cancel"), "dialog-cancel");
  KGuiItem loadGuiItem(i18n("Load comment from file"), "document-open");
  KPushButton *loadButton = new KPushButton(this);
  loadButton -> setGuiItem(loadGuiItem);
  kdialogbuttonbox -> addButton(okGuiItem, QDialogButtonBox::AcceptRole);
  kdialogbuttonbox -> addButton(cancelGuiItem, QDialogButtonBox::RejectRole);
  QSpacerItem *spacer = new QSpacerItem(50, 10, QSizePolicy::Minimum, QSizePolicy::Minimum);
  QGridLayout *buttonLayout = new QGridLayout();
  buttonLayout -> addWidget(kdialogbuttonbox, 1, 3);
  buttonLayout -> addWidget(loadButton, 1, 1);
  buttonLayout -> addItem (spacer, 1, 2);
  QGridLayout *baseLayout = new QGridLayout(this);
  baseLayout -> addWidget(textEdit, 1, 1);
  baseLayout -> addLayout(buttonLayout, 2, 1);
  connect(kdialogbuttonbox, SIGNAL(accepted()), this, SLOT(writeComment()));
  connect(kdialogbuttonbox, SIGNAL(rejected()), this, SLOT(reject()));
  connect(loadButton, SIGNAL(clicked()), this, SLOT(loadFromFile()));
}

akuComment::~akuComment()
{
  qDeleteAll(children());
}

void akuComment::writeComment()
{ 
  setCursor(Qt::WaitCursor);
  QString comment = textEdit -> toPlainText();
  if (!comment.isEmpty()) 
  {
    emit okClicked(comment);
  }
  accept();
}

void akuComment::loadFromFile()
{
  QString fname = KFileDialog::getOpenFileName(KUrl(QDir().homePath()), i18n("All Files *.*"), this, i18n("Select comment file"));
        
  if ( fname != "" )
  { 
    QFile file(fname);
    file.open(QIODevice::ReadOnly);
    textEdit -> setPlainText(QTextStream(&file).readAll());
  }
}
