#include "ricObject.h"

int ricfile::ricOpOptions::write(ofstream* file){
	write_header(file);
	write_word(file, options);
	write_word(file, width);
	write_word(file, height);
}


int ricfile::ricOpSprite::write(ofstream* file){
	write_header(file);
	write_word(file, sprite_ID);
	write_word(file, rows);
	write_word(file, columns);
	
	file->write( image, rows * columns );
	char temp[1];
	temp[0] = 0;
	if( (rows * columns) % 2 )
		file->write( temp, 1 );
}


int ricfile::ricOpVarMap::write(ofstream* file){
	write_header(file);
	write_word(file, VarMapID);
	write_word(file, size);
	
	for(int i=0; i<VarMap.size(); i++){
		write_word(file, VarMap[i].X);
		write_word(file, VarMap[i].Y);
	}
}


int ricfile::ricOpCopyBits::write(ofstream* file){
	write_header(file);
	write_word(file, CopyOptions);
	write_word(file, SpriteID);
	write_word(file, posX);
	write_word(file, posY);
	write_word(file, width);
	write_word(file, height);
	write_word(file, relX);
	write_word(file, relY);
}


int ricfile::ricOpPixel::write(ofstream* file){
	write_header(file);
	write_word(file, CopyOptions);
	write_word(file, posX);
	write_word(file, posY);
	write_word(file, value);
}


int ricfile::ricOpLine::write(ofstream* file){
	write_header(file);
	write_word(file, CopyOptions);
	write_word(file, startX);
	write_word(file, startY);
	write_word(file, endX);
	write_word(file, endY);
}


int ricfile::ricOpRectangle::write(ofstream* file){
	write_header(file);
	write_word(file, CopyOptions);
	write_word(file, posX);
	write_word(file, posY);
	write_word(file, width);
	write_word(file, height);
}


int ricfile::ricOpCicle::write(ofstream* file){
	write_header(file);
	write_word(file, CopyOptions);
	write_word(file, posX);
	write_word(file, posY);
	write_word(file, radius);
}


int ricfile::ricOpNumber::write(ofstream* file){
	write_header(file);
	write_word(file, CopyOptions);
	write_word(file, posX);
	write_word(file, posY);
	write_word(file, number);
}

