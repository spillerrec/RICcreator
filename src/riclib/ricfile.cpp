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
#include <string.h>

#include "ricfile.h"
#include "ricObjectChildren.h"
#include "nxtCanvas.h"
#include "nxtFile.h"
#include "nxtStream.h"

unsigned int ricfile::object_index( ricObject* obj_wanted ) const{
	for(unsigned int i=0; i<objects.size(); i++){
		if( objects[i] == obj_wanted )
			return i;
	}
	
	return INVALID_INDEX; //No object found
}



nxtIO::LoaderError ricfile::read( nxtIO *file ){
	unsigned int size;
	nxtVarWord opcode_size;
	nxtVarWord opcode;
	
	nxtIO::LoaderError result = nxtIO::LDR_SUCCESS;
	
	Reset();
	
	if( (result = file->open_read()) == nxtIO::LDR_SUCCESS ){
		
		while( (size = file->remaining_size()) ){
			//Is the header uncomplete or filesize wrong?
			if( size < 4 || size%2 ){
				result = nxtIO::LDR_UNDEFINEDERROR;
				break;	//file invalid, return error
			}
			
			//read header
			BREAK_ON_LOADER_ERROR( result, opcode_size.read( file ) );
			BREAK_ON_LOADER_ERROR( result, opcode.read( file ) );
			
			//Add new element
			ricObject* object = add_ric_object( opcode );
			if( object ){
				BREAK_ON_LOADER_ERROR( result, object->read( file ) );	//Read contents from file
			}
			else{	//If no object was added (unknown opcode), abort
				result = nxtIO::LDR_UNDEFINEDERROR;
				break;
			}
		}
		
		file->close();
	}
	
	return result;
}


nxtIO::LoaderError ricfile::write( nxtIO *file ) const{
	nxtIO::LoaderError result = nxtIO::LDR_SUCCESS;
	
	//TODO: specify filesize
	if( (result = file->open_write( filesize() )) == nxtIO::LDR_SUCCESS ){
		//Write every object
		for(unsigned int i=0; i<objects.size(); i++)
			BREAK_ON_LOADER_ERROR( result, objects[i]->write(file) );
		
		file->close();
	}
	
	return result;
}


nxtIO::LoaderError ricfile::readfile( const char* filename ){
	nxtFile file( filename );
	return read( &file );
}


nxtIO::LoaderError ricfile::writefile( const char* filename ){
	nxtFile file( filename );
	return write( &file );
}


nxtIO::LoaderError ricfile::write_header_file( const char* filename, const char *var_name ){
	unsigned int size = filesize();
	char *bytes = new char[ size ];
	nxtStream bytestream( bytes, size );
	RETURN_ON_LOADER_ERROR( write( &bytestream ) );
	
	//Strings to write
	const char *type = "unsigned char ";
		//Insert filename in here without fileending
	const char *start = "[] = { ";
		//insert bytes in here
	const char *end = " };\r\n";
	
	//Calculate the size of the header
	unsigned int header_size = 0;
	header_size += strlen( type );
	header_size += strlen( var_name );
	header_size += strlen( start );
	header_size += size*6 - 2;	//Size for all "0xHH, ", substracting the very last ", "
	header_size += strlen( end );
	
	nxtFile file( filename );
	RETURN_ON_LOADER_ERROR( file.open_write( header_size ) );
	
	//Write start
	RETURN_ON_LOADER_ERROR( file.WriteBytes( type, strlen( type ) ) );
	RETURN_ON_LOADER_ERROR( file.WriteBytes( var_name, strlen( var_name ) ) );
	RETURN_ON_LOADER_ERROR( file.WriteBytes( start, strlen( start ) ) );
	
	//Write bytes
	for( unsigned int i=0; i<size; i++ ){
		if( i != 0 )
			RETURN_ON_LOADER_ERROR( file.WriteBytes( ", ", 2 ) );
		RETURN_ON_LOADER_ERROR( file.write_formatted_hex( (unsigned char)bytes[i] ) );
	}
	
	//Write end
	RETURN_ON_LOADER_ERROR( file.WriteBytes( end, strlen( end ) ) );
	
	return nxtIO::LDR_SUCCESS;
}


unsigned int ricfile::filesize() const{
	unsigned int size = 0;
	
	for(unsigned int i=0; i<objects.size(); i++)
		size += objects[i]->filesize();
	
	return size;
}


void ricfile::Draw( nxtCanvas* canvas, unsigned int width, unsigned int height ) const{
	canvas->create( width, height );
	Draw( canvas );
}

