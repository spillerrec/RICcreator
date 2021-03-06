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

#ifndef RICOBJECTCONTAINER
#define RICOBJECTCONTAINER

#include <QStackedLayout>

class ricObject;
class ricObjectAbstract;
class ricObjectDescription;
class ricObjectSprite;
class ricObjectVarMap;
class ricObjectCopybits;
class ricObjectPixel;
class ricObjectLine;
class ricObjectRectangle;
class ricObjectCircle;
class ricObjectNumber;
class ricObjectEllipse;
class ricObjectPolygon;

class ricObjectContainer: public QStackedLayout{
	Q_OBJECT
	
	private:
		ricObjectDescription* ric_description;
		ricObjectSprite* ric_sprite;
		ricObjectVarMap* ric_varmap;
		ricObjectCopybits* ric_copybits;
		ricObjectPixel* ric_point;
		ricObjectLine* ric_line;
		ricObjectRectangle* ric_rect;
		ricObjectCircle* ric_circle;
		ricObjectNumber* ric_number;
		ricObjectEllipse* ric_ellipse;
		ricObjectPolygon* ric_polygon;
		
		bool add_control( ricObjectAbstract* control );
	
	public:
		ricObjectContainer( QWidget *parent = 0 );
		
		void view_object( ricObject* object );
	
	signals:
		void object_changed();
};

#endif
