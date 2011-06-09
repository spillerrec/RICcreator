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
		
		//Get the position on the widget from point
		int point_pos_x( int x ) const;
		int point_pos_y( int y ) const;
		
		//Get the point at the position on the widget
		int pos_point_x( int x ) const;
		int pos_point_y( int y ) const;
	
	protected:
		nxtCanvas* canvas;
		void paintEvent( QPaintEvent *event );
		
	
	//Stuff for tools
	public:
		enum tool_type{ TOOL_NONE, TOOL_PIXEL, TOOL_LINE, TOOL_RECT, TOOL_CIRCLE, TOOL_ELLIPSE, TOOL_SELECTION };
	private:
		tool_type current_tool;
		bool is_moveable;
		bool is_editable;
		QRect selection;
		
	private:
		int start_x;
		int start_y;
		bool pressed;
		nxtCopyOptions* options;
		bool options_inverted;
		
		void draw( int pos1_x, int pos1_y, int pos2_x, int pos2_y );
		
	public:
		void set_tool( tool_type new_tool ){ current_tool = new_tool; }
		QRect get_selection(){ return selection; }
		void set_options( nxtCopyOptions* new_options ){ options = new_options; }
		
	protected:
		void mousePressEvent( QMouseEvent *event );
		void mouseMoveEvent( QMouseEvent *event );
		void mouseReleaseEvent( QMouseEvent *event );
		
	public:
		explicit nxtCanvasWidget( QWidget* parent );
		void change_canvas( nxtCanvas* new_canvas, bool delete_old = false );
		bool change_object( nxtVariable* object );
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
