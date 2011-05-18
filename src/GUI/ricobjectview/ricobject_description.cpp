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

#include "ricobject_description.h"

ricobject_description::ricobject_description( QWidget *parent ):
		ricobject_abstract( parent ),
	//	options( this, "RIC options", NULL, "Is currently only used to specify RIC fonts" ),
		width( NULL, "Width", NULL, "Vertical font line-width" ),
		height( NULL, "Height", NULL, "Horizontial font line-height" )
	{
	layout.addWidget( (QWidget*)&width );
	layout.addWidget( (QWidget*)&height );
	layout.addStretch();
	
	
	connect( &width, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &height, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
}

bool ricobject_description::change_object( ricfile::ricObject* new_object ){
	if( new_object->object_type() == ricfile::ricObject::RIC_OP_OPTIONS ){
		ricfile::ricOpOptions* temp = (ricfile::ricOpOptions*)new_object;
	//	options.change_value_object( &temp->options );
		width.change_value_object( &temp->width );
		height.change_value_object( &temp->height );
		
		return true;
	}
	
	return false;
}

