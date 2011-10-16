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

/*
	This class manages nxtCanvasWidget in a layout, providing a border and scrollbars
*/

#ifndef NXTCANVASWIDGETCONTAINER_H
#define NXTCANVASWIDGETCONTAINER_H

#include "nxtVarEdits/nxtVarEditAbstract.h"

class QScrollBar;
class nxtCanvasWidget;
class nxtCopyOptions;
class copyoptions_value;

class nxtCanvasWidgetContainer: public nxtVarEditAbstract{
	Q_OBJECT
	
	private:
		nxtCanvasWidget& canvas_view;
		nxtCopyOptions* options;
		copyoptions_value* copyedit;
		
		QScrollBar* move_x;
		QScrollBar* move_y;
		
		enum tools{
			tool_freehand,
			tool_line,
			tool_rectangle,
			tool_ellipse,
			tool_selection,
			tool_fill,
			action_image,
			action_save,
			action_copy,
			action_paste,
			action_crop
		};
	
	public:
		explicit nxtCanvasWidgetContainer( nxtCanvasWidget& view, QWidget* parent = NULL, bool editable = false, bool moveable = false );
		~nxtCanvasWidgetContainer();
		
		bool change_object( nxtVariable* object );
	
	private slots:
		//scrollbar methods
		void scrollbar_set_ranges();
		void scrollbar_action( int action );
		
	//Edit buttons
	private slots:
		void set_tool( int tool );
};

#endif
