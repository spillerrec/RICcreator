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


unsigned int nxtCanvas::get_columns() const{
	unsigned int columns;
	if( width % 8 )
		columns = width / 8 + 1;
	else
		columns = width / 8;
	
	return columns;
}
unsigned int nxtCanvas::filesize() const{
	return 4 + height * get_columns() + (height * get_columns()) % 2;
}
void nxtCanvas::read(ifstream* file){
	unsigned int rows = nxtVariable::read_multibyte( file, 2 );
	unsigned int columns = nxtVariable::read_multibyte( file, 2 );
	
	create( columns*8, rows );
	
	for( int irow=rows-1; irow >= 0; irow--)
		for( unsigned int icolumn=0; icolumn < columns; icolumn++){
			unsigned char raw = file->get();
			
			for( int ibyte=7; ibyte >= 0; ibyte--){
				set_pixel( icolumn*8+ibyte, irow, raw % 2 );
				raw /= 2;
			}
		}
	
	
	//Padding
	if( (rows * columns) % 2 )
		file->get();
}
void nxtCanvas::write(ofstream* file) const{
	
	//Find the sizes of the bitmap to write and save it to the file
	unsigned int rows = height;
	unsigned int columns = get_columns();
	nxtVariable::write_multibyte( file, rows, 2 );
	nxtVariable::write_multibyte( file, columns, 2 );
	
	//Write the bitmap
	char temp[1];
	for( int irow=rows-1; irow >= 0; irow--)
		for( unsigned int icolumn=0; icolumn < columns; icolumn++ ){
			temp[0] = 0;
			
			//Contruct a byte of sprite
			for( int ibyte=0; ibyte < 8; ibyte++){
				temp[0] *= 2;	//The bits need to be shiftet, placed in the start as the first iteration will have no effekt on this value
				
				if( get_pixel( icolumn*8+ibyte, irow ) )
					temp[0] += 1;
			}
			
			//Write the byte to the file
			file->write( temp, 1 );
		}
	
	
	//Write padding byte
	temp[0] = 0;
	if( (rows * columns) % 2 )
		file->write( temp, 1 );
}


void nxtCanvas::copy_to( nxtCanvas *destination ) const{
	//Low-level copy
	destination->width = width;
	destination->height = height;
	if( destination->map )
		delete[] destination->map;
	destination->map = new bool[width*height];
	memcpy( destination->map, map, (width*height)*sizeof( bool ) );
	
	//Other variables
	destination->auto_resize = auto_resize;
	destination->size_changed = size_changed;
}

void nxtCanvas::resize( unsigned int width, unsigned int height ){
	//Make a copy
	nxtCanvas temp;
	copy_to( &temp );
	//Reinit with a new size
	create( width, height );
	//Copy the copy onto this
	copy_canvas( &temp, 0,0, temp.get_width(), temp.get_height(), 0,0 );
}

bool nxtCanvas::get_pixel(unsigned int X, unsigned int Y) const{
	if( (X >= width) || (Y >= height) || ( map == 0) )
		return false;
	
	return map[ X + Y*width ];
}

void nxtCanvas::set_pixel(unsigned int X, unsigned int Y, bool color){
	if( (X >= width) || (Y >= height) || ( map == 0) )
		return;
	
	map[ X + Y*width ] = color;
}

inline void nxtCanvas::apply_clear( const nxtCopyOptionsBase* options){
	if( options ){
		if( options->get_clear() )
			ClearScreen();
		
	//	if( options->clear_except_status )
	//		clear_except_status();
	}
}



void order( int &small, int &big ){
	if( big < small ){
		int temp = small;
		small = big;
		big = temp;
	}
}

