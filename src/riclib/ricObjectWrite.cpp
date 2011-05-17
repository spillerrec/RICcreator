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
	
	//Find the sizes of the bitmap to write and save it to the file
	unsigned int rows = sprite_data.get_height();
	unsigned int columns = get_columns();
	nxtVariable::write_multibyte( file, rows, 2 );
	nxtVariable::write_multibyte( file, columns, 2 );
	
	//Write the bitmap
	char temp[1];
	for( int irow=rows-1; irow >= 0; irow--)
		for( unsigned int icolumn=0; icolumn < columns; icolumn++ ){
			temp[0] = 0;
			
			//Contruct a byte of sprite
			for( int ibyte=0; ibyte < 8; ibyte++){
				temp[0] *= 2;	//The bits need to be shiftet, placed in the start as the first iteration will have no effekt on this value
				
				if( sprite_data.get_pixel( icolumn*8+ibyte, irow ) )
					temp[0] += 1;
			}
			
			//Write the byte to the file
			file->write( temp, 1 );
		}
	
	
	//Write padding byte
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

