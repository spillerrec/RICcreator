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


#include "nxtIO.h"

#include "nxtVariable.h"

nxtIO::LoaderError nxtIO::Read( nxtVariable *var ){
	return var->read( this );
}
nxtIO::LoaderError nxtIO::Write( const nxtVariable *var ){
	return var->write( this );
}


nxtIO::LoaderError nxtIO::read_multibyte_unsigned( unsigned char bytes, unsigned long &data ){
	data = 0;
	
	unsigned char file_data[ bytes ];
	RETURN_ON_LOADER_ERROR( ReadBytes( (char*)file_data, bytes ) );
	
	unsigned long multiplier = 1;
	for(int i=0; i<bytes; i++){
		data += file_data[i] * multiplier;
		multiplier *= 256;
	}
	
	return LDR_SUCCESS;
}
nxtIO::LoaderError nxtIO::write_multibyte_unsigned( unsigned char bytes, unsigned long number ){
	unsigned char file_data[ bytes ];
	
	//Format it correctly
	for(int i=0; i<bytes; i++){
		file_data[i] = number % 256;
		number /= 256;
	}
	
	return WriteBytes( (char*)file_data, bytes );
}

