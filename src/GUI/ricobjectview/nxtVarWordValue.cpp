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
#include <QHBoxLayout>

nxtVarWordValue::nxtVarWordValue( nxtVarWord* variable, QWidget* parent ): QWidget( parent ){
	ricfont = new QCheckBox( "RIC font", this );
	ricfont->setToolTip( "If checked this ricfile will be a RIC font" );
	
	QHBoxLayout *layout = new QHBoxLayout( this );
	setLayout( layout );
	layout->addWidget( ricfont );
	
	
	connect( ricfont, SIGNAL( stateChanged(int) ), this, SLOT( update_variable() ) );
	change_value_object( variable );
}


void nxtVarWordValue::change_value_object( nxtVarWord* new_value ){
	nxt_word = new_value;
	if( nxt_word && (nxt_word->value() == (unsigned int)32769) )
		ricfont->setChecked( true );
	else
		ricfont->setChecked( false );
}

void nxtVarWordValue::update_variable(){
	if( nxt_word ){
		if( ricfont->isChecked() )
			nxt_word->set_value( 32769 );
		else
			nxt_word->set_value( 0 );
		
		emit value_changed();
	}
}
