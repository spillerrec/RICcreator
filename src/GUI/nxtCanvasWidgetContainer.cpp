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

#include "nxtVarEdits/copyoptions_value.h"
#include "../riclib/nxtCopyOptions.h"

#include <QSizePolicy>
#include <QFrame>
#include <QGroupBox>
#include <QGridLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QSlider>
#include <QScrollBar>
#include <QMargins>

#include <QKeyEvent>
#include <QMouseEvent>


#define add_tool_button( icon, tool, x, y ){	\
	QPushButton *temp = new QPushButton( QIcon( icon ), "", this );	\
	temp->setFlat( true );	\
	temp->setCheckable( true );	\
	temp->setIconSize( QSize( 22, 22 ) );	\
	temp->setMinimumSize( 26, 26 );	\
	temp->setMaximumSize( 26, 26 );	\
	group->addButton( temp, tool );	\
	tool_btn_layout->addWidget( temp, x, y );	\
}

#define add_action_button( icon, action, x, y ){	\
	QPushButton *temp = new QPushButton( QIcon( icon ), "", this );	\
	temp->setIconSize( QSize( 22, 22 ) );	\
	temp->setMinimumSize( 28, 28 );	\
	temp->setMaximumSize( 28, 28 );	\
	connect( temp, SIGNAL( clicked(bool) ), &canvas_view, SLOT( action ) );	\
	action_btn_layout->addWidget( temp, x, y );	\
}


nxtCanvasWidgetContainer::nxtCanvasWidgetContainer( nxtCanvasWidget& view, QWidget* parent, bool editable, bool moveable ): nxtVarEditAbstract( parent ), canvas_view( view ){
	
	setSizePolicy( QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding) );
	
	
	connect( &canvas_view, SIGNAL( value_changed() ), this, SIGNAL( value_changed() ) );
	
	QFrame *frame = new QFrame( this );
	frame->setLayout( new QHBoxLayout );
	frame->layout()->addWidget( &canvas_view );
	frame->layout()->setContentsMargins( QMargins() );
	frame->setFrameShape( QFrame::StyledPanel );
	
	QGridLayout &gridlayout = *(new QGridLayout);
	setLayout( &gridlayout );
	gridlayout.setContentsMargins( QMargins() );
	gridlayout.addWidget( frame, 0,0 );
	gridlayout.setHorizontalSpacing( 0 );
	gridlayout.setVerticalSpacing( 0 );
	
	if( editable ){
		QHBoxLayout &btn_layout = *new QHBoxLayout;
		gridlayout.addLayout( &btn_layout, 2, 0, 1, -1 );
		btn_layout.setSpacing( 6 );
		
		//Add zoom slider
		QSlider *slider = new QSlider( Qt::Vertical, this );
		slider->setRange( 1, 10 );
		slider->setValue( 4 );
		slider->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );	//TODO: fix sizing
		btn_layout.addWidget( slider );
		connect( slider, SIGNAL( valueChanged(int) ), &canvas_view, SLOT( zoom(int) ) );
		canvas_view.zoom( slider->value() );
		
		//Add group boxes
		QGroupBox *tool_box = new QGroupBox( tr( "Tools" ), this );
		QGridLayout *tool_btn_layout = new QGridLayout;
		tool_btn_layout->setSpacing( 4 );
		tool_box->setLayout( tool_btn_layout );
		
		QGroupBox *action_box = new QGroupBox( tr( "Actions" ), this );
		QGridLayout *action_btn_layout = new QGridLayout;
		action_btn_layout->setSpacing( 2 );
		action_box->setLayout( action_btn_layout );
		
		btn_layout.addWidget( tool_box );
		btn_layout.addWidget( action_box );
		
		//Add tool buttons
		QButtonGroup *group = new QButtonGroup( this );
		connect( group, SIGNAL( buttonClicked(int) ), this, SLOT( set_tool(int) ) );
		
		add_tool_button( ":/edit/draw_freehand.png", tool_freehand, 0, 0 );
		group->button( tool_freehand )->setChecked( true );
		add_tool_button( ":/edit/draw_line.png", tool_line, 0, 1 );
		add_tool_button( ":/edit/draw_rectangle.png", tool_rectangle, 1, 0 );
		add_tool_button( ":/edit/draw_ellipse.png", tool_ellipse, 1, 1 );
		add_tool_button( ":/edit/selection.png", tool_selection, 2, 0 );
		add_tool_button( ":/edit/bucket_fill.png", tool_fill, 2, 1 );
		
		//Add action buttons
		add_action_button( ":/edit/copy.png", copy_to_clipboard(), 0, 0 );
		add_action_button( ":/edit/paste.png", paste_from_clipboard(), 0, 1 );
		add_action_button( ":/edit/import_image.png", import_image(), 1, 0 );
		add_action_button( ":/filehandling/save.png", save(), 1, 1 );
		add_action_button( ":/edit/crop.png", crop(), 2, 0 );
		
		
		//Add copy_options
		options = new nxtCopyOptions;
		copyedit = new copyoptions_value( options, this );
		btn_layout.addWidget( copyedit );
		canvas_view.set_options( options );
		set_tool( group->checkedId() );
		
		connect( &canvas_view, SIGNAL( options_changed() ), copyedit, SLOT( update() ) );
		
		btn_layout.addStretch();
	}
	else{
		options = NULL;
		copyedit = NULL;
	}
	
	if( moveable ){
		//Add scrollbars
		move_x = new QScrollBar( Qt::Horizontal, this );
		move_y = new QScrollBar( this );
		gridlayout.addWidget( move_x, 1,0 );
		gridlayout.addWidget( move_y, 0,1 );
		
		//Add parameters
		canvas_view.set_moveable( true );
		
		//Add signals
		connect( move_y, SIGNAL( actionTriggered( int ) ), this, SLOT( scrollbar_action( int ) ) );
		connect( move_y, SIGNAL( sliderReleased() ), this, SLOT( scrollbar_set_ranges() ) );
		connect( move_x, SIGNAL( actionTriggered( int ) ), this, SLOT( scrollbar_action( int ) ) );
		connect( move_x, SIGNAL( sliderReleased() ), this, SLOT( scrollbar_set_ranges() ) );
		connect( &canvas_view, SIGNAL( visible_area_changed() ), this, SLOT( scrollbar_set_ranges() ) );
		connect( &canvas_view, SIGNAL( canvas_changed() ), this, SLOT( scrollbar_set_ranges() ) );
	}
}

