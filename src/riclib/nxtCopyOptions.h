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

/**	nxtCopyOptions
	
	This implements handling of CopyOptions which is a bitwise representation
	of drawing modes.
	Reimplement set_raw() and get_raw() in a derivered class to apply this
	to a custom datatype.
*/


#ifndef NXTCOPYOPTIONS_H
#define NXTCOPYOPTIONS_H

#include "nxtConstants.h"

class nxtCopyOptions{
	//Merge types
	public:
		static const char MERGE_COPY = 0;
		static const char MERGE_AND = 1;
		static const char MERGE_OR = 2;
		static const char MERGE_XOR = 3;
		
	private:
		unsigned int variable;
		
	//Functions to access the variable. The class must ALWAYS use those as they might be overwritten
	protected:
		virtual unsigned int get_raw() const{ return variable; }
		virtual void set_raw( unsigned int new_raw ){ variable = new_raw; }
	
	private:
		//Set a specific bit
		void set_bit( unsigned int bit, bool value ){
			if( value )
				set_raw( get_raw() | ( 1 << bit ) );	//Set the bit to 1
			else
				set_raw( get_raw() & ~( 1 << bit ) );	//Set the bit to 0
		}
	
	public:
		//Extract settings from variable
		bool get_invert() const{ return get_raw() & DRAW_OPT_NOT; }
		char get_merge() const{ return (get_raw() & DRAW_OPT_XOR) >> 3; }
		bool get_clear() const{ return get_raw() & DRAW_OPT_CLEAR_BACKGROUND; }
		bool get_clear_except_status() const{ return get_raw() & DRAW_OPT_CLEAR_EXCEPT_STATUS; }
		bool get_fill_shape() const{ return get_raw() & DRAW_OPT_FILL_SHAPE; }
		bool get_polyline() const{ return get_raw() & DRAW_OPT_POLYGON_POLYLINE; }
		
		//Set a setting to the variable
		void set_invert( bool invert ){ set_bit( 2, invert ); }
		void set_merge( char merge ){
			set_raw( get_raw() & ~DRAW_OPT_LOGICAL_OPERATIONS );
			set_raw( get_raw() | ( merge << 3 ) );
		}
		void set_clear( bool clear ){ set_bit( 0, clear ); }
		void set_clear_except_status( bool clear_except_status ){ set_bit( 1, clear_except_status ); }
		void set_fill_shape( bool fill_shape ){ set_bit( 5, fill_shape ); }
		void set_polyline( bool polyline ){ set_bit( 10, polyline ); }
	
	
	public:
		nxtCopyOptions(){
			variable = 0;
		}
		
		void invert_switch(){
			set_invert( !get_invert() );
		}
		
		void copy_to( nxtCopyOptions* destination ) const{
			destination->set_raw( get_raw() );
		}
		
};

#endif
