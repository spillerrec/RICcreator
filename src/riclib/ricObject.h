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
	ricObject (abstract) class and child classes
	
	ricObject is an abstract base class for RIC file opcodes.
	
	The following methods must be implemented:
		read(...)	Read the opcode contents from a byte array
		write(...)	Write the opcode to an open file
		filesize(...)	return the size of the opcode when written to a file
		object_type(...)	return the opcode ID
		draw(...)	draw the opcode to a nxtCanvas
*/
#ifndef RICOBJECT_H
#define RICOBJECT_H

#include "nxtVariable.h"
#include "ricfile.h"
#include "pointArray.h"
#include "nxtCanvas.h"

#include <iostream>
#include <fstream>
using namespace std;


class ricfile::ricObject{
	//Type constants
	public:
		static const unsigned int RIC_OP_OPTIONS	= 0;
		static const unsigned int RIC_OP_SPRITE	= 1;
		static const unsigned int RIC_OP_VARMAP	= 2;
		static const unsigned int RIC_OP_COPYBITS	= 3;
		static const unsigned int RIC_OP_PIXEL	= 4;
		static const unsigned int RIC_OP_LINE	= 5;
		static const unsigned int RIC_OP_RECTANGLE	= 6;
		static const unsigned int RIC_OP_CICLE	= 7;
		static const unsigned int RIC_OP_NUMBER	= 8;
		static const unsigned int RIC_OP_ELLIPSE	= 9;
		static const unsigned int RIC_OP_POLYGON	= 10;
	
	
	protected:
		ricfile* pRIC;
		
		//Shared functions
		unsigned int object_size( unsigned int words_amount ) const{
			return 2 + 2 * words_amount;
		}
		void write_header(ofstream* file) const{
			nxtVariable::write_multibyte( file, filesize(), 2 );
			nxtVariable::write_multibyte( file, object_type(), 2 );
		}
	
	
	public:
		virtual void read(ifstream* file) = 0;
		virtual int write(ofstream* file) const = 0;
		virtual unsigned int filesize() const = 0;
		virtual unsigned int object_type() const = 0;
		virtual void draw(nxtCanvas* canvas){ return; }
		ricObject( ricfile *container ){ pRIC = container; }
		ricfile* parent() const{ return pRIC; }
		
		virtual nxtVariable& get_setting( unsigned int index ) = 0;
		virtual unsigned int setting_amount() const = 0;
};


class ricfile::ricOpOptions: public ricfile::ricObject{
	public:
		nxtVarWord options;
		nxtVarRicWord width;
		nxtVarRicWord height;
		
		
		nxtVariable& get_setting( unsigned int index ){
			switch( index ){
				case 0: return options;
				case 1: return width;
				case 2: return height;
			}
			return options; //TODO: find a solution so NULL can be passed
		}
		unsigned int setting_amount() const{ return 3; }
		
	public:
		unsigned int filesize() const{ return object_size( 3 ); }
		void read(ifstream* file);
		int write(ofstream* file) const;
		unsigned int object_type() const{ return RIC_OP_OPTIONS; }
		
		ricOpOptions( ricfile *container ): 
				ricfile::ricObject( container ), 
				width( this ), 
				height( this )
			{ ; }
		
		bool is_ricfont() const{
			if( options == 32769 )
				return true;
			else
				return false;
		}
		void set_ricfont(){ options = 32769; }
};


class ricfile::ricOpSprite: public ricfile::ricObject{
	public:
		nxtVarWord sprite_ID;
		nxtCanvas sprite_data;
		
		
		nxtVariable& get_setting( unsigned int index ){
			switch( index ){
				case 0: return sprite_ID;
			}
			return sprite_ID;
		}
		unsigned int setting_amount() const{ return 1; }
	
	private:
		unsigned int get_columns() const{
			unsigned int columns = sprite_data.get_width();
			if( columns % 8 )
				columns = columns / 8 + 1;
			else
				columns = columns / 8;
			
			return columns;
		}
		
	public:
		unsigned int filesize() const{ return 8 + sprite_data.get_height() * get_columns() + (sprite_data.get_height() * get_columns()) % 2; } //padding
		void read(ifstream* file);
		int write(ofstream* file) const;
		unsigned int object_type() const{ return RIC_OP_SPRITE; }
		
		ricOpSprite( ricfile *container ): ricObject( container ){
			sprite_data.create( 100, 64 );
		}
		
		unsigned int get_ID() const{ return sprite_ID; }
};


