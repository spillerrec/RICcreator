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
	options_fill_inverted = false;
	
	canvas = NULL;
	clipboard = NULL;
	
	setAttribute( Qt::WA_OpaquePaintEvent );
}


void nxtCanvasWidget::change_canvas( nxtCanvas* new_canvas, bool delete_old ){
	//Delete old canvas if wanted
	if( delete_old && canvas )
		delete canvas;
	
	canvas = new_canvas;
	
	//reset shown position
	if( is_moveable )
		reset_pos();
	
	update();	//Update the view
	emit canvas_changed();
}


QPoint nxtCanvasWidget::point_to_pos( QPoint pos ) const{
	QPoint delta = ( QPoint( pos_x, pos_y ) + pos ) * (int)current_zoom;
	return QPoint( delta.x(), height()-delta.y() );
}

QPoint nxtCanvasWidget::pos_to_point( QPoint pos ) const{
	QPoint result;
	result.setX( pos.x() / (int)current_zoom - pos_x );
	result.setY( ( height() - pos.y() ) / (int)current_zoom - pos_y );
	
	return result;
}

QRect nxtCanvasWidget::get_visible_area() const{
	QPoint pos_start = pos_to_point( QPoint( 0, height() ) );
	QPoint pos_end = pos_to_point( QPoint( width(), 0 ) );
	
	return QRect( pos_start, pos_end );
}

QRect nxtCanvasWidget::canvas_position() const{
	if( !canvas )
		return QRect();
	
	return QRect( -canvas->get_offset_x(), -canvas->get_offset_y(), canvas->get_width(), canvas->get_height() );
}

void nxtCanvasWidget::reset_pos(){
	QRect canvas_pos = canvas_position();
	QRect area = get_visible_area();
	
	if( canvas_pos.isEmpty() )
		return;
	
	pos_x = -canvas_pos.x();
	pos_y = -canvas_pos.y();
	
	if( canvas_pos.width() < area.width() )
		pos_x += ( area.width() - canvas_pos.width() ) / 2;
	
	if( canvas_pos.height() < area.height() )
		pos_y += ( area.height() - canvas_pos.height() ) / 2;
	
	update();
	emit visible_area_changed();
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
		//If size changed
		if( canvas->size_affected() )
			emit visible_area_changed();
		
		//Set up contants
		QRect canvas_area( -canvas->get_offset_x(),-canvas->get_offset_y(), canvas->get_width(), canvas->get_height() );
		qDebug( "nxtCanvasWidget: Redraw started" );
		
		//Find last visible point on the widget
		//QPoint end_point =
	
		//Find start point
		unsigned int start_x = -pos_x;
		unsigned int start_y = -pos_y;
		/* if( pos_x < 0 )
			start_x = -pos_x;
		if( pos_y < 0 )
			start_y = -pos_y; */
		
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
		
		//Find size
		unsigned int view_width = available_width / current_zoom;
		unsigned int view_height = available_height / current_zoom;
		if( available_width / current_zoom )
			view_width++;
		if( available_height / current_zoom )
			view_height++;
		
		QRect visible_area( start_x, start_y, view_width, view_height );
		QRect drawn_area = canvas_area & visible_area;
		
		
		//Transform the view
		painter.translate( 0, widget_height );
		painter.scale( current_zoom, current_zoom );
		painter.translate( pos_x, -pos_y );
		
		//Draw the canvas background
		painter.fillRect( drawn_area.x(), -(drawn_area.y()+drawn_area.height()), drawn_area.width(), drawn_area.height(), Qt::white );
		
		//Draw the active pixels
		for( int iy=drawn_area.y(); iy < drawn_area.y()+drawn_area.height(); iy++ ){
			bool *pixel = canvas->get_scanline( iy + canvas->get_offset_y() );
			pixel += drawn_area.x() + canvas->get_offset_x();
			for( int ix=drawn_area.x(); ix < drawn_area.x()+drawn_area.width(); ix++ ){
				if( *pixel )
					painter.fillRect( ix, -iy-1, 1, 1, Qt::black );
				pixel++;
			}
		}
		
		//Draw grid if zoom is large enough
		if( current_zoom > 2 ){
			//Draw base grid
			painter.setPen( QColor( 0xA0, 0xA0, 0xA4 ) );
			for( int ix=drawn_area.x(); ix < drawn_area.x()+drawn_area.width()+1; ix++ )
				painter.drawLine( ix,-drawn_area.y(), ix,-(drawn_area.y()+drawn_area.height()) );
			for( int iy=drawn_area.y(); iy < drawn_area.y()+drawn_area.height()+1; iy++ )
				painter.drawLine( drawn_area.x(),-iy, (drawn_area.x()+drawn_area.width()),-iy );
			
			//Draw larger grid
			painter.setPen( QColor( 0x58, 0x58, 0xFF ) );
			for( int ix=drawn_area.x(); ix < drawn_area.x()+drawn_area.width()+1; ix++ )
				if( (ix % 10) == 0 )
					painter.drawLine( ix,-drawn_area.y(), ix,-(drawn_area.y()+drawn_area.height()) );
			for( int iy=drawn_area.y(); iy < drawn_area.y()+drawn_area.height()+1; iy++ )
				if( (iy % 10) == 0 )
					painter.drawLine( drawn_area.x(),-iy, (drawn_area.x()+drawn_area.width()),-iy );
			
		}
		
		painter.fillRect( selection.x(), -(selection.y()+selection.height()), selection.width(), selection.height(), QColor( 255,0,0, 128 ) );
		
		canvas->reset_affected();
		
	}
}


