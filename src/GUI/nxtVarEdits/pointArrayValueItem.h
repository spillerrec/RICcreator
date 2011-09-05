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

#ifndef POINTARRAYVALUEITEM_H
#define POINTARRAYVALUEITEM_H

#include <QWidget>

class nxtVariable;
class pointArray;
class pointArrayValue;

class QSpinBox;

class pointArrayValueItem: public QWidget{
	Q_OBJECT
	
	private:
		QSpinBox *sbx_x;
		QSpinBox *sbx_y;
		
		pointArrayValue *container;
	
	private:
		pointArray *points;
		unsigned int index;
	
	public:
		explicit pointArrayValueItem( pointArray* point_array, unsigned int idx, pointArrayValue* parent );
		
		unsigned int get_index() const{ return index; }
	
	private slots:
		void action_add();
		void action_remove();
		void action_move_up();
		void action_move_down();
		void action_x_changed();
		void action_y_changed();
};

#endif
