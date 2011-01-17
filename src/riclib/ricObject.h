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
		static const unsigned int RIC_OP_OPTIONS = 0;
		static const unsigned int RIC_OP_SPRITE = 1;
		static const unsigned int RIC_OP_VARMAP = 2;
		static const unsigned int RIC_OP_COPYBITS = 3;
		static const unsigned int RIC_OP_PIXEL = 4;
		static const unsigned int RIC_OP_LINE = 5;
		static const unsigned int RIC_OP_RECTANGLE = 6;
		static const unsigned int RIC_OP_CICLE = 7;
		static const unsigned int RIC_OP_NUMBER = 8;
		static const unsigned int RIC_OP_ELLIPSE = 9;
	
	
	protected:
		ricfile* pRIC;
		
		//Shared functions for writing to file
		void write_word(ofstream* file, unsigned int number) const{
			char data[2] = {number % 256, number / 256};
			file->write(data, 2);
		}
		void write_header(ofstream* file) const{
			write_word( file, filesize() );
			write_word( file, object_type() );
		}
	
	
	public:
		virtual void read(ifstream* file) = 0;
		virtual int write(ofstream* file) const = 0;
		virtual unsigned int filesize() const = 0;
		virtual unsigned int object_type() const = 0;
		virtual void draw(nxtCanvas* canvas) const{ return; }
		ricObject( ricfile *container ){ pRIC = container; }
		ricfile* parent() const{ return pRIC; }
};


class ricfile::ricOpOptions: public ricfile::ricObject{
	public:
		nxtVarWord options;
		nxtVarRicWord width;
		nxtVarRicWord height;
		
	public:
		unsigned int filesize() const{ return 8; }
		void read(ifstream* file);
		int write(ofstream* file) const;
		unsigned int object_type() const{ return RIC_OP_OPTIONS; }
		
		ricOpOptions( ricfile *container ): 
				ricfile::ricObject( container ), 
				width( this ), 
				height( this )
			{ ; }
};


class ricfile::ricOpSprite: public ricfile::ricObject{
	public:
		nxtVarWord sprite_ID;
		nxtVarWord rows;
		nxtVarWord columns;
	
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
		
		
	public:
		unsigned int filesize() const{ return 6 + 4 * VarMap.size(); }
		void read(ifstream* file);
		int write(ofstream* file) const;
		unsigned int object_type() const{ return RIC_OP_VARMAP; }
		
		ricOpVarMap( ricfile *container ): ricObject( container ){ }
		
		unsigned int value( unsigned char x ){ return VarMap.value( x ); }
		unsigned int get_ID() const{ return VarMapID; }
		
};

class ricfile::ricOpCopyBits: public ricfile::ricObject{
	public:
		nxtVarRicWord CopyOptions;
		nxtVarRicWord SpriteID;
		nxtVarRicWord posX;
		nxtVarRicWord posY;
		nxtVarRicWord width;
		nxtVarRicWord height;
		nxtVarRicWord relX;
		nxtVarRicWord relY;
		
	public:
		unsigned int filesize() const{ return 18; }
		void read(ifstream* file);
		int write(ofstream* file) const;
		unsigned int object_type() const{ return RIC_OP_COPYBITS; }
		void draw(nxtCanvas* canvas) const;
		
		ricOpCopyBits( ricfile *container ): 
				ricObject( container ), 
				CopyOptions( this ), 
				SpriteID( this ), 
				posX( this ), 
				posY( this ), 
				width( this ), 
				height( this ),
				relX( this ),
				relY( this )
			{ }
		
};



class ricfile::ricOpPixel: public ricfile::ricObject{
	public:
		nxtVarRicWord CopyOptions;
		nxtVarRicWord posX;
		nxtVarRicWord posY;
		nxtVarRicWord value;
		