bool nxtCanvas::affected_area( int startX, int startY, int endX, int endY ){
	if( auto_resize ){
		order( startX, endX );
		order( startY, endY );
		
		//Find how much it starts before current canvas
		unsigned int negX = 0;
		unsigned int negY = 0;
		if( startX < 0 )
			negX -= startX;
		if( startY < 0 )
			negY -= startY;
		
		//Find how much it sticks out of current canvas
		unsigned int posX = 0;
		unsigned int posY = 0;
		if( endX >= (int)width )
			posX = endX - (width - 1);
		if( endY >= (int)height )
			posY = endY - (height - 1);
		
		//Only resize if it sticks out at all
		if( negX || negY || posX || posY ){
			unsigned int new_width = width + negX + posX;
			unsigned int new_height = height + negY + posY;
			
			//Create a copy
			nxtCanvas temp;
			copy_to( &temp );
			
			//Resize and copy the contents back
			create( new_width, new_height );
			copy_canvas( &temp, 0,0, temp.get_width(), temp.get_height(), negX, negY );
			
			//Make the drawing operations aware of the new offset
			deltaX = negX;
			deltaY = negY;
			
			size_changed = true;
			
			return true;
		}
	}
	
	return false;
}


void nxtCanvas::PointOut(unsigned int X, unsigned int Y, const nxtCopyOptionsBase* options){
	draw_depth++;
	if( draw_depth == 1 ){
		apply_clear( options );
		
		if( affected_area( X, Y, X, Y ) ){
			X += deltaX;
			Y += deltaY;
		}
	}
	
	//If there is no copyoptions, just draw and exit
	if( !options ){
		set_pixel( X, Y );
		draw_depth--;
		return;
	}
	
	bool foreground = !options->get_invert();	//Get color
	
	//Select merge mode
	switch( options->get_merge() ){
		case nxtCopyOptionsBase::MERGE_COPY:
				set_pixel( X, Y, foreground );
			break;
			
		case nxtCopyOptionsBase::MERGE_AND:
				set_pixel( X, Y, foreground && get_pixel( X, Y ) );
			break;
			
		case nxtCopyOptionsBase::MERGE_OR:
				set_pixel( X, Y, foreground || get_pixel( X, Y ) );
			break;
			
		case nxtCopyOptionsBase::MERGE_XOR:
				set_pixel( X, Y, get_pixel( X, Y ) ^ foreground );	//Warning, bitwise operation!
			break;
	}
	
	draw_depth--;
}


void nxtCanvas::LineOut( int startX, int startY, int endX, int endY, const nxtCopyOptionsBase* options, int offset ){
	draw_depth++;
	if( draw_depth == 1 ){
		apply_clear( options );
		
		if( affected_area( startX, startY, endX, endY ) ){
			startX += deltaX;
			startY += deltaY;
			endX += deltaX;
			endY += deltaY;
		}
	}
	
	
	//Start drawing
	if( startY == endY ){	//Horizontal line
		if( startX <= endX )
			for( int ix = startX+offset; ix <= endX; ix++ )
				PointOut( ix, startY, options );
		else
			for( int ix = endX; ix <= startX-offset; ix++ )
				PointOut( ix, startY, options );
	}
	else if( startX == endX ){	//Vertical line
		if( startY < endY )
			for( int iy = startY+offset; iy <= endY; iy++ )
				PointOut( startX, iy, options );
		else
			for( int iy = endY; iy <= startY-offset; iy++ )
				PointOut( startX, iy, options );
	}
	else{	//tilting line
		double a = (double)(endY - startY) / (endX - startX);
		double b = startY - a*startX;
		
		if( abs(endY - startY) > abs(endX - startX) ){
			//iterate over y values
			if( endY > startY )
				for( int iy = startY+offset; iy <= endY; iy++ )
					PointOut( (iy-b)/a + 0.5, iy, options );
			else
				for( int iy = endY; iy <= startY-offset; iy++ )
					PointOut( (iy-b)/a + 0.5, iy, options );
		}
		else{
			//iterate over x values
			b += 0.5; //We can add the rounding here instead of doing it each time
			if( endX > startX )
				for( int ix = startX+offset; ix <= endX; ix++ )
					PointOut( ix, a*ix + b, options );
			else
				for( int ix = endX; ix <= startX-offset; ix++ )
					PointOut( ix, a*ix + b, options );
		}
		
	}
	
	draw_depth--;
}


