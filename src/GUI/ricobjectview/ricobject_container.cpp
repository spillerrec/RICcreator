#include "ricobject_container.h"
#include "ricobject_description.h"
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
}


void ricobject_container::view_object( ricfile::ricObject* object ){
	switch( object->object_type() ){
		case ricfile::ricObject::RIC_OP_OPTIONS: ric_description->change_object( object ); setCurrentIndex( 2 ); break;
		default:
			setCurrentIndex( 1 );	//Set error handler
	}
}


