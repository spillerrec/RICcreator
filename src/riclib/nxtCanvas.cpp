#include "nxtCanvas.h"
#include <iostream>
#include <stdlib.h> //For abs()
#include <math.h>


#include "nxtVariable.h"

bool nxtCanvas::get_pixel(unsigned int X, unsigned int Y){
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

void nxtCanvas::apply_clear( copyoptions* options){
	if( options != 0 ){
		if( options->clear )
			ClearScreen();
		
	//	if( options->clear_except_status )
	//		clear_except_status();
	}
}


void nxtCanvas::PointOut(unsigned int X, unsigned int Y, copyoptions* options, bool clear){
	if( options == 0 ){
		set_pixel( X, Y );
		return;
	}
	
	if( clear )
		apply_clear( options );
	
	
	bool background = get_pixel( X, Y );
	bool foreground;
	if( options->invert )
		foreground = false;
	else
		foreground = true;
	
	
	switch( options->merge ){
		case copyoptions::MERGE_COPY:
				set_pixel( X, Y, foreground );
			break;
			
		case copyoptions::MERGE_AND:
				set_pixel( X, Y, foreground && background );
			break;
			
		case copyoptions::MERGE_OR:
				set_pixel( X, Y, foreground || background );
			break;
			
		case copyoptions::MERGE_XOR:
				if( (background && (!foreground)) || ((!background) && foreground) )
					set_pixel( X, Y, true );
				else
					set_pixel( X, Y, false );
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


void nxtCanvas::PlotLineY(int startX, int startY, int endX, int endY, copyoptions* options){
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
			PointOut( startX, i, options );
		else
			PointOut( (unsigned int)(FunctionY(i, startX, startY, endX, endY)+0.5), i, options, false );
	}
}


void nxtCanvas::PlotLineX(int startX, int startY, int endX, int endY, copyoptions* options){
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


void nxtCanvas::LineOut(int startX, int startY, int endX, int endY, copyoptions* options, bool clear){
	if( clear )
		apply_clear( options );
	
	//Determine if the line should be draw for each X value, or each Y value
	if( abs(endY - startY) > abs(endX - startX) )
		PlotLineY( startX, startY, endX, endY, options );
	else
		PlotLineX( startX, startY, endX, endY, options );
}


void nxtCanvas::RectOut(int X, int Y, int width, int height, copyoptions* options, bool clear){
	if( clear )
		apply_clear( options );
	
	//Draw the two horizontal lines
	LineOut( X,Y, X+width,Y, options, false );
	LineOut( X,Y+height, X+width,Y+height, options, false );
	
	//Draw the two vertical lines
	LineOut( X,Y, X,Y+height, options, false );
	LineOut( X+width,Y, X+width,Y+height, options, false );
}


//TODO: this still doesn't work properly as some pixels are drawn multiple times
//TODO: add fill_shape behaviour
void nxtCanvas::EllipseOut(int X, int Y, unsigned int radius_x, unsigned int radius_y, copyoptions* options, bool clear){
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


void nxtCanvas::TextOut(int X, int Y, char* text, copyoptions* options, bool clear){
	if( clear )
		apply_clear( options );
	
	//TODO: Convert to text and use default ricfont
}


void nxtCanvas::NumberOut(int X, int Y, int value, copyoptions* options, bool clear){
	if( clear )
		apply_clear( options );
	
	//TODO: use default ricfont
}


void nxtCanvas::PolyOut(const pointArray* points, copyoptions* options, bool clear){
	if( clear )
		apply_clear( options );
	
	//TODO: draw polygon
}


