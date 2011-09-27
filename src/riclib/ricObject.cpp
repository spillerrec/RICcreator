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
#include "ricfile.h"

ricObject::ricObject( ricfile *container, unsigned int var_amount ){
	if( (pRIC = container) )
		pRIC->add_ric_object( this );
	var_count = var_amount;
	vars = new nxtVariable* [ var_count ];
}

nxtIO::LoaderError ricObject::read( nxtIO* file){
	for( unsigned int i=0; i<var_count; i++ ){
		nxtIO::LoaderError result = vars[i]->read( file );
		
		if( result != nxtIO::LDR_SUCCESS )
			return result;
	}
	
	return nxtIO::LDR_SUCCESS;
}

nxtIO::LoaderError ricObject::write( nxtIO* file) const{
	RETURN_ON_LOADER_ERROR( file->write_multibyte_unsigned( 2, filesize()-2 ) );	//Don't include this word in size
	RETURN_ON_LOADER_ERROR( file->write_multibyte_unsigned( 2, object_type() ) );
	
	for( unsigned int i=0; i<var_count; i++ ){
		nxtIO::LoaderError result = vars[i]->write( file );
		
		if( result != nxtIO::LDR_SUCCESS )
			return result;
	}
	
	return nxtIO::LDR_SUCCESS;
}


unsigned int ricObject::filesize() const{
	unsigned int size = 4;	//Header size
	
	for( unsigned int i=0; i<var_count; i++ ){
		size += vars[i]->filesize();
	}
	
	return size;
}


nxtVariable* ricObject::get_setting( unsigned int index ){
	if( index < var_count )
		return vars[ index ];
	else
		return 0;
}