void nxtCanvas::RectOut(int X, int Y, int width, int height, const nxtCopyOptionsBase* options){
	draw_depth++;
	if( draw_depth == 1 ){
		apply_clear( options );
		
		if( affected_area( X, Y, X+width, Y+height ) ){
			X += deltaX;
			Y += deltaY;
		}
	}
	
	
	if( !( options && options->get_fill_shape() ) )	//If it is not a filled rectangle
		if( height > 1 && width > 1 ){
			
			LineOut( X, Y, X+width, Y, options );
			LineOut( X, Y+height, X+width, Y+height, options );
			
			LineOut( X, Y+1, X, Y+height-1, options );
			LineOut( X+width, Y+1, X+width, Y+height-1, options );
			
			draw_depth--;
			return;
		}
	
	//Draw a filled rectangle, either because of fill_shape or that it doesn't contain whitespace
	for( int ix = X; ix <= X+width; ix++ )
		for( int iy = Y; iy <= Y+height; iy++ )
			PointOut( ix, iy, options );
	
	draw_depth--;
}


//TODO: Try to reduce the amount of exceptions
void nxtCanvas::EllipseOut(int X, int Y, unsigned int radius_x, unsigned int radius_y, const nxtCopyOptionsBase* options){
	draw_depth++;
	if( draw_depth == 1 ){
		apply_clear( options );
		
		if( affected_area( X-radius_x, Y-radius_y, X+radius_x, Y+radius_y ) ){
			X += deltaX;
			Y += deltaY;
		}
	}
	
	
	
	if( radius_x == 0 ){
		if( radius_y == 0 )
			PointOut( X, Y, options );
		else
			LineOut( X, Y-radius_y, X, Y+radius_y, options );
	}
	else if( radius_y == 0 ){
		LineOut( X-radius_x, Y, X+radius_x, Y, options );
	}
	else{
		
		//Find the point (P) where the tangent is at a 45 deg angle
		double v = atan( (double)radius_y / radius_x );
		int Px = radius_x * cos( v ) + 0.5;
		int Py_start = radius_y * sin( acos( (double)Px/radius_x ) ) + 0.5;
		int Py;// = radius_y * sin( v ) + 0.5;
		
		//Draw ellipse path from left to P iterating with x-coordinates
		for( int i=0; i<=Px; i++ ){
			double angle = acos( (double)i/radius_x );
			Py = radius_y * sin( angle ) + 0.5;
			
			if( options && options->get_fill_shape() ){
				if( i ){
					LineOut( X+i, Y+Py, X+i, Y+Py_start, options );
					LineOut( X+i, Y-Py, X+i, Y-Py_start, options );
					LineOut( X-i, Y+Py, X-i, Y+Py_start, options );
					LineOut( X-i, Y-Py, X-i, Y-Py_start, options );
				}
				else{
					LineOut( X, Y+Py, X, Y+Py_start, options );
					LineOut( X, Y-Py, X, Y-Py_start, options );
				}
			}
			else{
				if( Py ){
					if( i ){
						PointOut( X+i, Y+Py, options );
						PointOut( X-i, Y+Py, options );
						PointOut( X+i, Y-Py, options );
						PointOut( X-i, Y-Py, options );
					}
					else{
						PointOut( X, Y+Py, options );
						PointOut( X, Y-Py, options );
					}
				}
				else{
					PointOut( X+i, Y, options );
					PointOut( X-i, Y, options );
				}
			}
		}
		
		
		//*Draw ellipse path from P and down iterating with y-coordinates
		for( int i=Py_start-1; i>=0; i-- ){ //This should be Py-1 to prevent overlap
			double angle = asin( (double)i/radius_y );
			int dx = radius_x * cos( angle ) + 0.5;
			
			if( options && options->get_fill_shape() ){
				if( i ){
					LineOut( X+dx, Y+i, X-dx, Y+i, options );
					LineOut( X+dx, Y-i, X-dx, Y-i, options );
				}
			}
			else{
				if( dx ){
					if( i ){
						PointOut( X+dx, Y+i, options );
						PointOut( X+dx, Y-i, options );
						PointOut( X-dx, Y+i, options );
						PointOut( X-dx, Y-i, options );
					}
					else{
						PointOut( X+dx, Y, options );
						PointOut( X-dx, Y, options );
					}
				}
				else{
					PointOut( X, Y+i, options );
					PointOut( X, Y-i, options );
				}
			}
		}
		
		if( options && options->get_fill_shape() ){
			LineOut( X+radius_x, Y, X-radius_x, Y, options );
		}
	}
	
	draw_depth--;
}


