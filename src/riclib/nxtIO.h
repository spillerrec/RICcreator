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
	Abstract base class for input/output for NXT types
	
*/


#ifndef NXTIO_H
#define NXTIO_H


#include <string>

#define RETURN_ON_LOADER_ERROR( expression ){ nxtIO::LoaderError result; if( ( result = (expression)) != nxtIO::LDR_SUCCESS ) return result; }
#define BREAK_ON_LOADER_ERROR( result, expression ){ if( ( result = (expression)) != nxtIO::LDR_SUCCESS ) break; }

class nxtVariable;

class nxtIO{
	public:
		enum LoaderError{
			LDR_SUCCESS = 0x0000,
			LDR_INPROGRESS = 0x0001,
			LDR_REQPIN = 0x0002,
			LDR_NOMOREHANDLES = 0x8100,
			LDR_NOSPACE = 0x8200,
			LDR_NOMOREFILES = 0x8300,
			LDR_EOFEXPECTED = 0x8400,
			LDR_ENDOFFILE = 0x8500,
			LDR_NOTLINEARFILE = 0x8600,
			LDR_FILENOTFOUND = 0x8700,
			LDR_HANDLEALREADYCLOSED = 0x8800,
			LDR_NOLINEARSPACE = 0x8900,
			LDR_UNDEFINEDERROR = 0x8A00,
			//...
			LDR_FILEISFULL = 0x8E00,
			LDR_FILEEXISTS = 0x8F00,
			//...
			LDR_INVALIDSEEK = 0x9C00,
			
			//Custom commands here
			LDR_TEXT_INVALID = 0xFF01 //Text parsing failed
		};
		
		LoaderError read_word( unsigned int &data ){
			unsigned long temp;
			LoaderError result = read_multibyte_unsigned( 2, temp );
			data = temp;
			return result;
		}
		LoaderError read_multibyte_unsigned( unsigned char bytes, unsigned long &data );
		LoaderError write_multibyte_unsigned( unsigned char bytes, unsigned long data );
		
		virtual LoaderError ReadBytes( char *data, unsigned int size ) = 0;
		virtual LoaderError WriteBytes( const char *data, unsigned int size ) = 0;
		
		virtual LoaderError open_read() = 0;
		virtual LoaderError open_write( unsigned int filesize ) = 0;
		virtual LoaderError close() = 0;
		LoaderError Read( nxtVariable *var );
		LoaderError Write( const nxtVariable *var );
		virtual unsigned int remaining_size() const = 0;
		
		
	//read text
	public:
		virtual char peek() = 0;
		LoaderError read_number( unsigned int &result, bool skip = true );
		LoaderError read_text( std::string &text, bool skip = true );
		LoaderError skip_whitespace();
		
		static bool hex2number( char hex, unsigned int &number ); //Returns true on successful conversion
		
		
	//String write functions
	public:
		LoaderError write_formatted_hex( unsigned char value );
};

#endif

