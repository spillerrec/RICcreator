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

#ifndef RICOBJECTABSTRACT_H
#define RICOBJECTABSTRACT_H

class nxtVarEditAbstract;

#include "../riclib/ricObject.h"
#include <vector>
#include <QWidget>
using namespace std;

class ricObjectAbstract: public QWidget{
	Q_OBJECT
	
	private:
		ricObject::object_op object_opcode;
		vector<nxtVarEditAbstract*> control_list;
		vector<unsigned int> control_index_list;
	
	protected:
		void add_control_to_list( nxtVarEditAbstract* control, unsigned int index );
		void add_control( unsigned int parameter_index );
		void controls_change_object( ricObject* new_object );
	
	public:
		ricObjectAbstract( ricObject::object_op object_type, bool autofill, QWidget *parent = 0 );
		
		virtual bool change_object( ricObject* new_object );
		
	signals:
		void changed();
};

#endif
