#ifndef AKUCOMPRESSIONWIDGET_H
#define AKUCOMPRESSIONWIDGET_H

#include <QWidget>
//#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <KComboBox>
#include <KLineEdit>
#include <QStringList>
//#include <QSlider>
#include <QSpacerItem>
#include <KGuiItem>
#include <KPushButton>
#include <KLocale>
#include <QTabWidget>
#include <KTextEdit>
#include <QCheckBox>
#include <KPushButton>

class akuCompressionWidget : public QWidget
{
Q_OBJECT

public:
  akuCompressionWidget(QWidget *parent = 0);
  ~akuCompressionWidget();
  QString getPassword();
  int getCompressionLevel();
  double getSplitSize();
  bool isSplitRequested();
private:
  KPushButton *createButton;
  KPushButton *cancelButton;
  KTextEdit *textEdit;
  KLineEdit *passwordLine;
  KComboBox *combo;
  KComboBox *comboSplit;
  KTextEdit *infoSplit;
  QCheckBox *checkNamescheme;

protected slots:
  virtual void enablePass (bool);
  virtual void enableSplit (bool);
  virtual void enableNamescheme(bool);
  virtual void create();
  virtual void updateTextEdit (int);
  virtual void handleSplitEditing(int);
signals:
  void creationCalled();
  void canceled();
};

#endif
