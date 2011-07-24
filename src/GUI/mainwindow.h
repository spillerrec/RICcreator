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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "about.h"
#include "programSettings.h"


namespace Ui{
	class MainWindow;
}

class ricfile_widget;

class MainWindow : public QMainWindow{
	Q_OBJECT

	public:
		explicit MainWindow( QString filenames = "", QWidget *parent = 0);
		~MainWindow();

	private:
		Ui::MainWindow *ui;
		programSettings perferences;
		ricfile_widget* get_current_ricfile() const;
		about_widget about_window;
		
	//Reimplemented functions
	protected:
		virtual void dragEnterEvent( QDragEnterEvent *event );
		virtual void dropEvent( QDropEvent *event );
		
		virtual void closeEvent( QCloseEvent *event );

	private slots:
		//File/tab handling
		void new_file();
		void open_file();
		void open_file( QString filename );
		bool save_file();
		bool save_file_as();
		void show_about();
		bool close_tab();
		bool close_tab( int tab );
		
		
		//Add new objects to file
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
		void add_polyline();
		
	private:
		void add_object( unsigned int object_type );
		
};

#endif // MAINWINDOW_H
