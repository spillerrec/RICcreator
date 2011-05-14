#include "ricobject_circle.h"

ricobject_circle::ricobject_circle( QWidget *parent ):
		ricobject_abstract( parent ),
		options( NULL, this ),
		posx( NULL, "X", NULL, "The center's X coordinate" ),
		posy( NULL, "Y", NULL, "The center's Y coordinate" ),
		radius( NULL, "Radius", NULL, "Distance from center to circumference" )
	{
	layout.addWidget( (QWidget*)&options );
	layout.addWidget( (QWidget*)&posx );
	layout.addWidget( (QWidget*)&posy );
	layout.addWidget( (QWidget*)&radius );
	layout.addStretch();
	
	
	connect( &posx, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &posy, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &radius, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &options, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
}

bool ricobject_circle::change_object( ricfile::ricObject* new_object ){
	if( new_object->object_type() == ricfile::ricObject::RIC_OP_CICLE ){
		ricfile::ricOpCicle* temp = (ricfile::ricOpCicle*)new_object;
		
		options.change_value_object( &temp->CopyOptions );
		posx.change_value_object( &temp->pos.X );
		posy.change_value_object( &temp->pos.Y );
		radius.change_value_object( &temp->radius );
		
		return true;
	}
	
	return false;
}

