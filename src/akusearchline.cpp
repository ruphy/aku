#include "akusearchline.h"

akuSearchLine::akuSearchLine ( QTreeWidget *treeWidget, QWidget *parent ) : QLineEdit ( parent )
{
  tree = treeWidget;
  filterBySelection = false;
  caseSensitive = false;
  connect ( this, SIGNAL ( textChanged ( QString ) ), this, SLOT ( filter ( QString ) ) );
  connect ( this, SIGNAL ( returnPressed() ), this, SLOT ( expandFoundItems() ) );
  redSearchLine = false; //di default la searchline non Ã¨ rossa
  greenSearchLine = false;
}

akuSearchLine::~akuSearchLine()
{
}

QList<QTreeWidgetItem *> akuSearchLine::recursiveFilter ( QTreeWidgetItem *toFilt, QString cfr )  //funzione ricorsiva per la scansione in profondità dei figli
{
  QList<QTreeWidgetItem*> tempList;
  if ( filterBySelection == false )   //filtro a esclusione
  {
    for ( int h = 0; h < toFilt -> childCount(); h++ )
    {
      if ( caseSensitive == false )   //distingue tra ricerca caseInsensitive ...
      {
        if ( ( ( toFilt -> child ( h ) ) -> text ( 0 ) ).indexOf ( cfr, 0, Qt::CaseInsensitive ) == -1 )
        {
          tree-> setItemHidden ( toFilt -> child ( h ), true );
          tempList << toFilt -> child ( h );
        }
        if ( ( ( toFilt -> child ( h ) ) -> text ( 0 ) ).indexOf ( cfr,0, Qt::CaseInsensitive ) != -1 )
        {
          QTreeWidgetItem *tmp = toFilt;
          while ( tmp != NULL )
          {
            tree-> setItemHidden ( tmp, false );
            tmp = tmp -> parent();
          }
        }
      }
      else //... e non
      {
        if ( ( ( toFilt -> child ( h ) ) -> text ( 0 ) ).indexOf ( cfr ) == -1 )
        {
          tree-> setItemHidden ( toFilt -> child ( h ), true );
          tempList << toFilt -> child ( h );
        }
        if ( ( ( toFilt -> child ( h ) ) -> text ( 0 ) ).indexOf ( cfr ) != -1 )
        {
          QTreeWidgetItem *tmp = toFilt;
          while ( tmp != NULL )
          {
            tree-> setItemHidden ( tmp, false );
            tmp = tmp -> parent();
          }
        }
      }
      if ( ( toFilt -> child ( h ) ) -> childCount() !=0 ) tempList << recursiveFilter ( toFilt->child ( h ), cfr );
    }
  }
  else  //filtro a selezione
  {
    for ( int h = 0; h < toFilt -> childCount(); h++ )
    {
      if ( caseSensitive == false )
      {
        if ( ( ( toFilt -> child ( h ) ) -> text ( 0 ) ).indexOf ( cfr,0,Qt::CaseInsensitive ) != -1 )
        {
          tree-> setItemSelected ( toFilt -> child ( h ), true );
          tempList << toFilt -> child ( h );
          redSearchLine = false; //se abbiamo trovato l'elemento non vogliamo che diventi rossa la searchline
          greenSearchLine = true;
        }
      }
      else
      {
        if ( ( ( toFilt -> child ( h ) ) -> text ( 0 ) ).indexOf ( cfr ) != -1 )
        {
          tree-> setItemSelected ( toFilt -> child ( h ), true );
          tempList << toFilt -> child ( h );
          redSearchLine = false;
          greenSearchLine = true;
        }
      }
      if ( ( toFilt -> child ( h ) ) -> childCount() !=0 ) 
        tempList << recursiveFilter ( toFilt->child ( h ), cfr );
    }
  }
  return tempList;
}

