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

#include "nxtVariable.h"

class pointArray;


class nxtCanvas{
	private:
		unsigned int width;
		unsigned int height;
		bool *map;
		
	public:
		nxtCanvas(){
			width = 0;
			height = 0;
			map = 0;
		}
		
		void create(unsigned int width, unsigned int height){
			this->width = width;
			this->height = height;
			
			if(map)
				delete[] map;
			map = new bool [width*height];
			ClearScreen();
		}
		
		bool get_pixel(unsigned int X, unsigned int Y);
		void set_pixel(unsigned int X, unsigned int Y, bool color=true);
		
		~nxtCanvas(){
			if(map)
				delete[] map;
		}
		
		
	private:
		void PlotLineX(int startX, int startY, int endX, int endY, ricfile::nxtVarRicCopyoptions* options = 0);
		void PlotLineY(int startX, int startY, int endX, int endY, ricfile::nxtVarRicCopyoptions* options = 0);
		void apply_clear( ricfile::nxtVarRicCopyoptions* options = 0);
		
	public:
		void ClearScreen(){
			for( unsigned int ix=0; ix<width; ix++)
				for( unsigned int iy=0; iy<height; iy++)
					set_pixel( ix, iy, false );
		}
		void PointOut(unsigned int X, unsigned int Y, ricfile::nxtVarRicCopyoptions* options = 0, bool clear = true);
		void LineOut(int startX, int startY, int endX, int endY, ricfile::nxtVarRicCopyoptions* options = 0, bool clear = true);
		void RectOut(int X, int Y, int width, int height, ricfile::nxtVarRicCopyoptions* options = 0, bool clear = true);
		void EllipseOut(int X, int Y, unsigned int radius_x, unsigned int radius_y, ricfile::nxtVarRicCopyoptions* options = 0, bool clear = true);
		void CircleOut(int X, int Y, unsigned int radius, ricfile::nxtVarRicCopyoptions* options = 0, bool clear = true){
			EllipseOut( X, Y, radius, radius, options, clear );
		}
		void PolyOut(const pointArray* points, ricfile::nxtVarRicCopyoptions* options = 0, bool clear = true);
		void NumberOut(int X, int Y, int value, ricfile::nxtVarRicCopyoptions* options = 0, bool clear = true);
		void TextOut(int X, int Y, char* text, ricfile::nxtVarRicCopyoptions* options = 0, bool clear = true);
};

#endif

