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

#include "ricobject_ellipse.h"

ricobject_ellipse::ricobject_ellipse( QWidget *parent ):
		ricobject_abstract( parent ),
		options( NULL, this ),
		posx( NULL, "X", NULL, "The center's X coordinate" ),
		posy( NULL, "Y", NULL, "The center's Y coordinate" ),
		radius_x( NULL, "Radius X", NULL, "Horizontial distance from center to circumference" ),
		radius_y( NULL, "Radius Y", NULL, "Vertical distance from center to circumference" )
	{
	add_control( (QWidget*)&options );
	add_control( (QWidget*)&posx );
	add_control( (QWidget*)&posy );
	add_control( (QWidget*)&radius_x );
	add_control( (QWidget*)&radius_y );
}

bool ricobject_ellipse::change_object( ricfile::ricObject* new_object ){
	if( new_object->object_type() == ricfile::ricObject::RIC_OP_ELLIPSE ){
		ricfile::ricOpEllipse* temp = (ricfile::ricOpEllipse*)new_object;
		
		options.change_value_object( &temp->CopyOptions );
		posx.change_value_object( &temp->pos.X );
		posy.change_value_object( &temp->pos.Y );
		radius_x.change_value_object( &temp->radius_x );
		radius_y.change_value_object( &temp->radius_y );
		
		return true;
	}
	
	return false;
}

