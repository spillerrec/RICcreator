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
			//Is the header uncomplete or filesize wrong?
			if(size < 4 || size%2 ){
				returncode = 1;	//file invalid, return error
				break;
			}
			
			//read header
			opcode_size.read( &file );
			opcode.read( &file );
			
			//Add new element
			ricObject* object = add_ric_object( opcode );
			if( object )
				object->read( &file );	//Read contents from file
			else{	//If no object was added (unknown opcode), abort
				returncode = 2;
				break;
			}
			
			//TODO: check if it actually ended the place it told to in filesize
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


ricfile::ricObject* ricfile::add_ric_object( unsigned int type ){
	//Create new element
	ricObject* object = NULL;
	switch( type ){
		case ricObject::RIC_OP_OPTIONS:		object = new ricOpOptions( this );	break;
		case ricObject::RIC_OP_SPRITE:		object = new ricOpSprite( this );	break;
		case ricObject::RIC_OP_VARMAP:		object = new ricOpVarMap( this );	break;
		case ricObject::RIC_OP_COPYBITS:	object = new ricOpCopyBits( this );	break;
		case ricObject::RIC_OP_PIXEL:		object = new ricOpPixel( this );	break;
		case ricObject::RIC_OP_LINE:		object = new ricOpLine( this );	break;
		case ricObject::RIC_OP_RECTANGLE:	object = new ricOpRectangle( this );	break;
		case ricObject::RIC_OP_CICLE:		object = new ricOpCicle( this );	break;
		case ricObject::RIC_OP_NUMBER:		object = new ricOpNumber( this );	break;
		case ricObject::RIC_OP_ELLIPSE:		object = new ricOpEllipse( this );	break;
		case ricObject::RIC_OP_POLYGON:		object = new ricOpPolygon( this );	break;
	}
	
	//Failed to create new element
	if( object == NULL )
		return NULL;
	
	//Add to list and return
	objects.push_back( object );
	return object;
}


ricfile::ricObject* ricfile::object_at_ID( unsigned char ID, unsigned int type, unsigned int from_index ) const{
	if( !( type == ricObject::RIC_OP_SPRITE || type == ricObject::RIC_OP_VARMAP ) )	//Must be one of these two
		return 0;
	
	if( from_index == INVALID_INDEX )	//if INVALID_INDEX, use the last object
		from_index = objects.size() - 1;
	if( from_index >= objects.size() )	//if from_index refers to an invalid object, abort
		return 0;
	
	for(unsigned int i=from_index; i>=0 && i < objects.size(); i--){
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
	
	return 0;	//Just in case...
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

