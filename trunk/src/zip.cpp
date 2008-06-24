#include "zip.h"

zip::zip()
{
}

zip::~zip()
{
}

int zip::parse(QTreeWidget * listv, QString bf, akuRatioWidget *ratioBar )
{
  bf.remove(0, (bf.indexOf("\n") + 1)); // elimino la prima riga
  bf.remove(bf.lastIndexOf("\n"), bf.size()); // elimino l'ultima riga
  // memorizzo l'ultima riga per conservare delle informazioni utili
  QString lastline = bf.mid((bf.lastIndexOf("\n") + 1), bf.size());
  bf.remove(bf.lastIndexOf("\n"), bf.size()); 
  //puts(bf.toAscii());
  //puts("----");
  //puts(lastline.toAscii());
  QStringList flist = bf.split ( "\n" ); //splitto basandomi sul carattere di newline
  int numeroPezziPercorso;
  QRegExp sep("(\\s+)");
  //inizio il ciclo per la visualizzazione dell'elenco
  for ( int i = 0; i < flist.size(); i++ ) {
    // estraggo prima il percorso del file a partire dal carattere 62
    //QString filepath = flist[i].mid(62, flist[i].size());
    QString filepath = flist[i].section(sep, 9 , -1);
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
        if ( foldFound == false ) {  //altrimenti alloco in lista in topLevel.. Questo accade anche se non è stato eseguito nessun ciclo
          fitem = new QTreeWidgetItem ( listv );
          fitem -> setText ( 0, singleItem[j] );
          tmpItem = fitem;
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
          fitem = new QTreeWidgetItem ( tmpItem );
          fitem -> setText ( 0, singleItem[j] );
          tmpItem = fitem;
        }
      }

    }

    //flist[i].remove(62, flist[i].size());
    QStringList dlist = (flist.at(i)).split(" ", QString::SkipEmptyParts ); // generiamo una lista contenente i parametri dei file
    //puts(dlist[4].toAscii() + dlist[6].toAscii());
    //if (!((dlist[6] == "stor") && (dlist[4] == "bx"))) {  //è inutile scrivere gli attributi della cartella
    // provo a vedere se basta leggere la "d" dai permessi per individuare una cartella invece di un controllo
    // fatto nella riga precedente.
    if (dlist[0][0] != QChar('d')) { 
      fitem -> setText(4, dlist[7] + " " + dlist[8]);  // modified

      QString size = KLocale( QString() ).formatByteSize(dlist[3].toDouble());  // 3 -> dimensione originale del file
      fitem -> setText(1, size);
      size = KLocale( QString() ).formatByteSize(dlist[5].toDouble()); // 5 -> dimensione packed del file
      fitem -> setText(2, size);

      fitem -> setText(5, dlist[0]);   // attributi 
      fitem -> setText(7, dlist[4] + " " + dlist[6]); // metodo
      fitem -> setText(8, dlist[1] + " " + dlist[2]); // version and so
      // calcolo il ratio
      float ratio = 0;
      if (dlist[3].toInt() != 0) {
        ratio = (100.0 * dlist[5].toFloat() / dlist[3].toFloat());
        ratio = abs(ratio - 100);   
      }
      fitem -> setText(3, QString().setNum(ratio) + "%");
        
//       //  akuRatioWidget *ratioWidget = new akuRatioWidget(ratio);
//       //  listv -> setItemWidget(fitem, 3, ratioWidget); 
// 
      fitem -> setTextAlignment ( 1, Qt::AlignRight | Qt::AlignVCenter );
      fitem -> setTextAlignment ( 2, Qt::AlignRight | Qt::AlignVCenter );
      fitem -> setTextAlignment ( 3, Qt::AlignHCenter | Qt::AlignVCenter );
      fitem -> setTextAlignment ( 5, Qt::AlignHCenter | Qt::AlignVCenter );
      fitem -> setTextAlignment ( 7, Qt::AlignRight | Qt::AlignVCenter );
      fitem -> setTextAlignment ( 8, Qt::AlignCenter | Qt::AlignVCenter );     

      KMimeType::Ptr mimePtr = KMimeType::findByUrl(KUrl(singleItem[numeroPezziPercorso]));
      KIcon icon(mimePtr -> iconName());
      fitem -> setIcon (0, icon);
      fitem -> setText(9, mimePtr->name());
    }
  }
}
