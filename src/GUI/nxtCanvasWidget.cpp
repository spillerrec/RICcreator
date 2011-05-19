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

nxtCanvasWidget::nxtCanvasWidget( QWidget* parent ): QGraphicsView( parent ){
	setScene(&scene);
}


void nxtCanvasWidget::change_canvas( nxtCanvas* new_canvas, bool delete_old ){
	
	if( delete_old && canvas ){
		delete canvas;
	}
	
	canvas = new_canvas;
	
	update();
}


void nxtCanvasWidget::update(){
	if( canvas ){
		
		QImage temp_image( canvas->get_width(), canvas->get_height(), QImage::Format_Mono );
		setSceneRect(0,0,  canvas->get_width(), canvas->get_height() );
		
		
		temp_image.fill(1);
		for(unsigned int ix=0; ix<canvas->get_width(); ix++)
			for(unsigned int iy=0; iy<canvas->get_height(); iy++){
				QPoint position( ix, canvas->get_height()-1-iy );
				
				if( canvas->get_pixel( ix, iy ) )
					temp_image.setPixel( position, 0 );
				else
					temp_image.setPixel( position, 1 );
			}
		
		scene.clear();
		scene.addPixmap( QPixmap::fromImage( temp_image ) );
		
		show();
	}
}


void nxtCanvasWidget::zoom( unsigned int zoom_level ){
	scale( zoom_level, zoom_level );
}
