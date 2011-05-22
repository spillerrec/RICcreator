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


#include "../../riclib/nxtCanvas.h"
#include "copyoptions_value.h"
#include "spriteValue.h"
#include "ui_spriteValue.h"
#include "../../riclib/nxtCopyOptions.h"

#include <QMouseEvent>
#include <QPointF>

spriteValue::spriteValue( QWidget* parent ): QWidget( parent ), ui( new Ui_spriteedit_form ), edit( (QWidget*)this ){
	ui->setupUi(this);
	
	ui->main_layout->insertWidget( 0, (QWidget*)&edit );
	
	
	connect( ui->zoom_slider, SIGNAL( valueChanged(int) ), this, SLOT( update_zoom() ) );
	update_zoom();
	
	connect( ui->tool_draw, SIGNAL( released() ), this, SLOT( update_tool() ) );
	connect( ui->tool_line, SIGNAL( released() ), this, SLOT( update_tool() ) );
	connect( ui->tool_rect, SIGNAL( released() ), this, SLOT( update_tool() ) );
	connect( ui->tool_ellipse, SIGNAL( released() ), this, SLOT( update_tool() ) );
	
	connect( (QWidget*)&edit, SIGNAL( value_changed() ), this, SIGNAL( value_changed() ) );
	
	options = new nxtCopyOptions;	//TODO: delete again
	edit.set_options( options );
	
	copyedit = new copyoptions_value( options, 0, this );
	
	ui->horizontalLayout->insertWidget( 1, (QWidget*)copyedit );
}


void spriteValue::update_zoom(){
	edit.zoom( ui->zoom_slider->value() );
}

void spriteValue::update_tool(){
	if( ui->tool_draw->isChecked() )
		edit.set_tool( 0 );
	if( ui->tool_line->isChecked() )
		edit.set_tool( 1 );
	if( ui->tool_rect->isChecked() )
		edit.set_tool( 2 );
	if( ui->tool_ellipse->isChecked() )
		edit.set_tool( 3 );
}

spriteValue::~spriteValue(){
	delete ui;
}