	public:
		unsigned int filesize() const{ return 10; }
		void read(ifstream* file);
		int write(ofstream* file) const;
		unsigned int object_type() const{ return RIC_OP_PIXEL; }
		void draw(nxtCanvas* canvas) const;
		
		ricOpPixel( ricfile *container ): 
				ricObject( container ), 
				CopyOptions( this ), 
				posX( this ), 
				posY( this ), 
				value( this )
			{ }
		
};


class ricfile::ricOpLine: public ricfile::ricObject{
	public:
		nxtVarRicWord CopyOptions;
		nxtVarRicWord startX;
		nxtVarRicWord startY;
		nxtVarRicWord endX;
		nxtVarRicWord endY;
		
	public:
		unsigned int filesize() const{ return 12; }
		void read(ifstream* file);
		int write(ofstream* file) const;
		unsigned int object_type() const{ return RIC_OP_LINE; }
		void draw(nxtCanvas* canvas) const;
		
		ricOpLine( ricfile *container ): 
				ricObject( container ), 
				CopyOptions( this ), 
				startX( this ), 
				startY( this ), 
				endX( this ), 
				endY( this )
			{ }
		
};


class ricfile::ricOpRectangle: public ricfile::ricObject{
	public:
		nxtVarRicWord CopyOptions;
		nxtVarRicWord posX;
		nxtVarRicWord posY;
		nxtVarRicWord width;
		nxtVarRicWord height;
		
	public:
		unsigned int filesize() const{ return 12; }
		void read(ifstream* file);
		int write(ofstream* file) const;
		unsigned int object_type() const{ return RIC_OP_RECTANGLE; }
		void draw(nxtCanvas* canvas) const;
		
		ricOpRectangle( ricfile *container ): 
				ricObject( container ), 
				CopyOptions( this ), 
				posX( this ), 
				posY( this ), 
				width( this ), 
				height( this )
			{ }
		
};


class ricfile::ricOpCicle: public ricfile::ricObject{
	public:
		nxtVarRicWord CopyOptions;
		nxtVarRicWord posX;
		nxtVarRicWord posY;
		nxtVarRicWord radius;
		
	public:
		unsigned int filesize() const{ return 10; }
		void read(ifstream* file);
		int write(ofstream* file) const;
		unsigned int object_type() const{ return RIC_OP_CICLE; }
		void draw(nxtCanvas* canvas) const;
		
		ricOpCicle( ricfile *container ): 
				ricObject( container ), 
				CopyOptions( this ), 
				posX( this ), 
				posY( this ), 
				radius( this )
			{ }
		
};


class ricfile::ricOpNumber: public ricfile::ricObject{
	public:
		nxtVarRicWord CopyOptions;
		nxtVarRicWord posX;
		nxtVarRicWord posY;
		nxtVarRicWord number;
		
	public:
		unsigned int filesize() const{ return 10; }
		void read(ifstream* file);
		int write(ofstream* file) const;
		unsigned int object_type() const{ return RIC_OP_NUMBER; }
		void draw(nxtCanvas* canvas) const;
		
		ricOpNumber( ricfile *container ): 
				ricObject( container ), 
				CopyOptions( this ), 
				posX( this ), 
				posY( this ), 
				number( this )
			{ }
		
};


class ricfile::ricOpEllipse: public ricfile::ricObject{
	public:
		nxtVarRicWord CopyOptions;
		nxtVarRicWord posX;
		nxtVarRicWord posY;
		nxtVarRicWord radius_x;
		nxtVarRicWord radius_y;
		
	public:
		unsigned int filesize() const{ return 12; }
		void read(ifstream* file);
		int write(ofstream* file) const;
		unsigned int object_type() const{ return RIC_OP_ELLIPSE; }
		void draw(nxtCanvas* canvas) const;
		
		ricOpEllipse( ricfile *container ): 
				ricObject( container ), 
				CopyOptions( this ), 
				posX( this ), 
				posY( this ), 
				radius_x( this ),
				radius_y( this )
			{ }
		
};


#endif

