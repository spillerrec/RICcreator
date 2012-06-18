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

#ifndef PROGRAMSETTINGS_H
#define PROGRAMSETTINGS_H

#include <QStringList>
#include <QDir>


class programSettings{
	private:
		bool changed;
		
		QDir last_filepath;	//Only reacts at open/save commands
		QStringList file_history;
		int history_lenght;
		
	private:
		QString filepath() const;
	
	public:
		programSettings(){
			last_filepath = QDir::homePath();
			history_lenght = 5;
		}
		void new_file( QString full_filename, bool update_last_path = true );
		QString get_last_path() const{ return last_filepath.absolutePath(); }
		
		bool load();
		bool save() const;
		
};

#endif