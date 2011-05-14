#include "ricobject_rect.h"

ricobject_rect::ricobject_rect( QWidget *parent ):
		ricobject_abstract( parent ),
		options( NULL, this ),
		posx( NULL, "X", NULL, "Lower left corner's X coordinate" ),
		posy( NULL, "Y", NULL, "Lower left corner's Y coordinate" ),
		width( NULL, "Width", NULL, "Amount of pixels the rectangle extends to the right" ),
		height( NULL, "Height", NULL, "Amount of pixels the rectangle extends to upwards" )
	{
	layout.addWidget( (QWidget*)&options );
	layout.addWidget( (QWidget*)&posx );
	layout.addWidget( (QWidget*)&posy );
	layout.addWidget( (QWidget*)&width );
	layout.addWidget( (QWidget*)&height );
	layout.addStretch();
	
	
	connect( &posx, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &posy, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &width, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &height, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &options, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
}

bool ricobject_rect::change_object( ricfile::ricObject* new_object ){
	if( new_object->object_type() == ricfile::ricObject::RIC_OP_RECTANGLE ){
		ricfile::ricOpRectangle* temp = (ricfile::ricOpRectangle*)new_object;
		
		options.change_value_object( &temp->CopyOptions );
		posx.change_value_object( &temp->rect.pos.X );
		posy.change_value_object( &temp->rect.pos.Y );
		width.change_value_object( &temp->rect.width );
		height.change_value_object( &temp->rect.height );
		
		return true;
	}
	
	return false;
}

