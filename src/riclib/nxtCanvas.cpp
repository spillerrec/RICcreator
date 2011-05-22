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

inline void nxtCanvas::apply_clear( nxtCopyOptions* options){
	if( options ){
		if( options->get_clear() )
			ClearScreen();
		
	//	if( options->clear_except_status )
	//		clear_except_status();
	}
}


void nxtCanvas::PointOut(unsigned int X, unsigned int Y, nxtCopyOptions* options, bool clear){
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


double FunctionX( int X, int startX, int startY, int endX, int endY){
	double a = (double)(endY - startY) / (endX - startX);
	double b = startY - a*startX;
	
	return a*X + b;
}

double FunctionY( int Y, int startX, int startY, int endX, int endY){
	double a = (double)(endY - startY) / (endX - startX);
	double b = startY - a*startX;
	
	return (Y-b)/a;
}


void nxtCanvas::PlotLineY(int startX, int startY, int endX, int endY, nxtCopyOptions* options){
	//Determine which point is the first
	int firstY, lastY;
	if( startY <= endY ){
		firstY = startY;
		lastY = endY;
	}
	else{
		firstY = endY;
		lastY = startY;
	}
	
	//For each Y value, draw a point
	for(int i=firstY; i<=lastY; i++){
		//If the line is vertical, do not try to convert it into a function
		if( (endX - startX) == 0)
			PointOut( startX, i, options, false );
		else
			PointOut( (unsigned int)(FunctionY(i, startX, startY, endX, endY)+0.5), i, options, false );
	}
}


void nxtCanvas::PlotLineX(int startX, int startY, int endX, int endY, nxtCopyOptions* options){
	//Determine which point is the first
	int firstX, lastX;
	if( startX <= endX ){
		firstX = startX;
		lastX = endX;
	}
	else{
		firstX = endX;
		lastX = startX;
	}
	
	//For each X value, draw a point
	for(int i=firstX; i<=lastX; i++)
		PointOut( i, (unsigned int)(FunctionX(i, startX, startY, endX, endY)+0.5), options, false );
}


void nxtCanvas::LineOut(int startX, int startY, int endX, int endY, nxtCopyOptions* options, bool clear){
	if( clear )
		apply_clear( options );
	
	//If both points are on the same spot, just draw a pixel
	if( startX == endX && startY == endY ){
		PointOut( startX, startY, options, false );
		return;
	}
	
	//Determine if the line should be draw for each X value, or each Y value
	if( abs(endY - startY) > abs(endX - startX) )
		PlotLineY( startX, startY, endX, endY, options );
	else
		PlotLineX( startX, startY, endX, endY, options );
}


void nxtCanvas::RectOut(int X, int Y, int width, int height, nxtCopyOptions* options, bool clear){
	if( clear )
		apply_clear( options );
	
	if( height < 0 || width < 0 )
		return;
	
	//Draw the two horizontal lines
	LineOut( X,Y, X+width,Y, options, false );
	if( height > 0 )	//Don't draw the second line on top of the other
		LineOut( X,Y+height, X+width,Y+height, options, false );
	
	//Draw the two vertical lines
	if( height > 1 ){
		LineOut( X,Y+1, X,Y+height-1, options, false );
		LineOut( X+width,Y+1, X+width,Y+height-1, options, false );
	}
	
	//Check for fill-shape
	if( options && options->get_fill_shape() ){
		if( width > 1 ){
			for( int i=1; i<height; i++ )
				LineOut( X+1, Y+i, X+width-1, Y+i, options, false );
		}
	}
}


//TODO: this still doesn't work properly as some pixels are drawn multiple times
//TODO: add fill_shape behaviour
void nxtCanvas::EllipseOut(int X, int Y, unsigned int radius_x, unsigned int radius_y, nxtCopyOptions* options, bool clear){
	if( clear )
		apply_clear( options );
	
//	options->merge = 3; //Debug
//	radius_y *= 2;
	
	//Find the point (P) where the tangent is at a 45 deg angle
	int Px = 0;
	int Py = 0;
	if( radius_x && radius_y ){
		if( radius_x > radius_y ){
			Py = radius_y / 2;
			double angle = asin( (double)Py/radius_y );
			Px = radius_x * cos( angle ) + 0.5;
		}
		else if( radius_x < radius_y ){
			Px = radius_x / 2;
			double angle = acos( (double)Px/radius_x );
			Py = radius_y * sin( angle ) + 0.5;
		}
		else{
			//TODO: this is properly incorrect...
			int Px2 = radius_x * cos( (double)3.141592654/4 ) + 0.5;
			int Py2 = radius_y * sin( (double)3.141592654/4 ) + 0.5;
			
			//filter it by using it again, after integer rounding
			Px = radius_x * cos( asin( (double)Py2/radius_y ) ) + 0.5;
			Py = radius_y * sin( acos( (double)Px2/radius_x ) ) + 0.5;
		}
	}
	else{
		if( radius_x )
			Px = radius_x/2;
		
		if( radius_y )
			Py = radius_y / 2;
	}
	
	cout << "Drawing ellipse: x=" << X << " y=" << Y << "\n";
	cout << "radius: x=" << radius_x << " y=" << radius_y << "\n";
	cout << "P: x=" << Px << " y=" << Py << "\n\n";
	
	
	//Draw ellipse path from P and down iterating with y-coordinates
	for( int i=Py; i>0; i-- ){ //This should be Py-1 to prevent overlap
		double angle = asin( (double)i/radius_y );
		int dx = radius_x * cos( angle ) + 0.5;
		
		PointOut( X+dx, Y+i, options, false );
		PointOut( X+dx, Y-i, options, false );
		PointOut( X-dx, Y+i, options, false );
		PointOut( X-dx, Y-i, options, false );
	}
	
	//Draw ellipse path from left to P iterating with x-coordinates
	for( int i=Px; i>0; i-- ){
		double angle = acos( (double)i/radius_x );
		int dy = radius_y * sin( angle ) + 0.5;
		
		PointOut( X+i, Y+dy, options, false );
		PointOut( X-i, Y+dy, options, false );
		PointOut( X+i, Y-dy, options, false );
		PointOut( X-i, Y-dy, options, false );
	}
	
	//Draw the outhermost points in the x-y axis
	PointOut( X+radius_x, Y, options, false );
	PointOut( X-radius_x, Y, options, false );
	PointOut( X, Y+radius_y, options, false );
	PointOut( X, Y-radius_y, options, false );
	//TODO: prevent this for radius = 0
	
//	PointOut( X+Px+1, Y+Py+1, options, false );	//DEBUG: The point P
	
	
}


void nxtCanvas::TextOut(int X, int Y, char* text, nxtCopyOptions* options, bool clear){
	if( clear )
		apply_clear( options );
	
	//TODO: Convert to text and use default ricfont
}


void nxtCanvas::NumberOut(int X, int Y, int value, nxtCopyOptions* options, bool clear){
	if( clear )
		apply_clear( options );
	
	//TODO: use default ricfont
}


void nxtCanvas::PolyOut(const pointArray* points, nxtCopyOptions* options, bool clear){
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
void nxtCanvas::copy_canvas( const nxtCanvas *source, unsigned int x, unsigned int y, unsigned int width, unsigned int height, int dest_x, int dest_y, nxtCopyOptions* options, bool clear ){
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
	for( unsigned int ix = 0; ix < end_width; ix++ )
		for( unsigned int iy = 0; iy < end_height; iy++ ){
			if( source->get_pixel( ix + start_x, iy + start_y ) )
				PointOut( dest_x + ix, dest_y + iy, options, false );
			else{
				options->invert_switch();
				PointOut( dest_x + ix, dest_y + iy, options, false );
				options->invert_switch();
			}
		}
}


