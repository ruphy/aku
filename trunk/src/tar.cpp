#include "tar.h"

tar::tar()
{
}

tar::~tar()
{
}

void tar::parse(QTreeWidget * listv, QString bf)
{ 
  QString link;
  bool islink = false;
  bf.remove(bf.lastIndexOf("\n"), bf.size()); // rimuovo l'ultima riga
  //puts(bf.toAscii());
  //puts("----");
  //puts(lastline.toAscii());
  QStringList flist = bf.split ("\n" ); //splitto basandomi sul carattere di newline
  archiveDetails << QString().setNum(flist.size());
  int numeroPezziPercorso;
  ulong archivesize = 0;
  QRegExp sep("(\\s+)");
  //inizio il ciclo per la visualizzazione dell'elenco
  for ( int i = 0; i < flist.size(); i++ ) {
    // estraggo prima il percorso del file a partire dal carattere 48
    //QString filepath = flist[i].mid(48, flist[i].size());
    QString filepath = flist[i].section(sep, 5 , -1);
    //puts (filepath.toAscii());
    // devo gestire la presenza di link simbolici all'interno dell'archivio
    // se è presente " ->" elimino il riferimento al link simbolico altrimenti la ricostruzione dell'archivio è errata
    if (flist[i][0] == QChar('l')) {
      link = filepath.mid(filepath.indexOf(" ->"));
      filepath.remove(filepath.indexOf(" ->"), filepath.size());
      link.remove(" -> ");
      // segno nella colonna informazioni la presenza del link simbolico
      //puts ("LINK SIMBOLICO");
      islink = true;

      //puts (filepath.toAscii());
    }
    QStringList singleItem = filepath.split (QDir().separator());  //splitto i nomi delle cartelle
    //puts(singleItem.join(" ").toAscii());
    numeroPezziPercorso = singleItem.size() - 1;
    QTreeWidgetItem *fitem; //nuovo elemento da allocare in lista
    QTreeWidgetItem *tmpItem; //puntatore a elemento temporaneo
    bool foldFound = false; //se ho trovato l'elemento...
    bool itemFound = false;
    for ( int j = 0; j < singleItem.size(); j++ ) {//per ogni cartella ramifico...
      itemFound = false;
      foldFound = false;
    //-------------------queste operazioni avvengono solo ed esclusivamente al primo ciclo---------------------//
      if ( j == 0 ) {  //se e' il primo ciclo limitiamo la ricerca ai top level items
        for ( int c = 0; c < listv -> topLevelItemCount(); c++ ) { //se trovo l'elemento tra i top level setto il current
          if ( ( listv -> topLevelItem ( c ) ) -> text ( 0 ) == singleItem[j] ) {
            tmpItem = listv -> topLevelItem ( c );
            foldFound = true;
            break; //se ho trovato l'elemento è inutile continuare il ciclo
          }
        }
        if ((foldFound == false) && (!singleItem[j].isEmpty())) {  //altrimenti alloco in lista in topLevel.. Questo accade anche se non è stato eseguito nessun ciclo
          fitem = new QTreeWidgetItem ( listv );
          fitem -> setText (0, singleItem[j] );
          tmpItem = fitem;
          // gestisco la presenza di un link simbolico
          if (islink) {
            fitem -> setIcon(10, KIcon("insert-link"));
            islink = false;
            fitem -> setToolTip(10, link);
          }
        }
      }
      else {  //------------------queste operazioni avvengono per tutti gli altri cicli-------------------//
      //cerco ciò che mi interessa tra i child dell currentItem() attuale
        for ( int c=0; c < tmpItem -> childCount(); c++ ) {
            if ( tmpItem->child ( c ) -> text ( 0 ) == singleItem[j] ) {
              tmpItem = tmpItem->child ( c ); //se lo trovo setto il current item
              itemFound = true;
              break; //se ho trovato l'elemento è inutile continuare il ciclo
            }
        }
        if (( itemFound == false ) && (!singleItem[j].isEmpty())) {  //altrimenti alloco e setto il currentItem
          fitem = new QTreeWidgetItem (tmpItem);
          fitem -> setText (0, singleItem[j] );
          tmpItem = fitem;
          if (islink) {
            fitem -> setIcon(10, KIcon("insert-link"));
            islink = false;
            fitem -> setToolTip(10, link);
          }
        }
      }

    }

    //flist[i].remove(48, flist[i].size());
    QStringList dlist = (flist.at(i)).split(" ", QString::SkipEmptyParts ); // generiamo una lista contenente i parametri dei file
    //puts(dlist[4].toAscii() + dlist[6].toAscii());
    //const QChar firstchar = dlist[0].at[0];   // assegno il primo carattere dei permessi alla variabile per distinguere directory e link
    
    if (dlist[0][0] != QChar('d')) {  //è inutile scrivere gli attributi della cartella
      fitem -> setText(4, dlist[3] + " " + dlist[4]);  // modified    
      //puts(dlist[2].toAscii());
      archivesize = archivesize + dlist[2].toULong();
      QString size = KLocale( QString() ).formatByteSize(dlist[2].toULong());  // 3 -> dimensione originale del file
      fitem -> setText(1, size);
      fitem -> setText(5, dlist[0]);   // attributi 
      fitem -> setText(11, dlist[1]);   // owner/group

      //  akuRatioWidget *ratioWidget = new akuRatioWidget(ratio);
      //  listv -> setItemWidget(fitem, 3, ratioWidget); 

      fitem -> setTextAlignment ( 1, Qt::AlignRight | Qt::AlignVCenter );
      //fitem -> setTextAlignment ( 2, Qt::AlignRight | Qt::AlignVCenter );
      //fitem -> setTextAlignment ( 3, Qt::AlignRight | Qt::AlignVCenter );
      fitem -> setTextAlignment ( 5, Qt::AlignLeft | Qt::AlignVCenter );
      fitem -> setTextAlignment ( 11, Qt::AlignHCenter | Qt::AlignVCenter );  
//      
      KMimeType::Ptr mimePtr = KMimeType::findByUrl(KUrl(singleItem[numeroPezziPercorso]));
      KIcon icon(mimePtr -> iconName());
      fitem -> setIcon (0, icon);
      fitem -> setText(9, mimePtr->name());
    }
    
  }
  archiveDetails << QString().setNum(archivesize);
}

QStringList tar::getArchiveDetails()
{
  return QStringList() = archiveDetails;
}
