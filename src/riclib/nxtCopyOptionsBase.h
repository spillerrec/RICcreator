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

/**	nxtCopyOptionsBase
	
	This abstract class implements CopyOptions which is a bitwise representation
	of drawing modes.
	Implement set_raw() and get_raw() to provide read/write access on the (at least)
	16-bit variable which this shall be stored in.
*/

//TODO: implement ric font properties


#ifndef NXTCOPYOPTIONSBASE_H
#define NXTCOPYOPTIONSBASE_H

class nxtCopyOptionsBase{
	private:
		//Copy options
		static const unsigned int DRAW_OPT_NORMAL = 0x0000;
		static const unsigned int DRAW_OPT_CLEAR_BACKGROUND = 0x0001;	//bit 0 = 1
		static const unsigned int DRAW_OPT_CLEAR_EXCEPT_STATUS = 0x0002;	//bit 1 = 1
		static const unsigned int DRAW_OPT_NOT = 0x0004;	//bit 2 = 1
		static const unsigned int DRAW_OPT_AND = 0x0008;	// bit 3 = 1 and bit 4 = 0
		static const unsigned int DRAW_OPT_OR = 0x0010;	// bit 3 = 0 and bit 4 = 1
		static const unsigned int DRAW_OPT_XOR = 0x0018;	// bit 3 = 1 and bit 4 = 1
		static const unsigned int DRAW_OPT_FILL_SHAPE = 0x0020;	//bit 5 = 1
		static const unsigned int DRAW_OPT_POLYGON_POLYLINE = 0x0400;	//bit 10 = 1

		//Combined copy options, for extraction
		static const unsigned int DRAW_OPT_LOGICAL_OPERATIONS = 0x0018;
	
	
	//Merge types
	public:
		static const char MERGE_COPY = 0;
		static const char MERGE_AND = 1;
		static const char MERGE_OR = 2;
		static const char MERGE_XOR = 3;
		
	//Functions to access the variable.
	protected:
		virtual unsigned int get_raw() const = 0;
		virtual void set_raw( unsigned int new_raw ) = 0;
	
	private:
		//Set a specific bit
		void set_bit( unsigned int bit, bool value, bool enabled = true ){
			if( enabled ){
				if( value )
					set_raw( get_raw() | ( 1 << bit ) );	//Set the bit to 1
				else
					set_raw( get_raw() & ~( 1 << bit ) );	//Set the bit to 0
			}
		}
	
	
	//Disables properties
	public:
		bool enabled_clear;
		bool enabled_fill_shape;
		bool enabled_polyline;
		bool enabled_font;
	
	public:
		//Extract settings from variable
		bool get_invert() const{ return get_raw() & DRAW_OPT_NOT; }
		char get_merge() const{ return (get_raw() & DRAW_OPT_XOR) >> 3; }
		bool get_clear() const{ return enabled_clear && (get_raw() & DRAW_OPT_CLEAR_BACKGROUND); }
		bool get_clear_except_status() const{ return enabled_clear && (get_raw() & DRAW_OPT_CLEAR_EXCEPT_STATUS); }
		bool get_fill_shape() const{ return enabled_fill_shape && (get_raw() & DRAW_OPT_FILL_SHAPE); }
		bool get_polyline() const{ return enabled_polyline && (get_raw() & DRAW_OPT_POLYGON_POLYLINE); }
		
		//Set a setting to the variable
		void set_invert( bool invert ){ set_bit( 2, invert ); }
		void set_merge( char merge ){
			set_raw( get_raw() & ~DRAW_OPT_LOGICAL_OPERATIONS );
			set_raw( get_raw() | ( merge << 3 ) );
		}
		void set_clear( bool clear ){ set_bit( 0, clear, enabled_clear ); }
		void set_clear_except_status( bool clear_except_status ){ set_bit( 1, clear_except_status, enabled_clear ); }
		void set_fill_shape( bool fill_shape ){ set_bit( 5, fill_shape, enabled_fill_shape ); }
		void set_polyline( bool polyline ){ set_bit( 10, polyline, enabled_polyline ); }
	
	
	public:
		nxtCopyOptionsBase(){
			enabled_clear = true;
			enabled_fill_shape = false;
			enabled_polyline = false;
			enabled_font = false;
		}
		
		void invert_switch(){
			set_invert( !get_invert() );
		}
		
		void copy_to( nxtCopyOptionsBase* destination ) const{
			destination->set_raw( get_raw() );
		}
		
};

#endif
