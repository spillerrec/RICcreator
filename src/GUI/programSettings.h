#ifndef PROGRAMSETTINGS_H
#define PROGRAMSETTINGS_H

#include <QStringList>
#include <QDir>


class programSettings{
	private:
		bool changed;
		
		QDir last_filepath;	//Only reacts at open/save commands
		QStringList file_history;
		unsigned int history_lenght;
	
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