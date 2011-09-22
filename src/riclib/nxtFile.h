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
	input/output from files
	
*/


#ifndef NXTFILE_H
#define NXTFILE_H

#include <fstream>

#include "nxtIO.h"

class nxtFile: public nxtIO{
	private:
		const char *filename;
		std::ifstream *i_file;
		std::ofstream *o_file;
		unsigned int filesize;
	
	public:
		nxtFile( const char *path );
		~nxtFile();
		
		LoaderError write_bytes( unsigned int lenght, char *data );
		
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

