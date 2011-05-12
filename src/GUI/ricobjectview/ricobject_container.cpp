#include "ricobject_container.h"
#include <QLabel>

ricobject_container::ricobject_container( QWidget *parent ): QStackedLayout( parent ){
	//Add widget for nothing selected
	QLabel* none_selected = new QLabel( "Nothing selected", NULL );
	addWidget( (QWidget*)none_selected );
	setCurrentIndex( 0 );	//And make default
	
	//Add widget for missing handler
	QLabel* no_handler = new QLabel( "No control for this type object as of yet", NULL );
	addWidget( (QWidget*)no_handler );
	
	//Add controls
	ric_description = new ricobject_description();
	connect( (QWidget*)ric_description, SIGNAL( changed() ),  this, SIGNAL( object_changed() ) );
	addWidget( (QWidget*)ric_description );
	
	ric_point = new ricobject_point();
	connect( (QWidget*)ric_point, SIGNAL( changed() ),  this, SIGNAL( object_changed() ) );
	addWidget( (QWidget*)ric_point );
	
	ric_line = new ricobject_line();
	connect( (QWidget*)ric_line, SIGNAL( changed() ),  this, SIGNAL( object_changed() ) );
	addWidget( (QWidget*)ric_line );
	
	ric_rect = new ricobject_rect();
	connect( (QWidget*)ric_rect, SIGNAL( changed() ),  this, SIGNAL( object_changed() ) );
	addWidget( (QWidget*)ric_rect );
	
	ric_circle = new ricobject_circle();
	connect( (QWidget*)ric_circle, SIGNAL( changed() ),  this, SIGNAL( object_changed() ) );
	addWidget( (QWidget*)ric_circle );
	
	ric_ellipse = new ricobject_ellipse();
	connect( (QWidget*)ric_ellipse, SIGNAL( changed() ),  this, SIGNAL( object_changed() ) );
	addWidget( (QWidget*)ric_ellipse );
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


