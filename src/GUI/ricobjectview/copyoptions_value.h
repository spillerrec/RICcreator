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

#ifndef COPYOPTIONS_VALUE_H
#define COPYOPTIONS_VALUE_H

#include "../nxtVarEdits/nxtVarEditAbstract.h"

class nxtCopyOptionsBase;

class copyoptions_value: public nxtVarEditAbstract{
	Q_OBJECT
	
	private:
		class Ui_copyoptions *ui;
		nxtCopyOptionsBase* copyoptions;
		
	
	private:
		void read();
	
	public:
		static const int RIC_OBJECT = 1;
		static const int RIC_POLYLINE = 2;
		static const int RIC_FILL_SHAPE = 4;
	
	
	public:
		explicit copyoptions_value( nxtVariable* value_object, int settings, QWidget* parent=NULL );
		bool change_object( nxtVariable* object );
	
	
	private slots:
		void write();
};

#endif