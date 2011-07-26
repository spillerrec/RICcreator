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

#ifndef NXTRICIDVALUE_H
#define NXTRICIDVALUE_H

#include "nxtVarEditAbstract.h"

class nxtVariable;
class ricVarId;
class ricObject;
class QSpinBox;

#include <QString>


class nxtRicIdValue: public nxtVarEditAbstract{
	Q_OBJECT
	
	private:
		//TODO: type
		ricVarId *nxt_word;
		QSpinBox *ric_id;
	
	public:
		explicit nxtRicIdValue( nxtVariable* variable, QWidget* parent = NULL );
		bool change_object( nxtVariable* object );
	
	public:
		const static QString style_ok;
		const static QString style_warning;
		const static QString style_error;
		const static QString style_none;
		static void apply_validation( QWidget *widget, unsigned int id, ricObject *object, bool native );
	
	private slots:
		void revalidate();
		void update_variable();
};

#endif
