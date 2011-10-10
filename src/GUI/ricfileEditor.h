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

#ifndef RICFILEEDITOR_H
#define RICFILEEDITOR_H

/*
	The abstract base class for all ricfile editors.
	The class must be able to change the current ricfile with change_file( ricfile* ).
	The class can supply a custom toolbar by reimplementing editor_toolbar().
*/

#include <QWidget>

class QToolBar;
class openRicfile;

class ricfileEditor: public QWidget{
	Q_OBJECT
	
	protected:
		openRicfile *file;
	protected slots:
		void file_changed();
		
	public:
		explicit ricfileEditor( QWidget *parent ) : QWidget( parent ){
			file = NULL;
		};
		virtual void change_file( openRicfile *new_file ) = 0;
		virtual QToolBar* editor_toolbar();
	
	signals:
		void file_edited();
};

#endif

