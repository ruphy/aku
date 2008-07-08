#ifndef QUICKEXTRACT_H
#define QUICKEXTRACT_H

#include <QHBoxLayout>
#include <QLabel>
#include <QDir>
#include <QHeaderView>
#include <QCheckBox>
#include <QSizePolicy>
#include <QSplitter>

#include <KDialog>
#include <KVBox>
#include <KLocale>
#include <KAction>
#include <KFileTreeView>
#include <KUrlCompletion>
#include <KHistoryComboBox>
#include <KDialogButtonBox>
#include <KGuiItem>
#include <KPushButton>

#include <kfileplacesview.h>
#include <kfileplacesmodel.h>

#include "rarprocess.h"

#include <KDebug>

class quickExtract : public KDialog
{
 Q_OBJECT

  public:
   quickExtract (QString, QWidget *parent = 0);
   ~quickExtract();

   KFileTreeView *treeView;
   KHistoryComboBox *khistory;
   
   QString archivename;

   QWidget *parentWidget;
  
  public slots:
   virtual void hiddenFiles(bool);
   virtual void updateCombo(KUrl);
   virtual void updateTreeViewSelection(QString);
   virtual void extract();
  
  protected slots:
   virtual void slotButtonClicked(int button);

  private slots:
   void urlSelected(const KUrl&);

  private:
   KAction *showhiddenAction;
};

#endif
