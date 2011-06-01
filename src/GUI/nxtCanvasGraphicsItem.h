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

#ifndef NXTCANVASGRAPHICSITEM_H
#define NXTCANVASGRAPHICSITEM_H

#include <QGraphicsItem>

class nxtCanvas;

class nxtCanvasGraphicsItem: public QGraphicsItem{
	
	private:
		nxtCanvas* canvas;
		
	public:
		explicit nxtCanvasGraphicsItem( QGraphicsItem* parent = NULL );
		
		void change_canvas( nxtCanvas* new_canvas ){
			prepareGeometryChange();
			canvas = new_canvas;
		}
	
		QRectF boundingRect() const;
		
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
		
		void refresh();

};

#endif
