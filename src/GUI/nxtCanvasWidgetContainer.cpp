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


#include "nxtCanvasWidgetContainer.h"
#include "nxtCanvasWidget.h"
#include <QSizePolicy>
#include <QFrame>
#include <QGridLayout>
#include <QScrollBar>
#include <QMargins>

#include <QKeyEvent>
#include <QMouseEvent>


nxtCanvasWidgetContainer::nxtCanvasWidgetContainer( nxtCanvasWidget* view, bool moveable, QWidget* parent ): QWidget( parent ){
	canvas_view = view;
	
	setSizePolicy( QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding) );
	
	QFrame *frame = new QFrame( this );
	frame->setLayout( new QHBoxLayout );
	frame->layout()->addWidget( canvas_view );
	frame->layout()->setContentsMargins( QMargins() );
	frame->setFrameShape( QFrame::Box );
	
	setLayout( new QGridLayout );
	QGridLayout *gridlayout = (QGridLayout*)layout();
	gridlayout->setContentsMargins( QMargins() );
	gridlayout->addWidget( frame, 0,0 );
	gridlayout->setHorizontalSpacing( 0 );
	gridlayout->setVerticalSpacing( 0 );
	
	if( moveable ){
		//Add scrollbars
		move_x = new QScrollBar( this );
		move_y = new QScrollBar( this );
		gridlayout->addWidget( move_x, 1,0 );
		gridlayout->addWidget( move_y, 0,1 );
		
		//Add parameters
		
		//Add signals
		connect( move_x, SIGNAL( valueChanged( int ) ), this, SLOT( update_scrollbars() ) );
		connect( move_y, SIGNAL( valueChanged( int ) ), this, SLOT( update_scrollbars() ) );
		connect( canvas_view, SIGNAL( visible_area_changed() ), this, SLOT( set_scrollbars() ) );
		connect( canvas_view, SIGNAL( canvas_changed() ), this, SLOT( reset_scrollbars() ) );
	}
}

void nxtCanvasWidgetContainer::set_scrollbars(){
	unsigned int width = canvas_view->canvas_width();
	move_x->setPageStep( width );
	move_x->setMinimum( -width );
	move_x->setMaximum( width );
	move_x->setOrientation( Qt::Horizontal );
	
	unsigned int height = canvas_view->canvas_height();
	move_y->setPageStep( height );
	move_y->setMinimum( -height );
	move_y->setMaximum( height );
}

void nxtCanvasWidgetContainer::reset_scrollbars(){
	set_scrollbars();
	move_x->setValue( 0 );
	move_y->setValue( 0 );
}

void nxtCanvasWidgetContainer::update_scrollbars(){
	canvas_view->change_pos_x( -move_x->value() );
	canvas_view->change_pos_y( move_y->value() );
}

//TODO:
void nxtCanvasWidgetContainer::keyPressEvent( QKeyEvent * event ){
	
	event->ignore();
}
void nxtCanvasWidgetContainer::keyReleaseEvent( QKeyEvent * event ){
	
	event->ignore();
}
void nxtCanvasWidgetContainer::mousePressEvent( QMouseEvent *event ){
	
	event->ignore();
}
void nxtCanvasWidgetContainer::mouseMoveEvent( QMouseEvent *event ){
	
	event->ignore();
}
void nxtCanvasWidgetContainer::mouseReleaseEvent( QMouseEvent *event ){
	
	event->ignore();
}

