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



void nxtCanvas::PointOut(unsigned int X, unsigned int Y, copyoptions* options){
	if( options == 0 ){
		set_pixel( X, Y );
		return;
	}
	
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
			PointOut( (unsigned int)(FunctionY(i, startX, startY, endX, endY)+0.5), i, options );
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
		PointOut( i, (unsigned int)(FunctionX(i, startX, startY, endX, endY)+0.5) );
}


void nxtCanvas::LineOut(int startX, int startY, int endX, int endY, copyoptions* options){
	//TODO: test this throughroughly!
	
	//Determine if the line should be draw for each X value, or each Y value
	if( abs(endY - startY) > abs(endX - startX) )
		PlotLineY( startX, startY, endX, endY, options );
	else
		PlotLineX( startX, startY, endX, endY, options );
}


void nxtCanvas::RectOut(int X, int Y, int width, int height, copyoptions* options){
	//Draw the two horizontal lines
	LineOut( X,Y, X+width,Y, options );
	LineOut( X,Y+height, X+width,Y+height, options );
	
	//Draw the two vertical lines
	LineOut( X,Y, X,Y+height, options );
	LineOut( X+width,Y, X+width,Y+height, options );
}


void nxtCanvas::SpriteOut(int X, int Y, char* image, int width, int height, copyoptions* options){
	
	copyoptions front(options), back(options);
	if( front.invert )
		front.invert = false;
	else
		front.invert = true;
	
	for(int iy=0; iy<height; iy++){
		for(int ix=0; ix<width; ix++){
			for(int iz=0; iz<8; iz++){
				if(image[iy*width + ix] & (128>>iz))
					set_pixel( X+ix*8+iz, Y+iy, front.invert );
				else
					set_pixel( X+ix*8+iz, Y+iy, back.invert );
			}
		}
	}
}
