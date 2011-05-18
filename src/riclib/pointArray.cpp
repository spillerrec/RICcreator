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

#include "pointArray.h"


pointArray::pointArray( bool keep_sorted ){
	this->keep_sorted = keep_sorted;
}


bool pointArray::add( point &p1 ){
	if( keep_sorted ){
		if( is_set( p1.X ) )
			return false;
		else{
			unsigned int index = find( p1.X );
			if( index == INVALID_INDEX ){
				insert( 0, p1 );
			}
			else{
				insert( index+1, p1 );
			}
			return true;
		}
	}
	else{
		VarMap.push_back( p1 );
		return true;
	}
	return false;
}

bool pointArray::add( point &p1, unsigned int at_index ){
	if( keep_sorted )
		return false; //Not allowed
	else{
		insert( at_index, p1 );
		return true;
	}
}

void pointArray::insert( unsigned int x, point p1 ){
	vector<point>::iterator it = VarMap.begin();
	it += x;
	VarMap.insert( it, p1 );
}


unsigned int pointArray::find( unsigned int x ) const{
	unsigned int index_closest = INVALID_INDEX;
	unsigned int value_of_closest;
	for(unsigned int i=0; i<VarMap.size(); i++){
		if( (unsigned int)VarMap[i].X == x )
			return i;
		if( (unsigned int)VarMap[i].X < x ){
			if( index_closest != INVALID_INDEX ){
				if( value_of_closest < (unsigned int)VarMap[i].X ){
					index_closest = i;
					value_of_closest = VarMap[i].X;
				}
			}
			else{
				index_closest = i;
				value_of_closest = VarMap[i].X;
			}
		}
	}
	
	//No match found, return the closest
	return index_closest; //This will automatically be INVALID_INDEX if no closest was found
}


bool pointArray::is_set( unsigned int x ) const{
	for(unsigned int i=0; i<(unsigned int)VarMap.size(); i++){
		if( (unsigned int)VarMap[i].X == x )
			return true;
	}
	
	return false; //none found
}


unsigned int pointArray::value( unsigned int x ) const{
	if( VarMap.size() == 0 )
		return 0;
	
	unsigned int prev_x = VarMap[0].X;
	unsigned int prev_y = VarMap[0].Y;
	
	if( prev_x >= x )
		return prev_y;
	
	for(unsigned int i=1; i<VarMap.size(); i++){
		if( VarMap[i].X > x ){
			return prev_y + ( ( x-prev_x )*( VarMap[i].Y-prev_y ) ) / ( VarMap[i].X - prev_x );	//equation from NXT firmware code
		}
		else{
			prev_x = VarMap[i].X;
			prev_y = VarMap[i].Y;
		}
	}
	
	return prev_y;
}


void pointArray::read(ifstream* file){
	nxtVarWord size;
	size.read( file );
	
	for(unsigned int i=0; i<size; i++){
		point temp;
		temp.X.read( file );
		temp.Y.read( file );
		add(temp);
	}
}


void pointArray::write(ofstream* file) const{
	nxtVariable::write_multibyte( file, VarMap.size(), 2 );
	for(unsigned int i=0; i<VarMap.size(); i++){
		VarMap[i].X.write(file);
		VarMap[i].Y.write(file);
	}
}


const point* pointArray::index( unsigned int x ) const{
	if( x < size() )
		return &VarMap[x];
	else
		return 0;
}


