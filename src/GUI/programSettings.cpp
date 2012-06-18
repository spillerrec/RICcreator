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

#include "programSettings.h"
#include <QCoreApplication>
#include <QFileInfo>
#include <string>

#include "pugixml/pugixml.hpp"
using namespace pugi;


QString programSettings::filepath() const{
	QFileInfo app_path = QCoreApplication::applicationFilePath();
	return app_path.absolutePath() + "/settings.xml";
}

bool programSettings::load(){
	xml_document doc;
	if( doc.load_file( filepath().toLocal8Bit().data() ) ){
		//Get settings node
		xml_node settings = doc.child( "RICCreator" ).child( "settings" );
		if( settings ){
			//Read basic settings
			last_filepath = QString( settings.child_value( "last_filepath" ) );
			history_lenght = settings.child( "history_lenght" ).text().as_int();
			
			//Read file history
			file_history.clear();
			for(	xml_node path = settings.child( "file_history" ).child( "filepath" );
					path;
					path = path.next_sibling( "filepath" )
				)
				file_history += QString( (char*)path.child_value() );
			
			return true;
		}
	}
	
	//File missing or incorrect file
	return false;
}


bool programSettings::save() const{
	xml_document doc;
	xml_node settings = doc.append_child( "RICCreator" ).append_child( "settings" );
	
	settings.append_child( "last_filepath" ).append_child( pugi::node_pcdata ).set_value( last_filepath.absolutePath().toUtf8().data() );
	settings.append_child( "history_lenght" ).append_child( pugi::node_pcdata ).set_value( QString::number( history_lenght ).toUtf8().data() );
	
	//Add file history
	xml_node history = settings.append_child( "file_history" );
	foreach( QString filepath, file_history )
		history.append_child( "filepath" ).append_child( pugi::node_pcdata ).set_value( filepath.toUtf8().data() );
	
	return doc.save_file( filepath().toLocal8Bit().data() );
}


void programSettings::new_file( QString full_filename, bool update_last_path ){
	//Remove duplicates
	int pos;
	while( ( pos = file_history.indexOf( full_filename ) ) != -1 )
		file_history.removeAt( pos );
	
	//Add the path
	file_history += full_filename;
	if( update_last_path )
		last_filepath = full_filename;
	
	//Prevent it from growing too large
	while( file_history.count() > history_lenght )
		file_history.removeFirst();
}

