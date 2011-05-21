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

#include <QImage>
#include <QPoint>
#include <QBrush>
#include <QColor>

nxtCanvasWidget::nxtCanvasWidget( QWidget* parent ): QGraphicsView( parent ), image(0,0), paint( &image ){
	setBackgroundBrush( QBrush( QColor( 196, 196, 196 ) ) );	//Set background color
	
	setScene(&scene);
	
	current_zoom = 1;
	
	is_buffered = false;
	uses_buffer = false;
		
		paint.setBrush( QBrush( QColor( 0,0,0 ) ) );
}


void nxtCanvasWidget::change_canvas( nxtCanvas* new_canvas, bool delete_old ){
	//Delete old canvas if wanted
	if( delete_old && canvas )
		delete canvas;
	
	canvas = new_canvas;
	
	update();	//Update the view
}


void nxtCanvasWidget::update(){	//TODO: IMPROVE PERFORMANCE!!
	if( canvas ){
		//Get size
		unsigned int width = canvas->get_width();
		unsigned int height = canvas->get_height();
		
		//Create a new QPixmap if not existent or wrong size
		if( image.height() != height || image.width() != width ){
			image = QPixmap( width, height );	//Resize QPixmap
			setSceneRect(0,0,  width, height );
			paint.begin( &image );
		}
		
		
		//Draw the nxtCanvas
		image.fill();
		for(unsigned int ix=0; ix<width; ix++)
			for(unsigned int iy=0; iy<height; iy++){
				if( canvas->get_pixel( ix, iy ) )
					paint.drawPoint( ix, height-1-iy );
			}
		
		//Delete any old images and add this one
		scene.clear();
		scene.addPixmap( image );
		
		show();
	}
}


void nxtCanvasWidget::zoom( unsigned int zoom_level ){
	//Calculate the scaling amount, scale, and store new zoom level
	double scale_amount = (double)zoom_level / current_zoom;
	scale( scale_amount, scale_amount );
	current_zoom = zoom_level;
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

