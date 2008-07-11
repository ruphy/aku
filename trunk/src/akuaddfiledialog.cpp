#include "akuaddfiledialog.h"

akuAddFileDialog::akuAddFileDialog(QWidget *parent) : KDialog(parent)
{
   QSplitter *mainlayout = new QSplitter(this);
   setMainWidget(mainlayout);
   KFileWidget *fw = new KFileWidget(KUrl(QDir::homePath()), mainlayout);
   fw -> setMode(KFile::Files);
   QCheckBox *test = new QCheckBox();
   fw -> setCustomWidget("ciao", test);
   //mainlayout -> addWidget(fw);
   setInitialSize(QSize(750, 400));
}

akuAddFileDialog::~akuAddFileDialog()
{
}
