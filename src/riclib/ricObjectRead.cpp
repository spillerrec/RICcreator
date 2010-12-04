#include "ricObject.h"
#include <iostream>
using namespace std;

void ricfile::ricOpOptions::read(ifstream* file){
	options.read(file);
	width.read(file);
	height.read(file);
	
	cout << "Options Object read:\n";
	cout << "options: " << options << "\n";
	cout << "width: " << width << "\n";
	cout << "height: " << height << "\n";
	cout << "\n";
}


void ricfile::ricOpSprite::read(ifstream* file){
	sprite_ID.read(file);
	rows.read(file);
	columns.read( file );
	
/*	if( image != NULL )	//Why doesn't this work???
		delete[] image; */
	image = new char [rows * columns];
	
	for(int i=0; i < rows * columns; i++){
		image[i] = file->get();
	}
	
	if( (rows * columns) % 2 )
		file->get();
	
	cout << "Sprite Object read:\n";
	cout << "sprite_ID: " << sprite_ID << "\n";
	cout << "rows: " << rows << "\n";
	cout << "columns: " << columns << "\n";
	for(int iy=0; iy<rows; iy++){
		for(int ix=0; ix<columns; ix++){
			for(int iz=0; iz<8; iz++){
				if(image[iy*columns + ix] & (128>>iz))
					cout << "XX";
				else
					cout << "--";
			}
		}
		cout << "\n";
	}
	cout << "\n";
	
	
}


void ricfile::ricOpVarMap::read(ifstream* file){
	VarMapID.read( file );
	size.read( file );
	
	for(int i=0; i<size; i++){
		point temp;
		temp.X.read( file );
		temp.Y.read( file );
		VarMap.push_back(temp);
	}
	
	cout << "ricVarMap Object read:\n";
	cout << "VarMapID: " << VarMapID << "\n";
	cout << "size: " << size << "\n";
	for(int i=0; i<VarMap.size(); i++){
		cout << "Point: " << VarMap[i].X << " - " << VarMap[i].Y << "\n";
	}
	
	cout << "\n";
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
	
	cout << "ricCopyBits Object read:\n";
	cout << "CopyOptions: " << CopyOptions << "\n";
	cout << "SpriteID: " << SpriteID << "\n";
	cout << "posX: " << posX << "\n";
	cout << "posY: " << posY << "\n";
	cout << "width: " << width << "\n";
	cout << "height: " << height << "\n";
	cout << "relX: " << relX << "\n";
	cout << "relY: " << relY << "\n";
	cout << "\n";
}


void ricfile::ricOpPixel::read(ifstream* file){
	CopyOptions.read( file );
	posX.read( file );
	posY.read( file );
	value.read( file );
	
	cout << "ricPixel Object read:\n";
	cout << "CopyOptions: " << CopyOptions << "\n";
	cout << "posX: " << posX << "\n";
	cout << "posY: " << posY << "\n";
	cout << "value: " << value << "\n";
	cout << "\n";
}


void ricfile::ricOpLine::read(ifstream* file){
	CopyOptions.read( file );
	startX.read( file );
	startY.read( file );
	endX.read( file );
	endY.read( file );
	
	cout << "ricLine Object read:\n";
	cout << "CopyOptions: " << CopyOptions << "\n";
	cout << "startX: " << startX << "\n";
	cout << "startY: " << startY << "\n";
	cout << "endX: " << endX << "\n";
	cout << "endY: " << endY << "\n";
	cout << "\n";
}


void ricfile::ricOpRectangle::read(ifstream* file){
	CopyOptions.read( file );
	posX.read( file );
	posY.read( file );
	width.read( file );
	height.read( file );
	
	cout << "ricRectangle Object read:\n";
	cout << "CopyOptions: " << CopyOptions << "\n";
	cout << "posX: " << posX << "\n";
	cout << "posY: " << posY << "\n";
	cout << "width: " << width << "\n";
	cout << "height: " << height << "\n";
	cout << "\n";
}


void ricfile::ricOpCicle::read(ifstream* file){
	CopyOptions.read( file );
	posX.read( file );
	posY.read( file );
	radius.read( file );
	
	cout << "ricCicle Object read:\n";
	cout << "CopyOptions: " << CopyOptions << "\n";
	cout << "posX: " << posX << "\n";
	cout << "posY: " << posY << "\n";
	cout << "radius: " << radius << "\n";
	cout << "\n";
}


void ricfile::ricOpNumber::read(ifstream* file){
	CopyOptions.read( file );
	posX.read( file );
	posY.read( file );
	number.read( file );
	
	cout << "ricNumber Object read:\n";
	cout << "CopyOptions: " << CopyOptions << "\n";
	cout << "posX: " << posX << "\n";
	cout << "posY: " << posY << "\n";
	cout << "number: " << number << "\n";
	cout << "\n";
}



