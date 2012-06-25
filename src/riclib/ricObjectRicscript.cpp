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


#include "ricObjectChildren.h"
#include "ricfile.h"


#define SET_INTERGER( result ) { unsigned int value; RETURN_ON_LOADER_ERROR( file->read_number( value ) ); result = value; }
#define GOTO_NEXT_PARAMETER() { char c; RETURN_ON_LOADER_ERROR( file->skip_whitespace() ); RETURN_ON_LOADER_ERROR( file->ReadBytes( &c, 1 ) ); qDebug( "next: %c", c ); if( c != ',' ) return nxtIO::LDR_TEXT_INVALID; }
#define SET_COPYOPTIONS( result ) { SET_INTERGER( ((nxtVarRicWord)result) ) }

#include <QObject>

static nxtIO::LoaderError read_ricword( nxtIO* file, nxtVarRicWord &var, bool separator = true ){
	//Try to see if it is arg( y ) or maparg( x, y )
	std::string text;
	RETURN_ON_LOADER_ERROR( file->read_text( text ) );
	if( text == "arg" || text == "maparg" ){
		unsigned int map = 0;
		unsigned int arg = 0;
		char c;
		
		//Read starting '('
		RETURN_ON_LOADER_ERROR( file->skip_whitespace() );
		RETURN_ON_LOADER_ERROR( file->ReadBytes( &c, 1 ) );
		if( c != '(' )
			return nxtIO::LDR_TEXT_INVALID;
		
		if( text == "maparg" ){
			//Read map
			RETURN_ON_LOADER_ERROR( file->read_number( map ) );
			GOTO_NEXT_PARAMETER();
		}
		
		//Read arg
		RETURN_ON_LOADER_ERROR( file->read_number( arg ) );
		
		//Set var
		var.set_extended( map, arg );
		
		//Read ending ')'
		RETURN_ON_LOADER_ERROR( file->skip_whitespace() );
		RETURN_ON_LOADER_ERROR( file->ReadBytes( &c, 1 ) );
		if( c != ')' )
			return nxtIO::LDR_TEXT_INVALID;
	}
	else if( text == "" ){
		//Normal number
		SET_INTERGER( var );
	}
	else
		return nxtIO::LDR_TEXT_INVALID; //Not a ricword
	
	if( separator ){
		GOTO_NEXT_PARAMETER();
	}
	
	return nxtIO::LDR_SUCCESS;
}


nxtIO::LoaderError ricOpOptions::ricscript_read( nxtIO* file ){
	
	SET_INTERGER( options );
	GOTO_NEXT_PARAMETER();
	
	SET_INTERGER( width );
	GOTO_NEXT_PARAMETER();
	
	SET_INTERGER( height );
	
	return nxtIO::LDR_SUCCESS;
}

nxtIO::LoaderError ricOpSprite::ricscript_read( nxtIO* file ){
	SET_INTERGER( sprite_ID );
	GOTO_NEXT_PARAMETER();
	
	//Start reading binary data
	int x=0, y=0;
	sprite_data.set_auto_resize( true );
	
	while( file->remaining_size() && y >= 0 ){
		RETURN_ON_LOADER_ERROR( file->skip_whitespace() );
		
		char c = file->peek();
		x = 0;
		
		if( c == '0' ){
			//It might be hex, check for 'x'
			RETURN_ON_LOADER_ERROR( file->ReadBytes( &c, 1 ) );
			c = file->peek();
			if( c == 'x' || c == 'X' ){
				//It is hex
				RETURN_ON_LOADER_ERROR( file->ReadBytes( &c, 1 ) );
				
				
				while( file->remaining_size() ){
					unsigned int hex;
					c = file->peek();
					
					if( nxtIO::hex2number( c, hex ) ){
						unsigned int mask = 0x8;
						for( int i=0; i<4; i++, x++ ){
							if( hex & mask )
								sprite_data.PointOut( x,y );
							mask >>= 1;
						}
						RETURN_ON_LOADER_ERROR( file->ReadBytes( &c, 1 ) );
					}
					else if( std::isspace( c ) || c == ',' ){
						//We hit the end of this line, go to next
						RETURN_ON_LOADER_ERROR( file->skip_whitespace() );
						RETURN_ON_LOADER_ERROR( file->ReadBytes( &c, 1 ) );
						break;
					}
					else if( c == ')' ){
						y = -1;
						sprite_data.set_auto_resize( false );
						break;
					}
					else{qDebug( "sprite reading failed: %c", c );
						return nxtIO::LDR_TEXT_INVALID;
						}
				}
				
				y++;
				continue; //Skip the binary reading
			}
			
		}
		else if( c == '1' ){
			//It is binary, add the first point
			sprite_data.PointOut( x, y );
			//Next points will be after this if construct
		}
		else if( c == ')' ){	//Sprite data ended
			sprite_data.set_auto_resize( false );
			return nxtIO::LDR_SUCCESS;
		}
		else	//Not sprite data!
			return nxtIO::LDR_TEXT_INVALID;
		
		x++;	//One point have been added already
		
		//It wasn't in hex, or the first point have been plotted
		//It must be in binary
		while( file->remaining_size() ){
			c = file->peek();
			if( c == '1' )
				sprite_data.PointOut( x, y );
			else if( std::isspace( c ) || c == ',' ){
				//We hit the end of this line, go to next
				RETURN_ON_LOADER_ERROR( file->skip_whitespace() );
				RETURN_ON_LOADER_ERROR( file->ReadBytes( &c, 1 ) );
				break;
			}
			else if( c == ')' ){
				y = -1;
				sprite_data.set_auto_resize( false );
				break;
			}
			else if( c != '1' )
				return nxtIO::LDR_TEXT_INVALID;
			
			//Read the character from peek()
			RETURN_ON_LOADER_ERROR( file->ReadBytes( &c, 1 ) );
			x++;
		}
		
		y++;
	}
	
	return nxtIO::LDR_SUCCESS;
}

