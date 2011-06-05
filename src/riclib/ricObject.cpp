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

#include "ricObject.h"
#include "nxtVariable.h"
#include <iostream>
using namespace std;

void ricfile::ricObject::read(ifstream* file){
	for( unsigned int i=0; i<var_count; i++ )
		vars[i]->read( file );
}

int ricfile::ricObject::write(ofstream* file){
	nxtVariable::write_multibyte( file, filesize(), 2 );
	nxtVariable::write_multibyte( file, object_type(), 2 );
	
	for( unsigned int i=0; i<var_count; i++ ){
		vars[i]->write( file );
	}
	
	return 0;
}


unsigned int ricfile::ricObject::filesize() const{
	unsigned int size = 4;	//Header size
	
	for( unsigned int i=0; i<var_count; i++ ){
		size += vars[i]->filesize();
	}
	
	return size;
}

