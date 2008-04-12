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
#include "akupreviewlabel.h"
akuPreviewLabel::akuPreviewLabel(QWidget *parent) : QLabel(parent)
{}

akuPreviewLabel::~akuPreviewLabel()
{
 qDeleteAll(children());
}

void akuPreviewLabel::setData(QByteArray array)
{
 data = array;
}
void akuPreviewLabel::mouseMoveEvent(QMouseEvent *event)
{
  if(event -> buttons() == Qt::LeftButton && !data.isEmpty())
  {
   QDrag *drag = new QDrag(this);
   QMimeData *mimeData = new QMimeData;
   KMimeType::Ptr mimePtr = KMimeType::findByContent(data);
  
   //mimeData -> setData(mimePtr -> name(), data);
   mimeData -> setData("image/jpeg", QVariant(*pixmap()).toByteArray());
   puts(mimeData -> formats().join(" ").toAscii());
   drag -> setMimeData(mimeData);
   drag -> setPixmap(*pixmap());
   Qt::DropAction dropAction = drag -> exec();
  } else return;
}
