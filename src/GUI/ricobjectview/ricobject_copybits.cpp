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

#include "ricobject_copybits.h"

ricobject_copybits::ricobject_copybits( QWidget *parent ):
		ricobject_abstract( parent ),
		options( NULL, this ),
		id( NULL, "Sprite ID", NULL, "The ID of the sprite it should copy from" ),
		src_x( NULL, "Sprite X", NULL, "X coordinate on the sprite where it should start copying from." ),
		src_y( NULL, "Sprite Y", NULL, "X coordinate on the sprite where it should start copying from." ),
		src_width( NULL, "Width", NULL, "How wide the area it should copy is." ),
		src_height( NULL, "Height", NULL, "How high the area it should copy is." ),
		dest_x( NULL, "X", NULL, "The X coordinate it should place the copied area." ),
		dest_y( NULL, "Y", NULL, "The Y coordinate it should place the copied area." )
	{
	layout.addWidget( (QWidget*)&options );
	layout.addWidget( (QWidget*)&id );
	layout.addWidget( (QWidget*)&src_x );
	layout.addWidget( (QWidget*)&src_y );
	layout.addWidget( (QWidget*)&src_width );
	layout.addWidget( (QWidget*)&src_height );
	layout.addWidget( (QWidget*)&dest_x );
	layout.addWidget( (QWidget*)&dest_y );
	layout.addStretch();
	
	
	connect( &options, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &id, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &src_x, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &src_y, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &src_width, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &src_height, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &dest_x, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &dest_y, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
}

bool ricobject_copybits::change_object( ricfile::ricObject* new_object ){
	if( new_object->object_type() == ricfile::ricObject::RIC_OP_COPYBITS ){
		ricfile::ricOpCopyBits* temp = (ricfile::ricOpCopyBits*)new_object;
		
		options.change_value_object( &temp->CopyOptions );
		id.change_value_object( &temp->SpriteID );
		src_x.change_value_object( &temp->src.pos.X );
		src_y.change_value_object( &temp->src.pos.Y );
		src_width.change_value_object( &temp->src.width );
		src_height.change_value_object( &temp->src.height );
		dest_x.change_value_object( &temp->dest.X );
		dest_y.change_value_object( &temp->dest.Y );
		
		return true;
	}
	
	return false;
}
