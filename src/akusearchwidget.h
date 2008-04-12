
#ifndef AKUSEARCHWIDGET_H
#define AKUSEARCHWIDGET_H
#include "akusearchline.h"
#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QRadioButton>
#include <QGroupBox>
#include <QTreeWidget>
#include <KLocale>
#include <QGridLayout>
#include <QToolButton>
#include <KIcon>
#include <KAction>
#include <KMenu>
#include <QActionGroup>

class akuSearchWidget : public QWidget
{

Q_OBJECT

public:
  akuSearchWidget(QTreeWidget*, QWidget *parent = 0);
  ~akuSearchWidget();
  akuSearchLine *searchLineEdit();
  KAction *getCloseAction();

private:
  akuSearchLine *searchLine;
  QToolButton *closeTool;
  KAction *closeAction;
  KMenu *optionMenu;

protected slots:
  virtual void hideMe();
  virtual void handleCloseAction(bool);
};
#endif