class ricfile::ricOpVarMap: public ricfile::ricObject{
	public:
		nxtVarWord VarMapID;
		pointArray VarMap;
		
		
		nxtVariable& get_setting( unsigned int index ){
			switch( index ){
				case 0: return VarMapID;
				case 1: return VarMap;
			}
			return VarMapID;
		}
		unsigned int setting_amount() const{ return 2; }
		
		
	public:
		unsigned int filesize() const{ return object_size( 1 ) + VarMap.filesize(); }
		void read(ifstream* file);
		int write(ofstream* file) const;
		unsigned int object_type() const{ return RIC_OP_VARMAP; }
		
		ricOpVarMap( ricfile *container ):
				ricObject( container ),
				VarMap( true )
			{ }
		
		unsigned int value( unsigned char x ){ return VarMap.value( x ); }
		unsigned int get_ID() const{ return VarMapID; }
		
};

class ricfile::ricOpCopyBits: public ricfile::ricObject{
	public:
		nxtVarRicCopyoptions CopyOptions;
		nxtVarRicWord SpriteID;
		ricvarRect src;
		ricvarPoint dest;
		
		
		nxtVariable& get_setting( unsigned int index ){
			switch( index ){
				case 0: return CopyOptions;
				case 1: return SpriteID;
				case 2: return src.pos.X;
				case 3: return src.pos.Y;
				case 4: return src.width;
				case 5: return src.height;
				case 6: return dest.X;
				case 7: return dest.Y;
			}
			return CopyOptions;
		}
		unsigned int setting_amount() const{ return 8; }
		
	public:
		unsigned int filesize() const{ return object_size( 8 ); }
		void read(ifstream* file);
		int write(ofstream* file) const;
		unsigned int object_type() const{ return RIC_OP_COPYBITS; }
		void draw(nxtCanvas* canvas);
		
		ricOpCopyBits( ricfile *container ): 
				ricObject( container ), 
				CopyOptions( this ), 
				SpriteID( this ), 
				src( this ), 
				dest( this )
			{ }
		
};



class ricfile::ricOpPixel: public ricfile::ricObject{
	public:
		nxtVarRicCopyoptions CopyOptions;
		ricvarPoint pos;
		nxtVarRicWord value;
		
		
		nxtVariable& get_setting( unsigned int index ){
			switch( index ){
				case 0: return CopyOptions;
				case 1: return pos.X;
				case 2: return pos.Y;
				case 3: return value;
			}
			return CopyOptions;
		}
		unsigned int setting_amount() const{ return 4; }
		
	public:
		unsigned int filesize() const{ return object_size( 4 ); }
		void read(ifstream* file);
		int write(ofstream* file) const;
		unsigned int object_type() const{ return RIC_OP_PIXEL; }
		void draw(nxtCanvas* canvas);
		
		ricOpPixel( ricfile *container ): 
				ricObject( container ), 
				CopyOptions( this ), 
				pos( this ), 
				value( this )
			{
			pos.X.set_normal( 10 );
			pos.Y.set_normal( 10 );
		}
		
};


class ricfile::ricOpLine: public ricfile::ricObject{
	public:
		nxtVarRicCopyoptions CopyOptions;
		ricvarPoint start;
		ricvarPoint end;
		
		
		nxtVariable& get_setting( unsigned int index ){
			switch( index ){
				case 0: return CopyOptions;
				case 1: return start.X;
				case 2: return start.Y;
				case 3: return end.X;
				case 4: return end.Y;
			}
			return CopyOptions;
		}
		unsigned int setting_amount() const{ return 5; }
		
	public:
		unsigned int filesize() const{ return object_size( 5 ); }
		void read(ifstream* file);
		int write(ofstream* file) const;
		unsigned int object_type() const{ return RIC_OP_LINE; }
		void draw(nxtCanvas* canvas);
		
		ricOpLine( ricfile *container ): 
				ricObject( container ), 
				CopyOptions( this ), 
				start( this ),
				end( this )
			{
			start.X.set_normal( 10 );
			start.Y.set_normal( 10 );
			end.X.set_normal( 20 );
			end.Y.set_normal( 20 );
		}
		
};


class ricfile::ricOpRectangle: public ricfile::ricObject{
	public:
		nxtVarRicCopyoptions CopyOptions;
		ricvarRect rect;
		
		
		nxtVariable& get_setting( unsigned int index ){
			switch( index ){
				case 0: return CopyOptions;
				case 1: return rect.pos.X;
				case 2: return rect.pos.Y;
				case 3: return rect.width;
				case 4: return rect.height;
			}
			return CopyOptions;
		}
		unsigned int setting_amount() const{ return 5; }
		
