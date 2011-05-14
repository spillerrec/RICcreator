#include "ricobject_line.h"

ricobject_line::ricobject_line( QWidget *parent ):
		ricobject_abstract( parent ),
		options( NULL, this ),
		posx1( NULL, "Start X", NULL, "Lower left point's X coordinate" ),
		posy1( NULL, "Start Y", NULL, "Lower left point's Y coordinate" ),
		posx2( NULL, "End X", NULL, "Upper right point's X coordinate" ),
		posy2( NULL, "End Y", NULL, "Upper right point's Y coordinate" )
	{
	layout.addWidget( (QWidget*)&options );
	layout.addWidget( (QWidget*)&posx1 );
	layout.addWidget( (QWidget*)&posy1 );
	layout.addWidget( (QWidget*)&posx2 );
	layout.addWidget( (QWidget*)&posy2 );
	layout.addStretch();
	
	
	connect( &posx1, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &posy1, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &posx2, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &posy2, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	connect( &options, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
}

bool ricobject_line::change_object( ricfile::ricObject* new_object ){
	if( new_object->object_type() == ricfile::ricObject::RIC_OP_LINE ){
		ricfile::ricOpLine* temp = (ricfile::ricOpLine*)new_object;
		
		options.change_value_object( &temp->CopyOptions );
		posx1.change_value_object( &temp->start.X );
		posy1.change_value_object( &temp->start.Y );
		posx2.change_value_object( &temp->end.X );
		posy2.change_value_object( &temp->end.Y );
		
		return true;
	}
	
	return false;
}

