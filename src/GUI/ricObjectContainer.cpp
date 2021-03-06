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

#include "ricObjectContainer.h"

#include "ricObjectAbstractChildren.h"

#include <QLabel>

bool ricObjectContainer::add_control( ricObjectAbstract* control ){
	addWidget( (QWidget*)control );
	return connect( (QWidget*)control, SIGNAL( changed() ),  this, SIGNAL( object_changed() ) );
}

ricObjectContainer::ricObjectContainer( QWidget *parent ): QStackedLayout( parent ){
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
	add_control( ric_description = new ricObjectDescription() );
	add_control( ric_sprite = new ricObjectSprite() );
	add_control( ric_varmap = new ricObjectVarMap() );
	add_control( ric_copybits = new ricObjectCopybits() );
	add_control( ric_point = new ricObjectPixel() );
	add_control( ric_line = new ricObjectLine() );
	add_control( ric_rect = new ricObjectRectangle() );
	add_control( ric_circle = new ricObjectCircle() );
	add_control( ric_number = new ricObjectNumber() );
	add_control( ric_ellipse = new ricObjectEllipse() );
	add_control( ric_polygon = new ricObjectPolygon() );
}


void ricObjectContainer::view_object( ricObject* object ){
	if( !object ){
		//No object, view "nothing selected" widget
		setCurrentIndex( 0 );
		return;
	}
	
	switch( object->object_type() ){
		case ricObject::RIC_OP_OPTIONS: ric_description->change_object( object ); setCurrentIndex( 2 ); break;
		case ricObject::RIC_OP_SPRITE: ric_sprite->change_object( object ); setCurrentIndex( 3 ); break;
		case ricObject::RIC_OP_VARMAP: ric_varmap->change_object( object ); setCurrentIndex( 4 ); break;
		case ricObject::RIC_OP_COPYBITS: ric_copybits->change_object( object ); setCurrentIndex( 5 ); break;
		case ricObject::RIC_OP_PIXEL: ric_point->change_object( object ); setCurrentIndex( 6 ); break;
		case ricObject::RIC_OP_LINE: ric_line->change_object( object ); setCurrentIndex( 7 ); break;
		case ricObject::RIC_OP_RECTANGLE: ric_rect->change_object( object ); setCurrentIndex( 8 ); break;
		case ricObject::RIC_OP_CICLE: ric_circle->change_object( object ); setCurrentIndex( 9 ); break;
		case ricObject::RIC_OP_NUMBER: ric_number->change_object( object ); setCurrentIndex( 10 ); break;
		case ricObject::RIC_OP_ELLIPSE: ric_ellipse->change_object( object ); setCurrentIndex( 11 ); break;
		case ricObject::RIC_OP_POLYGON: ric_polygon->change_object( object ); setCurrentIndex( 12 ); break;
		default:
			setCurrentIndex( 1 );	//Set error handler
	}
}