void nxtCanvas::TextOut(int X, int Y, const char* text, const nxtCopyOptionsBase* options){
	Y = Y/8*8;
	FontTextOut( X, Y, "font.ric", text, options );
}


void nxtCanvas::NumberOut(int X, int Y, int value, const nxtCopyOptionsBase* options){
	char text[20];
	sprintf( text, "%d", value );
	TextOut( X, Y, text, options );
}


//TODO: fill_shape
void nxtCanvas::PolyOut(const pointArray* points, const nxtCopyOptionsBase* options){
	draw_depth++;
	if( draw_depth == 1 ){
		apply_clear( options );
		
		//TODO: affected area
	}
	
	//Must be 3 points or more
	if( points->size() < 3 ){
		draw_depth--;
		return;
	}
	
	const point* first_point = points->index( 0 );
	const point* start_point = first_point;
	const point* end_point;
	for(unsigned int i=1; i<points->size(); i++){
		end_point = points->index( i );
		LineOut( start_point->X, start_point->Y, end_point->X, end_point->Y, options, 1 );
		start_point = end_point;
	}
	
	if( options && !options->get_polyline() )
		LineOut( end_point->X, end_point->Y, first_point->X, first_point->Y, options, 1 );
	else
		PointOut( first_point->X, first_point->Y, options );
	
	draw_depth--;
}



//TODO: test this throughoutly
void nxtCanvas::copy_canvas( const nxtCanvas *source, unsigned int x, unsigned int y, unsigned int width, unsigned int height, int dest_x, int dest_y, const nxtCopyOptionsBase* options ){
	draw_depth++;
	if( draw_depth == 1 ){
		apply_clear( options );
		
		//TODO: affected area
	}
	
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
	
	if( options && options->get_fill_shape() ){
		//Draw only in black
		RectOut( start_x, start_y, end_width-1, end_height-1, options );
	}
	else{
		//Copy the canvas
		nxtCopyOptions background;
		if( options )
			options->copy_to( &background );
		background.invert_switch();
		for( unsigned int ix = 0; ix < end_width; ix++ )
			for( unsigned int iy = 0; iy < end_height; iy++ ){
				if( source->get_pixel( ix + start_x, iy + start_y ) )
					PointOut( dest_x + ix, dest_y + iy, options );
				else{
					PointOut( dest_x + ix, dest_y + iy, &background );
				}
			}
	}
	
	draw_depth--;
}


#include "ricfile.h"
#include "ricObjectChildren.h"

