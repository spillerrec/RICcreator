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

#ifndef NXTCANVASWIDGET_H
#define NXTCANVASWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include <QPainter>

class nxtCanvas;

class nxtCanvasWidget: public QGraphicsView{
	Q_OBJECT
	
	private:
		unsigned int current_zoom;
		nxtCanvas* buffer;
		bool is_buffered;
		bool uses_buffer;
	
	protected:
		nxtCanvas* canvas;
		QGraphicsScene scene;
		QPixmap image;
		QPainter paint;
	
	public:
		explicit nxtCanvasWidget( QWidget* parent );
		void change_canvas( nxtCanvas* new_canvas, bool delete_old = false );
		void zoom( unsigned int zoom_level );
		
		void enable_buffer();
		void use_buffer();
		void write_buffer();
		void discard_buffer();
		void new_buffer();
	
	
	public slots:
		void update();
	
	signals:
		
};

#endif
