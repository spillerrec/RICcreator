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
#include <iostream>
using namespace std;

void ricfile::ricOpOptions::read(ifstream* file){
	options.read(file);
	width.read(file);
	height.read(file);
}


void ricfile::ricOpSprite::read(ifstream* file){
	sprite_ID.read(file);
	sprite_data.read(file);
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

