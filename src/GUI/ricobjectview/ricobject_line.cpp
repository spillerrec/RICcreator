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

#include "ricobject_line.h"

ricobject_line::ricobject_line( QWidget *parent ):
		ricobject_abstract( parent ),
		options( NULL, this ),
		posx1( NULL, "Start X", NULL, "Lower left point's X coordinate" ),
		posy1( NULL, "Start Y", NULL, "Lower left point's Y coordinate" ),
		posx2( NULL, "End X", NULL, "Upper right point's X coordinate" ),
		posy2( NULL, "End Y", NULL, "Upper right point's Y coordinate" )
	{
	add_control( (QWidget*)&options );
	add_control( (QWidget*)&posx1 );
	add_control( (QWidget*)&posy1 );
	add_control( (QWidget*)&posx2 );
	add_control( (QWidget*)&posy2 );
}

bool ricobject_line::change_object( ricfile::ricObject* new_object ){
	if( new_object->object_type() == ricfile::ricObject::RIC_OP_LINE ){
		ricfile::ricOpLine* temp = (ricfile::ricOpLine*)new_object;
		
		options.change_value_object( &temp->CopyOptions );
		posx1.change_value_object( &temp->start.X );
		posy1.change_value_object( &temp->start.Y );
		posx2.change_value_object( &temp->end.X );
		posy2.change_value_object( &temp->end.Y );
		
		return true;
	}
	
	return false;
}

