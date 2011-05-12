#include "ricobject_ellipse.h"

ricobject_ellipse::ricobject_ellipse( QWidget *parent ):
		ricobject_abstract( parent ),
	//	options( this, "RIC options", NULL, "Is currently only used to specify RIC fonts" ),
		posx( NULL, "X", NULL, "The center's X coordinate" ),
		posy( NULL, "Y", NULL, "The center's Y coordinate" ),
		radius_x( NULL, "Radius X", NULL, "Horizontial distance from center to circumference" ),
		radius_y( NULL, "Radius Y", NULL, "Vertical distance from center to circumference" )
	{
	layout.addWidget( (QWidget*)&posx );
	layout.addWidget( (QWidget*)&posy );
	layout.addWidget( (QWidget*)&radius_x );
	layout.addWidget( (QWidget*)&radius_y );
	layout.addStretch();
	
	
	connect( &posx, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &posy, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &radius_x, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &radius_y, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
}

bool ricobject_ellipse::change_object( ricfile::ricObject* new_object ){
	if( new_object->object_type() == ricfile::ricObject::RIC_OP_ELLIPSE ){
		ricfile::ricOpEllipse* temp = (ricfile::ricOpEllipse*)new_object;
		
	//	options.change_value_object( &temp->options );
		posx.change_value_object( &temp->pos.X );
		posy.change_value_object( &temp->pos.Y );
		radius_x.change_value_object( &temp->radius_x );
		radius_y.change_value_object( &temp->radius_y );
		
		return true;
	}
	
	return false;
}

