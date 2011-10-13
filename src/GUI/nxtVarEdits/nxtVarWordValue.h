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

#ifndef NXTVARWORDVALUE_H
#define NXTVARWORDVALUE_H

#include "nxtVarEditAbstract.h"

class nxtVarWord;
class QSpinBox;

class nxtVarWordValue: public nxtVarEditAbstract{
	Q_OBJECT
	
	private:
		nxtVarWord *nxt_word;
		QSpinBox *value;
	
	public:
		explicit nxtVarWordValue( nxtVarWord* variable, QString text = "", QWidget* parent = NULL, QString tooltip = "" );
		bool change_object( nxtVariable* object );
	
	
	private slots:
		void update_variable();
		
};

#endif
