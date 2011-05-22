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
	
	public:
		pointArray( bool keep_sorted = false );
		~pointArray();
		
		unsigned int size() const{ return VarMap.size(); }
		unsigned int filesize() const{ return 2 + size()*4; }
		
		//Functions for manipulating the array
	//	bool remove( unsigned int index );
		bool add( point &p1 );
		bool add( point &p1, unsigned int at_index );
	//	bool move( unsigned int at_index, unsigned int to_index );
	//	bool set( unsigned int x, unsigned int y );	//Set a specific index to value, create a new if it doesn't exists
		bool is_set( unsigned int x ) const;
		unsigned int find( unsigned int x ) const; //Returns index to the point where x eqaults point.X, if not found it returns the point with the biggest X value less than x, if none, it returns INVALID_INDEX
		
		unsigned int var_type() const{ return TYPE_POINT_ARRAY; }
	//	unsigned int var_amount() const{ return 1 + size()*2; }	//This wouldn't go well with ricObjectModel...
		void read(ifstream* file);
		void write(ofstream* file) const;
		
		unsigned int value( unsigned int x ) const;
		
		const point* index( unsigned int x ) const;
};


#endif
