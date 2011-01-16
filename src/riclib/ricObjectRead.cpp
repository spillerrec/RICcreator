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
	size.read( file );
	
	for(unsigned int i=0; i<size; i++){
		point temp;
		temp.X.read( file );
		temp.Y.read( file );
		VarMap.add(temp);
	}
}



void ricfile::ricOpCopyBits::read(ifstream* file){
	CopyOptions.read( file );
	SpriteID.read( file );
	posX.read( file );
	posY.read( file );
	width.read( file );
	height.read( file );
	relX.read( file );
	relY.read( file );
}


void ricfile::ricOpPixel::read(ifstream* file){
	CopyOptions.read( file );
	posX.read( file );
	posY.read( file );
	value.read( file );
}


void ricfile::ricOpLine::read(ifstream* file){
	CopyOptions.read( file );
	startX.read( file );
	startY.read( file );
	endX.read( file );
	endY.read( file );
}


void ricfile::ricOpRectangle::read(ifstream* file){
	CopyOptions.read( file );
	posX.read( file );
	posY.read( file );
	width.read( file );
	height.read( file );
}


void ricfile::ricOpCicle::read(ifstream* file){
	CopyOptions.read( file );
	posX.read( file );
	posY.read( file );
	radius.read( file );
}


void ricfile::ricOpNumber::read(ifstream* file){
	CopyOptions.read( file );
	posX.read( file );
	posY.read( file );
	number.read( file );
}

void ricfile::ricOpEllipse::read(ifstream* file){
	CopyOptions.read( file );
	posX.read( file );
	posY.read( file );
	radius_x.read( file );
	radius_y.read( file );
}



