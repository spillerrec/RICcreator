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


#ifndef NXTVARRICCOPYOPTIONS_H
#define NXTVARRICCOPYOPTIONS_H

#include "nxtVarRicWord.h"
#include "nxtCopyOptions.h"


class nxtVarRicCopyoptions: public nxtVarRicWord, public nxtCopyOptions{
	private:
		virtual unsigned int get_raw() const{ return value(); }
		virtual void set_raw( unsigned int new_raw ){ set_normal( new_raw ); }
	
	public:
		nxtVarRicCopyoptions( ricObject *container ): nxtVarRicWord( container ){  }
};

#endif