//TODO: add parameters, and make sure it works like on the firmware.
void nxtCanvas::FontTextOut( int X, int Y, ricfile* fontfile, const char* str, const nxtCopyOptionsBase* options){
	draw_depth++;
	if( draw_depth == 1 ){
		apply_clear( options );
	}
	
	if( fontfile && str ){
		for( unsigned int i=0; i < fontfile->object_amount(); i++){
			ricObject* object = fontfile->get_object( i );	//Get object
			
			//Check if object is an options element
			if( object && object->object_type() == ricObject::RIC_OP_OPTIONS ){
				ricOpOptions* opt_element = (ricOpOptions*)object;
				
				//If it is a ricfont
				if( opt_element->is_ricfont() ){
					//Start getting the elements
					ricOpSprite* sprite = (ricOpSprite*)fontfile->object_at_ID( 1, ricObject::RIC_OP_SPRITE, opt_element );
					ricOpVarMap* pos_x = (ricOpVarMap*)fontfile->object_at_ID( 2, ricObject::RIC_OP_VARMAP, opt_element );
					ricOpVarMap* pos_y = (ricOpVarMap*)fontfile->object_at_ID( 3, ricObject::RIC_OP_VARMAP, opt_element );
					ricOpVarMap* width = (ricOpVarMap*)fontfile->object_at_ID( 4, ricObject::RIC_OP_VARMAP, opt_element );
					ricOpVarMap* height = (ricOpVarMap*)fontfile->object_at_ID( 5, ricObject::RIC_OP_VARMAP, opt_element );
					
					//Stop if one failed
					if( !pos_x || !pos_y || !width || !height )
						return;
					
					//Start Drawing
					int current_x = X, current_y = Y;
					nxtCanvas* characters = &sprite->sprite_data;
					for( unsigned int pos=0; str[pos]; pos++ ){
						unsigned int char_width = width->value( str[pos] );
						unsigned int char_height = height->value( str[pos] );
						copy_canvas( characters, pos_x->value(str[pos]), pos_y->value(str[pos]), char_width, char_height, current_x, current_y, options );
						current_x += char_width;
					}
				}
			}
		}
	}
	
	draw_depth--;
}

void nxtCanvas::FontTextOut( int X, int Y, const char* filename, const char* str, const nxtCopyOptionsBase* options){
	ricfile temp;
	temp.readfile( filename );
	FontTextOut( X, Y, &temp, str, options );
}

void nxtCanvas::bucket_fill( int X, int Y, const nxtCopyOptionsBase *options ){
	bool color = options && options->get_invert();
	if( get_pixel( X, Y ) == color )
		set_pixel( X, Y, !color );
	else
		return;
	
	bucket_fill( X-1, Y, options );
	bucket_fill( X, Y-1, options );
	
	//Make sure the last two don't go out of the canvas
	if( Y+1 < height )
		bucket_fill( X, Y+1, options );
	if( X+1 < width )
		bucket_fill( X+1, Y, options );
}

void nxtCanvas::crop_to( int X, int Y, unsigned int width, unsigned int height ){
	//Crop parameters to current canvas
	if( X < 0 ){
		width += X;
		X = 0;
	}
	if( Y < 0 ){
		height += Y;
		Y = 0;
	}
	if( X+width > this->width )
		width = this->width - X;
	if( Y+height > this->height )
		height = this->height - Y;
	
	//Crop canvas
	nxtCanvas temp;
	copy_to( &temp );
	create( width, height );
	copy_canvas( &temp, X,Y, width, height, 0,0 );
}

void nxtCanvas::autocrop( const nxtCopyOptionsBase *options ){
	bool color = options && options->get_invert();
	
	//Crop width and height
	int iwidth;
	for( iwidth=width-1; iwidth >= 0; iwidth-- ){
		for( unsigned int iy=0; iy<height; iy++ ){
			if( get_pixel( iwidth, iy ) != color )
				goto width_loop_finish;
		}
	}
	width_loop_finish:
	
	int iheight;
	for( iheight=height-1; iheight >= 0; iheight-- ){
		for( unsigned int ix=0; ix<width; ix++ ){
			if( get_pixel( ix, iheight ) != color )
				goto height_loop_finish;
		}
	}
	height_loop_finish:
	
	crop_to( 0,0, iwidth+1, iheight+1 );
	
	unsigned int X;
	for( X=0; X<width; X++ )
		for( unsigned int iy=0; iy<height; iy++ )
			if( get_pixel( X, iy ) != color )
				goto X_loop_finish;
	X_loop_finish:
	
	
	unsigned int Y;
	for( Y=0; Y<height; Y++ )
		for( unsigned int ix=0; ix<width; ix++ )
			if( get_pixel( ix, Y ) != color )
				goto Y_loop_finish;
	Y_loop_finish:
	
	crop_to( X, Y, width-X, height-Y );
	
	
}

