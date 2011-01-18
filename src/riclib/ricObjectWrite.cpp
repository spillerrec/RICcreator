#include "ricObject.h"

int ricfile::ricOpOptions::write(ofstream* file) const{
	write_header(file);
	options.write(file);
	width.write(file);
	height.write(file);
	return 0;
}


int ricfile::ricOpSprite::write(ofstream* file) const{
	write_header(file);
	sprite_ID.write(file);
	rows.write(file);
	columns.write(file);
	
	file->write( image, rows * columns );
	char temp[1];
	temp[0] = 0;
	if( (rows * columns) % 2 )
		file->write( temp, 1 );
	
	return 0;
}


int ricfile::ricOpVarMap::write(ofstream* file) const{
	write_header(file);
	VarMapID.write(file);
	VarMap.write( file );
	
	return 0;
}


int ricfile::ricOpCopyBits::write(ofstream* file) const{
	write_header(file);
	CopyOptions.write(file);
	SpriteID.write(file);
	src.write(file);
	dest.write(file);
	
	return 0;
}


int ricfile::ricOpPixel::write(ofstream* file) const{
	write_header(file);
	CopyOptions.write(file);
	pos.write(file);
	value.write(file);
	
	return 0;
}


int ricfile::ricOpLine::write(ofstream* file) const{
	write_header(file);
	CopyOptions.write(file);
	start.write(file);
	end.write(file);
	
	return 0;
}


int ricfile::ricOpRectangle::write(ofstream* file) const{
	write_header(file);
	CopyOptions.write( file );
	rect.write( file );
	
	return 0;
}


int ricfile::ricOpCicle::write(ofstream* file) const{
	write_header(file);
	CopyOptions.write(file);
	pos.write(file);
	radius.write(file);
	
	return 0;
}


int ricfile::ricOpNumber::write(ofstream* file) const{
	write_header(file);
	CopyOptions.write(file);
	pos.write(file);
	number.write(file);
	
	return 0;
}


int ricfile::ricOpEllipse::write(ofstream* file) const{
	write_header(file);
	CopyOptions.write(file);
	pos.write(file);
	radius_x.write(file);
	radius_y.write(file);
	
	return 0;
}

int ricfile::ricOpPolygon::write(ofstream* file) const{
	write_header(file);
	CopyOptions.write(file);
	points.write(file);
	
	return 0;
}

