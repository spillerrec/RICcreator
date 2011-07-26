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
const double PI = atan(1.0)*4;
static double round_sym( double r ){
	return ( r > 0.0 ) ? floor( r + 0.5 ) : ceil( r - 0.5 );
}

#include <QSize>
#include <QFileDialog>
#include "importImageDialog.h"

nxtCanvasWidget::nxtCanvasWidget( QWidget* parent ): nxtVarEditAbstract( parent ){
	pos_x = 0;
	pos_y = 0;
	current_zoom = 1;
	
	is_buffered = false;
	
	is_moveable = false;
	is_editable = false;
	
	mouse_active = false;
	key_control = false;
	key_shift = false;
	current_tool = TOOL_NONE;
	active_tool = TOOL_NONE;
	options = NULL;
	options_inverted = false;
	
	canvas = NULL;
	clipboard = NULL;
	
	setAttribute( Qt::WA_OpaquePaintEvent );
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
	QPoint delta = ( QPoint( pos_x, pos_y ) + pos ) * current_zoom;
	return QPoint( delta.x(), height()-delta.y() );
}

QPoint nxtCanvasWidget::pos_to_point( QPoint pos ) const{
	QPoint result;
	result.setX( pos.x() / (int)current_zoom - pos_x );
	result.setY( ( height() - pos.y() ) / (int)current_zoom - pos_y );
	
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
		
		//Draw grid if zoom is large enough
		if( current_zoom > 2 ){
			//Draw base grid
			painter.setPen( QColor( 0xA0, 0xA0, 0xA4 ) );
			for( int ix=drawn_area.x(); ix < drawn_area.x()+drawn_area.width()+1; ix++ )
				painter.drawLine( ix,0, ix,-(drawn_area.y()+drawn_area.height()) );
			for( int iy=drawn_area.y(); iy < drawn_area.y()+drawn_area.height()+1; iy++ )
				painter.drawLine( 0,-iy, (drawn_area.x()+drawn_area.width()),-iy );
			
			//Draw larger grid
			painter.setPen( QColor( 0x58, 0x58, 0xFF ) );
			for( int ix=drawn_area.x(); ix < drawn_area.x()+drawn_area.width()+1; ix++ )
				if( (ix % 10) == 0 )
					painter.drawLine( ix,0, ix,-(drawn_area.y()+drawn_area.height()) );
			for( int iy=drawn_area.y(); iy < drawn_area.y()+drawn_area.height()+1; iy++ )
				if( (iy % 10) == 0 )
					painter.drawLine( 0,-iy, (drawn_area.x()+drawn_area.width()),-iy );
			
		}
		
		painter.fillRect( selection.x(), -(selection.y()+selection.height()), selection.width(), selection.height(), QColor( 255,0,0, 128 ) );
		
		canvas->reset_affected();
		
	}
}


//TODO: this isn't accurate enough!
void nxtCanvasWidget::zoom_at( QPoint pos, unsigned int zoom_level ){
	if( zoom_level < 1 )	//Prevent invalid zoom levels
		zoom_level = 1;
	
	//Calculate pos_x,pos_y so it keeps pos position
	QPoint point_before = pos_to_point( pos );
	current_zoom = zoom_level;
	QPoint p_delta = pos_to_point( pos ) - point_before;
	
	//change_pos( p_delta.x(), p_delta.y() );	//This function thinks the move causes the mouse to change!
	//So change manually:
	pos_x += p_delta.x();
	pos_y += p_delta.y();
	update();
	emit visible_area_changed();
}
void nxtCanvasWidget::zoom( unsigned int zoom_level ){
	zoom_at( QPoint( 0, height() ), zoom_level );
}



void nxtCanvasWidget::enable_buffer(){
	if( !is_buffered ){
		buffer = new nxtCanvas;
		canvas->copy_to( buffer );
		is_buffered = true;
	}
}
void nxtCanvasWidget::write_buffer(){
	if( is_buffered ){
		delete buffer;
		is_buffered = false;
		//TODO: create undo point?
	}
}
void nxtCanvasWidget::disable_buffer(){
	if( is_buffered ){
		buffer->copy_to( canvas );
		delete buffer;
		is_buffered = false;
	}
}

