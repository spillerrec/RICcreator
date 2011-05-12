#include "ricobject_container.h"
#include <QLabel>

bool ricobject_container::add_control( ricobject_abstract* control ){
	addWidget( (QWidget*)control );
	return connect( (QWidget*)control, SIGNAL( changed() ),  this, SIGNAL( object_changed() ) );
}

ricobject_container::ricobject_container( QWidget *parent ): QStackedLayout( parent ){
	//Add widget for nothing selected
	QLabel* none_selected = new QLabel( "Nothing selected", NULL );
	addWidget( (QWidget*)none_selected );
	setCurrentIndex( 0 );	//And make default
	
	//Add widget for missing handler
	QLabel* no_handler = new QLabel( "No control for this type object as of yet", NULL );
	addWidget( (QWidget*)no_handler );
	
	//Add controls
	add_control( ric_description = new ricobject_description() );
	add_control( ric_point = new ricobject_point() );
	add_control( ric_line = new ricobject_line() );
	add_control( ric_rect = new ricobject_rect() );
	add_control( ric_circle = new ricobject_circle() );
	add_control( ric_ellipse = new ricobject_ellipse() );
}


void ricobject_container::view_object( ricfile::ricObject* object ){
	switch( object->object_type() ){
		case ricfile::ricObject::RIC_OP_OPTIONS: ric_description->change_object( object ); setCurrentIndex( 2 ); break;
		case ricfile::ricObject::RIC_OP_PIXEL: ric_point->change_object( object ); setCurrentIndex( 3 ); break;
		case ricfile::ricObject::RIC_OP_LINE: ric_line->change_object( object ); setCurrentIndex( 4 ); break;
		case ricfile::ricObject::RIC_OP_RECTANGLE: ric_rect->change_object( object ); setCurrentIndex( 5 ); break;
		case ricfile::ricObject::RIC_OP_CICLE: ric_circle->change_object( object ); setCurrentIndex( 6 ); break;
		case ricfile::ricObject::RIC_OP_ELLIPSE: ric_ellipse->change_object( object ); setCurrentIndex( 7 ); break;
		default:
			setCurrentIndex( 1 );	//Set error handler
	}
}


