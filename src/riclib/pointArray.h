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

#ifndef POINTARRAY_H
#define POINTARRAY_H

#include <vector>
#include <fstream>
using namespace std;

#include "nxtVariable.h"

struct point{
	nxtVarWord X;
	nxtVarWord Y;
};

class pointArray: public nxtVariable{
	public:
		static const unsigned int INVALID_INDEX = 0-1; //make sure this is correct
		
	private:
		vector<point> VarMap;
		bool keep_sorted;
		
		void insert( unsigned int x, point p1 );
		
		void swap( unsigned int index1, unsigned int index2 );
		void order_swap( unsigned int from_index, unsigned int to_index );
	
	
	public:
		pointArray( bool keep_sorted = false );
		
	
	//nxtVariable virtual functions
	public:
		unsigned int size() const{ return VarMap.size(); }
		unsigned int filesize() const{ return 2 + size()*4; }
		unsigned int var_type() const{ return TYPE_POINT_ARRAY; }
	//	unsigned int var_amount() const{ return 1 + size()*2; }	//This wouldn't go well with ricObjectModel...
		nxtIO::LoaderError read( nxtIO* file );
		nxtIO::LoaderError write( nxtIO* file ) const;
		
	
	//Functions to access the array
	public:
		bool get_keep_sorted() const{ return keep_sorted; }
		bool is_set( unsigned int x ) const;
		unsigned int value( unsigned int x ) const;
		const point* index( unsigned int index ) const;
		
	
	//Functions to manipulate the array
	public:
		//Insert a new point just after "after_index". The point's actual index is returned.
		//If keep_sorted then x = after_index.x + 1 and y = value( after_index + 1 )
		//If not, then x = after_index.x and y = after_index.y
		unsigned int new_point( unsigned int after_index );
		
		void remove( unsigned int index );	//Removes the point at "index"
		
		//Moves the point at "from_index" to "to_index". If keep_sorted == true this does nothing.
		//Returns true on successful move
		bool move( unsigned int from_index, unsigned int to_index );
		
		//Change the x value of the point at "index" to "value"
		//If keep_sorted == true then the index and value might automatically change.
		//If so, the index offset is returned.
		int change_x( unsigned int index, unsigned int value );
		
		//Change the y value of the point at "index" to "value"
		void change_y( unsigned int index, unsigned int value );
		
		//Remove points that are in line between two other points in a x-y coordinate system
		void optimize_map();
};


#endif
