
#include "akucomment.h"

akuComment::akuComment(QString comment, QWidget *parent) : QDialog(parent)
{
  oldcomment = comment;
  setModal (true);
  textEdit = new KTextEdit(this);
  setWindowTitle(i18n("Add archive comment"));
  KDialogButtonBox *kdialogbuttonbox = new KDialogButtonBox(this);
  KGuiItem okGuiItem(i18n("Ok"), "dialog-ok");
  KGuiItem cancelGuiItem(i18n("Cancel"), "dialog-cancel");
  KGuiItem loadGuiItem(i18n("Load comment from file"), "document-open");
  KGuiItem previousText(i18n("Previous comment"), "document-revert");
  KPushButton *loadButton = new KPushButton(this);
  loadButton -> setGuiItem(loadGuiItem);
  KPushButton *previousButton = new KPushButton(this);
  previousButton -> setGuiItem(previousText);
  kdialogbuttonbox -> addButton(okGuiItem, QDialogButtonBox::AcceptRole);
  kdialogbuttonbox -> addButton(cancelGuiItem, QDialogButtonBox::RejectRole);
  QSpacerItem *spacer = new QSpacerItem(100, 10, QSizePolicy::Minimum, QSizePolicy::Minimum);
  QGridLayout *buttonLayout = new QGridLayout();
  buttonLayout -> addWidget(kdialogbuttonbox, 1, 3);
  buttonLayout -> addWidget(loadButton, 1, 1);
  buttonLayout -> addWidget(previousButton, 1, 2);
  buttonLayout -> addItem (spacer, 1, 3);
  QGridLayout *baseLayout = new QGridLayout(this);
  baseLayout -> addWidget(textEdit, 1, 1);
  baseLayout -> addLayout(buttonLayout, 2, 1);
  connect(kdialogbuttonbox, SIGNAL(accepted()), this, SLOT(writeComment()));
  connect(kdialogbuttonbox, SIGNAL(rejected()), this, SLOT(reject()));
  connect(loadButton, SIGNAL(clicked()), this, SLOT(loadFromFile()));
  connect(previousButton, SIGNAL(clicked()), this, SLOT(loadPreviousText()));
}

akuComment::~akuComment()
{
  qDeleteAll(children());
}

void akuComment::writeComment()
{ 
  setCursor(Qt::WaitCursor);
  QString comment = textEdit -> toPlainText();
  if (!comment.isEmpty()) {
    emit okClicked(comment);
  }
  accept();
}

void akuComment::loadPreviousText()
{
  textEdit -> setPlainText(oldcomment);
}

void akuComment::loadFromFile()
{
  QString fname = KFileDialog::getOpenFileName(KUrl(QDir().homePath()), i18n("All Files *.*"), this, i18n("Select comment file"));
        
  if ( !fname.isEmpty()) { 
    QFile file(fname);
    file.open(QIODevice::ReadOnly);
    textEdit -> setPlainText(QTextStream(&file).readAll());
  }
}
