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

#ifndef IDVALUE_H
#define IDVALUE_H

#include "../../riclib/nxtVariable.h"

#include <QWidget>
#include <QCheckBox>

class nxtVarWordValue: public QWidget{
	Q_OBJECT
	
	private:
		nxtVarWord *nxt_word;
		QCheckBox *ricfont;
	
	public:
		explicit nxtVarWordValue( nxtVarWord* variable, QWidget* parent = NULL );
		void change_value_object( nxtVarWord *new_word );
	
	
	private slots:
		void update_variable();
	
	signals:
		void value_changed();
		
};

#endif
