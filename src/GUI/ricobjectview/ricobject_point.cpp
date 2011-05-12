#include "ricobject_point.h"

ricobject_point::ricobject_point( QWidget *parent ):
		ricobject_abstract( parent ),
	//	options( this, "RIC options", NULL, "Is currently only used to specify RIC fonts" ),
		posx( NULL, "X", NULL, "Point's X coordinate" ),
		posy( NULL, "Y", NULL, "Point's Y coordinate" )
	{
	layout.addWidget( (QWidget*)&posx );
	layout.addWidget( (QWidget*)&posy );
	layout.addStretch();
	
	
	connect( &posx, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &posy, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
}

bool ricobject_point::change_object( ricfile::ricObject* new_object ){
	if( new_object->object_type() == ricfile::ricObject::RIC_OP_PIXEL ){
		ricfile::ricOpPixel* temp = (ricfile::ricOpPixel*)new_object;
		
	//	options.change_value_object( &temp->options );
		posx.change_value_object( &temp->pos.X );
		posy.change_value_object( &temp->pos.Y );
		
		return true;
	}
	
	return false;
}

