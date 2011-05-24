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

#include "nxtCanvas.h"
#include <iostream>
#include <stdlib.h> //For abs()
#include <math.h>
#include <string.h>	//For memcpy()


#include "nxtCopyOptions.h"
#include "pointArray.h"

void nxtCanvas::copy_to( nxtCanvas *destination ) const{
	//Low-level copy
	destination->width = width;
	destination->height = height;
	if( destination->map )
		delete[] destination->map;
	destination->map = new bool[width*height];
	memcpy( destination->map, map, (width*height)*sizeof( bool ) );
}

void nxtCanvas::resize( unsigned int width, unsigned int height ){
	//Make a copy
	nxtCanvas temp;
	copy_to( &temp );
	//Reinit with a new size
	create( width, height );
	//Copy the copy onto this
	copy_canvas( &temp, 0,0, temp.get_width(), temp.get_height(), 0,0 );
}

bool nxtCanvas::get_pixel(unsigned int X, unsigned int Y) const{
	if( (X >= width) || (Y >= height) || ( map == 0) )
		return false;
	
	return map[ X + Y*width ];
}

void nxtCanvas::set_pixel(unsigned int X, unsigned int Y, bool color){
	if( (X >= width) || (Y >= height) || ( map == 0) )
		return;
	
	if( color )
		map[ X + Y*width ] = true;
	else
		map[ X + Y*width ] = false;
}

inline void nxtCanvas::apply_clear( const nxtCopyOptions* options){
	if( options ){
		if( options->get_clear() )
			ClearScreen();
		
	//	if( options->clear_except_status )
	//		clear_except_status();
	}
}


void nxtCanvas::PointOut(unsigned int X, unsigned int Y, const nxtCopyOptions* options, bool clear){
	if( !options ){
		set_pixel( X, Y );
		return;
	}
	
	if( clear )
		apply_clear( options );
	
	
	bool foreground = !options->get_invert();
	
	
	switch( options->get_merge() ){
		case nxtCopyOptions::MERGE_COPY:
				set_pixel( X, Y, foreground );
			break;
			
		case nxtCopyOptions::MERGE_AND:
				set_pixel( X, Y, foreground && get_pixel( X, Y ) );
			break;
			
		case nxtCopyOptions::MERGE_OR:
				set_pixel( X, Y, foreground || get_pixel( X, Y ) );
			break;
			
		case nxtCopyOptions::MERGE_XOR:
				set_pixel( X, Y, get_pixel( X, Y ) ^ foreground );	//Warning, bitwise operation!
			break;
	}
}


void nxtCanvas::LineOut(int startX, int startY, int endX, int endY, const nxtCopyOptions* options, bool clear){
	if( clear )
		apply_clear( options );
	
	
	//Start drawing
	if( startY == endY ){	//Horizontal line
		if( startX <= endX )
			for( int ix = startX; ix <= endX; ix++ )
				PointOut( ix, startY, options, false );
		else
			for( int ix = endX; ix <= startX; ix++ )
				PointOut( ix, startY, options, false );
	}
	else if( startX == endX ){	//Vertical line
		if( startY < endY )
			for( int iy = startY; iy <= endY; iy++ )
				PointOut( startX, iy, options, false );
		else
			for( int iy = endY; iy <= startY; iy++ )
				PointOut( startX, iy, options, false );
	}
	else{	//tilting line
		double a = (double)(endY - startY) / (endX - startX);
		double b = startY - a*startX;
		
		if( abs(endY - startY) > abs(endX - startX) ){
			//iterate over y values
			if( endY > startY )
				for( int iy = startY; iy <= endY; iy++ )
					PointOut( (iy-b)/a + 0.5, iy, options, false );
			else
				for( int iy = endY; iy <= startY; iy++ )
					PointOut( (iy-b)/a + 0.5, iy, options, false );
		}
		else{
			//iterate over x values
			b += 0.5; //We can add the rounding here instead of doing it each time
			if( endX > startX )
				for( int ix = startX; ix <= endX; ix++ )
					PointOut( ix, a*ix + b, options, false );
			else
				for( int ix = endX; ix <= startX; ix++ )
					PointOut( ix, a*ix + b, options, false );
		}
		
	}
}


void nxtCanvas::RectOut(int X, int Y, int width, int height, const nxtCopyOptions* options, bool clear){
	if( clear )
		apply_clear( options );
	
	
	if( !( options && options->get_fill_shape() ) )	//If it is not a filled rectangle
		if( height > 1 && width > 1 ){
			
			LineOut( X, Y, X+width, Y, options, false );
			LineOut( X, Y+height, X+width, Y+height, options, false );
			
			LineOut( X, Y+1, X, Y+height-1, options, false );
			LineOut( X+width, Y+1, X+width, Y+height-1, options, false );
			
			
			return;
		}
	
	//Draw a filled rectangle, either because of fill_shape or that it doesn't contain whitespace
	for( int ix = X; ix <= X+width; ix++ )
		for( int iy = Y; iy <= Y+height; iy++ )
			PointOut( ix, iy, options, false );
}


