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
#include "../riclib/nxtCopyOptions.h"
#include "nxtCanvasWidget.h"

#include <QPainter>
#include <QBrush>
#include <QColor>
#include <QMouseEvent>

#include <QPoint>
#include <QSize>
#include <QRect>

nxtCanvasWidget::nxtCanvasWidget( QWidget* parent ): nxtVarEditAbstract( parent ){
	pos_x = 0;
	pos_y = 0;
	current_zoom = 1;
	
	is_buffered = false;
	enable_buffer();
	uses_buffer = false;
	is_moveable = false;
	is_editable = false;
	
	pressed = false;
	current_tool = TOOL_NONE;
	options = NULL;
	options_inverted = false;
	
	canvas = NULL;
}


void nxtCanvasWidget::change_canvas( nxtCanvas* new_canvas, bool delete_old ){
	//Delete old canvas if wanted
	if( delete_old && canvas )
		delete canvas;
	
	canvas = new_canvas;
	
	update();	//Update the view
	emit canvas_changed();
}



int nxtCanvasWidget::point_pos_x( int x ) const{
	return ( pos_x + x ) * current_zoom;
}

int nxtCanvasWidget::point_pos_y( int y ) const{
	return height() - ( pos_y + y + 1 ) * current_zoom;
}

//TODO: improve accuracy
int nxtCanvasWidget::pos_point_x( int x ) const{
	if( x )
		return x / (int)current_zoom - pos_x;
	else
		return pos_x;
}
int nxtCanvasWidget::pos_point_y( int y ) const{
	if( height() - y )
		return ( height() - y ) / (int)current_zoom - pos_y;
	else
		return pos_y;
}



void nxtCanvasWidget::paintEvent( QPaintEvent *event ){
	QPainter painter( this );
	
	//Set op some areas
	QRect drawing_area( QPoint(), size() );
	QPoint p_delta( pos_x, pos_y );
	
	
	//Draw the entire widget gray
	unsigned int widget_height = this->height();
	unsigned int widget_width = this->width();
	painter.fillRect( drawing_area, Qt::gray );
	
	//Reserve some width for borders and the like
	unsigned int left_area_width = 0;
	unsigned int top_area_height = 0;
	
	if( canvas ){
		//Set up contants
		QRect canvas_area( 0,0, canvas->get_width(), canvas->get_height() );
		
		QString test( "fdsjkl" "fdjkls" );
		//Find last visible point on the widget
		//QPoint end_point =
	
		//Find start point
		unsigned int start_x = 0;
		unsigned int start_y = 0;
		if( pos_x < 0 )
			start_x = -pos_x;
		if( pos_y < 0 )
			start_y = -pos_y;
		
		unsigned int available_width = widget_width - left_area_width;
		unsigned int available_height = widget_height - top_area_height;
		
		//Find end point
		unsigned int end_x = available_width / current_zoom - pos_x;
		unsigned int end_y = available_height / current_zoom - pos_y;
		//Round up instead of down
		if( available_width / current_zoom )
			end_x++;
		if( available_height / current_zoom )
			end_y++;
		
		QRect visible_area( start_x, start_y, end_x-start_x, end_y-start_y );
		QRect drawn_area = canvas_area & visible_area;
		
		
		//Transform the view
		painter.translate( 0, widget_height );
		painter.scale( current_zoom, current_zoom );
		painter.translate( pos_x, -pos_y );
		
		//Draw the canvas background
		painter.fillRect( drawn_area.x(), -(drawn_area.y()+drawn_area.height()), drawn_area.width(), drawn_area.height(), Qt::white );
		
		//Draw the active pixels
		for( int ix=drawn_area.x(); ix < drawn_area.x()+drawn_area.width(); ix++ )
			for( int iy=drawn_area.y(); iy < drawn_area.y()+drawn_area.height(); iy++ ){
				if( canvas->get_pixel( ix, iy ) )
					painter.fillRect( ix, -iy-1, 1, 1, Qt::black );
			}
		
		painter.fillRect( selection.x(), -(selection.y()+selection.height()), selection.width(), selection.height(), QColor( 255,0,0, 128 ) );
		
		canvas->reset_affected();
		
	}
}



