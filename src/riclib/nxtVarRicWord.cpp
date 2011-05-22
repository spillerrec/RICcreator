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

#include "nxtVarRicWord.h"
#include "ricObject.h"


unsigned int ricfile::nxtVarRicWord::value() const{
	if( extended ){
		unsigned int par_val = object->parent()->GetParameter( parameter );
		if( VarMapID ){
			return object->parent()->get_varmap_value( VarMapID, object, par_val );
		}
		else
			return par_val;
	}
	else
		return number;
}

void ricfile::nxtVarRicWord::read_raw( unsigned int raw ){
	if( raw & 0x1000 ){
		extended = true;
		number = 0;
		parameter = raw & 0x00FF;
		VarMapID = (raw & 0x0F00) >> 8;
	}
	else{
		extended = false;
		number = raw & 0x0FFF;
		parameter = 0;
		VarMapID = 0;
	}

}

void ricfile::nxtVarRicWord::read(ifstream* file){
	read_raw( read_multibyte( file, 2 ) );
}
void ricfile::nxtVarRicWord::write(ofstream* file) const{
	if( is_extended() ){
		unsigned int raw = 0x1000 + parameter + (VarMapID << 8);
		write_multibyte( file, raw, 2 );
	}
	else
		write_multibyte( file, number, 2 );
}

