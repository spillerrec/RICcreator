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

#include <QRect>
#include <QPoint>
#include "nxtVarEdits/nxtVarEditAbstract.h"

class nxtCanvas;
class nxtCopyOptions;

class nxtCanvasWidget: public nxtVarEditAbstract{
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
		
		//Get the point at the position on the widget
		QPoint pos_to_point( QPoint pos ) const;
		QPoint point_to_pos( QPoint pos ) const;
	
	protected:
		nxtCanvas* canvas;
		void paintEvent( QPaintEvent *event );
		
	
	//Stuff for tools
	public:
		enum tool_type{ TOOL_NONE, TOOL_PIXEL, TOOL_LINE, TOOL_RECT, TOOL_ELLIPSE, TOOL_BITMAP, TOOL_SELECTION, TOOL_MOVE };
	private:
		tool_type current_tool;
		bool is_moveable;
		bool is_editable;
		QRect selection;
		
	private:
		nxtCopyOptions* options;
		bool options_inverted;
		
	protected:
		bool mouse_active;
		tool_type active_tool;
		bool key_control;
		bool key_shift;
		QPoint mouse_start;
		QPoint mouse_current;
		QPoint mouse_last;
		
		enum action_event{ EVENT_MOUSE_DOWN, EVENT_MOUSE_MOVE, EVENT_MOUSE_UP, EVENT_MOVE, EVENT_MOD };
		void action( action_event event );
		
	public:
		void set_tool( tool_type new_tool ){ current_tool = new_tool; }
		QRect get_selection(){ return selection; }
		void set_options( nxtCopyOptions* new_options ){ options = new_options; }
		
	protected:
		void mousePressEvent( QMouseEvent *event );
		void mouseMoveEvent( QMouseEvent *event );
		void mouseReleaseEvent( QMouseEvent *event );
		void wheelEvent( QWheelEvent *event );
		
	public:
		explicit nxtCanvasWidget( QWidget* parent );
		void change_canvas( nxtCanvas* new_canvas, bool delete_old = false );
		bool change_object( nxtVariable* object );
		void zoom_at( QPoint pos, unsigned int zoom_level );
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
		void change_pos( int dx, int dy );
	
	
	
	signals:
		void canvas_changed();
		void canvas_edited();
		void visible_area_changed();
};

#endif
