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

#ifndef RICFILEEDITORSIMPLE_H
#define RICFILEEDITORSIMPLE_H

#include "ricfileEditor.h"


class ricfile;
class nxtCanvasWidget;
class nxtCanvasWidgetContainer;

class ricfileEditorSimple: public ricfileEditor{
	Q_OBJECT
	
	private:
		nxtCanvasWidget *edit;
		nxtCanvasWidgetContainer *container;
		
		
	//ricfileEditor functions
	public:
		explicit ricfileEditorSimple( QWidget *parent = 0 );
		
		void change_file( openRicfile *new_file );
		static bool file_supported( const ricfile &test_file );
	
	
};

#endif