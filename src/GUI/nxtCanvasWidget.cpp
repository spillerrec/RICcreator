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
#include <math.h>

#include <QSize>

nxtCanvasWidget::nxtCanvasWidget( QWidget* parent ): nxtVarEditAbstract( parent ){
	pos_x = 0;
	pos_y = 0;
	current_zoom = 1;
	
	is_buffered = false;
	enable_buffer();
	uses_buffer = false;
	is_moveable = false;
	is_editable = false;
	
	mouse_active = false;
	key_control = false;
	key_shift = false;
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


QPoint nxtCanvasWidget::point_to_pos( QPoint pos ) const{
	QPoint zero_point( 0, height() );
	QPoint delta = ( QPoint( pos_x, pos_y ) + pos ) * current_zoom;
	return QPoint( zero_point.x()+delta.x(), zero_point.y()-delta.y() );
}

QPoint nxtCanvasWidget::pos_to_point( QPoint pos ) const{
	QPoint result;
	
	//Set x
	if( pos.x() )
		result.setX( pos.x() / (int)current_zoom - pos_x );
	else
		result.setX( pos_x );
	
	//Set y
	if( height() - pos.y() )
		result.setY( ( height() - pos.y() ) / (int)current_zoom - pos_y );
	else
		result.setY( pos_y );
	
	return result;
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



void nxtCanvasWidget::zoom_at( QPoint pos, unsigned int zoom_level ){
	QPoint p_before = point_to_pos( pos );
	current_zoom = zoom_level;
	QPoint p_delta = pos_to_point( p_before ) - pos;
	
	change_pos( p_delta.x(), p_delta.y() );
}
void nxtCanvasWidget::zoom( unsigned int zoom_level ){
	zoom_at( QPoint( pos_x, pos_y ), zoom_level );
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
	change_pos( new_x - pos_x, 0 );
}
void nxtCanvasWidget::change_pos_y( int new_y ){
	change_pos( 0, new_y - pos_y );
}

void nxtCanvasWidget::change_pos( int dx, int dy ){
	pos_x += dx;
	pos_y += dy;
	
	//Update the cursor positions
	if( mouse_active ){
		mouse_current -= QPoint( dx, dy );
		mouse_last -= QPoint( dx, dy );
		//TODO: update action()
	}
	
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




QRect get_qrect_from_points( QPoint p1, QPoint p2 ){
	//Create Rectangle
	QRect rect( p1, QSize() );
	rect.setWidth( abs( p2.x() - p1.x() ) );
	rect.setHeight( abs( p2.y() - p1.y() ) );
	
	//move if p2 is smaller than p1
	if( p2.x() < p1.x() )
		rect.translate( p2.x() - p1.x(), 0 );
	if( p2.y() < p1.y() )
		rect.translate( 0, p2.y() - p1.y() );
	
	return rect;
}

void nxtCanvasWidget::action( action_event event ){
	if( active_tool == TOOL_NONE )
		return;
	
	//First handle the events that doesn't change the canvas
	if( active_tool == TOOL_SELECTION ){
		selection = get_qrect_from_points( mouse_start, mouse_current );
		update();
		//TODO: emit signal?
	}
	else if( active_tool == TOOL_MOVE )
		change_pos( mouse_current.x() - mouse_last.x(), mouse_current.y() - mouse_last.y() );
	else
		if( canvas ){
			//Create a fresh buffer, unless we are drawing in freehand
			if( active_tool != TOOL_PIXEL )
				new_buffer();
			
			//If this is the last action, enable auto-resize
			if( event == EVENT_MOUSE_UP )
				canvas->set_auto_resize( true );
			
			switch( active_tool ){
				case TOOL_PIXEL:
						canvas->LineOut( mouse_last.x(), mouse_last.y(), mouse_current.x(), mouse_current.y(), options );
						//TODO: Draw a pixel on first click, then lines on draw
					break;
					
				case TOOL_LINE: canvas->LineOut( mouse_start.x(), mouse_start.y(), mouse_current.x(), mouse_current.y(), options ); break;
				//TODO: limit it to specific angles
				
				case TOOL_RECT:{
						QRect rect = get_qrect_from_points( mouse_start, mouse_current );
						
						//Make it a square if control is pressed
						if( key_control ){
							if( rect.width() > rect.height() ){
								if( mouse_current.y() < mouse_start.y() )
									rect.translate( 0, rect.height() - rect.width() );
								rect.setHeight( rect.width() );
							}
							else{
								if( mouse_current.x() < mouse_start.x() )
									rect.translate( rect.width() - rect.height(), 0 );
								rect.setWidth( rect.height() );
							}
						}
						
						canvas->RectOut( rect.x(), rect.y(), rect.width(), rect.height(), options );
					} break;
					
				case TOOL_ELLIPSE:
						if( key_control ){
							QPoint lenght = mouse_current - mouse_start;
							int radius = sqrt( pow( lenght.x(), 2 ) + pow( lenght.y(), 2 ) );
							canvas->CircleOut( mouse_start.x(), mouse_start.y(), radius, options );
						}
						else
							canvas->EllipseOut( mouse_start.x(), mouse_start.y(), abs( mouse_current.x() - mouse_start.x() ), abs( mouse_current.y() - mouse_start.y() ), options );
						break;
				
				case TOOL_BITMAP:{
						//TODO:
					} break;
				
				default: qDebug( "nxtCanvasWidget::action() unhandled tool: %d", active_tool );
			}
			
			//If this is the last action, make the buffer permanent
			if( event == EVENT_MOUSE_UP ){
				canvas->set_auto_resize( false );
				write_buffer();
			}
			
			update();
			emit value_changed();
		}
	
	
}


void nxtCanvasWidget::mousePressEvent( QMouseEvent *event ){
	active_tool = current_tool;
	
	//Set modifiers
	key_control = event->modifiers() & Qt::ControlModifier;
	key_shift = event->modifiers() & Qt::ShiftModifier;
	
	//This shouldn't happen
	if( options && options_inverted ){	//Just to be safe, turn it off if wrong
		options_inverted = true;
		options->invert_switch();
	}
	
	//Check which button caused the event
	if( event->button() & Qt::LeftButton ){
		//Do nothing...
	}
	else if( event->button() & Qt::MidButton ){
		//Go into move mode
		active_tool = TOOL_MOVE;
	}
	else if( event->button() & Qt::RightButton ){
		//Invert the CopyOptions
		if( options && !options_inverted ){
			options_inverted = true;
			options->invert_switch();
		}
	}
	else{
		//We do not handle this event
		event->ignore();
		return;
	}
	
	mouse_start = pos_to_point( event->pos() );
	mouse_current = mouse_start;
	mouse_last = mouse_start;
	mouse_active = true;
	
	
	use_buffer();
	action( EVENT_MOUSE_DOWN );
}

void nxtCanvasWidget::mouseMoveEvent( QMouseEvent *event ){
	if( mouse_active ){
		mouse_current = pos_to_point( event->pos() );
		
		//Set modifiers
		key_control = event->modifiers() & Qt::ControlModifier;
		key_shift = event->modifiers() & Qt::ShiftModifier;
		
		action( EVENT_MOUSE_MOVE );
		
		mouse_last = mouse_current;
	}
	else
		event->ignore();
}


void nxtCanvasWidget::mouseReleaseEvent( QMouseEvent *event ){
	if( mouse_active ){
		mouse_current = pos_to_point( event->pos() );
		
		action( EVENT_MOUSE_UP );
		
		mouse_active = false;
	}
	
	//Restore inverted state now
	if( options && options_inverted ){
		options_inverted = false;
		options->invert_switch();
	}
	
}


void nxtCanvasWidget::wheelEvent( QWheelEvent *event ){
	//Set modifiers
	key_control = event->modifiers() & Qt::ControlModifier;
	key_shift = event->modifiers() & Qt::ShiftModifier;
	
	int amount = event->delta() / 8;
	
	if( key_control ){
		//Zoom
		if( amount > 0 )
			zoom_at( pos_to_point( event->pos() ), current_zoom+1 );
		else
			zoom_at( pos_to_point( event->pos() ), current_zoom-1 );
	}
	else if( key_shift )
		change_pos( amount, 0 );
	else
		change_pos( 0, amount );
}


bool nxtCanvasWidget::change_object( nxtVariable* object ){
	if( object && object->var_type() == nxtVariable::TYPE_BITMAP ){
		change_canvas( (nxtCanvas*)object, false );
		return true;
	}
	
	return false;
}




