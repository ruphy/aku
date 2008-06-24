#ifndef AKUSEARCHLINE_H
#define AKUSEARCHLINE_H

#include <QLineEdit>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QString>
#include <QMessageBox>
#include <QPalette>
#include <QAction>

class akuSearchLine : public QLineEdit
{

Q_OBJECT

public:
  akuSearchLine ( QTreeWidget *treeWidget, QWidget *parent = 0 );
  ~akuSearchLine();
  QList<QTreeWidgetItem *>          recursiveFilter ( QTreeWidgetItem *, QString );

public slots:
  virtual void filter ( QString );
  virtual void expandFoundItems();
  virtual void setFilterBySelection ( bool );
  virtual void setFilterByExclusion ( bool );
  virtual void setFilterCaseSensitivity ( bool );

private:
  bool filterBySelection;  //false = Exclusion Filter; true = Selection Filter
  bool caseSensitive;
  QTreeWidget *tree;
  void restoreList();
  bool redSearchLine;
  bool greenSearchLine;
  QList<QTreeWidgetItem*> filteredItems;
  QAction *selectionFilter;
  QAction *exclusionFilter;
  QAction *caseFilter;

};
#endif
