#include "akucompressionwidget.h"

akuCompressionWidget::akuCompressionWidget(QWidget *parent) : QWidget(parent)
{
   QTabWidget *tabWidget = new QTabWidget(this);
   tabWidget -> setTabPosition(QTabWidget::West);
   tabWidget -> setGeometry(10, 10, 280, 270);
   QWidget *generalWidget = new QWidget(this);
   generalWidget -> setGeometry(10, 30, 259, 231);
   //generalWidget -> setWindowTitle(i18n("General"));
   QVBoxLayout *verticalLayout = new QVBoxLayout(generalWidget);
   KHBox *hbox = new KHBox(generalWidget);
   hbox -> setSpacing(15);   
   QLabel *label = new QLabel(i18n("Compression level"), hbox);
   QLabel *licon = new QLabel(hbox);
   QPixmap mimeIcon = KIcon("view-statistics").pixmap(22,16);
   licon -> setPixmap(mimeIcon);
 
   verticalLayout -> addWidget(hbox);
   combo = new KComboBox(false, generalWidget);
   combo -> addItem(i18n("Store"));
   combo -> addItem(i18n("Fastest"));
   combo -> addItem(i18n("Fast"));
   combo -> addItem(i18n("Normal"));
   combo -> addItem(i18n("Good"));
   combo -> addItem(i18n("Best"));
   combo -> setCurrentIndex(3);
   verticalLayout -> addWidget(combo);
   textEdit = new KTextEdit(generalWidget);
   textEdit -> setReadOnly(true);
   textEdit -> setFocusPolicy(Qt::NoFocus);
   textEdit -> setContextMenuPolicy(Qt::NoContextMenu);
   verticalLayout -> addWidget(textEdit);
   QSpacerItem *spacer = new QSpacerItem(20,10);
   verticalLayout -> addSpacerItem(spacer);
   QCheckBox *checkbox = new QCheckBox(i18n("Set an archive password"), generalWidget);
   checkbox -> setIcon(KIcon("dialog-password"));
   verticalLayout -> addWidget(checkbox);
   passwordLine = new KLineEdit(generalWidget);
   verticalLayout -> addWidget(passwordLine);
   passwordLine -> setEnabled(false);
   QWidget *advancedWidget = new QWidget(this);
   advancedWidget -> setGeometry(10,30,259,231);
   QVBoxLayout *vLayout = new QVBoxLayout(advancedWidget);
   QCheckBox *checkSplit = new QCheckBox(i18n("Split to volumes, bytes"), advancedWidget);
   checkSplit -> setEnabled(false);
   vLayout -> addWidget(checkSplit);
   comboSplit = new KComboBox(true, advancedWidget);
   QDoubleValidator *validator = new QDoubleValidator(this);
   comboSplit -> setValidator(validator);
   comboSplit -> addItem(KIcon("user-properties"), "");
   comboSplit -> addItem(KIcon("media-floppy"), "Floppy: 1457664");
   comboSplit -> setItemData(1, QVariant(1457664));
   comboSplit -> addItem(KIcon("media-optical"), "CD700: 700 mb");
   comboSplit -> setItemData(2, QVariant(734003200));
   comboSplit -> addItem(KIcon("drive-optical"), "DVD+R: 4481 mb");
   comboSplit -> setItemData(3,QVariant(4698669056.0));
   comboSplit -> setCurrentIndex(0);
   comboSplit -> setEnabled(false);
   QSpacerItem *spacerAdvanced = new QSpacerItem(20,30);
   infoSplit = new KTextEdit(advancedWidget);
   infoSplit -> setFocusPolicy(Qt::NoFocus);
   infoSplit -> setContextMenuPolicy(Qt::NoContextMenu);
   infoSplit -> setReadOnly(true);
   checkNamescheme = new QCheckBox(i18n("Change naming scheme"), advancedWidget);
   checkNamescheme -> setEnabled(false);
   vLayout -> addWidget(comboSplit);
   vLayout -> addWidget(infoSplit);
   vLayout -> addWidget(checkNamescheme);
   vLayout -> addSpacerItem(spacerAdvanced);
   tabWidget -> addTab(generalWidget, i18n("General")+ "  ");   
   tabWidget -> addTab(advancedWidget, i18n("Advanced") + "  ");
   //tabWidget -> removeTab(1); //TODO to enable for second release
   //tabWidget -> appendTab(QPixmap(), 0, "Prova");   


   QWidget *buttonWidget = new QWidget(this);
   buttonWidget -> setGeometry(10,280,280,50); 
   QHBoxLayout *buttonLayout = new QHBoxLayout(buttonWidget);
   KGuiItem guiItem(i18n("Create"), "dialog-ok-apply");
   createButton = new KPushButton(guiItem);
   KGuiItem guiItem2(i18n("Cancel"), "dialog-close");
   cancelButton = new KPushButton(guiItem2);
   buttonLayout -> addWidget(createButton);
   buttonLayout -> addWidget(cancelButton);
   connect(checkNamescheme, SIGNAL(clicked(bool)), this, SLOT(enableNamescheme(bool)));
   connect(combo, SIGNAL(currentIndexChanged (int)), this, SLOT(updateTextEdit (int)));
   connect(createButton, SIGNAL(clicked()), this, SLOT(create()));
   connect(checkbox, SIGNAL(toggled(bool)), this, SLOT(enablePass(bool)));
   connect(checkSplit, SIGNAL(toggled(bool)), this, SLOT(enableSplit(bool)));
   connect(cancelButton, SIGNAL(clicked()), this, SIGNAL(canceled()));
   connect(comboSplit, SIGNAL(currentIndexChanged(int)), this, SLOT(handleSplitEditing(int)));
   updateTextEdit(3);
}

