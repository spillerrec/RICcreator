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

#include "ricobject_container.h"

#include "ricobject_description.h"
#include "ricobject_sprite.h"
#include "ricobject_copybits.h"
#include "ricobject_point.h"
#include "ricobject_line.h"
#include "ricobject_rect.h"
#include "ricobject_circle.h"
#include "ricobject_number.h"
#include "ricobject_ellipse.h"

#include <QLabel>

bool ricobject_container::add_control( ricobject_abstract* control ){
	addWidget( (QWidget*)control );
	return connect( (QWidget*)control, SIGNAL( changed() ),  this, SIGNAL( object_changed() ) );
}

ricobject_container::ricobject_container( QWidget *parent ): QStackedLayout( parent ){
	//Add widget for nothing selected
	QLabel* none_selected = new QLabel( "Nothing selected", NULL );
	none_selected->setAlignment( Qt::AlignCenter );
	addWidget( (QWidget*)none_selected );
	setCurrentIndex( 0 );	//And make default
	
	//Add widget for missing handler
	QLabel* no_handler = new QLabel( "No control for this type object as of yet", NULL );
	no_handler->setAlignment( Qt::AlignCenter );
	addWidget( (QWidget*)no_handler );
	
	//Add controls
	add_control( ric_description = new ricobject_description() );
	add_control( ric_sprite = new ricobject_sprite() );
	add_control( ric_copybits = new ricobject_copybits() );
	add_control( ric_point = new ricobject_point() );
	add_control( ric_line = new ricobject_line() );
	add_control( ric_rect = new ricobject_rect() );
	add_control( ric_circle = new ricobject_circle() );
	add_control( ric_number = new ricobject_number() );
	add_control( ric_ellipse = new ricobject_ellipse() );
}


void ricobject_container::view_object( ricfile::ricObject* object ){
	switch( object->object_type() ){
		case ricfile::ricObject::RIC_OP_OPTIONS: ric_description->change_object( object ); setCurrentIndex( 2 ); break;
		case ricfile::ricObject::RIC_OP_SPRITE: ric_sprite->change_object( object ); setCurrentIndex( 3 ); break;
		case ricfile::ricObject::RIC_OP_COPYBITS: ric_copybits->change_object( object ); setCurrentIndex( 4 ); break;
		case ricfile::ricObject::RIC_OP_PIXEL: ric_point->change_object( object ); setCurrentIndex( 5 ); break;
		case ricfile::ricObject::RIC_OP_LINE: ric_line->change_object( object ); setCurrentIndex( 6 ); break;
		case ricfile::ricObject::RIC_OP_RECTANGLE: ric_rect->change_object( object ); setCurrentIndex( 7 ); break;
		case ricfile::ricObject::RIC_OP_CICLE: ric_circle->change_object( object ); setCurrentIndex( 8 ); break;
		case ricfile::ricObject::RIC_OP_NUMBER: ric_number->change_object( object ); setCurrentIndex( 9 ); break;
		case ricfile::ricObject::RIC_OP_ELLIPSE: ric_ellipse->change_object( object ); setCurrentIndex( 10 ); break;
		default:
			setCurrentIndex( 1 );	//Set error handler
	}
}


