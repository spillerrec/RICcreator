#include <QtGui/QApplication>
#include "GUI/mainwindow.h"

int main(int argc, char *argv[]){
	QString files;
	if( argc == 2 )
		files = argv[1];
	
	QApplication a(argc, argv);
	MainWindow w(files);
	w.show();

	return a.exec();
}