nxtIO::LoaderError ricOpVarMap::ricscript_read( nxtIO* file ){
	
	return nxtIO::LDR_SUCCESS;
}

nxtIO::LoaderError ricOpCopyBits::ricscript_read( nxtIO* file ){
	SET_COPYOPTIONS( CopyOptions );
	GOTO_NEXT_PARAMETER();
	
	read_ricword( file, SpriteID );
	read_ricword( file, src.pos.X );
	read_ricword( file, src.pos.Y );
	read_ricword( file, src.width );
	read_ricword( file, src.height );
	read_ricword( file, dest.X );
	read_ricword( file, dest.Y, false );
	
	return nxtIO::LDR_SUCCESS;
}


nxtIO::LoaderError ricOpPixel::ricscript_read( nxtIO* file ){
	SET_COPYOPTIONS( CopyOptions );
	GOTO_NEXT_PARAMETER();
	
	read_ricword( file, pos.X );
	read_ricword( file, pos.Y );
	read_ricword( file, value, false );
	
	//TODO: make value optional
	
	return nxtIO::LDR_SUCCESS;
}

nxtIO::LoaderError ricOpLine::ricscript_read( nxtIO* file ){
	SET_COPYOPTIONS( CopyOptions );
	GOTO_NEXT_PARAMETER();
	
	read_ricword( file, start.X );
	read_ricword( file, start.Y );
	read_ricword( file, end.X );
	read_ricword( file, end.Y, false );
	
	return nxtIO::LDR_SUCCESS;
}

nxtIO::LoaderError ricOpRectangle::ricscript_read( nxtIO* file ){
	SET_COPYOPTIONS( CopyOptions );
	GOTO_NEXT_PARAMETER();
	
	read_ricword( file, rect.pos.X );
	read_ricword( file, rect.pos.Y );
	read_ricword( file, rect.width );
	read_ricword( file, rect.height, false );
	
	return nxtIO::LDR_SUCCESS;
}

nxtIO::LoaderError ricOpCircle::ricscript_read( nxtIO* file ){
	SET_COPYOPTIONS( CopyOptions );
	GOTO_NEXT_PARAMETER();
	
	read_ricword( file, pos.X );
	read_ricword( file, pos.Y );
	read_ricword( file, radius, false );
	
	return nxtIO::LDR_SUCCESS;
}

nxtIO::LoaderError ricOpNumber::ricscript_read( nxtIO* file ){
	SET_COPYOPTIONS( CopyOptions );
	GOTO_NEXT_PARAMETER();
	
	read_ricword( file, pos.X );
	read_ricword( file, pos.Y );
	read_ricword( file, number, false );
	
	return nxtIO::LDR_SUCCESS;
}


nxtIO::LoaderError ricOpEllipse::ricscript_read( nxtIO* file ){
	SET_COPYOPTIONS( CopyOptions );
	GOTO_NEXT_PARAMETER();
	
	read_ricword( file, pos.X );
	read_ricword( file, pos.Y );
	read_ricword( file, radius_x );
	read_ricword( file, radius_y, false );
	
	return nxtIO::LDR_SUCCESS;
}


nxtIO::LoaderError ricOpPolygon::ricscript_read( nxtIO* file ){
	SET_COPYOPTIONS( CopyOptions );
	GOTO_NEXT_PARAMETER();
	
	return nxtIO::LDR_SUCCESS;
}

