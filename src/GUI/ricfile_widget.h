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

#include <QWidget>
#include <QString>


#include "../riclib/ricfile.h"
#include "../riclib/nxtCanvas.h"
#include "ricObjectModel.h"
#include "ricParametersModel.h"
#include "nxtCanvasWidget.h"

class QItemSelectionModel;
class ricobject_container;

class ricfile_widget: public QWidget{
	Q_OBJECT
	
	private:
		class Ui_Form *ui;
		
		nxtCanvasWidget canvas;
		nxtCanvas drawing_canvas;
		
		ricfile graphics;
		ricModel model;
		ricParametersModel parameters;
		QItemSelectionModel *ricfile_selection_model;
		ricobject_container* ricobjectview;
		
		QString current_file;
		bool edited;
	
	public:
		explicit ricfile_widget( QString filename = "", QWidget *parent = 0 );
		~ricfile_widget();
		
		QString get_filename() const{ return current_file; }
	
	
	public:
		void open_file( QString filename );
		void reset();
		bool save_file();
		bool save_file( QString filename );
		bool replaceable() const;
		bool file_edited() const;
		bool is_original() const;
		
		bool add_object( unsigned int object_type );
	
	private slots:
		void file_changed();
		void move_object_up();
		void move_object_down();
		void remove_object();
	
	public slots:
		void update_selection();
		void update_model();
		void update_preview();
		void export_file(){ canvas.save(); }
};

#endif