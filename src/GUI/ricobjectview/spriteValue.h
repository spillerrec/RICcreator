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

#ifndef SPRITEVALUE_H
#define SPRITEVALUE_H

#include "nxtCanvasEdit.h"

#include <QWidget>

class spriteValue: public QWidget{
	Q_OBJECT
	
	private:
		class Ui_spriteedit_form *ui;
	
	public:
		nxtCanvasEdit edit;
	
	public:
		explicit spriteValue( QWidget* parent );
		~spriteValue();
	
	public slots:
	
	private slots:
		void update_zoom();
		void update_tool();
	
	signals:
		void value_changed();
		
};

#endif
