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

#include <math.h>	//For atan2 in optimize()


pointArray::pointArray( bool keep_sorted ){
	this->keep_sorted = keep_sorted;
}


void pointArray::insert( unsigned int index, point p1 ){
	vector<point>::iterator it = VarMap.begin();
	it += index;
	VarMap.insert( it, p1 );
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
			return prev_y + (int)( ( x-prev_x )*( VarMap[i].Y-prev_y ) ) / (int)( VarMap[i].X - prev_x );	//equation from NXT firmware code
		}
		else{
			prev_x = VarMap[i].X;
			prev_y = VarMap[i].Y;
		}
	}
	
	return prev_y;
}


nxtIO::LoaderError pointArray::read( nxtIO* file ){
	nxtVarWord size;
	RETURN_ON_LOADER_ERROR( size.read( file ) );
	
	for(unsigned int i=0; i<size; i++){
		point temp;
		RETURN_ON_LOADER_ERROR( temp.X.read( file ) );
		RETURN_ON_LOADER_ERROR( temp.Y.read( file ) );
		VarMap.push_back( temp );
	}
	
	return nxtIO::LDR_SUCCESS;
}


nxtIO::LoaderError pointArray::write( nxtIO* file ) const{
	RETURN_ON_LOADER_ERROR( file->write_multibyte_unsigned( 2, VarMap.size() ) );
	for(unsigned int i=0; i<VarMap.size(); i++){
		RETURN_ON_LOADER_ERROR( VarMap[i].X.write(file) );
		RETURN_ON_LOADER_ERROR( VarMap[i].Y.write(file) );
	}
	
	return nxtIO::LDR_SUCCESS;
}


const point* pointArray::index( unsigned int x ) const{
	if( x < size() )
		return &VarMap[x];
	else
		return 0;
}

//Swap the contents of two points
void pointArray::swap( unsigned int index1, unsigned int index2 ){
	point temp = VarMap[ index2 ];
	VarMap[ index2 ] = VarMap[ index1 ];
	VarMap[ index1] = temp;
}

//Move a point from "from_index" to "to_index" and move all the other points inbetween
void pointArray::order_swap( unsigned int from_index, unsigned int to_index ){
	if( to_index > from_index ){
		for( unsigned int i = from_index; i < to_index; i++ )
			swap( i, i+1 );
	}
	else{
		for( unsigned int i = from_index; i > to_index; i-- )
			swap( i, i-1 );
	}
}

unsigned int pointArray::new_point( unsigned int after_index ){
	point new_point;
		
	//Make sure "after_index" is valid
	if( after_index > VarMap.size() || after_index == INVALID_INDEX ){
		//Just insert an empty point
		new_point.X = 0;
		new_point.Y = 0;
		VarMap.push_back( new_point );
		return VarMap.size() - 1;
	}
	
	if( keep_sorted ){
		//Find the new point's position
		unsigned int value = VarMap[ after_index ].X + 1;
		unsigned int idx_new = after_index + 1;
		for( ; idx_new < VarMap.size(); idx_new++ ){
			if( value == VarMap[ idx_new ].X )
				value++;
			else
				break;
		}
		
		//Calculate new point and add
		new_point.X = value;
		new_point.Y = this->value( value );
		insert( idx_new, new_point );
		return idx_new;
	}
	else{
		//Copy "after_index" and place it right after it
		insert( after_index + 1, VarMap[ after_index ] );
		return after_index + 1;
	}
}


void pointArray::remove( unsigned int index ){
	order_swap( index, VarMap.size()-1 );
	VarMap.pop_back();
}


bool pointArray::move( unsigned int from_index, unsigned int to_index ){
	if( keep_sorted )
		return false;
	else if( from_index < VarMap.size() && to_index < VarMap.size() ){
		order_swap( from_index, to_index );
		return true;
	}
	return false;
}


int pointArray::change_x( unsigned int index, unsigned int value ){
	if( index < VarMap.size() ){
		if( keep_sorted ){
			unsigned int old_value = VarMap[ index ].X;
			if( old_value == value )
				return 0;
			
			//Find new index to the point
			unsigned int idx_new = index;
			if( old_value < value ){
				for( unsigned int idx = idx_new+1; idx < VarMap.size(); idx++ ){
					if( value < VarMap[idx].X )
						break;
					
					//Make sure it doesn't overlap another point
					if( value == VarMap[idx].X )
						value++;
					
					idx_new = idx;
				}
			}
			else{
				for( int idx = idx_new-1; idx >= 0; idx-- ){
					if( value > VarMap[idx].X )
						break;
					
					//Make sure it doesn't overlap another point
					if( value == VarMap[idx].X )
						value--;
					
					idx_new = idx;
				}
			}
			
			//Move point
			int offset = idx_new - index;
			if( offset != 0 )
				order_swap( index, idx_new );
			
			//Assign new x value
			VarMap[ idx_new ].X = value;
			return offset;
		}
		else{
			VarMap[ index ].X = value;
			return 0;
		}
	}
	
	return 0;
}


void pointArray::change_y( unsigned int index, unsigned int value ){
	if( VarMap.size() > index )
		VarMap[ index ].Y = value;
}


void pointArray::optimize_map(){
	if( keep_sorted ){
		for( unsigned int i=0; i+2 < VarMap.size(); i++ ){
			point p0 = VarMap[i];
			point p1 = VarMap[i+1];
			point p2 = VarMap[i+2];
			
			if( i == 0 ){
				if( p0.Y == p1.Y ){
					//Before and after last point the value doesn't change
					remove( i );
					i--;
					continue;
				}
			}
			//TODO: catch last point too in similar fasion
			
			unsigned int Y = p0.Y + (int)( ( p1.X-p0.X )*( p2.Y - p0.Y ) ) / (int)( p2.X - p0.X );
			unsigned int Y2 = p0.Y + (int)( ( p2.X-p0.X )*( p1.Y - p0.Y ) ) / (int)( p1.X - p0.X );
			
			if( Y == p1.Y && Y2 == p2.Y ){
				remove( i+1 );
				i--;
			}
		}
	}
	else{
		for( unsigned int i=0; i+2 < VarMap.size(); i++ ){
			point p0 = VarMap[i];
			point p1 = VarMap[i+1];
			point p2 = VarMap[i+2];
			
			//Remove points that are the same
			if( p0.X == p1.X && p0.Y == p1.Y ){
				remove( i+1 );
				i--;
				continue;
			}
			
			//p0 == p1, yet p0 != p1, nothing to do here
			if( p0.X == p2.X && p0.Y && p2.Y )
				continue;
			
			//Make sure the points are in order
			if( !( p0.X >= p1.X && p1.X >= p2.X ) && !( p0.X <= p1.X && p1.X <= p2.X ) )
				continue;
			if( !( p0.Y >= p1.Y && p1.Y >= p2.Y ) && !( p0.Y <= p1.Y && p1.Y <= p2.Y ) )
				continue;
			
			//Calculate the angle of p0p1 and p0p2
			double angle_01 = atan2( p1.Y - p0.Y, p1.X - p0.X );
			double angle_02 = atan2( p2.Y - p0.Y, p2.X - p0.X );
			
			//If the difference of the angles is small enough, remove p1
			if( angle_01 > angle_02 -0.001 && angle_01 < angle_02 + 0.001 ){
				remove( i+1 );
				i--;
			}
		}
	}
}



