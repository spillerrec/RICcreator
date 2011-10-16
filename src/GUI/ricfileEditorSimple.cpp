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

#include "ricfileEditorSimple.h"
#include "openRicfile.h"

#include "nxtCanvasWidget.h"
#include "nxtCanvasWidgetContainer.h"

#include "../riclib/ricfile.h"
#include "../riclib/ricObject.h"
#include "../riclib/ricObjectChildren.h"

#include <QVBoxLayout>

ricfileEditorSimple::ricfileEditorSimple( QWidget *parent ):
		ricfileEditor( parent )
{
	edit = new nxtCanvasWidget( this );
	container = new nxtCanvasWidgetContainer( *edit, this, true, true );
	setLayout( new QVBoxLayout() );
	layout()->addWidget( container );
	
	connect( container, SIGNAL( value_changed() ), this, SLOT( file_changed() ) );
}

void ricfileEditorSimple::change_file( openRicfile *new_file ){
	file = new_file;
	ricOpSprite *sprite = (ricOpSprite*)file->ric().last_object( ricObject::RIC_OP_SPRITE );
	edit->change_object( (nxtVariable*)&sprite->sprite_data );
}



bool ricfileEditorSimple::file_supported( const ricfile &ric ){
	int sprite_id = -1;
	
	for( unsigned int i=0; i<ric.object_amount(); i++ ){
		ricObject const* object = ric.get_object( i );
		switch( object->object_type() ){
			case ricObject::RIC_OP_OPTIONS:
					if( i != 0 )
						return false;	//Must be first
				break;
			
			case ricObject::RIC_OP_SPRITE:
					if( sprite_id != -1 )
						return false; 	//only one sprite allowed!
					sprite_id = ((ricOpSprite*)object)->get_ID();
				break;
			
			case ricObject::RIC_OP_COPYBITS:
					if( sprite_id == -1 )
						return false;
					
					return (unsigned int)sprite_id == ((ricOpCopyBits*)object)->SpriteID.value();
			
			default:
					return false;
		}
	}
	
	
	return false;
}


