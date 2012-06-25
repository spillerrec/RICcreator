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

#include <cctype>

nxtIO::LoaderError nxtIO::Read( nxtVariable *var ){
	return var->read( this );
}
nxtIO::LoaderError nxtIO::Write( const nxtVariable *var ){
	return var->write( this );
}


nxtIO::LoaderError nxtIO::read_multibyte_unsigned( unsigned char bytes, unsigned long &data ){
	data = 0;
	
	unsigned char *file_data = new unsigned char[ bytes ];
	RETURN_ON_LOADER_ERROR( ReadBytes( (char*)file_data, bytes ) );
	
	unsigned long multiplier = 1;
	for(int i=0; i<bytes; i++){
		data += file_data[i] * multiplier;
		multiplier *= 256;
	}
	
	delete file_data;
	return LDR_SUCCESS;
}
nxtIO::LoaderError nxtIO::write_multibyte_unsigned( unsigned char bytes, unsigned long number ){
	unsigned char *file_data = new unsigned char[ bytes ];
	
	//Format it correctly
	for(int i=0; i<bytes; i++){
		file_data[i] = number % 256;
		number /= 256;
	}
	
	nxtIO::LoaderError result = WriteBytes( (char*)file_data, bytes );
	delete file_data;
	return result;
}


nxtIO::LoaderError nxtIO::write_formatted_hex( unsigned char value ){
	RETURN_ON_LOADER_ERROR( WriteBytes( "0x", 2 ) );
	
	char h1;
	switch( value / 16 ){
		case 0: h1 = '0'; break;
		case 1: h1 = '1'; break;
		case 2: h1 = '2'; break;
		case 3: h1 = '3'; break;
		case 4: h1 = '4'; break;
		case 5: h1 = '5'; break;
		case 6: h1 = '6'; break;
		case 7: h1 = '7'; break;
		case 8: h1 = '8'; break;
		case 9: h1 = '9'; break;
		case 10: h1 = 'A'; break;
		case 11: h1 = 'B'; break;
		case 12: h1 = 'C'; break;
		case 13: h1 = 'D'; break;
		case 14: h1 = 'E'; break;
		case 15: h1 = 'F'; break;
	}
	RETURN_ON_LOADER_ERROR( WriteBytes( &h1, 1 ) );
	
	char h2;
	switch( value % 16 ){
		case 0: h2 = '0'; break;
		case 1: h2 = '1'; break;
		case 2: h2 = '2'; break;
		case 3: h2 = '3'; break;
		case 4: h2 = '4'; break;
		case 5: h2 = '5'; break;
		case 6: h2 = '6'; break;
		case 7: h2 = '7'; break;
		case 8: h2 = '8'; break;
		case 9: h2 = '9'; break;
		case 10: h2 = 'A'; break;
		case 11: h2 = 'B'; break;
		case 12: h2 = 'C'; break;
		case 13: h2 = 'D'; break;
		case 14: h2 = 'E'; break;
		case 15: h2 = 'F'; break;
	}
	RETURN_ON_LOADER_ERROR( WriteBytes( &h2, 1 ) );
	
	return LDR_SUCCESS;
}


nxtIO::LoaderError nxtIO::read_number( unsigned int &result, bool skip ){
	if( skip )
		skip_whitespace();
	
	
	char c = peek();
	if( std::isdigit( c ) ){
		RETURN_ON_LOADER_ERROR( ReadBytes( &c, 1 ) );
		
		if( c == '0' ){
			//It is either in octal or hex and it is not the number "0"
			result = 0;
			c = peek();
			
			if( c == 'x' || c == 'X' ){
				RETURN_ON_LOADER_ERROR( ReadBytes( &c, 1 ) );
				//In hex
				while( remaining_size() ){
					c = peek();
					
					unsigned int hex;
					if( !hex2number( c, hex ) )
						break;
					
					//Was valid, add and move to next character
					result = result * 16 + hex;
					RETURN_ON_LOADER_ERROR( ReadBytes( &c, 1 ) );
				}
			}
			else if( std::isdigit( c ) ){
				//In octal
				while( remaining_size() && std::isdigit( peek() ) && c < '8' ){
					result *= 8;
					RETURN_ON_LOADER_ERROR( ReadBytes( &c, 1 ) );
					result += c - '0';
				}
			}
			
			return LDR_SUCCESS;
		}
		else{
			//The number is in decimal
			result = c - '0'; //Add first digit
			
			while( remaining_size() && std::isdigit( peek() ) ){
				result *= 10;
				RETURN_ON_LOADER_ERROR( ReadBytes( &c, 1 ) );
				result += c - '0';
			}
			
			return LDR_SUCCESS;
		}
	}
	else
		return LDR_TEXT_INVALID;
	
	return LDR_SUCCESS;
}

nxtIO::LoaderError nxtIO::read_text( std::string &text, bool skip ){
	if( skip )
		skip_whitespace();
	
	text = "";
	while( remaining_size() && std::isalpha( peek() ) ){
		char c;
		RETURN_ON_LOADER_ERROR( ReadBytes( &c, 1 ) );
		text += c;
	}
	
	return LDR_SUCCESS;
}


nxtIO::LoaderError nxtIO::skip_whitespace(){
	while( remaining_size() ){
		char c = peek();
		if( std::isspace( c ) ){
			RETURN_ON_LOADER_ERROR( ReadBytes( &c, 1 ) );
		}
		else if( c == '/' ){
			//TODO: implement comments properly
			while( remaining_size() ){
				RETURN_ON_LOADER_ERROR( ReadBytes( &c, 1 ) );
				if( c == '\n' )
					break;
			}
		}
		else
			return LDR_SUCCESS;
	}
	
	return LDR_SUCCESS;
}


bool nxtIO::hex2number( char c, unsigned int &number ){
	if( std::isdigit( c ) )
		number = c - '0';
	else if( c >= 'A' && c <= 'F' )
		number = c - 'A';
	else if( c >= 'a' && c <= 'f' )
		number = c - 'a';
	else
		return false;
	
	return true;
}

