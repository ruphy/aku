#include <kdialog.h>
#include <klocale.h>

/********************************************************************************
** Form generated from reading ui file 'extracto.ui'
**
** Created: Wed Jun 18 23:00:09 2008
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_EXTRACTO_H
#define UI_EXTRACTO_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <kdialog.h>
#include <kfiletreeview.h>
#include "kcombobox.h"
#include "kdialogbuttonbox.h"
#include "khistorycombobox.h"
#include "kpushbutton.h"
#include "ktabwidget.h"

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QAction *actionShow_hidden_files;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    KDialogButtonBox *kdialogbuttonbox;
    KTabWidget *tabWidget;
    QWidget *tabGeneral;
    QGridLayout *gridLayout_3;
    QLabel *labelDestionation;
    KHistoryComboBox *khistorycombobox;
    QHBoxLayout *hboxLayout;
    KFileTreeView *treeView;
    QSpacerItem *spacerItem;
    QVBoxLayout *vboxLayout;
    QLabel *label;
    KComboBox *comboUpdateBox;
    QSpacerItem *spacerItem1;
    QLabel *label_2;
    KComboBox *comboOverwriteBox;
    QSpacerItem *spacer;
    QLabel *label_3;
    QCheckBox *checkKeepBroken;
    QCheckBox *checkOpenDestination;
    QSpacerItem *spacerItem2;
    QGridLayout *gridLayout_2;
    KPushButton *newdirButton;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *hboxLayout1;
    KPushButton *buttonSave;
    KPushButton *buttonDefault;
    QSpacerItem *verticalSpacer;
    QWidget *tabAdvanced;
    QGroupBox *groupFilePath;
    QWidget *verticalLayout_2;
    QVBoxLayout *vboxLayout1;
    QRadioButton *radioFullpath;
    QRadioButton *radioNofullpath;
    QRadioButton *radioAbsolutepath;
    QGroupBox *groupDeleteArchive;
    QWidget *verticalLayout_3;
    QVBoxLayout *vboxLayout2;
    QRadioButton *radioDeleteNever;
    QRadioButton *radioDeleteAsk;
    QRadioButton *radioDeleteAlways;
    QGroupBox *groupFilePath_3;
    QWidget *verticalLayout_7;
    QVBoxLayout *vboxLayout3;
    QCheckBox *checkTimeModification;
    QCheckBox *checkTimeCreation;
    QCheckBox *checkTimeLast;
    QCheckBox *checkLowerCase;
    QCheckBox *checkAuthenticity;

    void setupUi(KDialog *Dialog)
    {
    if (Dialog->objectName().isEmpty())
        Dialog->setObjectName(QString::fromUtf8("Dialog"));
    Dialog->setWindowModality(Qt::WindowModal);
    Dialog->resize(590, 459);
    QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(Dialog->sizePolicy().hasHeightForWidth());
    Dialog->setSizePolicy(sizePolicy);
    Dialog->setMinimumSize(QSize(590, 0));
    Dialog->setMaximumSize(QSize(600, 468));
    Dialog->setModal(true);
    actionShow_hidden_files = new QAction(Dialog);
    actionShow_hidden_files->setObjectName(QString::fromUtf8("actionShow_hidden_files"));
    actionShow_hidden_files->setCheckable(true);
    layoutWidget = new QWidget(Dialog);
    layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
    layoutWidget->setGeometry(QRect(10, 10, 571, 447));
    gridLayout_4 = new QGridLayout(layoutWidget);
    gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
    gridLayout_4->setContentsMargins(0, 0, 0, 0);
    gridLayout = new QGridLayout();
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    horizontalSpacer = new QSpacerItem(338, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);

    kdialogbuttonbox = new KDialogButtonBox(layoutWidget);
    kdialogbuttonbox->setObjectName(QString::fromUtf8("kdialogbuttonbox"));
    kdialogbuttonbox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    gridLayout->addWidget(kdialogbuttonbox, 0, 1, 1, 1);


    gridLayout_4->addLayout(gridLayout, 1, 0, 1, 1);

    tabWidget = new KTabWidget(layoutWidget);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
    tabWidget->setSizePolicy(sizePolicy1);
    tabWidget->setElideMode(Qt::ElideNone);
    tabGeneral = new QWidget();
    tabGeneral->setObjectName(QString::fromUtf8("tabGeneral"));
    tabGeneral->setGeometry(QRect(0, 0, 557, 369));
    gridLayout_3 = new QGridLayout(tabGeneral);
    gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
    labelDestionation = new QLabel(tabGeneral);
    labelDestionation->setObjectName(QString::fromUtf8("labelDestionation"));
    QFont font;
    font.setPointSize(9);
    labelDestionation->setFont(font);
    labelDestionation->setScaledContents(false);
    labelDestionation->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

    gridLayout_3->addWidget(labelDestionation, 0, 0, 1, 1);

    khistorycombobox = new KHistoryComboBox(tabGeneral);
    khistorycombobox->setObjectName(QString::fromUtf8("khistorycombobox"));
    khistorycombobox->setAutoCompletion(false);

    gridLayout_3->addWidget(khistorycombobox, 1, 0, 1, 1);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    treeView = new KFileTreeView(tabGeneral);
    treeView->setObjectName(QString::fromUtf8("treeView"));
    QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Expanding);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(treeView->sizePolicy().hasHeightForWidth());
    treeView->setSizePolicy(sizePolicy2);
    treeView->setMinimumSize(QSize(280, 0));
    treeView->setFocusPolicy(Qt::WheelFocus);
    treeView->setContextMenuPolicy(Qt::ActionsContextMenu);

    hboxLayout->addWidget(treeView);

    spacerItem = new QSpacerItem(16, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    vboxLayout = new QVBoxLayout();
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    vboxLayout->setSizeConstraint(QLayout::SetMinimumSize);
    label = new QLabel(tabGeneral);
    label->setObjectName(QString::fromUtf8("label"));
    label->setWindowModality(Qt::NonModal);
    QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Preferred);
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
    label->setSizePolicy(sizePolicy3);
    QFont font1;
    font1.setBold(true);
    font1.setWeight(75);
    label->setFont(font1);
    label->setFrameShape(QFrame::NoFrame);
    label->setFrameShadow(QFrame::Plain);
    label->setLineWidth(1);
    label->setScaledContents(false);
    label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
    label->setMargin(0);
    label->setIndent(-1);

    vboxLayout->addWidget(label);

    comboUpdateBox = new KComboBox(tabGeneral);
    comboUpdateBox->setObjectName(QString::fromUtf8("comboUpdateBox"));
    QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Fixed);
    sizePolicy4.setHorizontalStretch(0);
    sizePolicy4.setVerticalStretch(0);
    sizePolicy4.setHeightForWidth(comboUpdateBox->sizePolicy().hasHeightForWidth());
    comboUpdateBox->setSizePolicy(sizePolicy4);
    comboUpdateBox->setFont(font);

    vboxLayout->addWidget(comboUpdateBox);

    spacerItem1 = new QSpacerItem(16, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

    vboxLayout->addItem(spacerItem1);

    label_2 = new QLabel(tabGeneral);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    sizePolicy3.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
    label_2->setSizePolicy(sizePolicy3);
    label_2->setFont(font1);
    label_2->setFrameShape(QFrame::NoFrame);

    vboxLayout->addWidget(label_2);

    comboOverwriteBox = new KComboBox(tabGeneral);
    comboOverwriteBox->setObjectName(QString::fromUtf8("comboOverwriteBox"));
    sizePolicy4.setHeightForWidth(comboOverwriteBox->sizePolicy().hasHeightForWidth());
    comboOverwriteBox->setSizePolicy(sizePolicy4);
    comboOverwriteBox->setFont(font);

    vboxLayout->addWidget(comboOverwriteBox);

    spacer = new QSpacerItem(16, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

    vboxLayout->addItem(spacer);

    label_3 = new QLabel(tabGeneral);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    sizePolicy3.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
    label_3->setSizePolicy(sizePolicy3);
    label_3->setFont(font1);
    label_3->setFrameShape(QFrame::NoFrame);
    label_3->setFrameShadow(QFrame::Plain);

    vboxLayout->addWidget(label_3);

    checkKeepBroken = new QCheckBox(tabGeneral);
    checkKeepBroken->setObjectName(QString::fromUtf8("checkKeepBroken"));
    checkKeepBroken->setFont(font);

    vboxLayout->addWidget(checkKeepBroken);

    checkOpenDestination = new QCheckBox(tabGeneral);
    checkOpenDestination->setObjectName(QString::fromUtf8("checkOpenDestination"));
    checkOpenDestination->setFont(font);

    vboxLayout->addWidget(checkOpenDestination);

    spacerItem2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout->addItem(spacerItem2);


    hboxLayout->addLayout(vboxLayout);


    gridLayout_3->addLayout(hboxLayout, 2, 0, 1, 1);

    gridLayout_2 = new QGridLayout();
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    newdirButton = new KPushButton(tabGeneral);
    newdirButton->setObjectName(QString::fromUtf8("newdirButton"));

    gridLayout_2->addWidget(newdirButton, 0, 0, 1, 1);

    horizontalSpacer_2 = new QSpacerItem(378, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout_2->addItem(horizontalSpacer_2, 0, 1, 1, 1);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    hboxLayout1->setSizeConstraint(QLayout::SetMinimumSize);
    buttonSave = new KPushButton(tabGeneral);
    buttonSave->setObjectName(QString::fromUtf8("buttonSave"));

    hboxLayout1->addWidget(buttonSave);

    buttonDefault = new KPushButton(tabGeneral);
    buttonDefault->setObjectName(QString::fromUtf8("buttonDefault"));

    hboxLayout1->addWidget(buttonDefault);


    gridLayout_2->addLayout(hboxLayout1, 0, 2, 1, 1);


    gridLayout_3->addLayout(gridLayout_2, 4, 0, 1, 1);

    verticalSpacer = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Minimum);

    gridLayout_3->addItem(verticalSpacer, 3, 0, 1, 1);

    tabWidget->addTab(tabGeneral, QString());
    tabAdvanced = new QWidget();
    tabAdvanced->setObjectName(QString::fromUtf8("tabAdvanced"));
    tabAdvanced->setGeometry(QRect(0, 0, 557, 369));
    groupFilePath = new QGroupBox(tabAdvanced);
    groupFilePath->setObjectName(QString::fromUtf8("groupFilePath"));
    groupFilePath->setGeometry(QRect(20, 50, 241, 111));
    QFont font2;
    font2.setBold(true);
    font2.setItalic(false);
    font2.setUnderline(false);
    font2.setWeight(75);
    groupFilePath->setFont(font2);
    groupFilePath->setAlignment(Qt::AlignLeading);
    groupFilePath->setFlat(false);
    groupFilePath->setCheckable(false);
    groupFilePath->setChecked(false);
    verticalLayout_2 = new QWidget(groupFilePath);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    verticalLayout_2->setGeometry(QRect(10, 20, 221, 86));
    vboxLayout1 = new QVBoxLayout(verticalLayout_2);
    vboxLayout1->setSpacing(0);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    vboxLayout1->setContentsMargins(0, 10, 0, 0);
    radioFullpath = new QRadioButton(verticalLayout_2);
    radioFullpath->setObjectName(QString::fromUtf8("radioFullpath"));
    QFont font3;
    font3.setPointSize(9);
    font3.setBold(false);
    font3.setWeight(50);
    radioFullpath->setFont(font3);
    radioFullpath->setChecked(true);

    vboxLayout1->addWidget(radioFullpath);

    radioNofullpath = new QRadioButton(verticalLayout_2);
    radioNofullpath->setObjectName(QString::fromUtf8("radioNofullpath"));
    radioNofullpath->setFont(font3);
    radioNofullpath->setChecked(false);

    vboxLayout1->addWidget(radioNofullpath);

    radioAbsolutepath = new QRadioButton(verticalLayout_2);
    radioAbsolutepath->setObjectName(QString::fromUtf8("radioAbsolutepath"));
    radioAbsolutepath->setEnabled(true);
    radioAbsolutepath->setFont(font3);

    vboxLayout1->addWidget(radioAbsolutepath);

    groupDeleteArchive = new QGroupBox(tabAdvanced);
    groupDeleteArchive->setObjectName(QString::fromUtf8("groupDeleteArchive"));
    groupDeleteArchive->setGeometry(QRect(280, 50, 251, 111));
    groupDeleteArchive->setFont(font1);
    verticalLayout_3 = new QWidget(groupDeleteArchive);
    verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
    verticalLayout_3->setGeometry(QRect(10, 20, 221, 86));
    vboxLayout2 = new QVBoxLayout(verticalLayout_3);
    vboxLayout2->setSpacing(0);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    vboxLayout2->setContentsMargins(0, 10, 0, 0);
    radioDeleteNever = new QRadioButton(verticalLayout_3);
    radioDeleteNever->setObjectName(QString::fromUtf8("radioDeleteNever"));
    radioDeleteNever->setFont(font3);
    radioDeleteNever->setChecked(true);

    vboxLayout2->addWidget(radioDeleteNever);

    radioDeleteAsk = new QRadioButton(verticalLayout_3);
    radioDeleteAsk->setObjectName(QString::fromUtf8("radioDeleteAsk"));
    radioDeleteAsk->setFont(font3);
    radioDeleteAsk->setChecked(false);

    vboxLayout2->addWidget(radioDeleteAsk);

    radioDeleteAlways = new QRadioButton(verticalLayout_3);
    radioDeleteAlways->setObjectName(QString::fromUtf8("radioDeleteAlways"));
    radioDeleteAlways->setEnabled(true);
    radioDeleteAlways->setFont(font3);

    vboxLayout2->addWidget(radioDeleteAlways);

    groupFilePath_3 = new QGroupBox(tabAdvanced);
    groupFilePath_3->setObjectName(QString::fromUtf8("groupFilePath_3"));
    groupFilePath_3->setGeometry(QRect(20, 200, 241, 111));
    groupFilePath_3->setFont(font2);
    groupFilePath_3->setAlignment(Qt::AlignLeading);
    groupFilePath_3->setFlat(false);
    groupFilePath_3->setCheckable(false);
    groupFilePath_3->setChecked(false);
    verticalLayout_7 = new QWidget(groupFilePath_3);
    verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
    verticalLayout_7->setGeometry(QRect(10, 20, 221, 85));
    vboxLayout3 = new QVBoxLayout(verticalLayout_7);
    vboxLayout3->setSpacing(0);
    vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
    vboxLayout3->setContentsMargins(0, 10, 0, 0);
    checkTimeModification = new QCheckBox(verticalLayout_7);
    checkTimeModification->setObjectName(QString::fromUtf8("checkTimeModification"));
    QFont font4;
    font4.setBold(false);
    font4.setWeight(50);
    checkTimeModification->setFont(font4);

    vboxLayout3->addWidget(checkTimeModification);

    checkTimeCreation = new QCheckBox(verticalLayout_7);
    checkTimeCreation->setObjectName(QString::fromUtf8("checkTimeCreation"));
    checkTimeCreation->setFont(font4);

    vboxLayout3->addWidget(checkTimeCreation);

    checkTimeLast = new QCheckBox(verticalLayout_7);
    checkTimeLast->setObjectName(QString::fromUtf8("checkTimeLast"));
    checkTimeLast->setFont(font4);

    vboxLayout3->addWidget(checkTimeLast);

    checkLowerCase = new QCheckBox(tabAdvanced);
    checkLowerCase->setObjectName(QString::fromUtf8("checkLowerCase"));
    checkLowerCase->setGeometry(QRect(280, 230, 261, 23));
    checkAuthenticity = new QCheckBox(tabAdvanced);
    checkAuthenticity->setObjectName(QString::fromUtf8("checkAuthenticity"));
    checkAuthenticity->setGeometry(QRect(280, 260, 251, 23));
    checkAuthenticity->setChecked(true);
    tabWidget->addTab(tabAdvanced, QString());

    gridLayout_4->addWidget(tabWidget, 0, 0, 1, 1);

    QWidget::setTabOrder(treeView, tabWidget);
    QWidget::setTabOrder(tabWidget, kdialogbuttonbox);
    QWidget::setTabOrder(kdialogbuttonbox, khistorycombobox);
    QWidget::setTabOrder(khistorycombobox, comboUpdateBox);
    QWidget::setTabOrder(comboUpdateBox, comboOverwriteBox);
    QWidget::setTabOrder(comboOverwriteBox, checkKeepBroken);
    QWidget::setTabOrder(checkKeepBroken, buttonSave);
    QWidget::setTabOrder(buttonSave, buttonDefault);
    QWidget::setTabOrder(buttonDefault, radioFullpath);
    QWidget::setTabOrder(radioFullpath, radioNofullpath);
    QWidget::setTabOrder(radioNofullpath, radioAbsolutepath);
    QWidget::setTabOrder(radioAbsolutepath, radioDeleteNever);
    QWidget::setTabOrder(radioDeleteNever, radioDeleteAsk);
    QWidget::setTabOrder(radioDeleteAsk, radioDeleteAlways);
    QWidget::setTabOrder(radioDeleteAlways, checkTimeModification);
    QWidget::setTabOrder(checkTimeModification, checkTimeCreation);
    QWidget::setTabOrder(checkTimeCreation, checkTimeLast);
    QWidget::setTabOrder(checkTimeLast, checkLowerCase);
    QWidget::setTabOrder(checkLowerCase, checkAuthenticity);

    retranslateUi(Dialog);

    tabWidget->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(KDialog *Dialog)
    {
    Dialog->setWindowTitle(tr2i18n("Extraction path and options", 0));
    actionShow_hidden_files->setText(tr2i18n("Show hidden files", 0));
    labelDestionation->setText(tr2i18n("Destination Path (will be created if does not exist)", 0));
    label->setText(tr2i18n("Update Mode", 0));
    comboUpdateBox->insertItems(0, QStringList()
     << tr2i18n("Extract and replace files", 0)
     << tr2i18n("Extract and update files", 0)
     << tr2i18n("Fresh existing files only", 0)
    );
    label_2->setText(tr2i18n("Overwrite Mode", 0));
    comboOverwriteBox->insertItems(0, QStringList()
     << tr2i18n("Ask before overwrite", 0)
     << tr2i18n("Overwrite without prompt", 0)
     << tr2i18n("Skip existing files", 0)
     << tr2i18n("Rename automatically", 0)
    );
    label_3->setText(tr2i18n("Miscellaneous", 0));
    checkKeepBroken->setText(tr2i18n("Keep broken files", 0));
    checkOpenDestination->setText(tr2i18n("Open destination path ", 0));
    newdirButton->setText(tr2i18n("&New Directory", 0));
    buttonSave->setText(tr2i18n("&Save Settings", 0));
    buttonDefault->setText(tr2i18n("&Default", 0));
    tabWidget->setTabText(tabWidget->indexOf(tabGeneral), tr2i18n("General", 0));
    groupFilePath->setTitle(tr2i18n("File Paths", 0));
    radioFullpath->setText(tr2i18n("Extract full paths", 0));
    radioNofullpath->setText(tr2i18n("Do not extract full paths", 0));
    radioAbsolutepath->setText(tr2i18n("Extract absolute paths", 0));
    groupDeleteArchive->setTitle(tr2i18n("Delete Archive", 0));
    radioDeleteNever->setText(tr2i18n("Never", 0));
    radioDeleteAsk->setText(tr2i18n("Ask for confirmation", 0));
    radioDeleteAlways->setText(tr2i18n("Always", 0));
    groupFilePath_3->setTitle(tr2i18n("File Time", 0));
    checkTimeModification->setText(tr2i18n("Set modification time", 0));
    checkTimeCreation->setText(tr2i18n("Set creation time", 0));
    checkTimeLast->setText(tr2i18n("Set last access time", 0));
    checkLowerCase->setText(tr2i18n("Convert names to lower case", 0));
    checkAuthenticity->setText(tr2i18n("Check authenticity information", 0));
    tabWidget->setTabText(tabWidget->indexOf(tabAdvanced), tr2i18n("Advanced", 0));
    Q_UNUSED(Dialog);
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // EXTRACTO_H

