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


#include "pointArrayValueItem.h"
#include "pointArrayValue.h"
#include "../../riclib/pointArray.h"

#include <QIcon>
#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QSizePolicy>

static const QSize icon_size = QSize( 16, 16 );
static bool icons_loaded = false;
static QIcon icon_up = QIcon();
static QIcon icon_down = QIcon();
static QIcon icon_add = QIcon();
static QIcon icon_remove = QIcon();

static QSizePolicy fixed_policy = QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );


pointArrayValueItem::pointArrayValueItem( pointArray* point_array, unsigned int idx, pointArrayValue* parent ): QWidget( parent ){
	if( !icons_loaded ){
		icon_up.addFile( ":/edit/move_up.png" );
		icon_down.addFile( ":/edit/move_down.png" );
		icon_add.addFile( ":/edit/add.png" );
		icon_remove.addFile( ":/edit/remove.png" );
		icons_loaded = true;
	}
	
	points = point_array;
	index = idx;
	container = parent;
	
	setAttribute( Qt::WA_DeleteOnClose );
	
	//Create widget
	QHBoxLayout* layout = new QHBoxLayout( this );
	layout->setContentsMargins( 0, 0, 0, 0 );
	
	//Add move buttons
	if( points && !points->get_keep_sorted() ){
		QPushButton *btn_down = new QPushButton( this );
		QPushButton *btn_up = new QPushButton( this );
		
		btn_up->setIcon( icon_up );
		btn_down->setIcon( icon_down );
		btn_up->setIconSize( icon_size );
		btn_down->setIconSize( icon_size );
		btn_up->setSizePolicy( fixed_policy );
		btn_down->setSizePolicy( fixed_policy );
		connect( btn_up, SIGNAL( clicked() ), this, SLOT( action_move_up() ) );
		connect( btn_down, SIGNAL( clicked() ), this, SLOT( action_move_down() ) );
		
		layout->addWidget( btn_up );
		layout->addWidget( btn_down );
	}
	
	//Add spinboxes
	sbx_x = new QSpinBox( this );
	sbx_y = new QSpinBox( this );
	sbx_x->setMaximum( 65535 );
	sbx_y->setMaximum( 65535 );
	layout->addWidget( sbx_x );
	layout->addWidget( sbx_y );
	
	//Add add/remove buttons
	QPushButton *btn_add = new QPushButton( this );
	btn_add->setIcon( icon_add );
	btn_add->setIconSize( icon_size );
	btn_add->setSizePolicy( fixed_policy );
	layout->addWidget( btn_add );
	connect( btn_add, SIGNAL( clicked() ), this, SLOT( action_add() ) );
	
	QPushButton *btn_remove = new QPushButton( this );
	btn_remove->setIcon( icon_remove );
	btn_remove->setIconSize( icon_size );
	btn_remove->setSizePolicy( fixed_policy );
	layout->addWidget( btn_remove );
	connect( btn_remove, SIGNAL( clicked() ), this, SLOT( action_remove() ) );
	
	//Update spinboxes
	if( points ){
		const point *item = points->index( index );
		if( item ){
			sbx_x->setValue( item->X );
			sbx_y->setValue( item->Y );
			
			connect( sbx_x, SIGNAL( valueChanged(int) ), this, SLOT( action_x_changed() ) );
			connect( sbx_y, SIGNAL( valueChanged(int) ), this, SLOT( action_y_changed() ) );
			
			return;
		}
	}
	
	//Something went wrong, disable the widget!
	setEnabled( false );
}


void pointArrayValueItem::action_add(){
	if( container )
		container->add( index );
}

void pointArrayValueItem::action_remove(){
	if( container )
		container->remove( index );
}

void pointArrayValueItem::action_move_up(){
	if( container )
		container->move( index, -1 );
}

void pointArrayValueItem::action_move_down(){
	if( container )
		container->move( index, 1 );
}

void pointArrayValueItem::action_x_changed(){
	if( container )
		container->change_x( index, sbx_x->value() );
}

void pointArrayValueItem::action_y_changed(){
	if( container )
		container->change_y( index, sbx_y->value() );
}