void akuSearchLine::filter ( QString filt ) //funzione di filtro della lista basandosi su searchLine
{

  QList<QTreeWidgetItem*> selectedToClear = tree-> selectedItems();	//deselezioniamo gli elementi giï¿½ selezionati
  QPalette defaultPalette; //ripristiniamo il colore della searchline
  setPalette ( defaultPalette );
  redSearchLine = false;
  greenSearchLine = false;
  for ( int h = 0; h < selectedToClear.size(); h++ ) tree-> setItemSelected ( selectedToClear[h], false ); //deselezioniamo gli elementi giï¿½ selezionati
  restoreList();
  QString tmp;
  if ( text() != "" )
  {
    //filteredItems.clear();
    if ( filterBySelection == false ) //se ï¿½ abilitato il filtro ad esclusione
    {
      for ( int i = 0; i < tree-> topLevelItemCount(); i++ )
      {
        tmp = ( tree-> topLevelItem ( i ) ) -> text ( 0 );
        if ( caseSensitive == false )   //se non c'Ã¨ il case sensitive
        {
          if ( tmp.indexOf ( filt, 0, Qt::CaseInsensitive ) ==-1 )   //era diverso
          {
            tree->setItemHidden ( tree-> topLevelItem ( i ), true );
            filteredItems << tree-> topLevelItem ( i );
          }
        }
        else  //altrimenti col case sensitive
        {
          if ( tmp.indexOf ( filt ) ==-1 )   //era diverso
          {
            tree->setItemHidden ( tree-> topLevelItem ( i ), true );
            filteredItems << tree-> topLevelItem ( i );
          }

        }
        if ( ( tree-> topLevelItem ( i ) )->childCount() !=0 )
          filteredItems << recursiveFilter ( tree-> topLevelItem ( i ), filt );
      }
    }
    else  //altrimenti uso quello a selezione
    {
      //redSearchLine = false;
      redSearchLine = true;
      greenSearchLine = false;
      if ( caseSensitive == false )   //se non c'Ã¨ il case sensitive...
      {
        for ( int i = 0; i < tree-> topLevelItemCount(); i++ )
        {
          tmp = ( tree-> topLevelItem ( i ) ) -> text ( 0 );
          if ( tmp.indexOf ( filt, 0, Qt::CaseInsensitive ) !=-1 )   //se trovo l'elemento lo seleziono
          {
            tree->setItemSelected ( tree-> topLevelItem ( i ), true ); //lo seleziono
            filteredItems << tree-> topLevelItem ( i );//e comincio la ricerca ricorsiva
            redSearchLine = false;
            greenSearchLine = true;
          }
          if ( ( tree-> topLevelItem ( i ) )->childCount() !=0 )
            filteredItems << recursiveFilter ( tree-> topLevelItem ( i ), filt );
        }
      }
      else  //altrimenti col case sensitive
      {
        for ( int i = 0; i < tree-> topLevelItemCount(); i++ )
        {
          tmp = ( tree-> topLevelItem ( i ) ) -> text ( 0 );
          if ( tmp.indexOf ( filt ) !=-1 )   //se trovo l'elemento lo seleziono
          {
            tree->setItemSelected ( tree-> topLevelItem ( i ), true ); //lo seleziono
            filteredItems << tree-> topLevelItem ( i ); //e inizio la ricerca ricorsiva
            redSearchLine = false;
            greenSearchLine = true;
          }
          if ( ( tree-> topLevelItem ( i ) )->childCount() !=0 )
            filteredItems << recursiveFilter ( tree-> topLevelItem ( i ), filt );
        }
      }
    }
  }


  if ( filteredItems.size() == 1 )
    tree-> scrollToItem ( filteredItems[0] );
  //QMessageBox::information(this, "DEBUG", "redSearchline "+QString().setNum(redSearchline));
  if ( redSearchLine == true && filt != "" ) //se Ã¨ necessario rendiamo rossa la searchline
  {
    QPalette palette;
    QColor alertColor;
    alertColor.setHsv ( 0, 120, 255 );
    palette.setColor ( QPalette::Active, QPalette::Base, alertColor ); //la searchline diventa rossa se non trova la parola
    setPalette ( palette );
  }
  else
    if(greenSearchLine == true && filt != "")
    {
      QPalette palette;
      QColor greenColor;
      greenColor.setHsv ( 120, 120, 255 );
      palette.setColor ( QPalette::Active, QPalette::Base, greenColor ); //la searchline diventa verde se trova la parola
      setPalette ( palette );
    }
  
}

void akuSearchLine::restoreList()
{
  for ( int j = 0; j < filteredItems.size(); j++ )
  {
    if ( tree -> isItemHidden ( filteredItems[j] ) == true ) tree -> setItemHidden ( filteredItems[j], false );
    if ( tree -> isItemSelected ( filteredItems[j] ) == true ) tree -> setItemSelected ( filteredItems[j], false );
  }
  filteredItems.clear();
}

void akuSearchLine::expandFoundItems()
{
  for ( int j = 0; j < filteredItems.size(); j++ )
  {
    tree -> scrollToItem ( filteredItems[j] );
  }
}


void akuSearchLine::setFilterBySelection ( bool)
{
  filterBySelection = true;
  //-----------permettiamo il passaggio istantaneo da un metodo di filtro all'altro------//
  QString tmp = this -> text();
  setText ( "" );
  setText ( tmp );
}

void akuSearchLine::setFilterByExclusion ( bool opt )
{
  filterBySelection = !opt;
  //-----------permettiamo il passaggio istantaneo da un metodo di filtro all'altro------//
  QString tmp = this -> text();
  setText ( "" );
  setText ( tmp );
}

void akuSearchLine::setFilterCaseSensitivity ( bool flag )
{
  caseSensitive = flag;
  //-----------permettiamo il passaggio istantaneo da un metodo di filtro all'altro------//
  QString tmp = this -> text();
  setText ( "" );
  setText ( tmp );
  //-------------------------------------------------------------------------------------//
}
