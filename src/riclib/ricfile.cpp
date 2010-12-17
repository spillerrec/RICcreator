#include <iostream>
#include <fstream>
using namespace std;

#include "ricfile.h"
#include "ricObject.h"
#include "nxtCanvas.h"

int ricfile::readfile(char* filename){
	unsigned int size;
	nxtVarWord opcode_size;
	nxtVarWord opcode;
	int returncode = 0;
	
	Reset();
	
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
				case RIC_OP_OPTIONS:		object = new ricOpOptions( this );	break;
				case RIC_OP_SPRITE:		object = new ricOpSprite( this );	break;
				case RIC_OP_VARMAP:		object = new ricOpVarMap( this );	break;
				case RIC_OP_COPYBITS:	object = new ricOpCopyBits( this );	break;
				case RIC_OP_PIXEL:		object = new ricOpPixel( this );	break;
				case RIC_OP_LINE:			object = new ricOpLine( this );	break;
				case RIC_OP_RECTANGLE:	object = new ricOpRectangle( this );	break;
				case RIC_OP_CICLE:		object = new ricOpCicle( this );	break;
				case RIC_OP_NUMBER:		object = new ricOpNumber( this );	break;
				case RIC_OP_ELLIPSE:		object = new ricOpEllipse( this );	break;
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
		for(unsigned int i=0; i<objects.size(); i++){
			objects[i]->write(&file);
		}
		
		
		file.close();
	}
	
	return 0;
}


void ricfile::Draw(nxtCanvas* canvas, unsigned int width, unsigned int height){
	canvas->create(width, height);
	
	for(unsigned int i=0; i<objects.size(); i++){
		objects[i]->draw(canvas);
	}
	
}


void ricfile::Reset(){
	ResetParameters();
	
	for(unsigned int i=0; i<objects.size(); i++){
		delete objects[i];
	}
	objects.clear();
}



ricfile::ricOpSprite* ricfile::GetSprite( unsigned char SpriteID, unsigned int currListID ){
	if( currListID == -1 )
		currListID = objects.size() -1;
	else if( currListID >= objects.size() )
		return 0;
	
	
	cout << "Starts looking after sprite " << (int)SpriteID <<"\n";
	cout << "Starts at index " << currListID <<"\n";
	
	for(int i=currListID; i>=0; i--){
		cout << "Object: " << (int)objects[i]->object_type() << "\n";
		cout << "index: " << i << "\n";
		if( objects[i]->object_type() == RIC_OP_SPRITE ){
			ricOpSprite *sprite = (ricOpSprite*)objects[i];
			cout << "Sprite: " << sprite->get_ID() <<"\n";
			
			if( sprite->get_ID() == SpriteID ){
				cout << "Found Sprite\n";
				return sprite;
			}
		}
		cout << "\n";
	}
	cout << "No sprite found :\\\n";
	
	return 0;
}

