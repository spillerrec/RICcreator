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

#include "nxtStream.h"

	
nxtStream::nxtStream( char *array, unsigned int size ){
	arr = array;
	pos = 0;
	lenght = size;
}

nxtIO::LoaderError nxtStream::read_multibyte_unsigned( unsigned char bytes, unsigned long &data ){
	if( !arr )
		return LDR_HANDLEALREADYCLOSED;
	
	if( remaining_size() < bytes )
		return LDR_ENDOFFILE;
	
	data = 0;
	unsigned long multiplier = 1;
	
	for(int i=0; i<bytes; i++){
		data += (unsigned char)arr[pos] * multiplier;
		multiplier *= 256;
		pos++;
	}
	
	return LDR_SUCCESS;
}
nxtIO::LoaderError nxtStream::write_multibyte_unsigned( unsigned char bytes, unsigned long number ){
	if( !arr )
		return LDR_HANDLEALREADYCLOSED;
	
	if( remaining_size() < bytes )
		return LDR_ENDOFFILE;
	
	//Format it correctly
	for(int i=0; i<bytes; i++){
		arr[pos+i] = number % 256;
		number /= 256;
	}
	pos += bytes;
	
	return LDR_SUCCESS;
}

nxtIO::LoaderError nxtStream::open_read(){
	pos = 0;
		
	return LDR_SUCCESS;
}
nxtIO::LoaderError nxtStream::open_write( unsigned int filesize ){
	if( filesize <= lenght )
		return LDR_SUCCESS;
	else
		return LDR_NOSPACE;
}

nxtIO::LoaderError nxtStream::close(){
	return LDR_SUCCESS;
}

unsigned int nxtStream::remaining_size() const{
	return lenght - pos;
}