nxtCanvasWidgetContainer::~nxtCanvasWidgetContainer(){
	if( options )
		delete options;
}

bool nxtCanvasWidgetContainer::change_object( nxtVariable* object ){
	return canvas_view.change_object( object );
}


void nxtCanvasWidgetContainer::set_tool( int tool ){
	options->enabled_fill_shape = false;
	options->enabled_polyline = false;
	
	switch( (tools)tool ){
		case tool_freehand:
				canvas_view.set_tool( nxtCanvasWidget::TOOL_PIXEL );
			break;
		
		case tool_line:
				canvas_view.set_tool( nxtCanvasWidget::TOOL_LINE );
			break;
		
		case tool_rectangle:
				canvas_view.set_tool( nxtCanvasWidget::TOOL_RECT );
				options->enabled_fill_shape = true;
			break;
		
		case tool_ellipse:
				canvas_view.set_tool( nxtCanvasWidget::TOOL_ELLIPSE );
				options->enabled_fill_shape = true;
			break;
		
		case tool_selection:
				canvas_view.set_tool( nxtCanvasWidget::TOOL_SELECTION );
				//TODO: disable copy options?
			break;
		
		case tool_fill:
				canvas_view.set_tool( nxtCanvasWidget::TOOL_FILL );
			break;
		
		default:
				//Error
				canvas_view.set_tool( nxtCanvasWidget::TOOL_NONE );
				qWarning( "nxtCanvasWidget: invalid tool selected!" );
	}
	
	copyedit->update();
}





void nxtCanvasWidgetContainer::scrollbar_set_ranges(){
	unsigned int width = canvas_view.canvas_width();
	unsigned int height = canvas_view.canvas_height();
	int pos_x = canvas_view.get_pos_x();
	int pos_y = canvas_view.get_pos_y();
	
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
	canvas_view.change_pos_x( move_x->sliderPosition() );
	canvas_view.change_pos_y( move_y->sliderPosition() );
	
	//If this isn't a drag, reset right away
	if( action != QAbstractSlider::SliderMove )
		scrollbar_set_ranges();
}


