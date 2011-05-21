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

#ifndef RICOBJECT_ABSTRACT
#define RICOBJECT_ABSTRACT

#include <QBoxLayout>

#include "../../riclib/ricObject.h"
#include "../ric_value.h"
#include "copyoptions_value.h"

class ricobject_abstract: public QWidget{
	Q_OBJECT
	
	protected:
		void add_control( QWidget* control );
	
	public:
		ricobject_abstract( QWidget *parent = 0 );
		
		QBoxLayout layout;
		
		virtual bool change_object( ricfile::ricObject* new_object ) = 0;
		
	signals:
		void changed();
};

#endif
