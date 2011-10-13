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

#include "ricObjectAbstract.h"

#include <QVBoxLayout>
#include "nxtVarEdits/nxtVarEditAbstract.h"
#include "../riclib/nxtVariable.h"
#include "ricObjectTexter.h"

//Type of controls
#include "nxtVarEdits/nxtVarWordValue.h"
#include "nxtVarEdits/copyoptions_value.h"
#include "ric_value.h"
#include "nxtCanvasWidget.h"
#include "nxtVarEdits/nxtRicWordExtraValue.h"
#include "nxtVarEdits/nxtRicIdValue.h"
#include "nxtVarEdits/pointArrayValue.h"
#include "nxtVarEdits/spriteValue.h"

ricObjectAbstract::ricObjectAbstract( ricObject::object_op object_type, bool autofill, QWidget *parent ): QWidget( parent ){
	QVBoxLayout* new_layout = new QVBoxLayout( (QWidget*)this );
	setLayout( new_layout );
	((QVBoxLayout*)layout())->addStretch();
	
	object_opcode = object_type;
	if( autofill ){
		for( unsigned int i=0; i<ricObjectTexter::object_property_amount( object_opcode ); i++ )
			add_control( i );
	}
}


void ricObjectAbstract::add_control_to_list( nxtVarEditAbstract* control, unsigned int index ){
	((QVBoxLayout*)layout())->insertWidget( layout()->count()-1, control );
	
	control_list.push_back( control );
	control_index_list.push_back( index );
	
	connect( control, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
}

void ricObjectAbstract::add_control( unsigned int parameter_index ){
	ricObject* object = ricObjectTexter::object( object_opcode );
	if( object ){
		nxtVariable* new_var = object->get_setting( parameter_index );
		if( new_var ){
			QString name = ricObjectTexter::object_property_name( object_opcode, parameter_index );
			QString tooltip = ricObjectTexter::object_property_description( object_opcode, parameter_index );
			
			switch( new_var->var_type() ){
				case nxtVariable::TYPE_UWORD:{
						add_control_to_list( (nxtVarEditAbstract*) new nxtVarWordValue( NULL, name, this, tooltip ), parameter_index );
					} break;
				case nxtVariable::TYPE_RIC_WORD:{
						add_control_to_list( (nxtVarEditAbstract*) new ric_value( this, name, NULL, tooltip ), parameter_index );
					} break;
				case nxtVariable::TYPE_RIC_COPYOPTIONS:{
						add_control_to_list( (nxtVarEditAbstract*) new copyoptions_value( NULL, this ), parameter_index );
					} break;
				case nxtVariable::TYPE_RIC_POINT:{
						add_control_to_list( (nxtVarEditAbstract*) new nxtRicWordPointValue( name, tooltip, this ), parameter_index );
					} break;
				case nxtVariable::TYPE_RIC_RECT:{
						add_control_to_list( (nxtVarEditAbstract*) new nxtRicWordRectValue( name, tooltip, this ), parameter_index );
					} break;
				case nxtVariable::TYPE_RIC_ID:{
						add_control_to_list( (nxtVarEditAbstract*) new nxtRicIdValue( NULL, this ), parameter_index );
					} break;
				case nxtVariable::TYPE_POINT_ARRAY:{
						add_control_to_list( (nxtVarEditAbstract*) new pointArrayValue( NULL, this ), parameter_index );
					} break;
				case nxtVariable::TYPE_BITMAP:{
						//Add the control without adding it to the layout
						spriteValue* control = new spriteValue( this );
						control_list.push_back( (nxtVarEditAbstract*) &control->edit );
						control_index_list.push_back( parameter_index );
						
						//Add the container to the layout
						((QVBoxLayout*)layout())->insertWidget( layout()->count()-1, control );
						connect( control, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
					} break;
			}
		}
	}
}


void ricObjectAbstract::controls_change_object( ricObject* new_object ){
	if( new_object ){
		for( unsigned int i=0; i<control_list.size(); i++ ){
			control_list[i]->change_object( new_object->get_setting( control_index_list[i] ) );
		}
	}
}


bool ricObjectAbstract::change_object( ricObject* new_object ){
	if( new_object ){
		controls_change_object( new_object );
		setEnabled( true );
		return true;
	}
	else{
		setEnabled( false );
		return false;
	}
}

