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
#ifndef AKUTREEWIDGET_H
#define AKUTREEWIDGET_H

#include <QTreeWidget>
#include <KMenu>
#include <KAction>
#include <KLocale>
#include <QHeaderView>
#include <QPoint>
class akuTreeWidget : public QTreeWidget
{

Q_OBJECT

public:
      akuTreeWidget(QWidget *parent);
      ~akuTreeWidget();

public slots:
 
protected slots:
  virtual void setupActions();  
  virtual void headerCustomContextMenu(QPoint);
  virtual void showratio(bool);
  virtual void showattributes(bool);
  virtual void showmethod(bool);
  virtual void showversion(bool);
  virtual void showsize(bool);
  virtual void showpackedsize(bool);
  virtual void showmodified(bool);
  virtual void showcrc(bool);
  virtual void showmime(bool);

private:
   // header actions
  KAction *headerRatio;
  KAction *headerAttributes;
  KAction *headerMethod;
  KAction *headerVersion;
  KAction *headerSize;
  KAction *headerPackedSize;
  KAction *headerModified;
  KAction *headerCRC;
  KAction *headerMIME;
  KMenu *mPopup;

};
#endif
