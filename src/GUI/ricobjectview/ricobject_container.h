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

#ifndef RICOBJECT_CONTAINER
#define RICOBJECT_CONTAINER

#include <QStackedLayout>
#include "../../riclib/ricObject.h"

class ricobject_abstract;
class ricobject_description;
class ricobject_copybits;
class ricobject_sprite;
class ricobject_point;
class ricobject_line;
class ricobject_rect;
class ricobject_circle;
class ricobject_number;
class ricobject_ellipse;

class ricobject_container: public QStackedLayout{
	Q_OBJECT
	
	private:
		ricobject_description* ric_description;
		ricobject_sprite* ric_sprite;
		ricobject_copybits* ric_copybits;
		ricobject_point* ric_point;
		ricobject_line* ric_line;
		ricobject_rect* ric_rect;
		ricobject_circle* ric_circle;
		ricobject_number* ric_number;
		ricobject_ellipse* ric_ellipse;
		
		bool add_control( ricobject_abstract* control );
	
	public:
		ricobject_container( QWidget *parent = 0 );
		
		void view_object( ricfile::ricObject* object );
	
	signals:
		void object_changed();
};

#endif
