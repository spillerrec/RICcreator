#include "nxtCanvas.h"
#include <iostream>

ricPixel* nxtCanvas::GetPixel(unsigned int X, unsigned int Y){
	if( (X >= width) || (Y >= height) )
		return 0;
	
	return &map[ X + Y*width ];
}

void nxtCanvas::OutputCanvas(){
	for(int iy = height; iy; iy--){
		for(unsigned int ix = 0; ix < width; ix++){
			if( GetPixel(ix, iy-1)->GetColor() == PIXEL_COLOR_BLACK )
				std::cout << "XX";
			else
				std::cout << "  ";
		}
		std::cout << "|\n";
	}
}

void nxtCanvas::PointOut(unsigned int X, unsigned int Y){
	ricPixel* point = GetPixel( X, Y );
	
	if(point)
		point->Merge(&brush);
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


void nxtCanvas::PlotLineY(int startX, int startY, int endX, int endY){
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
			PointOut( startX, i );
		else
			PointOut( (unsigned int)(FunctionY(i, startX, startY, endX, endY)+0.5), i );
	}
}


void nxtCanvas::PlotLineX(int startX, int startY, int endX, int endY){
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


void nxtCanvas::LineOut(int startX, int startY, int endX, int endY){
	//TODO: test this throughroughly!
	
	//Determine if the line should be draw for each X value, or each Y value
	if( abs(endY - startY) > abs(endX - startX) )
		PlotLineY( startX, startY, endX, endY );
	else
		PlotLineX( startX, startY, endX, endY );
}


void nxtCanvas::RectOut(int X, int Y, int width, int height){
	//Draw the two horizontal lines
	LineOut( X,Y, X+width,Y );
	LineOut( X,Y+height, X+width,Y+height );
	
	//Draw the two vertical lines
	LineOut( X,Y, X,Y+height );
	LineOut( X+width,Y, X+width,Y+height );
}


void nxtCanvas::SpriteOut(int X, int Y, char* image, int width, int height, ricPixel* front, ricPixel* back){
	ricPixel* oldbrush = &brush;
	
	for(int iy=0; iy<height; iy++){
		for(int ix=0; ix<width; ix++){
			for(int iz=0; iz<8; iz++){
				if(image[iy*width + ix] & (128>>iz))
					brush.SetAll( front );
				else
					brush.SetAll( back );
				PointOut( X+ix*8+iz, Y+iy );
			}
		}
	}
	
	brush.SetAll( oldbrush );
}
