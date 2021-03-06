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

#include "nxtIO.h"

class nxtVariable{
	public:
		enum var_type{
			TYPE_SBYTE,
			TYPE_UBYTE,
			TYPE_UWORD,
			TYPE_SWORD,
			TYPE_ULONG,
			TYPE_SLONG,
			TYPE_FLOAT,
		
			TYPE_RIC_WORD,
			TYPE_RIC_POINT,
			TYPE_RIC_RECT,
			TYPE_RIC_ID,
			TYPE_POINT_ARRAY,
			TYPE_COPYOPTIONS,
			TYPE_RIC_COPYOPTIONS,
			TYPE_BITMAP
		};
		
		
	public:
		virtual unsigned int var_type() const = 0;
		virtual unsigned int var_amount() const{ return 1; }
		virtual unsigned int filesize() const = 0;
		virtual nxtIO::LoaderError read( nxtIO* file ) = 0;
		virtual nxtIO::LoaderError write( nxtIO* file ) const = 0;
};


class nxtVarWord: public nxtVariable{
	private:
		unsigned int variable;
		
	public:
		nxtVarWord(){
			variable = 0;
		}
		
		virtual unsigned int filesize() const{ return 2; }
		virtual unsigned int var_type() const{ return TYPE_UWORD; }
		virtual nxtIO::LoaderError read( nxtIO* file ){
			return file->read_word( variable );
		}
		virtual nxtIO::LoaderError write( nxtIO* file ) const{
			return file->write_multibyte_unsigned( 2, variable );
		}
		unsigned int value() const{ return variable; }
		void set_value( unsigned int new_value ){ variable = new_value; }
		
		operator unsigned int() const{ return variable; }
		nxtVarWord* operator=( int newValue ){
			variable = newValue;
			return this;
		}
};

#endif