void ricfile::Draw( nxtCanvas* canvas ) const{
	for(unsigned int i=0; i<objects.size(); i++)
		objects[i]->draw(canvas);
}


void ricfile::Reset(){
	ResetParameters();
	
	for(unsigned int i=0; i<objects.size(); i++){
		delete objects[i];
	}
	objects.clear();
}


ricObject* ricfile::add_ric_object( unsigned int type ){
	ricObject* object = NULL;
	
	//Create new element, which automatically adds the object
	switch( type ){
		case ricObject::RIC_OP_OPTIONS:		object = new ricOpOptions( this );	break;
		case ricObject::RIC_OP_SPRITE:		object = new ricOpSprite( this );	break;
		case ricObject::RIC_OP_VARMAP:		object = new ricOpVarMap( this );	break;
		case ricObject::RIC_OP_COPYBITS:	object = new ricOpCopyBits( this );	break;
		case ricObject::RIC_OP_PIXEL:		object = new ricOpPixel( this );	break;
		case ricObject::RIC_OP_LINE:		object = new ricOpLine( this );	break;
		case ricObject::RIC_OP_RECTANGLE:	object = new ricOpRectangle( this );	break;
		case ricObject::RIC_OP_CICLE:		object = new ricOpCircle( this );	break;
		case ricObject::RIC_OP_NUMBER:		object = new ricOpNumber( this );	break;
		case ricObject::RIC_OP_ELLIPSE:		object = new ricOpEllipse( this );	break;
		case ricObject::RIC_OP_POLYGON:		object = new ricOpPolygon( this );	break;
	}
	
	return object;
}


ricObject* ricfile::object_at_ID( unsigned char ID, unsigned int type, unsigned int from_index ) const{
	if( !( type == ricObject::RIC_OP_SPRITE || type == ricObject::RIC_OP_VARMAP ) )	//Must be one of these two
		return 0;
	
	if( from_index == INVALID_INDEX )	//if INVALID_INDEX, use the last object
		from_index = objects.size() - 1;
	if( from_index >= objects.size() )	//if from_index refers to an invalid object, abort
		return 0;
	
	for(unsigned int i=from_index-1; i>=0 && i < objects.size(); i--){
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


ricObject* ricfile::object_at_ID( unsigned char ID, unsigned int type, ricObject* from_object ) const{
	if( !from_object )
		return 0;
	
	return object_at_ID( ID, type, object_index( from_object ) );	//TODO: don't use last index if no match found!
}


ricObject* ricfile::last_object( unsigned int type, unsigned int from_index ) const{
	if( from_index == INVALID_INDEX )	//if INVALID_INDEX, use the last object
		from_index = objects.size() - 1;
	if( from_index >= objects.size() )	//if from_index refers to an invalid object, abort
		return 0;
	
	for(unsigned int i=from_index-1; i < objects.size(); i--){
		unsigned int object_type = objects[i]->object_type();
		if( object_type == type )
			return objects[i];
	}
	
	return 0;
}

ricObject* ricfile::last_object( unsigned int type, ricObject* from_object ) const{
	if( !from_object )
		return 0;
	
	return last_object( type, object_index( from_object ) );
}


//Genetic array function, combine?
bool ricfile::move_object( unsigned int from, unsigned int to ){
	//Validate paramters
	if( from >= objects.size() )
		return false;	//from is invalid!
	if( to >= objects.size() )
		to = objects.size() - 1; //Set it to the last element
	
	int direction = 1;
	if( from >= to )	//If it needs to move upwards, change direction
		direction = -1;
	//Start moving
	for( unsigned int i=from; i!=to; i+=direction ){
		//Swap objects
		ricObject *temp = objects[ i+direction ];
		objects[ i+direction ] = objects[i];
		objects[i] = temp;
	}
	
	return true;
}

bool ricfile::remove_object( unsigned int index ){
	if( index >= objects.size() )
		return false;	//index is invalid!
	
	move_object( index, INVALID_INDEX );	//Move to bottom
	//And remove it
	delete objects[ objects.size()-1 ];
	objects.pop_back();
	
	return true;
}


unsigned int ricfile::get_varmap_value( unsigned char varmapID, ricObject* from_object, unsigned int x ) const{
	unsigned int currListID = object_index( from_object );
	
	ricObject* varmap = object_at_ID( varmapID, ricObject::RIC_OP_VARMAP, currListID );
	if( varmap )
		return ((ricOpVarMap*)varmap)->value( x );
	else
		return 0;
}

