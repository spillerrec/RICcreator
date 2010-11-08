#include "ricObject.h"
#include <iostream>
using namespace std;

void ricOpOptions::read(char* file, unsigned int pos){
	options = word(file, pos);
	width = word(file, pos);
	height = word(file, pos);
	
	cout << "Options Object read:\n";
	cout << "options: " << options << "\n";
	cout << "width: " << width << "\n";
	cout << "height: " << height << "\n";
	cout << "\n";
}


void ricOpSprite::read(char* file, unsigned int pos){
	sprite_ID = word(file, pos);
	rows = word(file, pos);
	columns = word(file, pos);
	
/*	if( image != NULL )	//Why doesn't this work???
		delete[] image; */
	image = new char [rows * columns];
	
	for(int i=0; i < rows * columns; i++){
		image[i] = file[pos];
		pos++;
	}
	
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


void ricOpVarMap::read(char* file, unsigned int pos){
	VarMapID = word(file, pos);
	size = word(file, pos);
	
	for(int i=0; i<size; i++){
		point temp;
		temp.X = word(file, pos);
		temp.Y = word(file, pos);
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



void ricOpCopyBits::read(char* file, unsigned int pos){
	CopyOptions = word(file, pos);
	SpriteID = word(file, pos);
	posX = word(file, pos);
	posY = word(file, pos);
	width = word(file, pos);
	height = word(file, pos);
	relX = word(file, pos);
	relY = word(file, pos);
	
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


void ricOpPixel::read(char* file, unsigned int pos){
	CopyOptions = word(file, pos);
	posX = word(file, pos);
	posY = word(file, pos);
	value = word(file, pos);
	
	cout << "ricPixel Object read:\n";
	cout << "CopyOptions: " << CopyOptions << "\n";
	cout << "posX: " << posX << "\n";
	cout << "posY: " << posY << "\n";
	cout << "value: " << value << "\n";
	cout << "\n";
}


void ricOpLine::read(char* file, unsigned int pos){
	CopyOptions = word(file, pos);
	startX = word(file, pos);
	startY = word(file, pos);
	endX = word(file, pos);
	endY = word(file, pos);
	
	cout << "ricLine Object read:\n";
	cout << "CopyOptions: " << CopyOptions << "\n";
	cout << "startX: " << startX << "\n";
	cout << "startY: " << startY << "\n";
	cout << "endX: " << endX << "\n";
	cout << "endY: " << endY << "\n";
	cout << "\n";
}


void ricOpRectangle::read(char* file, unsigned int pos){
	CopyOptions = word(file, pos);
	posX = word(file, pos);
	posY = word(file, pos);
	width = word(file, pos);
	height = word(file, pos);
	
	cout << "ricRectangle Object read:\n";
	cout << "CopyOptions: " << CopyOptions << "\n";
	cout << "posX: " << posX << "\n";
	cout << "posY: " << posY << "\n";
	cout << "width: " << width << "\n";
	cout << "height: " << height << "\n";
	cout << "\n";
}


void ricOpCicle::read(char* file, unsigned int pos){
	CopyOptions = word(file, pos);
	posX = word(file, pos);
	posY = word(file, pos);
	radius = word(file, pos);
	
	cout << "ricCicle Object read:\n";
	cout << "CopyOptions: " << CopyOptions << "\n";
	cout << "posX: " << posX << "\n";
	cout << "posY: " << posY << "\n";
	cout << "radius: " << radius << "\n";
	cout << "\n";
}


void ricOpNumber::read(char* file, unsigned int pos){
	CopyOptions = word(file, pos);
	posX = word(file, pos);
	posY = word(file, pos);
	number = word(file, pos);
	
	cout << "ricNumber Object read:\n";
	cout << "CopyOptions: " << CopyOptions << "\n";
	cout << "posX: " << posX << "\n";
	cout << "posY: " << posY << "\n";
	cout << "number: " << number << "\n";
	cout << "\n";
}



