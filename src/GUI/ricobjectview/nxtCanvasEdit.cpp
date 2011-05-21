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
#include "nxtCanvasEdit.h"

#include <QMouseEvent>
#include <QPointF>
#include <math.h>	//For abs()

nxtCanvasEdit::nxtCanvasEdit( QWidget* parent ): nxtCanvasWidget( parent ){
	pressed = false;
	enable_buffer();
	current_tool = 0;
}


void nxtCanvasEdit::mousePressEvent( QMouseEvent *event ){
	if( event->buttons() & Qt::LeftButton ){
		QPointF pos = mapToScene( event->x(), event->y() );
		start_x = pos.x();
		start_y = canvas->get_height() - pos.y();
		pressed = true;
		
		
		use_buffer();
		switch( current_tool ){
			case 0: discard_buffer();	//Draw for each move event
			case 1:
			case 2:
			case 3: canvas->set_pixel( start_x, start_y ); break;	//All the tools look like a pixel
		}
		update();
		
		emit value_changed();
	}
}

void swap( unsigned int &x, unsigned int &y ){
	unsigned int temp = x;
	x = y;
	y = temp;
}

void nxtCanvasEdit::mouseMoveEvent( QMouseEvent *event ){
	if( pressed ){
		QPointF pos = mapToScene( event->x(), event->y() );
		pos.setY( canvas->get_height() - pos.y() );
		
		new_buffer();
		
		unsigned int pos1_x = start_x;
		unsigned int pos1_y = start_y;
		unsigned int pos2_x = pos.x();
		unsigned int pos2_y = pos.y();
		
		switch( current_tool ){
			case 0: canvas->set_pixel( pos2_x, pos2_y ); break;
			case 1: canvas->LineOut( pos1_x, pos1_y, pos2_x, pos2_y ); break;
			case 2:
					if( pos1_x > pos2_x )
						swap( pos1_x, pos2_x );
					if( pos1_y > pos2_y )
						swap( pos1_y, pos2_y );
					
					canvas->RectOut( pos1_x, pos1_y, pos2_x - pos1_x, pos2_y-pos1_y );
				break;
			case 3: canvas->EllipseOut( pos1_x, pos1_y, abs( pos2_x - pos1_x ), abs( pos2_y - pos1_y ) ); break;
		}
		update();
		
		emit value_changed();
	}
}


void nxtCanvasEdit::mouseReleaseEvent( QMouseEvent *event ){
	if( pressed ){
		write_buffer();
		pressed = false;
		emit value_changed();
	}
}

