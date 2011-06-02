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

#include "ricobject_number.h"

ricobject_number::ricobject_number( QWidget *parent ):
		ricobject_abstract( parent ),
		options( NULL, copyoptions_value::RIC_OBJECT, this ),
		posx( NULL, "X", NULL, "The Numbers's X coordinate" ),
		posy( NULL, "Y", NULL, "The Numbers's Y coordinate" ),
		value( NULL, "Number", NULL, "The value to display" )
	{
	add_control( (QWidget*)&options );
	add_control( (QWidget*)&posx );
	add_control( (QWidget*)&posy );
	add_control( (QWidget*)&value );
}

bool ricobject_number::change_object( ricfile::ricObject* new_object ){
	if( new_object->object_type() == ricfile::ricObject::RIC_OP_NUMBER ){
		ricfile::ricOpNumber* temp = (ricfile::ricOpNumber*)new_object;
		
		options.change_value_object( &temp->CopyOptions );
		posx.change_value_object( &temp->pos.X );
		posy.change_value_object( &temp->pos.Y );
		value.change_value_object( &temp->number );
		
		return true;
	}
	
	return false;
}

