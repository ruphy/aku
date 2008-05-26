#include "rar.h"

QString head_line = "-------------------------------------------------------------------------------";

rar::rar()
{
}

rar::~rar()
{
}

int rar::parse ( QTreeWidget * listv, QString bf, akuRatioWidget *ratioBar )
{
  int numeroPezziPercorso; // questa variabile dichiarata in questo punto ci permette di velocizzare le operazioni di assegnamento dell'icona  
                           // contiene il numero di elementi di cui è costituita la lista del percorso e.g. /cartella1/cartella2/cartella3
                           // e verrà assegnata nel momento in cui sarà ricavata la lista del percorso (con singleItem)
 
  QString nomec;
  int target; //mi serve per sapere cosa eliminare dall'output
  target = bf.indexOf ( head_line );
  bf.remove ( 0,target + 79 ); //escludo tutto l'output fino al tratteggio
  target = bf.indexOf ( head_line );
  QString stbarst;
  stbarst = bf;
  bf.remove ( target, bf.length() ); //escludo tutto l'output oltre il secondo tratteggio
  bf.remove ( 0,1 );
  bf.remove ( bf.length()-1,1 ); //alcune correzioni di parsing...
  QStringList flist = bf.split ( "\n" ); //splitto basandomi sul carattere di newline
  QStringList foldbuffer; //un contenitore per la lista di percorsi di cartella

  //inizio il ciclo per la visualizzazione dell'elenco
  bool crypted = false;
  for ( int i = 0; i < flist.size(); i++ )
  {
    if ( i%2 == 0 )
    {
      //(flist.at(i)).remove(0,1); //c'è uno spazio all'inizio di ogni linea, può creare problemi
      nomec = flist.at ( i );
      if(nomec[0] == '*') crypted = true; 
      else crypted = false;
      nomec.remove ( 0,1 );
      //te -> append("Percorso: "+flist[i]);
    }
    else
    {
      QStringList singleItem = nomec.split ( QDir().separator() );  //splitto i nomi delle cartelle
      numeroPezziPercorso = singleItem.size()-1;
      QTreeWidgetItem *fitem; //nuovo elemento da allocare in lista
      QTreeWidgetItem *tmpItem; //puntatore a elemento temporaneo
      bool foldFound = false; //se ho trovato l'elemento...
      bool itemFound = false;
      for ( int j = 0; j < singleItem.size(); j++ ) //per ogni cartella ramifico...
      {
        itemFound = false;
        foldFound = false;
        //-------------------queste operazioni avvengono solo ed esclusivamente al primo ciclo---------------------//
        if ( j == 0 )   //se e' il primo ciclo limitiamo la ricerca ai top level items
        {
          for ( int c = 0; c < listv -> topLevelItemCount(); c++ )  //se trovo l'elemento tra i top level setto il current
          {
            if ( ( listv -> topLevelItem ( c ) ) -> text ( 0 ) == singleItem[j] )
            {
              tmpItem = listv -> topLevelItem ( c );
              foldFound = true;
              break; //se ho trovato l'elemento è inutile continuare il ciclo
            }
          }
          if ( foldFound == false )   //altrimenti alloco in lista in topLevel.. Questo accade anche se non è stato eseguito nessun ciclo
          {
            fitem = new QTreeWidgetItem ( listv );
            fitem -> setText ( 0, singleItem[j] );
            tmpItem = fitem;
          }
        }
        //---------------------------------------------------------------------------------------------------------//
        else
        //--------------------------------queste operazioni avvengono per tutti gli altri cicli-------------------//
        {
        //cerco ciò che mi interessa tra i child dell currentItem() attuale
          for ( int c=0; c < tmpItem -> childCount(); c++ )
          {
            if ( tmpItem->child ( c ) -> text ( 0 ) == singleItem[j] )
            {
              tmpItem = tmpItem->child ( c ); //se lo trovo setto il current item
              itemFound = true;
              break; //se ho trovato l'elemento è inutile continuare il ciclo
            }
          }
          if ( itemFound == false )   //altrimenti alloco e setto il currentItem
          {
            fitem = new QTreeWidgetItem ( tmpItem );
            fitem -> setText ( 0, singleItem[j] );
            tmpItem = fitem;
          }
        }
     //---------------------------------------------------------------------------------------------------------//
      }

      QStringList dlist = ( flist.at ( i ) ).split ( " ", QString::SkipEmptyParts );// generiamo una lista contenente i parametri dei file
      if ( dlist[7] != "m0" )   //è inutile scrivere gli attributi della cartella
      {
            QString size = KLocale( QString() ).formatByteSize(dlist[0].toDouble());
            fitem -> setTextAlignment ( 1, Qt::AlignRight | Qt::AlignVCenter );
            fitem -> setTextAlignment ( 2, Qt::AlignRight | Qt::AlignVCenter );
            fitem -> setText(1, size);
            size = KLocale( QString() ).formatByteSize(dlist[1] .toDouble());
            fitem -> setText(2, size);

          //  dlist[2].remove("%");                                        // akuRatioWidget (or any other widget) slows down
          //  float ratio = dlist[2].toFloat();                            // the archive visualization, so it is deactivated 
          //  if (ratio > 100 || ratio == 0) ratio = 0;                    // waiting for a better solution
          //  else ratio = abs(ratio -100);
          //  akuRatioWidget *ratioWidget = new akuRatioWidget(ratio);
          //  listv -> setItemWidget(fitem, 3, ratioWidget); 

            QDateTime ts (QDate::fromString(dlist[ 3 ], "dd-MM-yy"),
                          QTime::fromString(dlist[ 4 ], "hh:mm"));
            fitem -> setText(4, ts.toString(Qt::LocaleDate));
            fitem -> setTextAlignment(4, Qt::AlignVCenter | Qt::AlignHCenter);
            //dlist.removeAt(4);
        for ( int g=5; g < dlist.size(); g++ )  
           fitem -> setText ( g, dlist[ g ] );


        KMimeType::Ptr mimePtr = KMimeType::findByUrl(KUrl(singleItem[numeroPezziPercorso]));
        KIcon icon(mimePtr -> iconName());
        fitem -> setIcon ( 0,icon );
        fitem -> setText(9, mimePtr->name());
      }
      if(crypted == true) fitem -> setIcon(10, KIcon("dialog-password"));
    }     
  }  
  // here we set status bar info
  stbarst.remove ( 0, target + 79 );
  QStringList archinfo;
  archinfo = stbarst.split ( " ", QString::SkipEmptyParts );
  archinfo[4].remove ( "\n" );
  archiveDetails << archinfo[1] << KLocale(archinfo[2] ).formatByteSize(archinfo[2].toULong())<<KLocale(archinfo[3] ).formatByteSize(archinfo[3].toULong());
  QString ratio = archinfo.at ( 4 );
  ratio.remove ( ratio.length()-1, 1 );
  float ratioNum = ratio.toFloat();
  if (ratioNum > 100.0 || ratioNum == 0.0) ratioNum = 0;
  else ratioNum = abs(ratioNum -100);
  ratioBar -> setRatio ( ratioNum );

  return archinfo[1].toInt();

}

