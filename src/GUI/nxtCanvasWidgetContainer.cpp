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
	frame->setFrameShape( QFrame::StyledPanel );
	
	setLayout( new QGridLayout );
	QGridLayout *gridlayout = (QGridLayout*)layout();
	gridlayout->setContentsMargins( QMargins() );
	gridlayout->addWidget( frame, 0,0 );
	gridlayout->setHorizontalSpacing( 0 );
	gridlayout->setVerticalSpacing( 0 );
	
	if( moveable ){
		//Add scrollbars
		move_x = new QScrollBar( this );
		move_x->setOrientation( Qt::Horizontal );
		move_y = new QScrollBar( this );
		gridlayout->addWidget( move_x, 1,0 );
		gridlayout->addWidget( move_y, 0,1 );
		
		//Add parameters
		view->set_moveable( true );
		
		//Add signals
		connect( move_y, SIGNAL( actionTriggered( int ) ), this, SLOT( scrollbar_action( int ) ) );
		connect( move_y, SIGNAL( sliderReleased() ), this, SLOT( scrollbar_set_ranges() ) );
		connect( move_x, SIGNAL( actionTriggered( int ) ), this, SLOT( scrollbar_action( int ) ) );
		connect( move_x, SIGNAL( sliderReleased() ), this, SLOT( scrollbar_set_ranges() ) );
		connect( canvas_view, SIGNAL( visible_area_changed() ), this, SLOT( scrollbar_set_ranges() ) );
		connect( canvas_view, SIGNAL( canvas_changed() ), this, SLOT( scrollbar_set_ranges() ) );
	}
}

void nxtCanvasWidgetContainer::scrollbar_set_ranges(){
	unsigned int width = canvas_view->canvas_width();
	unsigned int height = canvas_view->canvas_height();
	int pos_x = canvas_view->get_pos_x();
	int pos_y = canvas_view->get_pos_y();
	
	//Set ranges
	move_x->setMinimum( pos_x + width );
	move_x->setMaximum( pos_x - width );
	move_x->setPageStep( width );	//TODO: this should be the width viewable on the screen
	move_x->setValue( pos_x );
	
	move_y->setMinimum( pos_y - height );
	move_y->setMaximum( pos_y + height );
	move_y->setPageStep( height );
	move_y->setValue( pos_y );
}
void nxtCanvasWidgetContainer::scrollbar_action( int action ){
	//Move view
	canvas_view->change_pos_x( move_x->sliderPosition() );
	canvas_view->change_pos_y( move_y->sliderPosition() );
	
	//If this isn't a drag, reset right away
	if( action != QAbstractSlider::SliderMove )
		scrollbar_set_ranges();
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

