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

//TODO: implement this properly...

#include "programSettings.h"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include <fstream>
#include <string>
using namespace rapidxml;


bool programSettings::load(){
	xml_document<> doc;
	
	std::ifstream file( "settings.xml", std::ios_base::in );
	if( file.is_open() ){
		
		//Find ending
		file.seekg( 0, std::ios_base::end );
		unsigned long file_end = file.tellg();	//Return is steampos, improve?
		
		//Read file
		char *file_contents = new char [file_end+1];
		file.seekg( 0, std::ios_base::beg );
		file.read( file_contents, file_end );
		file_contents[file_end] = 0;
		
		file.close();
		if( file_contents ){
			doc.parse<0>( file_contents );
			xml_node<>* settings = doc.first_node( "RICCreator" )->first_node( "settings" );
			last_filepath = QString( settings->first_node( "last_filepath" )->first_node()->value() );
			history_lenght = QString( settings->first_node( "history_lenght" )->first_node()->value() ).toInt();
			
			file_history.clear();
			for(	xml_node<>* path = settings->first_node( "file_history" )->first_node();
					path;
					path = path->next_sibling()
				)
				file_history += QString( path->value() );
			
			return true;
			
		}
		else
			return false;
	}
	else
		return false;
}


bool programSettings::save() const{
	xml_document<> doc;
	
	//Add xml declaration
	xml_node<>* declaration = doc.allocate_node( node_declaration );
	declaration->append_attribute( doc.allocate_attribute( "version", "1.0" ) );
	declaration->append_attribute( doc.allocate_attribute( "encoding", "utf-8" ) );
	doc.append_node( declaration );
	
	//Add root and settings node
	xml_node<> *root = doc.allocate_node( node_element, "RICCreator" );
	doc.append_node( root );
	xml_node<> *settings = doc.allocate_node( node_element, "settings" );
	root->append_node( settings );
	
	//Add settings
	char *text_last_filepath = doc.allocate_string( last_filepath.absolutePath().toUtf8().data() );
	xml_node<> *last_path = doc.allocate_node( node_element, "last_filepath", text_last_filepath );
	settings->append_node( last_path );
	QString temp_num;
	temp_num.setNum( history_lenght );
	char *text_history_amount = doc.allocate_string( temp_num.toUtf8().data() );
	xml_node<> *history_amount = doc.allocate_node( node_element, "history_lenght", text_history_amount );
	settings->append_node( history_amount );
	
	//Add file_history
	xml_node<> *history = doc.allocate_node( node_element, "file_history" );
	settings->append_node( history );
	foreach( QString filepath, file_history ){
		char *text_filepath = doc.allocate_string( filepath.toUtf8().data() );
		xml_node<> *file = doc.allocate_node( node_element, "filepath", text_filepath );
		history->append_node( file );
	}
	
	//Print to a file
	std::string s;
	rapidxml::print(std::back_inserter(s), doc, print_no_indenting);
	std::ofstream file( "settings.xml", std::ios_base::out|std::ios_base::trunc );
	file.write( s.c_str(), s.size() );
	file.close();
	
	return true;	//Everything went fine as we didn't check for it anyway...
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