void nxtCanvasWidget::new_buffer(){
	if( is_buffered ){
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
		mouse_last = mouse_current;	//If this is called within action()
		mouse_current -= QPoint( dx, dy );
		
		action( EVENT_MOVE );	//Update the tool
		
		mouse_last = mouse_current;
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
	else if( active_tool == TOOL_MOVE ){
		if( is_moveable && event != EVENT_MOVE )	//Prevent infinitive loop
			change_pos( mouse_current.x() - mouse_last.x(), mouse_current.y() - mouse_last.y() );
	}
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
						//Draw a dot first time, then follow with lines
						if( event == EVENT_MOUSE_DOWN )
							canvas->PointOut( mouse_current.x(), mouse_current.y(), options );
						else
							canvas->LineOut( mouse_last.x(), mouse_last.y(), mouse_current.x(), mouse_current.y(), options, 1 );
					break;
					
				case TOOL_LINE:
						if( key_control ){	//Limit to angles in steps of 15 degrees
							int dx = mouse_current.x() - mouse_start.x();
							int dy = mouse_current.y() - mouse_start.y();
							
							double angle = atan2( (double)( dy ), (double)( dx ) );
							double lenght = sqrt( (double)( dx*dx + dy*dy ) );
							double limiter = PI * 15 / 180;
							double new_angle = round_sym( angle / limiter ) * limiter;
							dy = round_sym( sin( new_angle ) * lenght );
							dx = round_sym( cos( new_angle ) * lenght );
							
							canvas->LineOut( mouse_start.x(), mouse_start.y(), mouse_start.x() + dx, mouse_start.y() + dy, options );
						}
						else
							canvas->LineOut( mouse_start.x(), mouse_start.y(), mouse_current.x(), mouse_current.y(), options );
					break;
				
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
							int radius = sqrt( (double)( lenght.x()*lenght.x() + lenght.y()*lenght.y() ) );
							canvas->CircleOut( mouse_start.x(), mouse_start.y(), radius, options );
						}
						else
							canvas->EllipseOut( mouse_start.x(), mouse_start.y(), abs( mouse_current.x() - mouse_start.x() ), abs( mouse_current.y() - mouse_start.y() ), options );
						break;
				
				case TOOL_BITMAP:{
						if( event == EVENT_MOUSE_DOWN ){
							selection.setWidth( clipboard->get_width() );
							selection.setHeight( clipboard->get_height() );
						}
						selection.moveTo( selection.topLeft() + mouse_current - mouse_last );
						canvas->copy_canvas( clipboard, 0, 0, selection.width(), selection.height(), selection.x(), selection.y(), options );
					} break;
				
				case TOOL_FILL:	canvas->bucket_fill( mouse_current.x(), mouse_current.y(), options ); break;
				
				default: qDebug( "nxtCanvasWidget::action() unhandled tool: %d", active_tool );
			}
			
			//If this is the last action, make the buffer permanent
			if( event == EVENT_MOUSE_UP && active_tool != TOOL_BITMAP ){
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
		options_inverted = false;
		options->invert_switch();
	}
	
	//Check which button caused the event
	if( event->button() & Qt::LeftButton ){
		if( mouse_active ){
			stop_drawing();
			return;
		}
	}
	else if( event->button() & Qt::MidButton ){
		if( mouse_active ){
			stop_drawing();
			return;
		}
		
		//Go into move mode
		active_tool = TOOL_MOVE;
	}
	else if( event->button() & Qt::RightButton ){
		if( mouse_active ){
			stop_drawing();
			return;
		}
		
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
	
	
	enable_buffer();
	action( EVENT_MOUSE_DOWN );
}