//TODO: this isn't accurate enough!
void nxtCanvasWidget::zoom_at( QPoint pos, int zoom_level ){
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
void nxtCanvasWidget::zoom( int zoom_level ){
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
	pos_x = new_x;
	update();
}
void nxtCanvasWidget::change_pos_y( int new_y ){
	pos_y = new_y;
	update();
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
	static QPoint prev_last_mouse;
	
	if( active_tool == TOOL_NONE )
		return;
	
	//First handle the events that doesn't change the canvas
	if( active_tool == TOOL_SELECTION ){
		selection = get_qrect_from_points( mouse_start, mouse_current );
		if( selection.height() >= 0 )
			selection.setHeight( selection.height()+1 );
		if( selection.width() >= 0 )
			selection.setWidth( selection.width()+1 );
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
			//if( event == EVENT_MOUSE_UP )
				canvas->set_auto_resize( true );
				//TODO: enable, then restore to previous state
			
			switch( active_tool ){
				case TOOL_PIXEL:
						//Draw a dot first time, then follow with lines
						if( event == EVENT_MOUSE_DOWN )
							canvas->PointOut( mouse_current.x(), mouse_current.y(), options );
						else
							canvas->connected_line_out( prev_last_mouse.x(), prev_last_mouse.y(), mouse_last.x(), mouse_last.y(), mouse_current.x(), mouse_current.y(), options );
						
						prev_last_mouse = mouse_last;
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
						if( clipboard ){
							if( event == EVENT_MOUSE_DOWN ){
								selection.setWidth( clipboard->get_width() );
								selection.setHeight( clipboard->get_height() );
							}
							selection.moveTo( selection.topLeft() + mouse_current - mouse_last );
							canvas->copy_canvas( clipboard, 0, 0, selection.width(), selection.height(), selection.x(), selection.y(), options );
						}
					} break;
				
				case TOOL_FILL:	canvas->bucket_fill( mouse_current.x(), mouse_current.y(), options ); break;
				
				default: qDebug( "nxtCanvasWidget::action() unhandled tool: %d", active_tool );
			}
			
			//If this is the last action, make the buffer permanent
			if( event == EVENT_MOUSE_UP && active_tool != TOOL_BITMAP ){
			//	canvas->set_auto_resize( false );
				write_buffer();
			}
			
			update();
			emit value_changed();
		}
	
	
}


void nxtCanvasWidget::mousePressEvent( QMouseEvent *event ){
	if( mouse_active ){
		stop_drawing();
		return;
	}
	
	active_tool = current_tool;
	
	//Set modifiers
	key_control = event->modifiers() & Qt::ControlModifier;
	key_shift = event->modifiers() & Qt::ShiftModifier;
	set_options_fill( key_shift );
	
	
	set_options_inverted( false );	//Just to be safe, turn it off if wrong
	
	//Check which button caused the event
	if( event->button() & Qt::LeftButton ){
	}
	else if( event->button() & Qt::MidButton )
		active_tool = TOOL_MOVE;	//Go into move mode
	else if( event->button() & Qt::RightButton )
		set_options_inverted( true );	//Invert the CopyOptions
	else{
		//We do not handle this event
		event->ignore();
		return;
	}
	
	//Make sure to start correctly if canvas is empty
	if( canvas && canvas->get_width() == 0 && canvas->get_height() == 0 ){
		pos_x = pos_y = 0;
		QPoint offset = pos_to_point( event->pos() );
		pos_x = offset.x();
		pos_y = offset.y();
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
			canvas->copy_canvas( clipboard, 0, 0, selection.width(), selection.height(), selection.x(), selection.y(), options );
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
		set_options_fill( key_shift );
		
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
	set_options_inverted( false );
	set_options_fill( false );
	
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
		set_options_inverted( false );
		set_options_fill( false );
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
			emit options_changed();
		}
		update();
	}
	
	current_tool = new_tool;
	
}

void nxtCanvasWidget::set_options_inverted( bool setting ){
	if( options ){
		if( setting != options_inverted ){
			options_inverted = !options_inverted;
			options->invert_switch();
			emit options_changed();
		}
	}
}

void nxtCanvasWidget::set_options_fill( bool setting ){
	if( options ){
		if( setting != options_fill_inverted ){
			options_fill_inverted = !options_fill_inverted;
			options->set_fill_shape( !options->get_fill_shape() );
			emit options_changed();
		}
	}
}


void nxtCanvasWidget::import_image(){
	//TODO:
	importImageDialog dialog( this );
	if( dialog.exec() == importImageDialog::Accepted ){
		paste( dialog.get_canvas() );
	}
}

void nxtCanvasWidget::copy_to_clipboard(){
	if( canvas && !selection.isEmpty() ){
		if( clipboard )
			delete clipboard;
		
		//TODO: check why new nxtCanvas( x, y ) fails!!!
		clipboard = new nxtCanvas();//( selection.width(), selection.height() );
		clipboard->create( selection.width(), selection.height() );
		clipboard->copy_canvas( canvas, selection.x(), selection.y(), selection.width(), selection.height(), 0, 0 );
		//TODO: Crop selection to canvas
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


