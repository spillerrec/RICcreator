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
		virtual void draw(nxtCanvas* canvas) const{ return; }
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
		nxtVarWord rows;
		nxtVarWord columns;
		
		
		nxtVariable& get_setting( unsigned int index ){
			switch( index ){
				case 0: return sprite_ID;
			}
			return sprite_ID;
		}
		unsigned int setting_amount() const{ return 1; }
	
	private:
		char* image;
		
	public:
		unsigned int filesize() const{ return 8 + rows * columns + (rows * columns) % 2; } //padding
		void read(ifstream* file);
		int write(ofstream* file) const;
		unsigned int object_type() const{ return RIC_OP_SPRITE; }
		
		ricOpSprite( ricfile *container ): ricObject( container ){
			image = NULL;
		}
		
		unsigned int get_ID() const{ return sprite_ID; }
		
		int pixel( unsigned int x, unsigned int y ){
			if( columns*8 <= x )
				return 0;
			if( rows <= y )
				return 0;
			
			if( (image[ (rows-y-1)*columns + x/8] & (128>>x%8)) )
				return 1;
			else
				return 0;
		}
		
		
		//TODO: add destructor!
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
		nxtVarRicWord CopyOptions;
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
		void draw(nxtCanvas* canvas) const;
		
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
		nxtVarRicWord CopyOptions;
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
		void draw(nxtCanvas* canvas) const;
		
		ricOpPixel( ricfile *container ): 
				ricObject( container ), 
				CopyOptions( this ), 
				pos( this ), 
				value( this )
			{ }
		
};


class ricfile::ricOpLine: public ricfile::ricObject{
	public:
		nxtVarRicWord CopyOptions;
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
		void draw(nxtCanvas* canvas) const;
		
		ricOpLine( ricfile *container ): 
				ricObject( container ), 
				CopyOptions( this ), 
				start( this ),
				end( this )
			{ }
		
};


class ricfile::ricOpRectangle: public ricfile::ricObject{
	public:
		nxtVarRicWord CopyOptions;
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
		void draw(nxtCanvas* canvas) const;
		
		ricOpRectangle( ricfile *container ): 
				ricObject( container ),
				CopyOptions( this ),
				rect( this )
			{ }
		
};


class ricfile::ricOpCicle: public ricfile::ricObject{
	public:
		nxtVarRicWord CopyOptions;
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
		void draw(nxtCanvas* canvas) const;
		
		ricOpCicle( ricfile *container ): 
				ricObject( container ), 
				CopyOptions( this ),
				pos( this ),
				radius( this )
			{ }
		
};


class ricfile::ricOpNumber: public ricfile::ricObject{
	public:
		nxtVarRicWord CopyOptions;
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
		void draw(nxtCanvas* canvas) const;
		
		ricOpNumber( ricfile *container ): 
				ricObject( container ),
				CopyOptions( this ),
				pos( this ),
				number( this )
			{ }
		
};


class ricfile::ricOpEllipse: public ricfile::ricObject{
	public:
		nxtVarRicWord CopyOptions;
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
		void draw(nxtCanvas* canvas) const;
		
		ricOpEllipse( ricfile *container ): 
				ricObject( container ), 
				CopyOptions( this ), 
				pos( this ), 
				radius_x( this ),
				radius_y( this )
			{ }
		
};


class ricfile::ricOpPolygon: public ricfile::ricObject{
	public:
		nxtVarRicWord CopyOptions;
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
		void draw(nxtCanvas* canvas) const;
		
		ricOpPolygon( ricfile *container ):
				ricObject( container ),
				CopyOptions( this )
			{ }
};


#endif

