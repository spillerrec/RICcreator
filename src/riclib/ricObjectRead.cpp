#include "ricObject.h"
#include <iostream>
using namespace std;

void ricfile::ricOpOptions::read(ifstream* file){
	options.read(file);
	width.read(file);
	height.read(file);
}


void ricfile::ricOpSprite::read(ifstream* file){
	sprite_ID.read(file);
	unsigned int rows = nxtVariable::read_multibyte( file, 2 );
	unsigned int columns = nxtVariable::read_multibyte( file, 2 );
	
	sprite_data.create( columns*8, rows );
	
	for( int irow=rows-1; irow >= 0; irow--)
		for( unsigned int icolumn=0; icolumn < columns; icolumn++){
			unsigned char raw = file->get();
			
			for( int ibyte=7; ibyte >= 0; ibyte--){
				sprite_data.set_pixel( icolumn*8+ibyte, irow, raw % 2 );
				raw /= 2;
			}
		}
	
	
	//Padding
	if( (rows * columns) % 2 )
		file->get();
}


void ricfile::ricOpVarMap::read(ifstream* file){
	VarMapID.read( file );
	VarMap.read( file );
}



void ricfile::ricOpCopyBits::read(ifstream* file){
	CopyOptions.read( file );
	SpriteID.read( file );
	src.read( file );
	dest.read( file );
}


void ricfile::ricOpPixel::read(ifstream* file){
	CopyOptions.read( file );
	pos.read( file );
	value.read( file );
}


void ricfile::ricOpLine::read(ifstream* file){
	CopyOptions.read( file );
	start.read( file );
	end.read( file );
}


void ricfile::ricOpRectangle::read(ifstream* file){
	CopyOptions.read( file );
	rect.read( file );
}


void ricfile::ricOpCicle::read(ifstream* file){
	CopyOptions.read( file );
	pos.read( file );
	radius.read( file );
}


void ricfile::ricOpNumber::read(ifstream* file){
	CopyOptions.read( file );
	pos.read( file );
	number.read( file );
}

void ricfile::ricOpEllipse::read(ifstream* file){
	CopyOptions.read( file );
	pos.read( file );
	radius_x.read( file );
	radius_y.read( file );
}

void ricfile::ricOpPolygon::read(ifstream* file){
	CopyOptions.read( file );
	points.read( file );
}