	public:
		unsigned int filesize() const{ return object_size( 5 ); }
		void read(ifstream* file);
		int write(ofstream* file) const;
		unsigned int object_type() const{ return RIC_OP_RECTANGLE; }
		void draw(nxtCanvas* canvas);
		
		ricOpRectangle( ricfile *container ): 
				ricObject( container ),
				CopyOptions( this ),
				rect( this )
			{
			rect.pos.X.set_normal( 10 );
			rect.pos.Y.set_normal( 10 );
			rect.width.set_normal( 10 );
			rect.height.set_normal( 5 );
		}
		
};


class ricfile::ricOpCicle: public ricfile::ricObject{
	public:
		nxtVarRicCopyoptions CopyOptions;
		ricvarPoint pos;
		nxtVarRicWord radius;
		
		
		nxtVariable& get_setting( unsigned int index ){
			switch( index ){
				case 0: return CopyOptions;
				case 1: return pos.X;
				case 2: return pos.Y;
				case 3: return radius;
			}
			return CopyOptions;
		}
		unsigned int setting_amount() const{ return 4; }
		
	public:
		unsigned int filesize() const{ return object_size( 4 ); }
		void read(ifstream* file);
		int write(ofstream* file) const;
		unsigned int object_type() const{ return RIC_OP_CICLE; }
		void draw(nxtCanvas* canvas);
		
		ricOpCicle( ricfile *container ): 
				ricObject( container ), 
				CopyOptions( this ),
				pos( this ),
				radius( this )
			{
			pos.X.set_normal( 25 );
			pos.Y.set_normal( 25 );
			radius.set_normal( 7 );
		}
		
};


class ricfile::ricOpNumber: public ricfile::ricObject{
	public:
		nxtVarRicCopyoptions CopyOptions;
		ricvarPoint pos;
		nxtVarRicWord number;
		
		
		nxtVariable& get_setting( unsigned int index ){
			switch( index ){
				case 0: return CopyOptions;
				case 1: return pos.X;
				case 2: return pos.Y;
				case 3: return number;
			}
			return CopyOptions;
		}
		unsigned int setting_amount() const{ return 4; }
		
	public:
		unsigned int filesize() const{ return object_size( 4 ); }
		void read(ifstream* file);
		int write(ofstream* file) const;
		unsigned int object_type() const{ return RIC_OP_NUMBER; }
		void draw(nxtCanvas* canvas);
		
		ricOpNumber( ricfile *container ): 
				ricObject( container ),
				CopyOptions( this ),
				pos( this ),
				number( this )
			{ }
		
};


class ricfile::ricOpEllipse: public ricfile::ricObject{
	public:
		nxtVarRicCopyoptions CopyOptions;
		ricvarPoint pos;
		nxtVarRicWord radius_x;
		nxtVarRicWord radius_y;
		
		
		nxtVariable& get_setting( unsigned int index ){
			switch( index ){
				case 0: return CopyOptions;
				case 1: return pos.X;
				case 2: return pos.Y;
				case 3: return radius_x;
				case 4: return radius_y;
			}
			return CopyOptions;
		}
		unsigned int setting_amount() const{ return 5; }
		
	public:
		unsigned int filesize() const{ return object_size( 5 ); }
		void read(ifstream* file);
		int write(ofstream* file) const;
		unsigned int object_type() const{ return RIC_OP_ELLIPSE; }
		void draw(nxtCanvas* canvas);
		
		ricOpEllipse( ricfile *container ): 
				ricObject( container ), 
				CopyOptions( this ), 
				pos( this ), 
				radius_x( this ),
				radius_y( this )
			{
			pos.X.set_normal( 25 );
			pos.Y.set_normal( 25 );
			radius_x.set_normal( 14 );
			radius_y.set_normal( 7 );
		}
		
};


class ricfile::ricOpPolygon: public ricfile::ricObject{
	public:
		nxtVarRicCopyoptions CopyOptions;
		pointArray points;
		
		
		nxtVariable& get_setting( unsigned int index ){
			switch( index ){
				case 0: return CopyOptions;
				case 1: return points;
			}
			return CopyOptions;
		}
		unsigned int setting_amount() const{ return 2; }
	
	public:
		unsigned int filesize() const{ return object_size( 1 ) + points.filesize(); }
		void read(ifstream* file);
		int write(ofstream* file) const;
		unsigned int object_type() const{ return RIC_OP_POLYGON; }
		void draw(nxtCanvas* canvas);
		
		ricOpPolygon( ricfile *container ):
				ricObject( container ),
				CopyOptions( this )
			{ }
};


#endif

