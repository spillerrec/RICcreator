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

#ifndef NXTRICWORDEXTRAVALUE_H
#define NXTRICWORDEXTRAVALUE_H

#include "nxtVarEditAbstract.h"
#include "../ric_value.h"
#include <QString>

class ricvarPoint;
class ricvarRect;

class nxtRicWordPointValue: public nxtVarEditAbstract{
	Q_OBJECT
	
	private:
		ricvarPoint* variable;
		ric_value control_x;
		ric_value control_y;
	
	public:
		nxtRicWordPointValue( QString name, QString tooltip, QWidget *parent = 0 );
		
		bool change_object( nxtVariable* object );
};


class nxtRicWordRectValue: public nxtVarEditAbstract{
	Q_OBJECT
	
	private:
		ricvarRect* variable;
		ric_value control_x;
		ric_value control_y;
		ric_value control_width;
		ric_value control_height;
	
	public:
		nxtRicWordRectValue( QString name, QString tooltip, QWidget *parent = 0 );
		
		bool change_object( nxtVariable* object );
};

#endif
