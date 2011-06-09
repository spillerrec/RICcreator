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

#include "nxtRicWordExtraValue.h"
#include "../../riclib/nxtVarRicWord.h"
#include <QGroupBox>
#include <QVBoxLayout>

nxtRicWordPointValue::nxtRicWordPointValue( QString name, QString tooltip, QWidget *parent ):
		nxtVarEditAbstract( parent ),
		control_x( this, "X", NULL, "" ),
		control_y( this, "Y", NULL, "" )
{
	//Create the groupbox
	QGroupBox *gbox = new QGroupBox( name, this );
	gbox->setToolTip( tooltip );
	
	//Set up layouts
	QVBoxLayout *mainlayout = new QVBoxLayout;
	QVBoxLayout *boxlayout = new QVBoxLayout;
	setLayout( mainlayout );
	gbox->setLayout( boxlayout );
	mainlayout->addWidget( gbox );
	
	//Add ric_value controls
	boxlayout->addWidget( (QWidget*)&control_x );
	boxlayout->addWidget( (QWidget*)&control_y );
	connect( &control_x, SIGNAL( value_changed() ), this, SIGNAL( value_changed() ) );
	connect( &control_y, SIGNAL( value_changed() ), this, SIGNAL( value_changed() ) );
}

bool nxtRicWordPointValue::change_object( nxtVariable* object ){
	//If NULL is passed, disable the control
	if( !object ){
		setEnabled( false );
		variable = NULL;
		
		return true;
	}
	else if( object->var_type() == nxtVariable::TYPE_RIC_POINT ){
		//Change to the new variable
		variable = (ricvarPoint*)object;
		control_x.change_object( (nxtVariable*) &variable->X );
		control_y.change_object( (nxtVariable*) &variable->Y );
		setEnabled( true );
		
		return true;
	}
	
	return false;
}




nxtRicWordRectValue::nxtRicWordRectValue( QString name, QString tooltip, QWidget *parent ):
		nxtVarEditAbstract( parent ),
		control_x( this, tr("X"), NULL, "" ),
		control_y( this, tr("Y"), NULL, "" ),
		control_width( this, tr("Width"), NULL, "" ),
		control_height( this, tr("Height"), NULL, "" )
{
	//Create the groupbox
	QGroupBox *gbox = new QGroupBox( name, this );
	gbox->setToolTip( tooltip );
	
	//Set up layouts
	QVBoxLayout *mainlayout = new QVBoxLayout;
	QVBoxLayout *boxlayout = new QVBoxLayout;
	setLayout( mainlayout );
	gbox->setLayout( boxlayout );
	mainlayout->addWidget( gbox );
	
	//Add ric_value controls
	boxlayout->addWidget( (QWidget*)&control_x );
	boxlayout->addWidget( (QWidget*)&control_y );
	boxlayout->addWidget( (QWidget*)&control_width );
	boxlayout->addWidget( (QWidget*)&control_height );
	connect( &control_x, SIGNAL( value_changed() ), this, SIGNAL( value_changed() ) );
	connect( &control_y, SIGNAL( value_changed() ), this, SIGNAL( value_changed() ) );
	connect( &control_width, SIGNAL( value_changed() ), this, SIGNAL( value_changed() ) );
	connect( &control_height, SIGNAL( value_changed() ), this, SIGNAL( value_changed() ) );
}

bool nxtRicWordRectValue::change_object( nxtVariable* object ){
	//If NULL is passed, disable the control
	if( !object ){
		setEnabled( false );
		variable = NULL;
		
		return true;
	}
	else if( object->var_type() == nxtVariable::TYPE_RIC_RECT ){
		//Change to the new variable
		variable = (ricvarRect*)object;
		control_x.change_object( (nxtVariable*) &variable->pos.X );
		control_y.change_object( (nxtVariable*) &variable->pos.Y );
		control_width.change_object( (nxtVariable*) &variable->width );
		control_height.change_object( (nxtVariable*) &variable->height );
		setEnabled( true );
		
		return true;
	}
	
	return false;
}

