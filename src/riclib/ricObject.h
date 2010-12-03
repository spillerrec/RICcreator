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

#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

inline unsigned int word(char* file, unsigned int &pos){
	pos += 2;
	return (unsigned char)file[pos-2] + (unsigned char)file[pos-1] * 256;
}

class nxtCanvas;

const unsigned int RIC_OP_OPTIONS = 0;
const unsigned int RIC_OP_SPRITE = 1;
const unsigned int RIC_OP_VARMAP = 2;
const unsigned int RIC_OP_COPYBITS = 3;
const unsigned int RIC_OP_PIXEL = 4;
const unsigned int RIC_OP_LINE = 5;
const unsigned int RIC_OP_RECTANGLE = 6;
const unsigned int RIC_OP_CICLE = 7;
const unsigned int RIC_OP_NUMBER = 8;




class ricObject{
	protected:
		void write_word(ofstream* file, unsigned int number){
			char data[2] = {number % 256, number / 256};
			file->write(data, 2);
		}
		void write_header(ofstream* file){
			write_word( file, filesize() );
			write_word( file, object_type() );
		}
		
	public:
		virtual void read(ifstream* file) = 0;
		virtual int write(ofstream* file) = 0;
		virtual unsigned int filesize() = 0;
		virtual unsigned int object_type() = 0;
		virtual void draw(nxtCanvas* canvas){ return; };
};


class ricOpOptions: public ricObject{
	private:
		nxtVarWord options;
		nxtVarRicWord width;
		nxtVarRicWord height;
		
	public:
		unsigned int filesize(){ return 8; }
		void read(ifstream* file);
		int write(ofstream* file);
		unsigned int object_type(){ return RIC_OP_OPTIONS; }
};


class ricOpSprite: public ricObject{
	private:
		nxtVarWord sprite_ID;
		nxtVarWord rows;
		nxtVarWord columns;
		char* image;
		
	public:
		unsigned int filesize(){ return 8 + rows * columns + (rows * columns) % 2; } /* padding? */
		void read(ifstream* file);
		int write(ofstream* file);
		unsigned int object_type(){ return RIC_OP_SPRITE; }
		
		ricOpSprite(){
			image = NULL;
		}
		
		
		
		//TODO: add destructor!
};


class ricOpVarMap: public ricObject{
	private:
		nxtVarWord VarMapID;
		nxtVarWord size;
		
		struct point{
			nxtVarWord X;
			nxtVarWord Y;
		};
		std::vector<point> VarMap;
		//TODO: add XY array here
		
	public:
		unsigned int filesize(){ return 6 + 4 * size; }
		void read(ifstream* file);
		int write(ofstream* file);
		unsigned int object_type(){ return RIC_OP_VARMAP; }
		
		
		//TODO: add destructor!
};

class ricOpCopyBits: public ricObject{
	private:
		nxtVarRicWord CopyOptions;
		nxtVarRicWord SpriteID;
		nxtVarRicWord posX;
		nxtVarRicWord posY;
		nxtVarRicWord width;
		nxtVarRicWord height;
		nxtVarRicWord relX;
		nxtVarRicWord relY;
		
	public:
		unsigned int filesize(){ return 18; }
		void read(ifstream* file);
		int write(ofstream* file);
		unsigned int object_type(){ return RIC_OP_COPYBITS; }
		void draw(nxtCanvas* canvas);
		
};



class ricOpPixel: public ricObject{
	private:
		nxtVarRicWord CopyOptions;
		nxtVarRicWord posX;
		nxtVarRicWord posY;
		nxtVarRicWord value;
		
	public:
		unsigned int filesize(){ return 10; }
		void read(ifstream* file);
		int write(ofstream* file);
		unsigned int object_type(){ return RIC_OP_PIXEL; }
		void draw(nxtCanvas* canvas);
		
};


class ricOpLine: public ricObject{
	private:
		nxtVarRicWord CopyOptions;
		nxtVarRicWord startX;
		nxtVarRicWord startY;
		nxtVarRicWord endX;
		nxtVarRicWord endY;
		
	public:
		unsigned int filesize(){ return 12; }
		void read(ifstream* file);
		int write(ofstream* file);
		unsigned int object_type(){ return RIC_OP_LINE; }
		void draw(nxtCanvas* canvas);
		
};


class ricOpRectangle: public ricObject{
	private:
		nxtVarRicWord CopyOptions;
		nxtVarRicWord posX;
		nxtVarRicWord posY;
		nxtVarRicWord width;
		nxtVarRicWord height;
		
	public:
		unsigned int filesize(){ return 12; }
		void read(ifstream* file);
		int write(ofstream* file);
		unsigned int object_type(){ return RIC_OP_RECTANGLE; }
		void draw(nxtCanvas* canvas);
		
};


class ricOpCicle: public ricObject{
	private:
		nxtVarRicWord CopyOptions;
		nxtVarRicWord posX;
		nxtVarRicWord posY;
		nxtVarRicWord radius;
		
	public:
		unsigned int filesize(){ return 10; }
		void read(ifstream* file);
		int write(ofstream* file);
		unsigned int object_type(){ return RIC_OP_CICLE; }
		void draw(nxtCanvas* canvas);
		
};


class ricOpNumber: public ricObject{
	private:
		nxtVarRicWord CopyOptions;
		nxtVarRicWord posX;
		nxtVarRicWord posY;
		nxtVarRicWord number;
		
	public:
		unsigned int filesize(){ return 10; }
		void read(ifstream* file);
		int write(ofstream* file);
		unsigned int object_type(){ return RIC_OP_NUMBER; }
		void draw(nxtCanvas* canvas);
		
};

/*
class ricOpXxxx: public ricObject{
	private:
		int 
		
	public:
		unsigned int filesize(){ return 18; }
		void read(char* file, unsigned int pos);
		int write(ofstream* file);
		unsigned int object_type(){ return 20983; }
		
};
*/

#endif

