#include <vector>
#include <iostream>
using namespace std;

inline unsigned int word(char* file, unsigned int &pos){
	pos += 2;
	return (unsigned char)file[pos-2] + (unsigned char)file[pos-1] * 256;
}



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
	private:
		
	public:
		virtual void read(char* file, unsigned int pos) = 0;
		//virtual bool write() = 0;
		virtual unsigned int filesize() = 0;
		virtual unsigned int object_type() = 0;
};


class ricOptions: public ricObject{
	private:
		int options;
		int width;
		int height;
		
	public:
		unsigned int filesize(){ return 8; }
		void read(char* file, unsigned int pos);
		unsigned int object_type(){ return RIC_OP_OPTIONS; }
};


class ricSprite: public ricObject{
	private:
		int sprite_ID;
		int rows;
		int columns;
		char* image;
		
	public:
		unsigned int filesize(){ return 8 + rows * columns + (rows * columns) % 2; } /* padding? */
		void read(char* file, unsigned int pos);
		unsigned int object_type(){ return RIC_OP_SPRITE; }
		
		ricSprite(){
			image = NULL;
		}
		
		
		
		//TODO: add destructor!
};


class ricVarMap: public ricObject{
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
		unsigned int object_type(){ return RIC_OP_VARMAP; }
		
		
		//TODO: add destructor!
};

class ricCopyBits: public ricObject{
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
		unsigned int object_type(){ return RIC_OP_COPYBITS; }
		
};



class ricPixel: public ricObject{
	private:
		int CopyOptions;
		int posX;
		int posY;
		int value;
		
	public:
		unsigned int filesize(){ return 10; }
		void read(char* file, unsigned int pos);
		unsigned int object_type(){ return RIC_OP_PIXEL; }
		
};


class ricLine: public ricObject{
	private:
		int CopyOptions;
		int startX;
		int startY;
		int endX;
		int endY;
		
	public:
		unsigned int filesize(){ return 20; }
		void read(char* file, unsigned int pos);
		unsigned int object_type(){ return RIC_OP_LINE; }
		
};


class ricRectangle: public ricObject{
	private:
		int CopyOptions;
		int posX;
		int posY;
		int width;
		int height;
		
	public:
		unsigned int filesize(){ return 20; }
		void read(char* file, unsigned int pos);
		unsigned int object_type(){ return RIC_OP_RECTANGLE; }
		
};


class ricCicle: public ricObject{
	private:
		int CopyOptions;
		int posX;
		int posY;
		int radius;
		
	public:
		unsigned int filesize(){ return 10; }
		void read(char* file, unsigned int pos);
		unsigned int object_type(){ return RIC_OP_CICLE; }
		
};


class ricNumber: public ricObject{
	private:
		int CopyOptions;
		int posX;
		int posY;
		int number;
		
	public:
		unsigned int filesize(){ return 18; }
		void read(char* file, unsigned int pos);
		unsigned int object_type(){ return RIC_OP_NUMBER; }
		
};

/*
class ric: public ricObject{
	private:
		int 
		
	public:
		unsigned int filesize(){ return 18; }
		void read(char* file, unsigned int pos);
		unsigned int object_type(){ return 20983; }
		
};
*/

