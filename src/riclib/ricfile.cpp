#include <iostream>
#include <fstream>
using namespace std;

#include "ricfile.h"
#include "ricObject.h"
#include "nxtCanvas.h"

unsigned int ricfile::object_index( ricObject* obj_wanted ) const{
	for(unsigned int i=0; i<objects.size(); i++){
		if( objects[i] == obj_wanted )
			return i;
	}
	
	return INVALID_INDEX; //No object found
}


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
				case ricObject::RIC_OP_OPTIONS:		object = new ricOpOptions( this );	break;
				case ricObject::RIC_OP_SPRITE:		object = new ricOpSprite( this );	break;
				case ricObject::RIC_OP_VARMAP:		object = new ricOpVarMap( this );	break;
				case ricObject::RIC_OP_COPYBITS:	object = new ricOpCopyBits( this );	break;
				case ricObject::RIC_OP_PIXEL:		object = new ricOpPixel( this );	break;
				case ricObject::RIC_OP_LINE:			object = new ricOpLine( this );	break;
				case ricObject::RIC_OP_RECTANGLE:	object = new ricOpRectangle( this );	break;
				case ricObject::RIC_OP_CICLE:		object = new ricOpCicle( this );	break;
				case ricObject::RIC_OP_NUMBER:		object = new ricOpNumber( this );	break;
				case ricObject::RIC_OP_ELLIPSE:		object = new ricOpEllipse( this );	break;
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


ricfile::ricObject* ricfile::object_at_ID( unsigned char ID, unsigned int type, unsigned int from_index ) const{
	if( !( type == ricObject::RIC_OP_SPRITE || type == ricObject::RIC_OP_VARMAP ) )	//Must be one of these two
		return 0;
	
	if( from_index == INVALID_INDEX )	//if INVALID_INDEX, use the last object
		from_index = objects.size() - 1;
	else if( from_index >= objects.size() )	//if from_index refers to an invalid object, abort
		return 0;
	
	for(unsigned int i=from_index; i>=0; i--){
		unsigned int object_type = objects[i]->object_type();
		
		if( object_type == ricObject::RIC_OP_SPRITE ){
			if( ((ricOpSprite*)objects[i])->get_ID() == ID ){
				if( type == ricObject::RIC_OP_SPRITE )
					return objects[i];
				else
					return 0;
			}
		}
		else if( object_type == ricObject::RIC_OP_VARMAP ){
			if( ((ricOpVarMap*)objects[i])->get_ID() == ID ){
				if( type == ricObject::RIC_OP_VARMAP )
					return objects[i];
				else
					return 0;
			}
			
		}
	}
}


ricfile::ricObject* ricfile::object_at_ID( unsigned char ID, unsigned int type, ricObject* from_object ) const{
	if( !from_object )
		return 0;
	
	return object_at_ID( ID, type, object_index( from_object ) );	//TODO: don't use last index if no match found!
}


unsigned int ricfile::get_varmap_value( unsigned char varmapID, ricObject* from_object, unsigned int x ) const{
	unsigned int currListID = object_index( from_object );
	
	ricObject* varmap = object_at_ID( varmapID, ricObject::RIC_OP_VARMAP, currListID );
	if( varmap )
		return ((ricOpVarMap*)varmap)->value( x );
	else
		return 0;
}

