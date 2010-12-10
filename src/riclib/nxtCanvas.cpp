#include "nxtCanvas.h"
#include <iostream>
#include <stdlib.h> //For abs()


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
			Clear();
		
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


