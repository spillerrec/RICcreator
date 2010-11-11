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
		virtual void read(char* file, unsigned int pos) = 0;
		virtual int write(ofstream* file) = 0;
		virtual unsigned int filesize() = 0;
		virtual unsigned int object_type() = 0;
		virtual void draw(nxtCanvas* canvas){ return; };
};


class ricOpOptions: public ricObject{
	private:
		int options;
		int width;
		int height;
		
	public:
		unsigned int filesize(){ return 8; }
		void read(char* file, unsigned int pos);
		int write(ofstream* file);
		unsigned int object_type(){ return RIC_OP_OPTIONS; }
};


class ricOpSprite: public ricObject{
	private:
		int sprite_ID;
		int rows;
		int columns;
		char* image;
		
	public:
		unsigned int filesize(){ return 8 + rows * columns + (rows * columns) % 2; } /* padding? */
		void read(char* file, unsigned int pos);
		int write(ofstream* file);
		unsigned int object_type(){ return RIC_OP_SPRITE; }
		
		ricOpSprite(){
			image = NULL;
		}
		
		
		
		//TODO: add destructor!
};


class ricOpVarMap: public ricObject{
	private:
		int VarMapID;
		int size;
		
		struct point{
			int X;
			int Y;
		};
		std::vector<point> VarMap;
		//TODO: add XY array here
		
	public:
		unsigned int filesize(){ return 6 + 4 * size; }
		void read(char* file, unsigned int pos);
		int write(ofstream* file);
		unsigned int object_type(){ return RIC_OP_VARMAP; }
		
		
		//TODO: add destructor!
};

class ricOpCopyBits: public ricObject{
	private:
		int CopyOptions;
		int SpriteID;
		int posX;
		int posY;
		int width;
		int height;
		int relX;
		int relY;
		
	public:
		unsigned int filesize(){ return 18; }
		void read(char* file, unsigned int pos);
		int write(ofstream* file);
		unsigned int object_type(){ return RIC_OP_COPYBITS; }
		void draw(nxtCanvas* canvas);
		
};



class ricOpPixel: public ricObject{
	private:
		int CopyOptions;
		int posX;
		int posY;
		int value;
		
	public:
		unsigned int filesize(){ return 10; }
		void read(char* file, unsigned int pos);
		int write(ofstream* file);
		unsigned int object_type(){ return RIC_OP_PIXEL; }
		void draw(nxtCanvas* canvas);
		
};


class ricOpLine: public ricObject{
	private:
		int CopyOptions;
		int startX;
		int startY;
		int endX;
		int endY;
		
	public:
		unsigned int filesize(){ return 12; }
		void read(char* file, unsigned int pos);
		int write(ofstream* file);
		unsigned int object_type(){ return RIC_OP_LINE; }
		void draw(nxtCanvas* canvas);
		
};


class ricOpRectangle: public ricObject{
	private:
		int CopyOptions;
		int posX;
		int posY;
		int width;
		int height;
		
	public:
		unsigned int filesize(){ return 12; }
		void read(char* file, unsigned int pos);
		int write(ofstream* file);
		unsigned int object_type(){ return RIC_OP_RECTANGLE; }
		void draw(nxtCanvas* canvas);
		
};


class ricOpCicle: public ricObject{
	private:
		int CopyOptions;
		int posX;
		int posY;
		int radius;
		
	public:
		unsigned int filesize(){ return 10; }
		void read(char* file, unsigned int pos);
		int write(ofstream* file);
		unsigned int object_type(){ return RIC_OP_CICLE; }
		void draw(nxtCanvas* canvas);
		
};


class ricOpNumber: public ricObject{
	private:
		int CopyOptions;
		int posX;
		int posY;
		int number;
		
	public:
		unsigned int filesize(){ return 10; }
		void read(char* file, unsigned int pos);
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

