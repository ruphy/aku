
#ifndef AKUVIEWER_H
#define AKUVIEWER_H

#include <KTextEdit>
#include <QFontComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QGridLayout>
#include <QDialog>
#include <KLocale>
#include <KMessageBox>
#include <KMimeType>
#include <KIcon>
class akuViewer : public QDialog
{
Q_OBJECT

public:
  akuViewer ( QWidget *parent = 0, Qt::WFlags fl = 0 );
  ~akuViewer();
  
public slots:
  virtual void setText ( QString );
  virtual void setTextFont ( QFont );
  virtual void setTextSize ( QString );
  virtual void find();
  virtual void searchBackward();
  virtual void searchForward();
  virtual void startTimer ( QString );
  virtual void setFileName ( QString );
  virtual void setupTextView();
  virtual void setData(QByteArray);
private:
  QTimer *timer;
  KTextEdit *viewer;
  QString testo;
  QString daCercare;
  QFontComboBox *fontCombo;
  QComboBox *sizeCombo;
  QLineEdit *searchLine;
  void initSizeCombo();
  QGridLayout *comboLayout;
  QGridLayout *searchLayout;
  QGridLayout *layout;
  QLabel *fileName;
  QLabel *fileIcon;
  QLabel *fileMime;
};
#endif
