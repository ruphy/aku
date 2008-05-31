
#ifndef AKUVIEWER_H
#define AKUVIEWER_H

#include <KTextEdit>
#include <KFontComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <KDialog>
#include <KLocale>
#include <KMessageBox>
#include <KMimeType>
#include <KIcon>
#include <QVBoxLayout>
class akuViewer : public KDialog
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
  virtual void setData(const QByteArray&);
private:
  QTimer *timer;
  KTextEdit *viewer;
  QString testo;
  QString daCercare;
  KFontComboBox *fontCombo;
  QComboBox *sizeCombo;
  QLineEdit *searchLine;
  void initSizeCombo();
  QVBoxLayout *layout;
  QLabel *fileName;
  QLabel *fileIcon;
  QLabel *fileMime;
  QLabel *imageViewer;
  QWidget *blayout;
  QPixmap imageBuffer;
  QLabel *am;
protected slots:
  virtual void resizePreview(int);
};
#endif
