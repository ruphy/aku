
#ifndef AKUCOMMENT_H
#define AKUCOMMENT_H

#include <KTextEdit>
#include <QDialog>
#include <KDialogButtonBox>
#include <KGuiItem>
#include <KPushButton>
#include <KLocale>
#include <QGridLayout>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QTextStream>
#include <KFileDialog>

class akuComment : public QDialog
{
  Q_OBJECT;

public: 
  akuComment(QWidget *parent = 0);
  ~akuComment();

public slots:
  virtual void writeComment();
  virtual void loadFromFile();

private:
  KTextEdit *textEdit;

signals:
  void okClicked(QString);

};

#endif