void nxtCanvasWidget::mouseDoubleClickEvent( QMouseEvent *event ){
	if( active_tool == TOOL_BITMAP ){
		//Paste bitmap finally
		if( canvas ){
			canvas->set_auto_resize( true );
			canvas->copy_canvas( clipboard, 0, 0, selection.width(), selection.height(), selection.x(), selection.y(), options );
			canvas->set_auto_resize( false );
			write_buffer();
		}
		
		//Hide seletion
		selection.setWidth( 0 );
		selection.setHeight( 0 );
		
		update();
		emit value_changed();
	}
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
	if( !is_moveable )
		return;
	
	//Set modifiers
	key_control = event->modifiers() & Qt::ControlModifier;
	key_shift = event->modifiers() & Qt::ShiftModifier;
	
	int amount = event->delta() / 8;
	
	if( key_control ){
		//Zoom
		if( amount > 0 )
			zoom_at( event->pos(), current_zoom+1 );
		else
			zoom_at( event->pos(), current_zoom-1 );
	}
	else if( key_shift )
		change_pos( amount, 0 );
	else
		change_pos( 0, -amount );
}

void nxtCanvasWidget::stop_drawing(){
	if( mouse_active ){
		
		//Restore variables
		disable_buffer();
		if( options && options_inverted ){
			options_inverted = false;
			options->invert_switch();
		}
		mouse_active = false;
		
		update();
	}
}


bool nxtCanvasWidget::change_object( nxtVariable* object ){
	if( object && object->var_type() == nxtVariable::TYPE_BITMAP ){
		change_canvas( (nxtCanvas*)object, false );
		return true;
	}
	
	return false;
}


void nxtCanvasWidget::set_tool( tool_type new_tool ){
	//TOOL_BITMAP is constant on, so disable it now
	if( current_tool == TOOL_BITMAP ){
		disable_buffer();
		selection.setWidth( 0 );
		selection.setHeight( 0 );
		if( options && options_inverted ){
			options_inverted = false;
			options->invert_switch();
		}
		update();
	}
	
	current_tool = new_tool;
	
}

void nxtCanvasWidget::copy_to_clipboard(){
	if( canvas && !selection.isEmpty() ){
		if( clipboard )
			delete clipboard;
		
		clipboard = new nxtCanvas( selection.width(), selection.height() );
		clipboard->copy_canvas( canvas, selection.x(), selection.y(), selection.width(), selection.height(), 0, 0 );
	}
}
void nxtCanvasWidget::paste_from_clipboard(){
	if( clipboard ){
		selection.setWidth( clipboard->get_width() );
		selection.setHeight( clipboard->get_height() );
		
		//Init tool
		active_tool = current_tool = TOOL_BITMAP;
		enable_buffer();
		action( EVENT_MOUSE_DOWN );
	}
}
void nxtCanvasWidget::paste( nxtCanvas *copy ){
	if( clipboard )
		delete clipboard;
	clipboard = copy;
	
	paste_from_clipboard();
}

void nxtCanvasWidget::save(){
	//TODO: limit to selection if TOOL_SELECTION
	QString filename = QFileDialog::getSaveFileName(this, tr("Export to png"), "", tr("Portable network graphics (*.png)") );
	
	//Limit saved portion to current selection
	if(current_tool == TOOL_SELECTION && !selection.isNull() ){
		nxtCanvas *temp = new nxtCanvas( selection.width(), selection.height() );
		temp->copy_canvas( canvas, selection.x(), selection.y(), selection.width(), selection.height(), 0, 0 );
		importImageDialog::export_canvas( temp, filename );
		delete temp;
	}
	else
		importImageDialog::export_canvas( canvas, filename );
}


void nxtCanvasWidget::crop(){
	if( !canvas )
		return;
	
	if(current_tool == TOOL_SELECTION && !selection.isNull() ){
		//Crop to selection
		canvas->crop_to( selection.x(), selection.y(), selection.width(), selection.height() );
	}
	else
		canvas->autocrop( options );
	
	update();
	emit value_changed();
}


