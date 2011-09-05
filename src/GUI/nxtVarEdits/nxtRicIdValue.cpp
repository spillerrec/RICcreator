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


#include "nxtRicIdValue.h"
#include "../../riclib/nxtVarRicWord.h"

#include <QLabel>
#include <QSpinBox>
#include <QHBoxLayout>


const QString nxtRicIdValue::style_ok = "QSpinBox{ background: #c2f39e }";
const QString nxtRicIdValue::style_warning = "QSpinBox{ background: #fff953 }";
const QString nxtRicIdValue::style_error = "QSpinBox{ background: #ff5353 }";
const QString nxtRicIdValue::style_none = "QSpinBox{ background: white }";


nxtRicIdValue::nxtRicIdValue( nxtVariable* variable, QWidget* parent ): nxtVarEditAbstract( parent ){
	ric_id = new QSpinBox( this );
	ric_id->setRange( 0, 0xFFFF );
	
	QLabel *lbl = new QLabel( tr("ID"), this );
	
	QHBoxLayout *layout = new QHBoxLayout( this );
	setLayout( layout );
	layout->addWidget( lbl );
	layout->addWidget( ric_id );
	layout->addStretch();
	
	
	connect( ric_id, SIGNAL( valueChanged(int) ), this, SLOT( update_variable() ) );
	change_object( variable );
}


//Change the ric value that is in use
bool nxtRicIdValue::change_object( nxtVariable* object ){
	//If NULL is passed, disable the control
	if( !object ){
		setEnabled( false );
		nxt_word = NULL;
		
		return true;
	}
	else if( object->var_type() == nxtVariable::TYPE_RIC_ID ){
		//Change to the new variable
		nxt_word = (ricVarId*)object;
		
		//Update variable
		//Disconnect to avoid emitting value_changed() !
		disconnect( ric_id, SIGNAL( valueChanged(int) ), this, SLOT( update_variable() ) );
		ric_id->setValue( nxt_word->value() );
		connect( ric_id, SIGNAL( valueChanged(int) ), this, SLOT( update_variable() ) );
		revalidate();
		setEnabled( true );
		
		return true;
	}
	
	return false;
}

void nxtRicIdValue::revalidate(){
		switch( nxt_word->is_valid() ){
			case ricVarId::ID_VALID: setStyleSheet( style_ok ); break;
			case ricVarId::ID_INVALID: setStyleSheet( style_error ); break;
			case ricVarId::ID_OWERWRITES: setStyleSheet( style_warning ); break;
			default: setStyleSheet( style_none );
		}
}

void nxtRicIdValue::update_variable(){
	if( nxt_word ){
		nxt_word->set_value( ric_id->value() );
		revalidate();
		
		emit value_changed();
	}
}

void nxtRicIdValue::apply_validation( QWidget *widget, unsigned int id, ricObject *object, bool native ){
	switch( ricVarId::validate_id( id, object, native ) ){
		case ricVarId::ID_VALID: widget->setStyleSheet( style_ok ); break;
		case ricVarId::ID_INVALID: widget->setStyleSheet( style_error ); break;
		case ricVarId::ID_OWERWRITES: widget->setStyleSheet( style_warning ); break;
		default: widget->setStyleSheet( style_none );
	}
}
