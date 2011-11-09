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

class nxtCopyOptionsBase;

class pointArray;
class ricfile;

#include "nxtVariable.h"


class nxtCanvas: public nxtVariable{
	//The sprite data
	private:
		bool *map;	//The sprite, saved as true/false values (true = black)
		unsigned int width;
		unsigned int height;
		unsigned int get_columns() const;	//Width when saved to file
		void set_pixel(unsigned int X, unsigned int Y, bool color=true);
	public:
		unsigned int get_width() const{ return width; }
		unsigned int get_height() const{ return height; }
		bool get_pixel(unsigned int X, unsigned int Y) const;
		bool* get_scanline( unsigned int Y ) const{
			if( map )
				return &map[ Y*width ];
			else
				return 0;
		}
		
		void create(unsigned int width, unsigned int height){
			this->width = width;
			this->height = height;
			
			if(map)
				delete[] map;
			map = new bool [width*height];
			ClearScreen();
			
			size_changed = true;
		}
		void resize( unsigned int width, unsigned int height );	//Change dimentions, but keep content
		
		
		
		
	//Variables dealing with redraws
	private:
		int draw_depth;
		bool auto_resize;
		bool size_changed;
		int offset_x;
		int offset_y;
		int deltaX;
		int deltaY;
	public:
		bool size_affected(){ return size_changed; }
		void reset_affected(){ size_changed = false; }
		int get_offset_x() const{ return offset_x; }
		int get_offset_y() const{ return offset_y; }
		void set_offset_x( int x ){ offset_x = x; }
		void set_offset_y( int y ){ offset_x = y; }
		
		
		
	public:
		nxtCanvas(){
			width = 0;
			height = 0;
			map = 0;
			auto_resize = false;
			size_changed = false;
			draw_depth = 0;
			offset_x = 0;
			offset_y = 0;
		}
		nxtCanvas( unsigned int width, unsigned int height ){
			nxtCanvas();
			//nxtCanvas() doesn't init?
			create( width, height );
		}
		
		unsigned int filesize() const;
		unsigned int var_type() const{ return TYPE_BITMAP; }
		nxtIO::LoaderError read( nxtIO* file );
		nxtIO::LoaderError write( nxtIO* file ) const;
		
		
		~nxtCanvas(){
			if(map)
				delete[] map;
		}
		
		void copy_to( nxtCanvas *destination ) const;
		
	private:
		void apply_clear( const nxtCopyOptionsBase* options = 0);
		bool affected_area( int startX, int startY, int endX, int endY );
		int line_x( int x1, int y1, int dx, int dy, int y ){
			return (int)( (double)( dx * ( y - y1 )) / (double)dy + x1 + 0.5 );
		}
		int line_y( int x1, int y1, int dx, int dy, int x ){
			return line_x( y1, x1, dy, dx, x );
		}
		
	public:
		void set_auto_resize( bool setting ){ auto_resize = setting; }
		
	public:
		void ClearScreen(){
			for( unsigned int ix=0; ix<width; ix++)
				for( unsigned int iy=0; iy<height; iy++)
					set_pixel( ix, iy, false );
		}
		void PointOut(unsigned int X, unsigned int Y, const nxtCopyOptionsBase* options = 0);
		void LineOut( int startX, int startY, int endX, int endY, const nxtCopyOptionsBase* options = 0 );
		void connected_line_out( int x0, int y0, int x1, int y1, int x2, int y2, const nxtCopyOptionsBase *options = 0 );
		void RectOut(int X, int Y, int width, int height, const nxtCopyOptionsBase* options = 0);
		void EllipseOut(int X, int Y, unsigned int radius_x, unsigned int radius_y, const nxtCopyOptionsBase* options = 0);
		void CircleOut(int X, int Y, unsigned int radius, const nxtCopyOptionsBase* options = 0){
			EllipseOut( X, Y, radius, radius, options );
		}
		void PolyOut(const pointArray* points, const nxtCopyOptionsBase* options = 0);
		void NumberOut(int X, int Y, int value, const nxtCopyOptionsBase* options = 0);
		void TextOut(int X, int Y, const char* text, const nxtCopyOptionsBase* options = 0);
		
		void copy_canvas( const nxtCanvas *source, int x, int y, unsigned int width, unsigned int height, int dest_x, int dest_y, const nxtCopyOptionsBase* options = 0 );
		
		void FontTextOut( int X, int Y, ricfile* fontfile, const char* str, const nxtCopyOptionsBase* options = 0 );
		void FontTextOut( int X, int Y, const char* filename, const char* str, const nxtCopyOptionsBase* options = 0 );
		
		//Unstandardd draw commands
		void bucket_fill( int X, int Y, const nxtCopyOptionsBase *options = 0 );
		void crop_to( int X, int Y, unsigned int width, unsigned int height );
		void autocrop( const nxtCopyOptionsBase *options = 0 );
};

#endif

