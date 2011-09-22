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
#include "ricfile.h"


unsigned int nxtVarRicWord::value() const{
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

void nxtVarRicWord::read_raw( unsigned int raw ){
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

nxtIO::LoaderError nxtVarRicWord::read( nxtIO* file ){
	unsigned int temp;
	nxtIO::LoaderError result = file->read_word( temp );
	read_raw( temp );
	return result;
}
nxtIO::LoaderError nxtVarRicWord::write( nxtIO* file ) const{
	if( is_extended() ){
		unsigned int raw = 0x1000 + parameter + (VarMapID << 8);
		return file->write_multibyte_unsigned( 2, raw );
	}
	else
		return file->write_multibyte_unsigned( 2, number );
}



void ricVarId::autoassign_id(){
	//Autoset ID if possible
	for( unsigned int i=1; i<11; i++ ){
		set_value( i );
		if( is_valid() == ID_VALID )
			return;
	}
	
	set_value( 0 );	//Autoassign failed
}

ricVarId::valid ricVarId::validate_id( unsigned int id, ricObject *object, bool native ){
	if( object && object->parent() ){
		if( id == 0 || id > 10 )
			return ID_INVALID;
		
		if( native ){
			if( object->object_type() == ricObject::RIC_OP_COPYBITS ){
				if( object->parent()->object_at_ID( id, ricObject::RIC_OP_SPRITE, object ) )
					return ID_VALID;
				else
					return ID_INVALID;
			}
			
			if(	object->parent()->object_at_ID( id, ricObject::RIC_OP_SPRITE, object )
					|| object->parent()->object_at_ID( id, ricObject::RIC_OP_VARMAP, object ) )
			{
				return ID_OWERWRITES;
			}
			else
				return ID_VALID;
		}
		else{
			//This is a ricword lookup for a VARMAP, so just confirm this
			if( object->parent()->object_at_ID( id, ricObject::RIC_OP_VARMAP, object ) )
				return ID_VALID;
			else
				return ID_INVALID;
		}
		
	}
	
	return ID_INVALID;
}

