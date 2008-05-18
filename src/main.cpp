/***************************************************************************
 *   Copyright (C) 2008 by alessandro   *
 *   alessandro@gentux   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include <QTextCodec>
#include <KIconLoader>
#include "mainwindow.h"
#include "rarprocesshandler.h"

int main ( int argc, char *argv[] )
{

  KAboutData aboutData( "aku", 0, ki18n("aKu"),
      "2.0-SVN", ki18n("The RAR application for KDE4"), KAboutData::License_GPL_V3,
      ki18n("Copyright (c) 2008"));
   
  aboutData.addAuthor(ki18n("Alessandro Diaferia aka \"The Speedy Coder\""), ki18n("Main Developer"), "alediaferia@gmail.com");
  aboutData.addAuthor(ki18n("Francesco Grieco aka \"The Japanese GUI stylist\""), ki18n("Developer"), "fgrieco@gmail.com");
  // chiamata per disattivare il report dei bugs a kde4
  aboutData.setCustomAuthorText(ki18n(0), ki18n(0));
  KCmdLineArgs::init(argc, argv, &aboutData);
  KCmdLineOptions options;
  options.add("+[archive]", ki18n("Makes aku open the archive specified"));
  options.add("extractto <destination>", ki18n("Extracts the archive to <destination>"));
  KCmdLineArgs::addCmdLineOptions( options );

  KApplication app;
  QString loIcon = KIconLoader().iconPath("aku",-KIconLoader::SizeEnormous );
  QApplication::setWindowIcon(KIcon(loIcon));
  //QApplication::setWindowIcon( KIcon( "utilities-file-archiver" ) ); ///TEMP ICON
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
  MainWindow * mw = new MainWindow();
  mw->show();
 // KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

  return app.exec();
}

