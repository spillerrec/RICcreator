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

#include <QWidget>
class QScrollBar;
class nxtCanvasWidget;

class nxtCanvasWidgetContainer: public QWidget{
	Q_OBJECT
	
	private:
		nxtCanvasWidget* canvas_view;
		QScrollBar* move_x;
		QScrollBar* move_y;
		
	protected:
		void keyPressEvent( QKeyEvent * event );
		void keyReleaseEvent( QKeyEvent * event );
		void mousePressEvent( QMouseEvent *event );
		void mouseMoveEvent( QMouseEvent *event );
		void mouseReleaseEvent( QMouseEvent *event );
	
	public:
		explicit nxtCanvasWidgetContainer( nxtCanvasWidget* view, bool moveable = false, QWidget* parent = NULL );
	
	private slots:
		void set_scrollbars();
		void reset_scrollbars();
		void update_scrollbars();
};

#endif
