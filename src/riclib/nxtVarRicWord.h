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


#ifndef NXTVARRICWORD_H
#define NXTVARRICWORD_H

#include "nxtVariable.h"
#include "ricfile.h"

#include <fstream>
using namespace std;

class ricfile::nxtVarRicWord: public nxtVariable{
	private:
		unsigned int number;
		bool extended;
		unsigned char parameter;
		unsigned char VarMapID;
		ricObject *object;
		
	public:
		nxtVarRicWord( ricObject *container ){
			number = 0;
			parameter = 0;
			VarMapID = 0;
			extended = false;
			object = container;
		}
		unsigned int filesize() const{ return 2; }
		unsigned int var_type() const{ return TYPE_RIC_WORD; }
		
		bool is_extended() const{ return extended; }
		unsigned int get_number() const{ return number; }
		unsigned char get_parameter() const{ return parameter; }
		unsigned char get_varmap() const{ return VarMapID; }
		
		bool set_extended( unsigned char map, unsigned char para ){
			if( map < 16 ){
				extended = true;
				VarMapID = map;
				parameter = para;
				
				return true;
			}
			
			return false;
		}
		bool set_normal( unsigned int new_value ){
			if( new_value <= 0x0FFF ){
				extended = false;
				number = new_value;
				
				return true;
			}
			
			return false;
		}
		
		void read_raw( unsigned int raw );
		virtual void read(ifstream* file);
		virtual void write(ofstream* file) const;
		unsigned int value() const;
		
		operator unsigned int() const{ return value(); }
		nxtVarRicWord* operator=( int newValue ){
			number = newValue;
			return this;
		}
};


//Some convinience classes...
class ricfile::ricvarPoint: public nxtVariable{
	public:
		nxtVarRicWord X;
		nxtVarRicWord Y;
		
		ricvarPoint( ricObject *container ):
				X( container ),
				Y( container )
			{ }
		
		unsigned int filesize() const{ return 2; }
		unsigned int var_type() const{ return TYPE_RIC_POINT; }
		unsigned int var_amount() const{ return 2; }
		void read(ifstream* file){
			X.read( file );
			Y.read( file );
		}
		void write(ofstream* file) const{
			X.write( file );
			Y.write( file );
		}
};

class ricfile::ricvarRect: public nxtVariable{
	public:
		ricvarPoint pos;
		nxtVarRicWord width;
		nxtVarRicWord height;
		
		ricvarRect( ricObject *container ):
				pos( container ),
				width( container ),
				height( container )
			{ }
		
		unsigned int filesize() const{ return 2; }
		unsigned int var_type() const{ return TYPE_RIC_RECT; }
		unsigned int var_amount() const{ return 4; }
		void read(ifstream* file){
			pos.read( file );
			width.read( file );
			height.read( file );
		}
		void write(ofstream* file) const{
			pos.write( file );
			width.write( file );
			height.write( file );
		}
};


#endif