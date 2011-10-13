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

#include "openRicfile.h"
#include "ricfileEditorAdvanced.h"

#include <QObject>

QWidget* openRicfile::parent = NULL;
ricfileEditorAdvanced* openRicfile::editor_advanced = NULL;


ricfileEditor* openRicfile::get_viewer(){
	switch( editor ){
		case simple_mode: return NULL;
		case advanced_mode:
				if( !editor_advanced ){
					editor_advanced = new ricfileEditorAdvanced( openRicfile::parent );
					QObject::connect( editor_advanced, SIGNAL(file_edited()), parent, SLOT( update_tab() ) );
				}
				return editor_advanced;
		case font_mode: return NULL;
	}
	
	return NULL;	//This will only happen if it was passed an faulty viewer!
}