void rar::simpleParse ( QTreeWidget * listv, QString bf )
{

  QString nomec;
  int target;
  target = bf.indexOf ( head_line );
  bf.remove ( 0,target + 79 );
  target = bf.indexOf ( head_line );
  QString stbarst;
  stbarst = bf;
  bf.remove ( target, bf.length() ); 
  bf.remove ( 0,1 );
  bf.remove ( bf.length()-1,1 );
  QStringList flist = bf.split ( "\n" );

  QStringList foldbuffer; //un contenitore per la lista di percorsi di cartella

  for ( int i = 0; i < flist.size(); i++ )
  {
    if ( i%2 == 0 )
    {
      nomec = flist.at ( i );
      nomec.remove ( 0,1 );
    }
    else
    {
      QTreeWidgetItem *fitem = new QTreeWidgetItem ( listv );
      fitem -> setText ( 0, nomec );
      QStringList dlist = ( flist.at ( i ) ).split ( " ", QString::SkipEmptyParts );
      for ( int g=0; g < dlist.size(); g++ )
      {
        //questo if gestisce la trasformazione in kilobytes
        if ( g == 0 || g == 1 )
        {
          dlist[g] = KLocale(dlist[g] ).formatByteSize(dlist[g] .toLong());
          fitem -> setTextAlignment ( 1, Qt::AlignRight | Qt::AlignVCenter );
          fitem -> setTextAlignment ( 2, Qt::AlignRight | Qt::AlignVCenter );
        }
        fitem -> setText ( g+1, dlist.at ( g ) );
      }
    }
  }

}

QStringList rar::getAttributeList(QString TOC)
{
  int target; //mi serve per sapere cosa eliminare dall'output
  target = TOC.indexOf ( head_line );
  TOC.remove ( 0,target + 79 ); //escludo tutto l'output fino al tratteggio
  target = TOC.indexOf ( head_line );
  TOC.remove ( target, TOC.length() ); //escludo tutto l'output oltre il secondo tratteggio
  TOC.remove ( 0,1 );
  TOC.remove ( TOC.length()-1,1 );
  QStringList tempAttributeList = TOC.split ( "\n" ); //splitto basandomi sul carattere di newline
  QStringList attributeList;
  for ( int i = 0; i < tempAttributeList.size(); i++ )
  {
    if ( i%2 != 0 )
    {
      attributeList << tempAttributeList.at ( i );
  
    }
  }
  return attributeList;
}

