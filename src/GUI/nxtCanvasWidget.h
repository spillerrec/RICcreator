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

#include <QWidget>

class nxtCanvas;

class nxtCanvasWidget: public QWidget{
	Q_OBJECT
	
	private:
		nxtCanvas* buffer;
		bool is_buffered;
		bool uses_buffer;
		
	//Values for positioning the canvas
	protected:
		//The left corner of the canvas
		int pos_x;
		int pos_y;
		unsigned int current_zoom;
		
		//Get the position on the widget from point
		int point_pos_x( int x ) const;
		int point_pos_y( int y ) const;
		
		//Get the point at the position on the widget
		int pos_point_x( int x ) const;
		int pos_point_y( int y ) const;
	
	protected:
		nxtCanvas* canvas;
		void paintEvent( QPaintEvent *event );
	
	public:
		explicit nxtCanvasWidget( QWidget* parent );
		void change_canvas( nxtCanvas* new_canvas, bool delete_old = false );
		void zoom( unsigned int zoom_level );
		
		void enable_buffer();
		void use_buffer();
		void write_buffer();
		void discard_buffer();
		void new_buffer();
		
		unsigned int canvas_width() const;
		unsigned int canvas_height() const;
		
		void change_pos_x( int new_x );
		void change_pos_y( int new_y );
	
	
	
	signals:
		void canvas_changed();
		void canvas_edited();
		void visible_area_changed();
};

#endif
