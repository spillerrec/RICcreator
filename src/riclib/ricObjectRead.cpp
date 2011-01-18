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
	rows.read(file);
	columns.read( file );
	
/*	if( image != NULL )	//Why doesn't this work???
		delete[] image; */
	image = new char [rows * columns];
	
	for(unsigned int i=0; i < rows * columns; i++){
		image[i] = file->get();
	}
	
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

