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


#include "ui_ric_value.h"
#include "ric_value.h"

#include "../riclib/nxtVarRicWord.h"
#include "nxtVarEdits/nxtRicIdValue.h"

ric_value::ric_value( QWidget* parent, QString text, nxtVarRicWord* value_object, QString tooltip ): nxtVarEditAbstract(parent), ui(new Ui_ric_value_select){
	ui->setupUi(this);
	connect( ui->parameter_mode, SIGNAL( stateChanged( int ) ), this, SLOT( update_mode() ) );
	
	connect( ui->varmap_id, SIGNAL( valueChanged( int ) ), this, SLOT( write() ) );
	connect( ui->parameter, SIGNAL( valueChanged( int ) ), this, SLOT( write() ) );
	connect( ui->value, SIGNAL( valueChanged( int ) ), this, SLOT( write() ) );
	
	
	//Set settings
	ui->text->setText( text );
	ui->text->setToolTip( tooltip );
	change_object( (nxtVariable*)value_object );
}


//Read the value in ricword and update the values
void ric_value::read(){
	if( ricword ){
		//Prevent it to overwrite ricword while reading, since this triggers the spinboxes valueChanged() signal
		nxtVarRicWord* temp = ricword;
		ricword = NULL;
		
		//Get the correct values
		ui->varmap_id->setValue( temp->get_varmap() );
		ui->parameter->setValue( temp->get_parameter() );
		ui->value->setValue( temp->get_number() );
		ui->parameter_mode->setChecked( temp->is_extended() );
		
		//Reenable writing
		ricword = temp;
		
		revalidate();
	}
	
	refresh_mode();	//Change mode if neccessary
}

//Write the values back into ricword
void ric_value::write(){
	if( !ricword )
		return;	//Nothing to write to...
	
	if( ui->parameter_mode->isChecked() )
		ricword->set_extended( ui->varmap_id->value(), ui->parameter->value() );
	else
		ricword->set_normal( ui->value->value() );
	
	revalidate();
	emit value_changed();
}

//Change the ric value that is in use
bool ric_value::change_object( nxtVariable* object ){
	//If NULL is passed, disable the control
	if( !object ){
		setEnabled( false );
		ricword = NULL;
		
		return true;
	}
	else if( object->var_type() == nxtVariable::TYPE_RIC_WORD ){
		//Change to the new variable
		ricword = (nxtVarRicWord*)object;
		read();
		setEnabled( true );
		
		return true;
	}
	
	return false;
}


//Make sure the right spinboxes are visible
void ric_value::refresh_mode(){
	if( ui->parameter_mode->isChecked() ){
		ui->value->hide();
		ui->varmap_id->show();
		ui->parameter->show();
	}
	else{
		ui->varmap_id->hide();
		ui->parameter->hide();
		ui->value->show();
	}
}

void ric_value::update_mode(){
	refresh_mode();
	write();
}

void ric_value::revalidate(){
	if( ricword ){
		if( ui->varmap_id->value() == 0 )
			ui->varmap_id->setStyleSheet( nxtRicIdValue::style_none );
		else
			nxtRicIdValue::apply_validation( (QWidget*)ui->varmap_id, ui->varmap_id->value(), ricword->parent(), false );
		
		if( ricword->is_id() )
			nxtRicIdValue::apply_validation( (QWidget*)ui->value, ui->value->value(), ricword->parent(), true );
	}
}

