#include "ricObject.h"

int ricOptions::write(ofstream* file){
	write_header(file);
	write_word(file, options);
	write_word(file, width);
	write_word(file, height);
}


int ricSprite::write(ofstream* file){
	write_header(file);
	write_word(file, sprite_ID);
	write_word(file, rows);
	write_word(file, columns);
	
	file->write( image, rows * columns );
	if( (rows * columns) % 2 )
		file->write( (char*)0, 1 );
}


int ricVarMap::write(ofstream* file){
	write_header(file);
	write_word(file, VarMapID);
	write_word(file, size);
	
	for(int i=0; i<VarMap.size(); i++){
		write_word(file, VarMap[i].X);
		write_word(file, VarMap[i].Y);
	}
}


int ricCopyBits::write(ofstream* file){
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


int ricPixel::write(ofstream* file){
	write_header(file);
	write_word(file, CopyOptions);
	write_word(file, posX);
	write_word(file, posY);
	write_word(file, value);
}


int ricLine::write(ofstream* file){
	write_header(file);
	write_word(file, CopyOptions);
	write_word(file, startX);
	write_word(file, startY);
	write_word(file, endX);
	write_word(file, endY);
}


int ricRectangle::write(ofstream* file){
	write_header(file);
	write_word(file, CopyOptions);
	write_word(file, posX);
	write_word(file, posY);
	write_word(file, width);
	write_word(file, height);
}


int ricCicle::write(ofstream* file){
	write_header(file);
	write_word(file, CopyOptions);
	write_word(file, posX);
	write_word(file, posY);
	write_word(file, radius);
}


int ricNumber::write(ofstream* file){
	write_header(file);
	write_word(file, CopyOptions);
	write_word(file, posX);
	write_word(file, posY);
	write_word(file, number);
}

