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

#include "nxtFile.h"

using namespace std;
	
nxtFile::nxtFile( const char *path ){
	i_file = NULL;
	o_file = NULL;
	filename = path;
	filesize = 0;
}

nxtFile::~nxtFile(){
	if( i_file )
		delete i_file;
	if( o_file )
		delete o_file;
}

nxtIO::LoaderError nxtFile::write_bytes( unsigned int lenght, char *data ){
	if( !o_file )
		return LDR_HANDLEALREADYCLOSED;
	
	if( remaining_size() < lenght )
		return LDR_ENDOFFILE;
	
	o_file->write( data, lenght );
	
	return LDR_SUCCESS;
}

nxtIO::LoaderError nxtFile::read_multibyte_unsigned( unsigned char bytes, unsigned long &data ){
	if( !i_file )
		return LDR_HANDLEALREADYCLOSED;
	
	if( remaining_size() < bytes )
		return LDR_ENDOFFILE;
	
	data = 0;
	unsigned long multiplier = 1;
	
	for(int i=0; i<bytes; i++){
		data += i_file->get() * multiplier;
		multiplier *= 256;
	}
	
	return LDR_SUCCESS;
}

nxtIO::LoaderError nxtFile::write_multibyte_unsigned( unsigned char bytes, unsigned long number ){
	if( !o_file )
		return LDR_HANDLEALREADYCLOSED;
	
	if( remaining_size() !=0 && remaining_size() < bytes )
		return LDR_ENDOFFILE;
	
	//Format it correctly
	char* data = new char [bytes];
	for(int i=0; i<bytes; i++){
		data[i] = number % 256;
		number /= 256;
	}
	o_file->write(data, bytes); //Write it to stream
	
	delete[] data;
	
	return LDR_SUCCESS;
}

nxtIO::LoaderError nxtFile::open_read(){
	i_file = new ifstream( filename, ios::in|ios::binary|ios::ate );
	
	if( i_file->is_open() ){
		filesize = i_file->tellg();
		i_file->seekg( 0, ios::beg );
		
		return LDR_SUCCESS;
	}
	else
		return LDR_FILENOTFOUND;
}

nxtIO::LoaderError nxtFile::open_write( unsigned int filesize ){
	o_file = new ofstream( filename, ofstream::binary|ofstream::trunc|ofstream::out );
	this->filesize = filesize;
	return LDR_SUCCESS;
}

nxtIO::LoaderError nxtFile::close(){
	if( i_file )
		i_file->close();
	if( o_file )
		o_file->close();
	return LDR_SUCCESS;
}

unsigned int nxtFile::remaining_size() const{
	if( i_file )
		return filesize - i_file->tellg();
	else
		return 0;
}



