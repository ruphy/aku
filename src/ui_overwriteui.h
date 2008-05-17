#include <kdialog.h>
#include <klocale.h>

/********************************************************************************
** Form generated from reading ui file 'overwriteui.ui'
**
** Created: Thu May 15 23:58:17 2008
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_OVERWRITEUI_H
#define UI_OVERWRITEUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "klineedit.h"
#include "kpushbutton.h"
#include "ktextedit.h"

QT_BEGIN_NAMESPACE

class Ui_OverwriteDialog
{
public:
    QFrame *frame;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout;
    QLabel *destIconLabel;
    KTextEdit *destDetails;
    QHBoxLayout *horizontalLayout_2;
    QLabel *sourceIconLabel;
    KTextEdit *sourceDetails;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    KPushButton *yesButton;
    KPushButton *noButton;
    KPushButton *toallButton;
    KLineEdit *klineedit;
    KPushButton *renameButton;
    KPushButton *cancelButton;

    void setupUi(QDialog *OverwriteDialog)
    {
    if (OverwriteDialog->objectName().isEmpty())
        OverwriteDialog->setObjectName(QString::fromUtf8("OverwriteDialog"));
    OverwriteDialog->setWindowModality(Qt::WindowModal);
    OverwriteDialog->resize(573, 286);
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(OverwriteDialog->sizePolicy().hasHeightForWidth());
    OverwriteDialog->setSizePolicy(sizePolicy);
    OverwriteDialog->setMinimumSize(QSize(573, 286));
    OverwriteDialog->setMaximumSize(QSize(573, 286));
    frame = new QFrame(OverwriteDialog);
    frame->setObjectName(QString::fromUtf8("frame"));
    frame->setGeometry(QRect(10, 10, 381, 271));
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Sunken);
    gridLayoutWidget = new QWidget(frame);
    gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
    gridLayoutWidget->setGeometry(QRect(10, 10, 361, 251));
    gridLayout = new QGridLayout(gridLayoutWidget);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);
    label = new QLabel(gridLayoutWidget);
    label->setObjectName(QString::fromUtf8("label"));
    QFont font;
    font.setPointSize(9);
    font.setBold(true);
    font.setWeight(75);
    label->setFont(font);
    label->setTextInteractionFlags(Qt::NoTextInteraction);

    gridLayout->addWidget(label, 0, 0, 1, 1);

    label_3 = new QLabel(gridLayoutWidget);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setFont(font);
    label_3->setLineWidth(1);
    label_3->setMidLineWidth(1);
    label_3->setScaledContents(false);
    label_3->setWordWrap(false);
    label_3->setTextInteractionFlags(Qt::NoTextInteraction);

    gridLayout->addWidget(label_3, 4, 0, 1, 1);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    destIconLabel = new QLabel(gridLayoutWidget);
    destIconLabel->setObjectName(QString::fromUtf8("destIconLabel"));
    destIconLabel->setMinimumSize(QSize(57, 50));
    destIconLabel->setMaximumSize(QSize(57, 50));
    destIconLabel->setScaledContents(false);
    destIconLabel->setOpenExternalLinks(false);

    horizontalLayout->addWidget(destIconLabel);

    destDetails = new KTextEdit(gridLayoutWidget);
    destDetails->setObjectName(QString::fromUtf8("destDetails"));
    QFont font1;
    font1.setPointSize(9);
    destDetails->setFont(font1);
    destDetails->setTextInteractionFlags(Qt::LinksAccessibleByKeyboard|Qt::LinksAccessibleByMouse|Qt::TextBrowserInteraction|Qt::TextEditable|Qt::TextEditorInteraction|Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

    horizontalLayout->addWidget(destDetails);


    gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);

    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    sourceIconLabel = new QLabel(gridLayoutWidget);
    sourceIconLabel->setObjectName(QString::fromUtf8("sourceIconLabel"));
    sourceIconLabel->setMinimumSize(QSize(57, 50));
    sourceIconLabel->setMaximumSize(QSize(57, 50));
    sourceIconLabel->setScaledContents(false);

    horizontalLayout_2->addWidget(sourceIconLabel);

    sourceDetails = new KTextEdit(gridLayoutWidget);
    sourceDetails->setObjectName(QString::fromUtf8("sourceDetails"));
    sourceDetails->setFont(font1);

    horizontalLayout_2->addWidget(sourceDetails);


    gridLayout->addLayout(horizontalLayout_2, 5, 0, 1, 1);

    label->raise();
    label_3->raise();
    destDetails->raise();
    destIconLabel->raise();
    verticalLayoutWidget = new QWidget(OverwriteDialog);
    verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
    verticalLayoutWidget->setGeometry(QRect(419, 10, 141, 271));
    verticalLayout = new QVBoxLayout(verticalLayoutWidget);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    yesButton = new KPushButton(verticalLayoutWidget);
    yesButton->setObjectName(QString::fromUtf8("yesButton"));
    yesButton->setFlat(false);

    verticalLayout->addWidget(yesButton);

    noButton = new KPushButton(verticalLayoutWidget);
    noButton->setObjectName(QString::fromUtf8("noButton"));

    verticalLayout->addWidget(noButton);

    toallButton = new KPushButton(verticalLayoutWidget);
    toallButton->setObjectName(QString::fromUtf8("toallButton"));
    QFont font2;
    font2.setBold(false);
    font2.setItalic(false);
    font2.setWeight(50);
    toallButton->setFont(font2);
    toallButton->setCheckable(true);
    toallButton->setChecked(false);
    toallButton->setFlat(false);
    toallButton->setProperty("isDragEnabled", QVariant(true));

    verticalLayout->addWidget(toallButton);

    klineedit = new KLineEdit(verticalLayoutWidget);
    klineedit->setObjectName(QString::fromUtf8("klineedit"));
    klineedit->setEnabled(false);
    klineedit->setProperty("showClearButton", QVariant(true));

    verticalLayout->addWidget(klineedit);

    renameButton = new KPushButton(verticalLayoutWidget);
    renameButton->setObjectName(QString::fromUtf8("renameButton"));

    verticalLayout->addWidget(renameButton);

    cancelButton = new KPushButton(verticalLayoutWidget);
    cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

    verticalLayout->addWidget(cancelButton);


    retranslateUi(OverwriteDialog);

    QMetaObject::connectSlotsByName(OverwriteDialog);
    } // setupUi

    void retranslateUi(QDialog *OverwriteDialog)
    {
    OverwriteDialog->setWindowTitle(tr2i18n("Dialog", 0));
    label->setText(tr2i18n("The following file already exists", 0));
    label_3->setText(tr2i18n("Would you like to replace the existing file\n"
"with this one?", 0));

#ifndef UI_QT_NO_TOOLTIP
    destIconLabel->setToolTip(QString());
#endif // QT_NO_TOOLTIP

    destIconLabel->setText(QString());
    sourceIconLabel->setText(QString());
    yesButton->setText(tr2i18n("&Yes", 0));
    noButton->setText(tr2i18n("&No", 0));

#ifndef UI_QT_NO_TOOLTIP
    toallButton->setToolTip(tr2i18n("Assume Yes/No on all queries", 0));
#endif // QT_NO_TOOLTIP

    toallButton->setText(tr2i18n("Apply to All", 0));
    renameButton->setText(tr2i18n("&Rename", 0));
    cancelButton->setText(tr2i18n("&Cancel", 0));
    Q_UNUSED(OverwriteDialog);
    } // retranslateUi

};

namespace Ui {
    class OverwriteDialog: public Ui_OverwriteDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // OVERWRITEUI_H

