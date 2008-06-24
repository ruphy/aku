#ifndef AKUVIEWER_H
#define AKUVIEWER_H

#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QSlider>

#include <KLineEdit>
#include <KTextEdit>
#include <KFontComboBox>
#include <KDialog>
#include <KLocale>
#include <KMessageBox>
#include <KMimeType>
#include <KIcon>
#include <KVBox>
#include <KHBox>
#include <KIconLoader>

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
  QWidget *blayout;

  QPixmap imageBuffer;
  
  QTimer *timer;

  QComboBox *sizeCombo;

  QString testo;
  QString daCercare;

  QLabel *am;
  QLabel *fileName;
  QLabel *fileIcon;
  QLabel *fileMime;
  QLabel *imageViewer;
  
  QVBoxLayout *layout;

  KFontComboBox *fontCombo;
  
  KTextEdit *viewer;
  
  KLineEdit *searchLine;
 
  void initSizeCombo();

protected slots:
  virtual void resizePreview(int);

};
#endif