akuCompressionWidget::~akuCompressionWidget()
{
}

double akuCompressionWidget::getSplitSize()
{
  if(comboSplit -> currentIndex() > 0 ) 
    return comboSplit -> itemData(comboSplit -> currentIndex()).toDouble();
  else {
    QString currentString(comboSplit -> itemText(comboSplit -> currentIndex()));
    return currentString.toDouble();
  } 
}

bool akuCompressionWidget::isSplitRequested()
{
 return comboSplit -> isEnabled();
}

void akuCompressionWidget::handleSplitEditing(int index)
{
 if (index > 0 ) 
   comboSplit -> setEditable(false);
 else {
   comboSplit -> setEditable(true);
   QDoubleValidator *validator = new QDoubleValidator(this);
   comboSplit -> setValidator(validator);
 }
}
void akuCompressionWidget::enableNamescheme(bool state)
{
  if (state)
    infoSplit -> setText(i18n("Switch to another, extension based naming scheme, where the first volume file in a multi-volume set has the extension .rar, following volumes are numbered from .r00 to .r99."));
  else
    infoSplit -> setText(i18n("By default RAR volumes have names like 'volname.partNNN.rar', where NNN is the volume number"));
}

void akuCompressionWidget::enablePass(bool state)
{
  if (state) {
    passwordLine -> setEnabled(true);
    passwordLine -> clear();
  }
  else {
    passwordLine -> clear();
    passwordLine -> setEnabled(false);
  }
}

void akuCompressionWidget::enableSplit(bool state)
{
  if (state) {
    comboSplit -> setEnabled(true);
    infoSplit -> setText(i18n("By default RAR volumes have names like 'volname.partNNN.rar', where NNN is the volume number"));
    checkNamescheme -> setEnabled(true);
  }
  else {
    comboSplit -> setEnabled(false);
    infoSplit -> clear();
    checkNamescheme -> setEnabled(false);
    checkNamescheme -> setChecked(false);
  }
}

void akuCompressionWidget::updateTextEdit (int index)
{
  switch (index) {
    case 0:
      textEdit -> setText(i18n("Merges files into an archive without compression"));
      break;
    case 1:
      textEdit -> setText(i18n("Use fastest method (less compressive)"));
      break;
    case 2:
      textEdit -> setText(i18n("Use fast compression method"));
      break;
    case 3:
      textEdit -> setText(i18n("Use normal (default) compression method. It is usually the best choice"));
      break;
    case 4:
      textEdit -> setText(i18n("Use good compression method (more compressive, but slower)"));
      break;
    case 5:
      textEdit -> setText(i18n("Use best compression method (slightly more compressive, but slowest)"));
      break;
  }
}

void akuCompressionWidget::create()
{
  emit creationCalled();
}

QString akuCompressionWidget::getPassword()
{
  return passwordLine -> text();
}

int akuCompressionWidget::getCompressionLevel()
{
  return combo -> currentIndex();
}
