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


#include "nxtVarWordValue.h"
#include "../../riclib/nxtVariable.h"

#include <QHBoxLayout>
#include <QSpinBox>
#include <QLabel>

nxtVarWordValue::nxtVarWordValue( nxtVarWord* variable, QString text, QWidget* parent, QString tooltip ): nxtVarEditAbstract( parent ){
	setToolTip( tooltip );
	
	//Set layout
	QHBoxLayout *layout = new QHBoxLayout( this );
	layout->setContentsMargins( QMargins( 9, 0, 9, 0 ) );
	setLayout( layout );
	
	//Add a label if text is provided
	if( !text.isEmpty() ){
		QLabel* label = new QLabel( text, parent );
		layout->addWidget( label );
	}
	
	//Add the spinbox
	value = new QSpinBox( this );
	value->setMinimum( 0 );
	value->setMaximum( 65535 );
	layout->addWidget( value );
	
	
	connect( value, SIGNAL( valueChanged(int) ), this, SLOT( update_variable() ) );
	change_object( (nxtVariable*)variable );
}

//Change the ric value that is in use
bool nxtVarWordValue::change_object( nxtVariable* object ){
	//If NULL is passed, disable the control
	if( !object ){
		setEnabled( false );
		nxt_word = NULL;
		
		return true;
	}
	else if( object->var_type() == nxtVariable::TYPE_UWORD ){
		//Change to the new variable
		nxt_word = (nxtVarWord*)object;
		value->setValue( nxt_word->value() );
		setEnabled( true );
		
		return true;
	}
	
	return false;
}

void nxtVarWordValue::update_variable(){
	if( nxt_word ){
		nxt_word->set_value( value->value() );
		
		emit value_changed();
	}
}
