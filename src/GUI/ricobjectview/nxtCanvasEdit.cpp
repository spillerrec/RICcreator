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


#include "../../riclib/nxtCanvas.h"
#include "../../riclib/nxtCopyOptions.h"
#include "nxtCanvasEdit.h"

#include <QMouseEvent>
#include <QPointF>
#include <math.h>	//For abs()

nxtCanvasEdit::nxtCanvasEdit( QWidget* parent ): nxtCanvasWidget( parent ){
	pressed = false;
	enable_buffer();
	current_tool = 0;
	options = NULL;
	options_inverted = false;
}

void swap( int &x, int &y ){
	int temp = x;
	x = y;
	y = temp;
}


void nxtCanvasEdit::draw( int pos1_x, int pos1_y, int pos2_x, int pos2_y ){
	qDebug( "nxtCanvasEdit::draw()" );
	qDebug( "pos1_x: %d", pos1_x );
	qDebug( "pos1_y: %d", pos1_y );
	qDebug( "pos2_x: %d", pos2_x );
	qDebug( "pos2_y: %d", pos2_y );
	if( canvas )
		switch( current_tool ){
			case 0: discard_buffer(); canvas->PointOut( pos2_x, pos2_y, options ); break;
			case 1: canvas->LineOut( pos1_x, pos1_y, pos2_x, pos2_y, options ); break;
			case 2:
					if( pos1_x > pos2_x )
						swap( pos1_x, pos2_x );
					if( pos1_y > pos2_y )
						swap( pos1_y, pos2_y );
					
					canvas->RectOut( pos1_x, pos1_y, pos2_x - pos1_x, pos2_y-pos1_y, options );
				break;
			case 3: canvas->EllipseOut( pos1_x, pos1_y, abs( pos2_x - pos1_x ), abs( pos2_y - pos1_y ), options ); break;
		}

}


void nxtCanvasEdit::mousePressEvent( QMouseEvent *event ){
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

void nxtCanvasEdit::mouseMoveEvent( QMouseEvent *event ){
	qDebug( "nxtCanvasEdit::mouseMoveEvent()" );
	qDebug( "event->x(): %d", event->x() );
	qDebug( "event->y(): %d", event->y() );
	if( pressed ){
		new_buffer();
		draw( start_x, start_y, pos_point_x( event->x() ), pos_point_y( event->y() ) );
		update();
		
		emit value_changed();
	}
	else
		event->ignore();
}


void nxtCanvasEdit::mouseReleaseEvent( QMouseEvent *event ){
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

