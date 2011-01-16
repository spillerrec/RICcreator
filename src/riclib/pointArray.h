#ifndef ABOUT_H
#define ABOUT_H

#include <vector>
#include <fstream>
using namespace std;

#include "nxtVariable.h"


class pointArray{
	public:
		
		static const unsigned int INVALID_INDEX = 0-1; //make sure this is correct
		
	private:
		vector<point> VarMap;
		bool keep_sorted;
		
		void insert( unsigned int x, point p1 );
	
	public:
		pointArray( bool keep_sorted = false );
		~pointArray();
		
		//Functions for manipulating the array
	//	bool remove( unsigned int index );
		bool add( point &p1 );
		bool add( point &p1, unsigned int at_index );
	//	bool move( unsigned int at_index, unsigned int to_index );
	//	bool set( unsigned int x, unsigned int y );	//Set a specific index to value, create a new if it doesn't exists
		bool is_set( unsigned int x ) const;
		unsigned int find( unsigned int x ) const; //Returns index to the point where x eqaults point.X, if not found it returns the point with the biggest X value less than x, if none, it returns INVALID_INDEX
		
		int write(ofstream* file);
		
		unsigned int value( unsigned int x ) const;
};


#endif
