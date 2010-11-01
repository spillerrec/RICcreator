#include <iostream>
#include <fstream>
using namespace std;

#include "ricfile.h"
#include "ricObject.h"

int ricfile::readfile(char* filename){
	char* contents;
	unsigned int pos = 0;
	unsigned int size;
	
	ifstream file( filename, ios::in|ios::binary|ios::ate );
	
	if( file.is_open() ){
		size = file.tellg();
		contents = new char [size];

		file.seekg(0, ios::beg);
		file.read( contents, size );
		file.close();
	}
	else
		return 3;	//TODO: make these error codes contants instead...
	
	int returncode = 0;
	
	
	while(pos < size){
		//Is the header uncomplete?
		if(size < 4){
			returncode = 1;	//file invalid, return error
			break;
		}
		
		//read header
		int opcode_size = word( contents, pos );
		int opcode = word( contents, pos );
		
		//is the contents uncomplete?
		if( (size - pos + 2) < opcode_size ){
			returncode = 3;
			cout << "Reading contents failed\n";
			cout << "opcode_size: " << opcode_size << "\n";
			cout << "opcode: " << opcode << "\n";
			cout << "pos: " << pos << "\n";
			break;
		}

		
		//Add new element
		ricObject* object = NULL;
		switch(opcode){
			case RIC_OP_OPTIONS:		object = new ricOptions;	break;
			case RIC_OP_SPRITE:		object = new ricSprite;		break;
			case RIC_OP_VARMAP:		object = new ricVarMap;		break;
			case RIC_OP_COPYBITS:	object = new ricCopyBits;	break;
			case RIC_OP_PIXEL:		object = new ricPixel;		break;
			case RIC_OP_LINE:			object = new ricLine;		break;
			case RIC_OP_RECTANGLE:	object = new ricRectangle;	break;
			case RIC_OP_CICLE:		object = new ricCicle;		break;
			case RIC_OP_NUMBER:		object = new ricNumber;		break;
		}
		
		//If no object was added (unknown opcode), abort
		if(object == NULL){
			returncode = 2;
			break;
		}
		
		//Read object contents and add to the object array
		object->read( contents, pos );
		objects.push_back( object );
		
		pos += opcode_size - 2;
	}
	
	delete[] contents;	//Clean up
	
	cout << returncode;
	return returncode;
}
