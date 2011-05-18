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

#include "ricobject_circle.h"

ricobject_circle::ricobject_circle( QWidget *parent ):
		ricobject_abstract( parent ),
		options( NULL, this ),
		posx( NULL, "X", NULL, "The center's X coordinate" ),
		posy( NULL, "Y", NULL, "The center's Y coordinate" ),
		radius( NULL, "Radius", NULL, "Distance from center to circumference" )
	{
	layout.addWidget( (QWidget*)&options );
	layout.addWidget( (QWidget*)&posx );
	layout.addWidget( (QWidget*)&posy );
	layout.addWidget( (QWidget*)&radius );
	layout.addStretch();
	
	
	connect( &posx, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &posy, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &radius, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &options, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
}

bool ricobject_circle::change_object( ricfile::ricObject* new_object ){
	if( new_object->object_type() == ricfile::ricObject::RIC_OP_CICLE ){
		ricfile::ricOpCicle* temp = (ricfile::ricOpCicle*)new_object;
		
		options.change_value_object( &temp->CopyOptions );
		posx.change_value_object( &temp->pos.X );
		posy.change_value_object( &temp->pos.Y );
		radius.change_value_object( &temp->radius );
		
		return true;
	}
	
	return false;
}

