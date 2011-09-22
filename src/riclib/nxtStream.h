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


/*
	input/output from char* arrays
	
*/


#ifndef NXTSTREAM_H
#define NXTSTREAM_H


#include "nxtIO.h"

class nxtStream: public nxtIO{
	private:
		char *arr;
		unsigned int pos;
		unsigned int lenght;
	
	public:
		nxtStream( char *array, unsigned int size );
		
	//Abstract functions
	public:
		LoaderError read_multibyte_unsigned( unsigned char bytes, unsigned long &data );
		LoaderError write_multibyte_unsigned( unsigned char bytes, unsigned long data );
		
		LoaderError open_read();
		LoaderError open_write( unsigned int filesize = 0 );
		LoaderError close();
		
		unsigned int remaining_size() const;
	
};


#endif

