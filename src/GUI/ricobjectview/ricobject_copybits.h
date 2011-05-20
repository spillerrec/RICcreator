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

#ifndef RICOBJECT_COPYBITS
#define RICOBJECT_COPYBITS

#include "../../riclib/ricObject.h"
#include "ricobject_abstract.h"

class ricobject_copybits: public ricobject_abstract{
	Q_OBJECT
	
	private:
		//The objects
		copyoptions_value options;
		ric_value id;
		ric_value src_x;
		ric_value src_y;
		ric_value src_width;
		ric_value src_height;
		ric_value dest_x;
		ric_value dest_y;
	
	public:
		ricobject_copybits( QWidget *parent = 0 );
		
		bool change_object( ricfile::ricObject* new_object );
};

#endif