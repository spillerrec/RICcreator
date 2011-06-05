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
	sprite_data.write(file);
	
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

