#include "programSettings.h"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include <fstream>
#include <string>
using namespace rapidxml;


bool programSettings::load(){
	xml_document<> doc;

	std::ifstream file( "settings.xml", std::ios_base::in );
	
	//Find ending
	file.seekg( 0, std::ios_base::end );
	unsigned long file_end = file.tellg();	//Return is steampos, improve?
	
	//Read file
	char *file_contents = new char [file_end];
	file.seekg( 0, std::ios_base::beg );
	file.read( file_contents, file_end );
	
	file.close();
	
	doc.parse<0>( file_contents );
	std::string temp = doc.first_node( "RICCreator" )->first_node( "settings" )->first_node( "last_filepath" )->first_node()->value();
	last_filepath = QString(temp.c_str());
	
	return true;
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
	
	//Print to a file
	std::string s;
	rapidxml::print(std::back_inserter(s), doc, print_no_indenting);
	std::ofstream file( "settings.xml", std::ios_base::out|std::ios_base::trunc );
	file.write( s.c_str(), s.size() );
	file.close();
	
	return true;	//Everything went fine as we didn't check for it anyway...
}


void programSettings::new_file( QString full_filename, bool update_last_path ){
	file_history += full_filename;
	if( update_last_path )
		last_filepath = full_filename;
}

