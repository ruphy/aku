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
#ifndef AKUPREVIEWLABEL_H
#define AKUPREVIEWLABEL_H
#include <QLabel>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <KMimeType>
#include <QBuffer>
class akuPreviewLabel : public QLabel
{
 Q_OBJECT
 public:
  akuPreviewLabel(QWidget *parent = 0);
  ~akuPreviewLabel();
  void setData(QByteArray);
 private:
   QByteArray data;
 protected: 
  virtual void mouseMoveEvent(QMouseEvent*);
};

#endif