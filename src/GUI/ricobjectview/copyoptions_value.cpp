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

copyoptions_value::copyoptions_value( ricfile::nxtVarRicCopyoptions* value_object, QWidget* parent ): QWidget(parent), ui(new Ui_copyoptions){
	ui->setupUi(this);
	connect( ui->clear, SIGNAL( stateChanged( int ) ), this, SLOT( write() ) );
	connect( ui->invert, SIGNAL( stateChanged( int ) ), this, SLOT( write() ) );
	connect( ui->fill_shape, SIGNAL( stateChanged( int ) ), this, SLOT( write() ) );
	connect( ui->polyline, SIGNAL( stateChanged( int ) ), this, SLOT( write() ) );
	
	connect( ui->merge_and, SIGNAL( toggled() ), this, SLOT( write() ) );
	connect( ui->merge_normal, SIGNAL( toggled() ), this, SLOT( write() ) );
	connect( ui->merge_or, SIGNAL( toggled() ), this, SLOT( write() ) );
	connect( ui->merge_xor, SIGNAL( toggled() ), this, SLOT( write() ) );
	
	change_value_object( value_object );
}


//Read the value in ricword and update the values
void copyoptions_value::read(){
	if( copyoptions ){
		//Prevent it to overwrite ricword while reading, since this triggers the spinboxes valueChanged() signal
		ricfile::nxtVarRicCopyoptions* temp = copyoptions;
		copyoptions = NULL;
		
		//Get the correct values
		//Clear
		ui->clear->setCheckState( Qt::Unchecked );
		if( temp->clear_except_status )
			ui->clear->setCheckState( Qt::PartiallyChecked );
		if( temp->clear )
			ui->clear->setCheckState( Qt::Checked );
		
		//Other checkboxes
		ui->invert->setChecked( temp->invert );
		ui->fill_shape->setChecked( temp->fill_shape );
		ui->polyline->setChecked( temp->polyline );
		
		//Radio buttons
		switch( temp->merge ){
			case ricfile::nxtVarRicCopyoptions::MERGE_COPY: ui->merge_normal->setChecked( true ); break;
			case ricfile::nxtVarRicCopyoptions::MERGE_AND: ui->merge_and->setChecked( true ); break;
			case ricfile::nxtVarRicCopyoptions::MERGE_OR: ui->merge_or->setChecked( true ); break;
			case ricfile::nxtVarRicCopyoptions::MERGE_XOR: ui->merge_xor->setChecked( true ); break;
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
				copyoptions->clear = true;
				copyoptions->clear_except_status = false;
			break;
		
		case Qt::PartiallyChecked:
				copyoptions->clear = false;
				copyoptions->clear_except_status = true;
			break;
		
		case Qt::Unchecked:
				copyoptions->clear = false;
				copyoptions->clear_except_status = false;
			break;
	}
	
	//Write other checkboxes
	copyoptions->invert = ui->invert->isChecked();
	copyoptions->fill_shape = ui->fill_shape->isChecked();
	copyoptions->polyline = ui->polyline->isChecked();
	
	//Write merge	//TODO: this seems to fail :\ Or at least something does fail...
	if( ui->merge_normal->isChecked() )
		copyoptions->merge = ricfile::nxtVarRicCopyoptions::MERGE_COPY;
	else if( ui->merge_and->isChecked() )
		copyoptions->merge = ricfile::nxtVarRicCopyoptions::MERGE_AND;
	else if( ui->merge_xor->isChecked() )
		copyoptions->merge = ricfile::nxtVarRicCopyoptions::MERGE_XOR;
	else if( ui->merge_or->isChecked() )
		copyoptions->merge = ricfile::nxtVarRicCopyoptions::MERGE_OR;
	
	emit value_changed();
}

//Change the ric value that is in use
void copyoptions_value::change_value_object( ricfile::nxtVarRicCopyoptions* new_value_object ){
	copyoptions = new_value_object;
	if( copyoptions ){
		read();
		setEnabled( true );
	}
	else
		setEnabled( false );
}


