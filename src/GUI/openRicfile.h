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

#ifndef OPENRICFILE_H
#define OPENRICFILE_H

/*
	The details about a ricfile which have been opened for editing is defined here.
	This includes its origin, its name and if it have been edited.
*/

#include <QString>
#include <assert.h>

#include "../riclib/ricfile.h"
class QWidget;
class ricfile;
class ricfileEditor;
class ricfileEditorAdvanced;

class openRicfile{
	public:
		enum file_source{
			original,	//It was created in this program
			pc_file,
			nxt_flash	//It originates from a NXT
		};
		
		enum file_editor{
			simple_mode,
			advanced_mode,
			font_mode
		};
		
		file_source source;
		QString file_name;
		bool edited;
		
	private:
		ricfile &file;
		file_editor editor;
		
		static QWidget* parent;
		static ricfileEditorAdvanced* editor_advanced;
		
	public:
		//file must be valid and will be destructed together with openRicfile
		openRicfile( ricfile &file, QString file_name, file_source source, file_editor editor ): file( file ){
			this->file_name = file_name;
			this->source = source;
			this->editor = editor;
			edited = false;
		}
		~openRicfile(){
			delete &file;
		}
		
		static void set_parent( QWidget *editor_parent ){
			parent = editor_parent;
		}
		
		bool replaceable(){ return source == original && edited == false; }
		bool file_edited(){ return edited; }
		bool is_original(){ return source == original; }
		
		ricfile& ric(){ return file; }
		
		//TODO: some methods for the viewers
		ricfileEditor* get_viewer();
};

#endif

