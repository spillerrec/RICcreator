/*
	This file is part of RICcreator.

	RICcreator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RICcreator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RICcreator.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "../riclib/nxtCanvas.h"
#include "nxtCanvasGraphicsItem.h"

#include <QPainter>
#include <QBrush>
#include <QColor>
#include <QRect>

nxtCanvasGraphicsItem::nxtCanvasGraphicsItem( QGraphicsItem* parent ): QGraphicsItem( parent ){
	canvas = NULL;
}

void nxtCanvasGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
	if( canvas ){
	//	painter->setClipRect( option->exposedRect );
	//	TODO: make this work...
		
		unsigned int height = canvas->get_height();
		unsigned int width = canvas->get_width();
		painter->fillRect( 0,0, width, height, Qt::white );
		
		for( unsigned int ix=0; ix < width; ix++ )
			for( unsigned int iy=0; iy < height; iy++ ){
				if( canvas->get_pixel( ix, iy ) )
					painter->fillRect( ix, height-iy-1, 1, 1, Qt::black );
					//fillRect with an with of one, because the view might be scaled
			}
	}
}


QRectF nxtCanvasGraphicsItem::boundingRect() const{
	if( canvas )
		return QRect( 0,0, canvas->get_width(), canvas->get_height() );
	else
		return QRect( 0,0, 0,0 );
}


void nxtCanvasGraphicsItem::refresh(){
	if( canvas ){
		update( boundingRect() );
	}
}
