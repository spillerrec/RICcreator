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

#include "ricobject_abstract.h"

ricobject_abstract::ricobject_abstract( QWidget *parent ): QWidget( parent ), layout( QBoxLayout::TopToBottom, parent ){
	setLayout( &layout );
	layout.addStretch();
}


void ricobject_abstract::add_control( QWidget* control ){
	layout.insertWidget( layout.count()-1, control );
	
	connect( control, SIGNAL( value_changed() ),  this, SIGNAL( changed() ) );
	
}

