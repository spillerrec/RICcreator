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

/*
	variable types used on the NXT
*/


#ifndef NXTVARIABLE_H
#define NXTVARIABLE_H

#include <fstream>
using namespace std;

#include "nxtConstants.h"

class nxtVariable{
	public:
		static const unsigned int TYPE_SBYTE = 0;
		static const unsigned int TYPE_UBYTE = 1;
		static const unsigned int TYPE_UWORD = 2;
		static const unsigned int TYPE_SWORD = 3;
		static const unsigned int TYPE_ULONG = 4;
		static const unsigned int TYPE_SLONG = 5;
		static const unsigned int TYPE_FLOAT = 6;
		
		static const unsigned int TYPE_RIC_WORD = 32;
		static const unsigned int TYPE_RIC_POINT = 33;
		static const unsigned int TYPE_RIC_RECT = 34;
		static const unsigned int TYPE_POINT_ARRAY = 35;
		static const unsigned int TYPE_COPYOPTIONS = 36;
		
	
	public:
		static unsigned long read_multibyte(ifstream* file, unsigned char size);
		static void write_multibyte(ofstream* file, unsigned long number, unsigned char size);
		
	public:
		virtual unsigned int var_type() const = 0;
		virtual unsigned int var_amount() const{ return 1; }
		virtual unsigned int filesize() const = 0;
		virtual void read(ifstream* file) = 0;
		virtual void write(ofstream* file) const = 0;
};


class nxtVarWord: public nxtVariable{
	private:
		unsigned int variable;
		
	public:
		unsigned int filesize() const{ return 2; }
		unsigned int var_type() const{ return TYPE_UWORD; }
		virtual void read(ifstream* file){
			variable = read_multibyte( file, 2 );
		}
		virtual void write(ofstream* file) const{
			write_multibyte( file, variable, 2 );
		}
		unsigned int value(){ return variable; }
		void set_value( unsigned int new_value ){ variable = new_value; }
		
		operator unsigned int() const{ return variable; }
		nxtVarWord* operator=( int newValue ){
			variable = newValue;
			return this;
		}
};

#endif