//TODO: Try to reduce the amount of exceptions
void nxtCanvas::EllipseOut(int X, int Y, unsigned int radius_x, unsigned int radius_y, const nxtCopyOptions* options, bool clear){
	if( clear )
		apply_clear( options );
	
	
	
	if( radius_x == 0 ){
		if( radius_y == 0 )
			PointOut( X, Y, options, false );
		else
			LineOut( X, Y-radius_y, X, Y+radius_y, options, false );
	}
	else if( radius_y == 0 ){
		LineOut( X-radius_x, Y, X+radius_x, Y, options, false );
	}
	else{
		
		//Find the point (P) where the tangent is at a 45 deg angle
		double v = atan( (double)radius_y / radius_x );
		int Px = radius_x * cos( v ) + 0.5;
		int Py_start = radius_y * sin( acos( (double)Px/radius_x ) ) + 0.5;
		int Py;// = radius_y * sin( v ) + 0.5;
		
		//Draw ellipse path from left to P iterating with x-coordinates
		for( int i=0; i<=Px; i++ ){
			double angle = acos( (double)i/radius_x );
			Py = radius_y * sin( angle ) + 0.5;
			
			if( options && options->get_fill_shape() ){
				if( i ){
					LineOut( X+i, Y+Py, X+i, Y+Py_start, options, false );
					LineOut( X+i, Y-Py, X+i, Y-Py_start, options, false );
					LineOut( X-i, Y+Py, X-i, Y+Py_start, options, false );
					LineOut( X-i, Y-Py, X-i, Y-Py_start, options, false );
				}
				else{
					LineOut( X, Y+Py, X, Y+Py_start, options, false );
					LineOut( X, Y-Py, X, Y-Py_start, options, false );
				}
			}
			else{
				if( Py ){
					if( i ){
						PointOut( X+i, Y+Py, options, false );
						PointOut( X-i, Y+Py, options, false );
						PointOut( X+i, Y-Py, options, false );
						PointOut( X-i, Y-Py, options, false );
					}
					else{
						PointOut( X, Y+Py, options, false );
						PointOut( X, Y-Py, options, false );
					}
				}
				else{
					PointOut( X+i, Y, options, false );
					PointOut( X-i, Y, options, false );
				}
			}
		}
		
		
		//*Draw ellipse path from P and down iterating with y-coordinates
		for( int i=Py_start-1; i>=0; i-- ){ //This should be Py-1 to prevent overlap
			double angle = asin( (double)i/radius_y );
			int dx = radius_x * cos( angle ) + 0.5;
			
			if( options && options->get_fill_shape() ){
				if( i ){
					LineOut( X+dx, Y+i, X-dx, Y+i, options, false );
					LineOut( X+dx, Y-i, X-dx, Y-i, options, false );
				}
			}
			else{
				if( dx ){
					if( i ){
						PointOut( X+dx, Y+i, options, false );
						PointOut( X+dx, Y-i, options, false );
						PointOut( X-dx, Y+i, options, false );
						PointOut( X-dx, Y-i, options, false );
					}
					else{
						PointOut( X+dx, Y, options, false );
						PointOut( X-dx, Y, options, false );
					}
				}
				else{
					PointOut( X, Y+i, options, false );
					PointOut( X, Y-i, options, false );
				}
			}
		}
		
		if( options && options->get_fill_shape() ){
			LineOut( X+radius_x, Y, X-radius_x, Y, options, false );
		}
	}
}


void nxtCanvas::TextOut(int X, int Y, char* text, const nxtCopyOptions* options, bool clear){
	if( clear )
		apply_clear( options );
	
	//TODO: Convert to text and use default ricfont
}


void nxtCanvas::NumberOut(int X, int Y, int value, const nxtCopyOptions* options, bool clear){
	if( clear )
		apply_clear( options );
	
	//TODO: use default ricfont
}


void nxtCanvas::PolyOut(const pointArray* points, const nxtCopyOptions* options, bool clear){
	if( clear )
		apply_clear( options );
	
	//Must be 3 points or more
	if( points->size() < 3 )
		return;
	
	const point* first_point = points->index( 0 );
	const point* start_point = first_point;
	const point* end_point;
	for(unsigned int i=1; i<points->size(); i++){
		end_point = points->index( i );
		LineOut( start_point->X, start_point->Y, end_point->X, end_point->Y, options, false );
		start_point = end_point;
	}
	
	if( options && !options->get_polyline() )
		LineOut( first_point->X, first_point->Y, end_point->X, end_point->Y, options, false );
	//TODO: draw polygon
}



//TODO: test this throughoutly
void nxtCanvas::copy_canvas( const nxtCanvas *source, unsigned int x, unsigned int y, unsigned int width, unsigned int height, int dest_x, int dest_y, const nxtCopyOptions* options, bool clear ){
	if( clear )
		apply_clear( options );
	
	//Find starting point
	int start_x = x;
	if( dest_x < 0 ){
		start_x -= dest_x;
		dest_x = 0;
	}
	int start_y = y;
	if( dest_y < 0 ){
		start_y -= dest_y;
		dest_y = 0;
	}
	
	//Find size
	unsigned int end_width = width;
	if( width + dest_x > get_width() ){
		end_width = get_width() - dest_x;
	}
	unsigned int end_height = height;
	if( height + dest_y > get_height() ){
		end_height = get_height() - dest_y;
	}
	
	//Copy the canvas
	nxtCopyOptions background;
	options->copy_to( &background );
	background.invert_switch();
	for( unsigned int ix = 0; ix < end_width; ix++ )
		for( unsigned int iy = 0; iy < end_height; iy++ ){
			if( source->get_pixel( ix + start_x, iy + start_y ) )
				PointOut( dest_x + ix, dest_y + iy, options, false );
			else{
				PointOut( dest_x + ix, dest_y + iy, &background, false );
			}
		}
}


