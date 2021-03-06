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

#ifndef RIC_VALUE_H
#define RIC_VALUE_H

#include "nxtVarEdits/nxtVarEditAbstract.h"

class nxtVarRicWord;

class ric_value: public nxtVarEditAbstract{
	Q_OBJECT
	
	private:
		class Ui_ric_value_select *ui;
		nxtVarRicWord* ricword;
		
	
	private:
		void read();
		void refresh_mode();
	
	public:
		explicit ric_value( QWidget* parent, QString text, nxtVarRicWord* value_object, QString tooltip = "" );
		bool change_object( nxtVariable* object );
	
	
	private slots:
		void write();
		void revalidate();
		void update_mode();
};

#endif