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


class copyoptions;


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
			for(unsigned int i=0; i<width*height; i++)
				map[i] = false;
		}
		
		bool get_pixel(unsigned int X, unsigned int Y);
		void set_pixel(unsigned int X, unsigned int Y, bool color=true);
		
		~nxtCanvas(){
			if(map)
				delete[] map;
		}
		
		
	private:
		void PlotLineX(int startX, int startY, int endX, int endY, copyoptions* options = 0);
		void PlotLineY(int startX, int startY, int endX, int endY, copyoptions* options = 0);
		void apply_clear( copyoptions* options = 0);
		
	public:
		void Clear(){
			for(unsigned int i=0; i<width*height; i++)
				map[i] = false;
		}
		void PointOut(unsigned int X, unsigned int Y, copyoptions* options = 0, bool clear = true);
		void LineOut(int startX, int startY, int endX, int endY, copyoptions* options = 0, bool clear = true);
		void RectOut(int X, int Y, int width, int height, copyoptions* options = 0, bool clear = true);
		void EllipseOut(int X, int Y, unsigned int radius_x, unsigned int radius_y, copyoptions* options = 0, bool clear = true);
		void CircleOut(int X, int Y, unsigned int radius, copyoptions* options = 0, bool clear = true){
			EllipseOut( X, Y, radius, radius, options, clear );
		}
		void NumberOut(int X, int Y, int value, copyoptions* options = 0, bool clear = true);
	//	void TextOut(int X, int Y, int value, copyoptions* options = 0, bool clear = true);
};

#endif

