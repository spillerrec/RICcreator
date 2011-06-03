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

#ifndef NXTCANVASEDIT_H
#define NXTCANVASEDIT_H

#include "../nxtCanvasWidget.h"

class nxtCopyOptions;

class nxtCanvasEdit: public nxtCanvasWidget{
	Q_OBJECT
	
	private:
		int start_x;
		int start_y;
		bool pressed;
		unsigned int current_tool;
		nxtCopyOptions* options;
		bool options_inverted;
		
		void draw( int pos1_x, int pos1_y, int pos2_x, int pos2_y );
	
	protected:
		void mousePressEvent( QMouseEvent *event );
		void mouseMoveEvent( QMouseEvent *event );
		void mouseReleaseEvent( QMouseEvent *event );
	
	public:
		explicit nxtCanvasEdit( QWidget* parent );
		void set_tool( unsigned int tool ){ current_tool = tool; }
		void set_options( nxtCopyOptions* new_options ){ options = new_options; }
	
	signals:
		void value_changed();
		
};

#endif
