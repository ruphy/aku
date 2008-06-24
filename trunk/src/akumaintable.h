#ifndef AKUMAINTABLE_H
#define AKUMAINTABLE_H

#include <QTreeWidget>
#include <QHeaderView>
#include <QPoint>

#include <KMenu>
#include <KAction>
#include <KLocale>


class akuMainTable : public QTreeWidget
{

Q_OBJECT

public:
  akuMainTable(QWidget *parent);
  ~akuMainTable();

public slots:
  void setFormat(QString);
 
protected slots:
  virtual void setupActions();  
  virtual void headerCustomContextMenu(QPoint);
  virtual void showratio(bool);
  virtual void showattributes(bool);
  virtual void showmethod(bool);
  virtual void showversion(bool);
  virtual void showsize(bool);
  virtual void showpackedsize(bool);
  virtual void showmodified(bool);
  virtual void showcrc(bool);
  virtual void showmime(bool);
  virtual void showog(bool);


private:
   // header actions
  KAction *headerRatio;
  KAction *headerAttributes;
  KAction *headerMethod;
  KAction *headerSize;
  KAction *headerPackedSize;
  KAction *headerModified;
  KAction *headerCRC;
  KAction *headerVersion;
  KAction *headerMIME;
  KAction *headerOG;

  //KAction *crcAction();

  KMenu *mPopup;
  

};
#endif
