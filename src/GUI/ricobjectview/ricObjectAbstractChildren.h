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

#ifndef RICOBJECTABSTRACTCHILDREN_H
#define RICOBJECTABSTRACTCHILDREN_H

#include "ricObjectAbstract.h"

class ricObjectDescription: public ricObjectAbstract{
	Q_OBJECT
	
	public:
		ricObjectDescription( QWidget *parent = 0 ): ricObjectAbstract( ricObject::RIC_OP_OPTIONS, true, parent )
			{ ; }
};


class ricObjectSprite: public ricObjectAbstract{
	Q_OBJECT
	
	public:
		ricObjectSprite( QWidget *parent = 0 ): ricObjectAbstract( ricObject::RIC_OP_SPRITE, true, parent )
			{ ; }
};


class ricObjectVarMap: public ricObjectAbstract{
	Q_OBJECT
	
	public:
		ricObjectVarMap( QWidget *parent = 0 ): ricObjectAbstract( ricObject::RIC_OP_VARMAP, true, parent )
			{ ; }
};


class ricObjectCopybits: public ricObjectAbstract{
	Q_OBJECT
	
	public:
		ricObjectCopybits( QWidget *parent = 0 ): ricObjectAbstract( ricObject::RIC_OP_COPYBITS, true, parent )
			{ ; }
};


class ricObjectPixel: public ricObjectAbstract{
	Q_OBJECT
	
	public:
		ricObjectPixel( QWidget *parent = 0 ): ricObjectAbstract( ricObject::RIC_OP_PIXEL, true, parent )
			{ ; }
};


class ricObjectLine: public ricObjectAbstract{
	Q_OBJECT
	
	public:
		ricObjectLine( QWidget *parent = 0 ): ricObjectAbstract( ricObject::RIC_OP_LINE, true, parent )
			{ ; }
};


class ricObjectRectangle: public ricObjectAbstract{
	Q_OBJECT
	
	public:
		ricObjectRectangle( QWidget *parent = 0 ): ricObjectAbstract( ricObject::RIC_OP_RECTANGLE, true, parent )
			{ ; }
};


class ricObjectCircle: public ricObjectAbstract{
	Q_OBJECT
	
	public:
		ricObjectCircle( QWidget *parent = 0 ): ricObjectAbstract( ricObject::RIC_OP_CICLE, true, parent )
			{ ; }
};


class ricObjectNumber: public ricObjectAbstract{
	Q_OBJECT
	
	public:
		ricObjectNumber( QWidget *parent = 0 ): ricObjectAbstract( ricObject::RIC_OP_NUMBER, true, parent )
			{ ; }
};


class ricObjectEllipse: public ricObjectAbstract{
	Q_OBJECT
	
	public:
		ricObjectEllipse( QWidget *parent = 0 ): ricObjectAbstract( ricObject::RIC_OP_ELLIPSE, true, parent )
			{ ; }
};


class ricObjectPolygon: public ricObjectAbstract{
	Q_OBJECT
	
	public:
		ricObjectPolygon( QWidget *parent = 0 ): ricObjectAbstract( ricObject::RIC_OP_POLYGON, true, parent )
			{ ; }
};

#endif
