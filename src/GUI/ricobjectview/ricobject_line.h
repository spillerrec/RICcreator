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

#ifndef RICOBJECT_LINE
#define RICOBJECT_LINE

#include "../../riclib/ricObject.h"
#include "ricobject_abstract.h"

class ricobject_line: public ricobject_abstract{
	Q_OBJECT
	
	private:
		//The objects
		copyoptions_value options;
		ric_value posx1;
		ric_value posy1;
		ric_value posx2;
		ric_value posy2;
	
	public:
		ricobject_line( QWidget *parent = 0 );
		
		bool change_object( ricfile::ricObject* new_object );
};

#endif
