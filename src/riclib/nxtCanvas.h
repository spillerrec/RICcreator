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

class nxtCopyOptions;

class pointArray;
class ricfile;


class nxtCanvas{
	private:
		unsigned int width;
		unsigned int height;
		bool *map;
		bool auto_resize;
		int deltaX;
		int deltaY;
		
		//Variables dealing with redraws
		bool size_changed;
		
	public:
		nxtCanvas(){
			width = 0;
			height = 0;
			map = 0;
			auto_resize = false;
			size_changed = false;
		}
		
		bool size_affected(){ return size_changed; }
		void reset_affected(){ size_changed = false; }
		
		void create(unsigned int width, unsigned int height){
			this->width = width;
			this->height = height;
			
			if(map)
				delete[] map;
			map = new bool [width*height];
			ClearScreen();
			
			size_changed = true;
		}
		void resize( unsigned int width, unsigned int height );
		
		bool get_pixel(unsigned int X, unsigned int Y) const;
		void set_pixel(unsigned int X, unsigned int Y, bool color=true);
		
		unsigned int get_width() const{ return width; }
		unsigned int get_height() const{ return height; }
		
		~nxtCanvas(){
			if(map)
				delete[] map;
		}
		
		void copy_to( nxtCanvas *destination ) const;
		
	private:
		void PlotLineX(int startX, int startY, int endX, int endY, const nxtCopyOptions* options = 0);
		void PlotLineY(int startX, int startY, int endX, int endY, const nxtCopyOptions* options = 0);
		void apply_clear( const nxtCopyOptions* options = 0);
		bool affected_area( int startX, int startY, int endX, int endY );
		
	public:
		void set_auto_resize( bool setting ){ auto_resize = setting; }
		
	public:
		void ClearScreen(){
			for( unsigned int ix=0; ix<width; ix++)
				for( unsigned int iy=0; iy<height; iy++)
					set_pixel( ix, iy, false );
		}
		void PointOut(unsigned int X, unsigned int Y, const nxtCopyOptions* options = 0, bool clear = true);
		void LineOut(int startX, int startY, int endX, int endY, const nxtCopyOptions* options = 0, bool clear = true);
		void RectOut(int X, int Y, int width, int height, const nxtCopyOptions* options = 0, bool clear = true);
		void EllipseOut(int X, int Y, unsigned int radius_x, unsigned int radius_y, const nxtCopyOptions* options = 0, bool clear = true);
		void CircleOut(int X, int Y, unsigned int radius, const nxtCopyOptions* options = 0, bool clear = true){
			EllipseOut( X, Y, radius, radius, options, clear );
		}
		void PolyOut(const pointArray* points, const nxtCopyOptions* options = 0, bool clear = true);
		void NumberOut(int X, int Y, int value, const nxtCopyOptions* options = 0, bool clear = true);
		void TextOut(int X, int Y, const char* text, const nxtCopyOptions* options = 0, bool clear = true);
		
		void copy_canvas( const nxtCanvas *source, unsigned int x, unsigned int y, unsigned int width, unsigned int height, int dest_x, int dest_y, const nxtCopyOptions* options = 0, bool clear = true );
		
		void FontTextOut( int X, int Y, ricfile* fontfile, const char* str, const nxtCopyOptions* options = 0, bool clear = true );
		void FontTextOut( int X, int Y, const char* filename, const char* str, const nxtCopyOptions* options = 0, bool clear = true );
};

#endif