void nxtCanvasWidget::zoom( unsigned int zoom_level ){
	current_zoom = zoom_level;
	update();
	emit visible_area_changed();
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


		
void nxtCanvasWidget::change_pos_x( int new_x ){
	pos_x = new_x;
	update();
	emit visible_area_changed();
}
void nxtCanvasWidget::change_pos_y( int new_y ){
	pos_y = new_y;
	update();
	emit visible_area_changed();
}



unsigned int nxtCanvasWidget::canvas_width() const{
	if( canvas )
		return canvas->get_width();
	else
		return 0;
}
unsigned int nxtCanvasWidget::canvas_height() const{
	if( canvas )
		return canvas->get_height();
	else
		return 0;
}





//TODO: remove
void order( int &small, int &big );

void nxtCanvasWidget::draw( int pos1_x, int pos1_y, int pos2_x, int pos2_y ){
	if( canvas )
		switch( current_tool ){
			case TOOL_NONE: break;
			case TOOL_PIXEL:
					discard_buffer();
					start_x = pos2_x;
					start_y = pos2_y;
					//Draw a normal line now
			case TOOL_LINE: canvas->LineOut( pos1_x, pos1_y, pos2_x, pos2_y, options ); break;
			case TOOL_RECT:
					order( pos1_x, pos2_x );
					order( pos1_y, pos2_y );
					
					canvas->RectOut( pos1_x, pos1_y, pos2_x - pos1_x, pos2_y-pos1_y, options );
				break;
			case TOOL_CIRCLE:{
					//Calculate manhattenlenght
					int radius = abs( pos2_x - pos1_x ) + abs( pos2_y - pos1_y );
					canvas->CircleOut( pos1_x, pos1_y, radius, options );
				} break;
			case TOOL_ELLIPSE: canvas->EllipseOut( pos1_x, pos1_y, abs( pos2_x - pos1_x ), abs( pos2_y - pos1_y ), options ); break;
			case TOOL_SELECTION:{
					order( pos1_x, pos2_x );
					order( pos1_y, pos2_y );
					
					selection.setCoords( pos1_x, pos2_y, pos2_x, pos1_y );
				} break;
		}

}


void nxtCanvasWidget::mousePressEvent( QMouseEvent *event ){
	if( event->buttons() & Qt::LeftButton ){
		if( options && options_inverted ){	//Just to be safe, turn it off if wrong
			options_inverted = true;
			options->invert_switch();
		}
	}
	else if( event->buttons() & Qt::RightButton ){
		if( options && !options_inverted ){
			options_inverted = true;
			options->invert_switch();
		}
	}
	else{
		event->ignore();
		return;
	}
	
	start_x = pos_point_x( event->x() );
	start_y = pos_point_y( event->y() );
	pressed = true;
	
	
	use_buffer();
	draw( start_x, start_y, start_x, start_y );
	update();
	
	emit value_changed();
}

void nxtCanvasWidget::mouseMoveEvent( QMouseEvent *event ){
	if( pressed ){
		new_buffer();
		draw( start_x, start_y, pos_point_x( event->x() ), pos_point_y( event->y() ) );
		update();
		
		emit value_changed();
	}
	else
		event->ignore();
}


void nxtCanvasWidget::mouseReleaseEvent( QMouseEvent *event ){
	if( pressed ){
		new_buffer();
		canvas->set_auto_resize( true );
		draw( start_x, start_y, pos_point_x( event->x() ), pos_point_y( event->y() ) );
		canvas->set_auto_resize( false );
		write_buffer();
		pressed = false;
		update();
		emit value_changed();
	}
	
	//Restore inverted state now
	if( options && options_inverted ){
		options_inverted = false;
		options->invert_switch();
	}
	
}


bool nxtCanvasWidget::change_object( nxtVariable* object ){
	if( object && object->var_type() == nxtVariable::TYPE_BITMAP ){
		change_canvas( (nxtCanvas*)object, false );
		return true;
	}
	
	return false;
}




