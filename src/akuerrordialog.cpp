#include "akuerrordialog.h"

#include <KVBox>
#include <QHBoxLayout>
#include <KIconLoader>
#include <QLabel>
#include <KLocale>

akuErrorDialog::akuErrorDialog(QWidget *parent) : KDialog(parent)
{
 setButtons(Close);
 KVBox *layout = new KVBox(this);
 setMainWidget(layout);
 QWidget *topW = new QWidget(layout);
 QHBoxLayout *topLayout = new QHBoxLayout(topW);
 QLabel *icon = new QLabel(topW);
 icon->setPixmap(KIconLoader::global()->loadIcon("dialog-warning", KIconLoader::Desktop));
 icon->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
 topLayout->addWidget(icon);
 QLabel *title = new QLabel(topW);
 title->setText("<b>"+i18n("One or more errors occurred:")+"</b>");
 title->setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
 topLayout->addWidget(title);
 t = new KTextEdit(layout);
}

akuErrorDialog::~akuErrorDialog()
{}

void akuErrorDialog::setError(QString er)
{
 t->setText(er);
}
