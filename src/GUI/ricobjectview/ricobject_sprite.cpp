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

#include "ricobject_sprite.h"

ricobject_sprite::ricobject_sprite( QWidget *parent ):
		ricobject_abstract( parent ),
		canvas( NULL )
	{
	add_control( (QWidget*)&canvas );
}

bool ricobject_sprite::change_object( ricfile::ricObject* new_object ){
	if( new_object->object_type() == ricfile::ricObject::RIC_OP_SPRITE ){
		ricfile::ricOpSprite* temp = (ricfile::ricOpSprite*)new_object;
		
		canvas.edit.change_canvas( &temp->sprite_data );
	//	width.change_value_object( &temp->width );
	//	height.change_value_object( &temp->height );
		
		return true;
	}
	
	return false;
}

