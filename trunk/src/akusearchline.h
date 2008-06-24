#ifndef AKUSEARCHLINE_H
#define AKUSEARCHLINE_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QPalette>

#include <KLineEdit>

class akuSearchLine : public KLineEdit
{

Q_OBJECT

public:
  akuSearchLine (QTreeWidget *treeWidget, QWidget *parent = 0);
  ~akuSearchLine();
  QList<QTreeWidgetItem *>  recursiveFilter( QTreeWidgetItem *, QString);

public slots:
  virtual void filter ( QString );
  virtual void expandFoundItems();
  virtual void setFilterBySelection ( bool );
  virtual void setFilterByExclusion ( bool );
  virtual void setFilterCaseSensitivity ( bool );

private:
  bool filterBySelection;  //false = Exclusion Filter; true = Selection Filter
  bool caseSensitive;
  bool redSearchLine;
  bool greenSearchLine;
 
  void restoreList();

  QTreeWidget *tree;  

  QList<QTreeWidgetItem*> filteredItems;
  
  //QAction *selectionFilter;
  //QAction *exclusionFilter;
  //QAction *caseFilter;

};
#endif
