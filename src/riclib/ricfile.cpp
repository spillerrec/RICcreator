#include <iostream>
#include <fstream>
using namespace std;

#include "ricfile.h"
#include "ricObject.h"

int ricfile::readfile(char* filename){
	unsigned int size;
	nxtVarWord opcode_size;
	nxtVarWord opcode;
	int returncode = 0;
	
	ifstream file( filename, ios::in|ios::binary|ios::ate );
	
	if( file.is_open() ){
		size = file.tellg();
		file.seekg(0, ios::beg);
		
		while( !file.eof() ){
			//Is the header uncomplete?
			if(size < 4){
				returncode = 1;	//file invalid, return error
				break;
			}
			
			//read header
			opcode_size.read( &file );
			opcode.read( &file );
			cout << "opcode_size: " << opcode_size << "\n";
			cout << "opcode: " << opcode << "\n";
			
			//Add new element
			ricObject* object = NULL;
			switch( opcode ){
				case RIC_OP_OPTIONS:		object = new ricOpOptions;		break;
				case RIC_OP_SPRITE:		object = new ricOpSprite;		break;
				case RIC_OP_VARMAP:		object = new ricOpVarMap;		break;
				case RIC_OP_COPYBITS:	object = new ricOpCopyBits;	break;
				case RIC_OP_PIXEL:		object = new ricOpPixel;		break;
				case RIC_OP_LINE:			object = new ricOpLine;			break;
				case RIC_OP_RECTANGLE:	object = new ricOpRectangle;	break;
				case RIC_OP_CICLE:		object = new ricOpCicle;		break;
				case RIC_OP_NUMBER:		object = new ricOpNumber;		break;
			}
			
			//If no object was added (unknown opcode), abort
			if(object == NULL){
				returncode = 2;
				break;
			}
			
			//Read object contents and add to the object array
			object->read( &file );
			objects.push_back( object );
		}
		
		file.close();

	}
	else
		returncode = 3;	//TODO: make these error codes contants instead...
	
	
	
	cout << "readfile() returned with code: " << returncode << "\n\n";
	return returncode;
}



int ricfile::writefile(char* filename){
	ofstream file( filename, ofstream::binary|ofstream::trunc|ofstream::out );
	
	if( file.is_open() ){
		for(int i=0; i<objects.size(); i++){
			objects[i]->write(&file);
		}
		
		
		file.close();
	}
}


void ricfile::Draw(nxtCanvas* canvas, unsigned int width, unsigned int height){
	canvas->create(width, height);
	
	for(int i=0; i<objects.size(); i++){
		objects[i]->draw(canvas);
	}
	
}



