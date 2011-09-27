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

#include <QtGui/QApplication>
#include <QStringList>
#include "GUI/mainwindow.h"

//For conversion
#include "riclib/ricfile.h"
#include "riclib/nxtCanvas.h"
#include "GUI/importImageDialog.h"

#include <string.h>

int main(int argc, char *argv[]){
	QApplication a(argc, argv);
	
	//Parse parameters
	QStringList files;
	if( argc > 1 ){
		if( argv[1][0] == '-' ){
			if( strcmp( argv[1], "-convert") == 0 && argc == 5 ){
				//Convert from one format to another
				//Parse format is "-convert from_format to_format file"
				ricfile org_file;
				//TODO: lookup strcmp return codes!!
				
				//Read file
				if( strcmp( argv[2], "RIC" ) == 0 ){
					if( org_file.readfile( argv[4] ) != nxtIO::LDR_SUCCESS )
						return 3;	//Errors while reading
				}
				//else if( strcmp( argv[2], "some format" ) == 0 )
				else
					return 2;	//No matching format
				
				//Read to format
				if( strcmp( argv[3], "RIC" ) == 0 )
					org_file.writefile( strcat( argv[4], ".ric" ) );
				else if( strcmp( argv[3], "PNG" ) == 0 ){
					nxtCanvas image;
					image.set_auto_resize( true );
					org_file.Draw( &image );
					importImageDialog::export_canvas( &image, strcat( argv[4], ".png" ) );
				}
				else
					return 2;	//No matching format
				
				return 0;	//Everything went fine, nothing more to do
			}
			else
				return 1;
		}
		else
			//Treat all remaining entries as filenames to open
			for( int i=1; i<argc; i++ )
				files << QString::fromLocal8Bit( argv[i] );
	
	}
	
	//Open files
	MainWindow w(files);
	w.show();

	return a.exec();
}