QStringList rar::getFileList(QString TOC)
{
  if(TOC.indexOf("Pathname/Comment") !=-1)
  {
    int target; //mi serve per sapere cosa eliminare dall'output
    target = TOC.indexOf ( head_line );
    TOC.remove ( 0,target + 79 ); //escludo tutto l'output fino al tratteggio
    target = TOC.indexOf ( head_line );
    TOC.remove ( target, TOC.length() ); //escludo tutto l'output oltre il secondo tratteggio
    TOC.remove ( 0,1 );
    TOC.remove ( TOC.length()-1,1 );
    QStringList tempFileList = TOC.split ( "\n" ); //splitto basandomi sul carattere di newline
    QStringList fileList;
    for ( int i = 0; i < tempFileList.size(); i++ )
    {
      if ( i%2 == 0 )
      {
        QString tmpLine = tempFileList.at ( i );
        tmpLine.remove ( 0,1 );
        //let's make sure that the following item is not a folder already included before
        QStringList attributeCheck = tempFileList[i+1].split(" ", QString::SkipEmptyParts);
        bool alreadyIn = false;
        if(attributeCheck[7] == "m0")
        {
          for(int j = 0; j < fileList.size(); j++)
            if(fileList[j].startsWith(tmpLine, Qt::CaseInsensitive) == true)
            {
              alreadyIn = true;
              break;
            }
        }
        if(alreadyIn == false)
          fileList << tmpLine;


      }
    }
    return fileList;
  }
  else return QStringList();
}

QStringList rar::getListFromPath(QString path, QString TOC)
{
  int target; //mi serve per sapere cosa eliminare dall'output
  target = TOC.indexOf ( head_line );
  if(target != -1)
  {
    TOC.remove ( 0,target + 79 ); //escludo tutto l'output fino al tratteggio
    target = TOC.indexOf ( head_line );
    TOC.remove ( target, TOC.length() ); //escludo tutto l'output oltre il secondo tratteggio
    TOC.remove ( 0,1 );
    TOC.remove ( TOC.length()-1,1 );
    QStringList tempFileList = TOC.split ( "\n" ); //splitto basandomi sul carattere di newline
    QStringList fileList;
    for ( int i = 0; i < tempFileList.size(); i++ )
    {
      if ( i%2 == 0 )
      {
        QString tmpLine = tempFileList.at ( i );
        tmpLine.remove ( 0,1 );
        //let's make sure that the following item is not a folder already included before
        QStringList attributeCheck = tempFileList[i+1].split(" ", QString::SkipEmptyParts);
        bool alreadyIn = false;
        if(attributeCheck[7] == "m0")
        {
          for(int j = 0; j < fileList.size(); j++)
          {
            if(fileList[j].startsWith(tmpLine, Qt::CaseInsensitive) == true)
            {
              alreadyIn = true;
              break;
            }
          }
        }
        if(tmpLine.startsWith(path) == true && alreadyIn == false)
        {
          fileList << tmpLine;
        }
      }
    }
    return fileList;
  }
  else return QStringList();
}

QString rar::getSingleFileAttributes(QString TOC, QString file)
{
  int target; //mi serve per sapere cosa eliminare dall'output
  target = TOC.indexOf ( head_line );
  TOC.remove ( 0,target + 79 ); //escludo tutto l'output fino al tratteggio
  target = TOC.indexOf ( head_line );
  TOC.remove( head_line );
  TOC.remove ( 0,1 );
  TOC.remove ( TOC.length()-1,1 );
  QStringList tempFileList = TOC.split ( "\n" ); //splitto basandomi sul carattere di newline

  for(int i = 0; i < tempFileList.size(); i++)
  {
   if(tempFileList[i].contains(file))
     return tempFileList[i+1];
  }
   return QString();
}

KDateTime rar::getSingleFileModificationTime(QString TOC, QString file)  //pass the output of a rar v process and the file path
{
  // Size:PackedSize:Ratio:Date:Time:Attributes:CRC:Method:Version
  QStringList attributes = rar().getSingleFileAttributes(TOC, file).split(" ", QString::SkipEmptyParts);
  KDateTime dateTime(QDate().fromString(attributes[3], "dd-MM-yy"), QTime().fromString(attributes[4], "hh:mm"));
  return dateTime;
}

QString rar::getSingleFileSize(QString TOC, QString file)
{
  QStringList attributes = rar().getSingleFileAttributes(TOC, file).split(" ", QString::SkipEmptyParts);
  QString size;
  if(!attributes.isEmpty()) size = KLocale(attributes[0]).formatByteSize(attributes[0].toLong());
  return size;
}

QStringList rar::getArchiveDetails()
{
  return QStringList() = archiveDetails;
}
