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

#ifndef POINTARRAYVALUE_H
#define POINTARRAYVALUE_H

#include "nxtVarEditAbstract.h"

class nxtVariable;
class pointArray;
class QVBoxLayout;
class QScrollArea;

class pointArrayValue: public nxtVarEditAbstract{
	Q_OBJECT
	
	private:
		pointArray *points;
		QWidget *list_widget;
		QVBoxLayout *list_area;
		QScrollArea *scrollbars;
		
		void empty_list();
		void build_list();
	
	public:
		explicit pointArrayValue( nxtVariable* variable, QWidget* parent = NULL );
		bool change_object( nxtVariable* object );
	
	public slots:
		void add();
		void add( unsigned int after_index );
		void remove( unsigned int index );
		void move( unsigned int index, int offset );
		void optimize();
		
		//Warning, these will not update the widgets values!
		void change_x( unsigned int index, unsigned int value );
		void change_y( unsigned int index, unsigned int value );
};

#endif
