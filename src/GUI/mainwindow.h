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
#include <QList>
#include "about.h"
#include "programSettings.h"



namespace Ui{
	class MainWindow;
}

class ricfileEditor;
class QTabBar;
#include "../riclib/nxtIO.h"
#include "openRicfile.h"

class MainWindow : public QMainWindow{
	Q_OBJECT

	public:
		explicit MainWindow( QStringList filenames, QWidget *parent = 0);
		~MainWindow();

	private:
		Ui::MainWindow *ui;
		programSettings perferences;
		openRicfile *const get_current_ricfile() const;
		about_widget about_window;
		
		QList<openRicfile*> files;
		ricfileEditor *current_editor;
		
		openRicfile::file_editor select_editor( ricfile &file );
		void change_editor( ricfileEditor *new_editor );
		void change_file( openRicfile *file );
		void change_to_editor( openRicfile::file_editor editor_type );
		
		
	//Functions related to the tabbar
	private:
		QTabBar *tab_bar;
	private slots:
		void tab_changed( int index );
		void tab_moved( int from, int to );
		bool close_tab();
		bool close_tab( int tab );
		void update_tab();
		
		
	//Reimplemented functions
	protected:
		virtual void dragEnterEvent( QDragEnterEvent *event );
		virtual void dropEvent( QDropEvent *event );
		
		virtual void closeEvent( QCloseEvent *event );
	
	//File handling
	private:
		void show_loader_error( QString filepath, QString operation, nxtIO::LoaderError code );
	private slots:
		void new_file();
		void open_file();
		void open_file( QString filename );
		bool save_file();
		bool save_file_as();
		void export_file();
		void export_header();
		
	//Change editor
	private slots:
		void change_to_simple_editor();
		void change_to_advanced_editor();
		void change_to_font_editor();
	//Widget stuff
	private slots:
		void show_help();
		void show_about();
		void enter_fullscreen( bool action );
		
		
};

#endif // MAINWINDOW_H
