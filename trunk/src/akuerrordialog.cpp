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
 textdialog = new KTextEdit(layout);
 textdialog->setReadOnly(true);
 setCaption(i18n("Error"));
}

akuErrorDialog::~akuErrorDialog()
{
}

void akuErrorDialog::setError(QString er)
{
 textdialog->setText(er);
}