/*
	nxtCanvas class
	
	A nxtCanvas object provides a 2 dimentional drawing space.
	A point has a X and Y value and defines the position in the drawing space. A point at (0,0) is positioned at the lower-left corner of the canvas. A positive X cordinate will move the point to the right and a positive Y value will more the point to the top. TODO: correct definition!
	Each point has the properties of a ricPixel.
	
	The following drawing methods are supported:
		Point
		Line
		Rectangle
		Polygon*
		Circle
		Ellipse*
		Text
		Number
*/
#ifndef NXTCANVAS_H
#define NXTCANVAS_H

#include "ricPixel.h"


class nxtCanvas{
	private:
		unsigned int width;
		unsigned int height;
		ricPixel* map;
		
	public:
		ricPixel brush;
		
		nxtCanvas(){
			width = 0;
			height = 0;
			map = 0;
			brush.SetColor( PIXEL_COLOR_BLACK );
		}
		
		void create(unsigned int width, unsigned int height){
			this->width = width;
			this->height = height;
			
			if(map)
				delete[] map;
			map = new ricPixel [width*height];
		}
		
		ricPixel* GetPixel(unsigned int X, unsigned int Y);
		
		~nxtCanvas(){
			if(map)
				delete[] map;
		}
		
		void OutputCanvas();
		
	private:
		void PlotLineX(int startX, int startY, int endX, int endY);
		void PlotLineY(int startX, int startY, int endX, int endY);
		
	public:
		void PointOut(unsigned int X, unsigned int Y);
		void LineOut(int startX, int startY, int endX, int endY);
		void RectOut(int X, int Y, int width, int height);
		void SpriteOut(int X, int Y, char* image, int width, int height, ricPixel* front, ricPixel* back);
};

#endif