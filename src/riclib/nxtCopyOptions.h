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

#include "nxtCopyOptionsBase.h"
#include "nxtVariable.h"

class nxtCopyOptions: public nxtCopyOptionsBase, public nxtVariable{
	//Implements the read/write functions
	private:
		unsigned int variable;
		unsigned int get_raw() const{ return variable; }
		void set_raw( unsigned int new_raw ){ variable = new_raw; }
	
	public:
		nxtCopyOptions(){
			variable = 0;
		}
		
		unsigned int filesize() const{ return 2; }
		unsigned int var_type() const{ return TYPE_COPYOPTIONS; }
		nxtIO::LoaderError read( nxtIO* file ){
			return file->read_word( variable );
		}
		nxtIO::LoaderError write( nxtIO* file ) const{
			return file->write_multibyte_unsigned( 2, variable );
		}
		
};

#endif
