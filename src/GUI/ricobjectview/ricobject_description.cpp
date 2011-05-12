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

