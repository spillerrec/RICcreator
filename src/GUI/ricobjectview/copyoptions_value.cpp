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


#include "ui_copyoptions_value.h"
#include "copyoptions_value.h"
#include "../../riclib/nxtVariable.h"
#include "../../riclib/nxtVarRicWord.h"
#include "../../riclib/nxtCopyOptionsBase.h"
#include "../../riclib/nxtCopyOptions.h"

copyoptions_value::copyoptions_value( nxtVariable* value_object, int settings, QWidget* parent ): nxtVarEditAbstract(parent), ui(new Ui_copyoptions){
	ui->setupUi(this);
	copyoptions = NULL;
	
	//Hide unneeded settings
	if( settings & RIC_OBJECT ){
		ui->clear->hide();
		ui->fill_shape->hide();
		ui->polyline->hide();
		
		if( settings & RIC_POLYLINE )
			ui->polyline->show();
		if( settings & RIC_FILL_SHAPE )
			ui->fill_shape->show();
	}
	
	//Connect signals to slots
	connect( ui->clear, SIGNAL( stateChanged( int ) ), this, SLOT( write() ) );
	connect( ui->invert, SIGNAL( stateChanged( int ) ), this, SLOT( write() ) );
	connect( ui->fill_shape, SIGNAL( stateChanged( int ) ), this, SLOT( write() ) );
	connect( ui->polyline, SIGNAL( stateChanged( int ) ), this, SLOT( write() ) );
	
	connect( ui->merge_and, SIGNAL( toggled(bool) ), this, SLOT( write() ) );
	connect( ui->merge_normal, SIGNAL( toggled(bool) ), this, SLOT( write() ) );
	connect( ui->merge_or, SIGNAL( toggled(bool) ), this, SLOT( write() ) );
	connect( ui->merge_xor, SIGNAL( toggled(bool) ), this, SLOT( write() ) );
	
	change_object( value_object );
}


//Read the value in ricword and update the values
void copyoptions_value::read(){
	if( copyoptions ){
		//Prevent it to overwrite ricword while reading, since this triggers the spinboxes valueChanged() signal
		nxtCopyOptionsBase* temp = copyoptions;
		copyoptions = NULL;
		
		//Get the correct values
		//Clear
		ui->clear->setCheckState( Qt::Unchecked );
		if( temp->get_clear_except_status() )
			ui->clear->setCheckState( Qt::PartiallyChecked );
		if( temp->get_clear() )
			ui->clear->setCheckState( Qt::Checked );
		
		//Other checkboxes
		ui->invert->setChecked( temp->get_invert() );
		ui->fill_shape->setChecked( temp->get_fill_shape() );
		ui->polyline->setChecked( temp->get_polyline() );
		
		//Radio buttons
		switch( temp->get_merge() ){
			case nxtCopyOptionsBase::MERGE_COPY: ui->merge_normal->setChecked( true ); break;
			case nxtCopyOptionsBase::MERGE_AND: ui->merge_and->setChecked( true ); break;
			case nxtCopyOptionsBase::MERGE_OR: ui->merge_or->setChecked( true ); break;
			case nxtCopyOptionsBase::MERGE_XOR: ui->merge_xor->setChecked( true ); break;
		}
		
		//Reenable writing
		copyoptions = temp;
	}
}

//Write the values back into ricword
void copyoptions_value::write(){
	if( !copyoptions )
		return;	//Nothing to write to...
	
	//Write clear
	switch( ui->clear->checkState() ){
		case Qt::Checked:
				copyoptions->set_clear( true );
				copyoptions->set_clear_except_status( false );
			break;
		
		case Qt::PartiallyChecked:
				copyoptions->set_clear( false );
				copyoptions->set_clear_except_status( true );
			break;
		
		case Qt::Unchecked:
				copyoptions->set_clear( false );
				copyoptions->set_clear_except_status( false );
			break;
	}
	
	//Write other checkboxes
	copyoptions->set_invert( ui->invert->isChecked() );
	copyoptions->set_fill_shape( ui->fill_shape->isChecked() );
	copyoptions->set_polyline( ui->polyline->isChecked() );
	
	//Write merge
	if( ui->merge_normal->isChecked() )
		copyoptions->set_merge( nxtCopyOptionsBase::MERGE_COPY );
	else if( ui->merge_and->isChecked() )
		copyoptions->set_merge( nxtCopyOptionsBase::MERGE_AND );
	else if( ui->merge_xor->isChecked() )
		copyoptions->set_merge( nxtCopyOptionsBase::MERGE_XOR );
	else if( ui->merge_or->isChecked() )
		copyoptions->set_merge( nxtCopyOptionsBase::MERGE_OR );
	
	emit value_changed();
}

//Change the ric value that is in use
bool copyoptions_value::change_object( nxtVariable* object ){
	//If NULL is passed, disable the control
	if( !object ){
		setEnabled( false );
		copyoptions = NULL;
		
		return true;
	}
	else if( object->var_type() == nxtVariable::TYPE_COPYOPTIONS || object->var_type() == nxtVariable::TYPE_RIC_COPYOPTIONS ){
		//Change to the new variable
		if( object->var_type() == nxtVariable::TYPE_COPYOPTIONS )
			copyoptions = (nxtCopyOptionsBase*)(nxtCopyOptions*)object;
		else
			copyoptions = (nxtCopyOptionsBase*)(nxtVarRicWord*)object;
		read();
		setEnabled( true );
		
		return true;
	}
	
	return false;
}


