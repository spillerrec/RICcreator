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

#ifndef RICFILE_WIDGET_H
#define RICFILE_WIDGET_H

#include <QString>

#include "ricfileEditor.h"

#include "../riclib/ricfile.h"
#include "../riclib/nxtCanvas.h"
#include "ricObjectModel.h"
#include "ricParametersModel.h"
#include "nxtCanvasWidget.h"

class QItemSelectionModel;
class ricobject_container;
class QToolBar;

class ricfile_widget: public ricfileEditor{
	Q_OBJECT
	
	private:
		class Ui_Form *ui;
		QToolBar *toolbar;
		
		nxtCanvasWidget canvas;
		nxtCanvas drawing_canvas;
		
		ricModel model;
		ricParametersModel parameters;
		QItemSelectionModel *ricfile_selection_model;
		ricobject_container* ricobjectview;
		
		
	
	public:
		explicit ricfile_widget( QWidget *parent = 0 );
		~ricfile_widget();
		
		void change_file( openRicfile *new_file );
		QToolBar* editor_toolbar();
	
	
	//Add new objects to file
	private slots:
		void add_options();
		void add_sprite();
		void add_copybits();
		void add_varmap();
		void add_pixel();
		void add_line();
		void add_rectangle();
		void add_circle();
		void add_number();
		void add_ellipse();
		void add_polygon();
		bool add_object( unsigned int object_type );
	
	private slots:
		// void file_changed();
		void move_object_up();
		void move_object_down();
		void remove_object();
	
	public slots:
		void update_selection();
		void update_model();
		void update_preview();
};

#endif