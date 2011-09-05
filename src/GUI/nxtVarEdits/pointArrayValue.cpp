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


#include "pointArrayValue.h"
#include "pointArrayValueItem.h"
#include "../../riclib/pointArray.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QScrollArea>
#include <QSizePolicy>

#include <QPushButton>
#include <QIcon>


pointArrayValue::pointArrayValue( nxtVariable* variable, QWidget* parent ): nxtVarEditAbstract( parent ){
	scrollbars = new QScrollArea( this );
	
	list_widget = new QWidget( scrollbars );
	list_area = new QVBoxLayout( list_widget );
	list_widget->setLayout( list_area );
	
	scrollbars->setWidget( list_widget );
	scrollbars->setWidgetResizable( true );
	
	setLayout( new QVBoxLayout( this ) );
	layout()->addWidget( scrollbars );
	layout()->setContentsMargins( 0, 0, 0, 0 );
	
	//Add buttons
	QHBoxLayout *btn_layout = new QHBoxLayout();
	
	//Add button
	QPushButton *add_point = new QPushButton( this );
	add_point->setIcon( QIcon( ":/edit/add.png" ) );
	add_point->setIconSize( QSize( 16, 16 ) );
	add_point->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );
	connect( add_point, SIGNAL( clicked() ), this, SLOT( add() ) );
	
	//Optimize button
	QPushButton *optimize_map = new QPushButton( tr("Optimize"), this );
	optimize_map->setToolTip( tr( "Remove reluctant points" ) );
//	optimize_map->setIcon( QIcon( ":/edit/add.png" ) );
//	optimize_map->setIconSize( QSize( 16, 16 ) );
	optimize_map->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );
	connect( optimize_map, SIGNAL( clicked() ), this, SLOT( optimize() ) );
	
	btn_layout->addWidget( add_point );
	btn_layout->addWidget( optimize_map );
	btn_layout->addStretch();
	
	((QVBoxLayout*)layout())->addLayout( btn_layout );
	
	change_object( variable );
}


//TODO: FIX THIS!!!
void pointArrayValue::empty_list(){
	QLayoutItem *child;
	while( ( child = list_area->takeAt(0) ) ){
		QWidget *temp = child->widget();
		if( temp )
			temp->close();
	//	if( child->widget() )
	//		delete child->widget();
		
	//	if( child->layout() )
		//	delete child->layout();
		
	//	if( child->spacerItem() ){
		//	delete child->spacerItem();
	//	}	//Memory leak? Crashes otherwise
		
		delete child;
	}
}

void pointArrayValue::build_list(){
	//Remember scrollbar position to keep visiable area in place
	int current_position = scrollbars->verticalScrollBar()->sliderPosition();
	
	empty_list();
	
	if( !points )
		return;
	
	//Add points
	for( unsigned int i=0; i<points->size(); i++ )
		list_area->addWidget( new pointArrayValueItem( points, i, this ) );
	
	list_area->addStretch();
	
	//Reset scrollbar position
	scrollbars->verticalScrollBar()->setSliderPosition( current_position );
}


//Change the ric value that is in use
bool pointArrayValue::change_object( nxtVariable* object ){
	//If NULL is passed, disable the control
	if( !object ){
		setEnabled( false );
		points = NULL;
		
		return true;
	}
	else if( object->var_type() == nxtVariable::TYPE_POINT_ARRAY ){
		//Change to the new variable
		points = (pointArray*)object;
		build_list();
		setEnabled( true );
		
		return true;
	}
	
	return false;
}



void pointArrayValue::add(){
	add( pointArray::INVALID_INDEX );
}

void pointArrayValue::add( unsigned int after_index ){
	if( points ){
		unsigned int index = points->new_point( after_index );
		//TODO: add widget and inform others
		build_list();
		emit value_changed();
	}
}


void pointArrayValue::remove( unsigned int index ){
	if( points ){
		points->remove( index );
		//TODO: delete widget and inform others
		build_list();
		emit value_changed();
	}
}


void pointArrayValue::move( unsigned int index, int offset ){
	if( points && points->move( index, index + offset ) ){
		//TODO: move widget and inform others
		build_list();
		emit value_changed();
	}
}


void pointArrayValue::change_x( unsigned int index, unsigned int value ){
	if( points ){
		qDebug( "change_x - index: %d - value: %d", index, value );
		int offset = points->change_x( index, value );
		if( offset != 0 ){
			//TODO: move widget and inform others
			build_list();
		}
		emit value_changed();
	}
}


void pointArrayValue::change_y( unsigned int index, unsigned int value ){
	if( points ){
		points->change_y( index, value );
		emit value_changed();
	}
}

void pointArrayValue::optimize(){
	if( points ){
		points->optimize_map();
		build_list();
		emit value_changed();
	}
}

