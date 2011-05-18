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

#include "ricobject_point.h"

ricobject_point::ricobject_point( QWidget *parent ):
		ricobject_abstract( parent ),
		options( NULL, this ),
		posx( NULL, "X", NULL, "Point's X coordinate" ),
		posy( NULL, "Y", NULL, "Point's Y coordinate" )
	{
	layout.addWidget( (QWidget*)&options );
	layout.addWidget( (QWidget*)&posx );
	layout.addWidget( (QWidget*)&posy );
	layout.addStretch();
	
	
	connect( &posx, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &posy, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &options, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
}

bool ricobject_point::change_object( ricfile::ricObject* new_object ){
	if( new_object->object_type() == ricfile::ricObject::RIC_OP_PIXEL ){
		ricfile::ricOpPixel* temp = (ricfile::ricOpPixel*)new_object;
		
		options.change_value_object( &temp->CopyOptions );
		posx.change_value_object( &temp->pos.X );
		posy.change_value_object( &temp->pos.Y );
		
		return true;
	}
	
	return false;
}

