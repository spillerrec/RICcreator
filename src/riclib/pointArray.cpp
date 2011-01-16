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

int pointArray::write(ofstream* file){
	for(unsigned int i=0; i<VarMap.size(); i++){
		VarMap[i].X.write(file);
		VarMap[i].Y.write(file);
	}
	
	return 0;
}


