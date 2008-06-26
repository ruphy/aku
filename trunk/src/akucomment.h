#ifndef AKUCOMMENT_H
#define AKUCOMMENT_H

#include <QDialog>
#include <QGridLayout>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QTextStream>

#include <KFileDialog>
#include <KTextEdit>
#include <KDialogButtonBox>
#include <KGuiItem>
#include <KPushButton>
#include <KLocale>

class akuComment : public QDialog
{
  Q_OBJECT;

public: 
  akuComment(QString, QWidget *parent = 0);
  ~akuComment();

public slots:
  virtual void writeComment();
  virtual void loadFromFile();
  virtual void loadPreviousText();

private:
  KTextEdit *textEdit;
  QString oldcomment;

signals:
  void okClicked(QString);

};

#endif
