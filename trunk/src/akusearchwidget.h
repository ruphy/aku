#ifndef AKUSEARCHWIDGET_H
#define AKUSEARCHWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QRadioButton>
#include <QGroupBox>
#include <QTreeWidget>
#include <QGridLayout>
#include <QToolButton>
#include <QActionGroup>

#include <KLocale>
#include <KIcon>
#include <KAction>
#include <KMenu>

#include "akusearchline.h"

class akuSearchWidget : public QWidget
{

Q_OBJECT

public:
  akuSearchWidget(QTreeWidget*, QWidget *parent = 0);
  ~akuSearchWidget();
 
  akuSearchLine *searchLineEdit();

  KAction *getCloseAction();

  virtual QSize sizeHint() const;

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
