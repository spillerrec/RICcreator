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

#ifndef RICOBJECT_DESCRIPTION
#define RICOBJECT_DESCRIPTION

#include "../../riclib/ricObject.h"
#include "ricobject_abstract.h"
#include "optionsValue.h"

class ricobject_description: public ricobject_abstract{
	Q_OBJECT
	
	private:
		//The objects
		optionsValue options;
		ric_value width;
		ric_value height;
	
	public:
		ricobject_description( QWidget *parent = 0 );
		
		bool change_object( ricfile::ricObject* new_object );
};

#endif
