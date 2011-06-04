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
#include "nxtCanvasWidget.h"

#include <QPainter>
#include <QBrush>
#include <QColor>

nxtCanvasWidget::nxtCanvasWidget( QWidget* parent ): QWidget( parent ){
//	setBackgroundBrush( QBrush( QColor( 196, 196, 196 ) ) );	//Set background color
	pos_x = 0;
	pos_y = 0;
	current_zoom = 1;
	
	is_buffered = false;
	uses_buffer = false;
}


void nxtCanvasWidget::change_canvas( nxtCanvas* new_canvas, bool delete_old ){
	//Delete old canvas if wanted
	if( delete_old && canvas )
		delete canvas;
	
	canvas = new_canvas;
//	image.change_canvas( canvas );
	
	update();	//Update the view
}



int nxtCanvasWidget::point_pos_x( int x ) const{
	return ( pos_x + x ) * current_zoom;
}

int nxtCanvasWidget::point_pos_y( int y ) const{
	return height() - ( pos_y + y ) * current_zoom;
}

//TODO: improve accuracy
int nxtCanvasWidget::pos_point_x( int x ) const{
	if( x )
		return x / (int)current_zoom + pos_x;
	else
		return pos_x;
}
int nxtCanvasWidget::pos_point_y( int y ) const{
	if( height() - y )
		return ( height() - y ) / (int)current_zoom + pos_y;
	else
		return pos_y;
}



void nxtCanvasWidget::paintEvent( QPaintEvent *event ){
	QPainter painter( this );
	
	//Draw the entire widget gray
	unsigned int widget_height = this->height();
	unsigned int widget_width = this->width();
	painter.fillRect( 0,0, widget_width, widget_height, Qt::gray );
	
	//Reserve some width for borders and the like
	unsigned int left_area_width = 0;
	unsigned int top_area_height = 0;
	
	if( canvas ){
	
		//Find start point
		unsigned int start_x = 0;
		unsigned int start_y = 0;
		if( pos_x < 0 )
			start_x = -pos_x;
		if( pos_y < 0 )
			start_y = -pos_y;
		
		unsigned int available_width = widget_width - left_area_width;
		unsigned int available_height = widget_height - top_area_height;
		unsigned int height = canvas->get_height();
		unsigned int width = canvas->get_width();
		
		//Find end point
		unsigned int end_x = available_width / current_zoom;
		unsigned int end_y = available_height / current_zoom;
		if( end_x >= width )
			end_x = width;
		if( end_y >= height )
			end_y = height;
		
		
		//Draw the canvas background
		painter.fillRect( point_pos_x(start_x),point_pos_y(end_y)+current_zoom, (end_x-start_x)*current_zoom, (end_y-start_y)*current_zoom, Qt::white );
		
		//Draw the active pixels
		for( unsigned int ix=start_x; ix < end_x; ix++ )
			for( unsigned int iy=start_x; iy < end_y; iy++ ){
				if( canvas->get_pixel( ix, iy ) )
					painter.fillRect( point_pos_x(ix), point_pos_y(iy), current_zoom, current_zoom, Qt::black );
			}
		
		canvas->reset_affected();
		
	}
}



void nxtCanvasWidget::zoom( unsigned int zoom_level ){
	current_zoom = zoom_level;
	update();
}



void nxtCanvasWidget::enable_buffer(){
	if( !is_buffered ){
		buffer = new nxtCanvas;
		is_buffered = true;
	}
}
void nxtCanvasWidget::use_buffer(){
	if( is_buffered ){
		if( !uses_buffer ){
			canvas->copy_to( buffer );
			uses_buffer = true;
		}
	}
}
void nxtCanvasWidget::write_buffer(){
	if( is_buffered && uses_buffer ){
		uses_buffer = false;
	}
}
void nxtCanvasWidget::discard_buffer(){
	if( is_buffered && uses_buffer ){
		buffer->copy_to( canvas );
		
		uses_buffer = false;
	}
}

void nxtCanvasWidget::new_buffer(){
	if( is_buffered && uses_buffer ){
		buffer->copy_to( canvas );
	}
}

