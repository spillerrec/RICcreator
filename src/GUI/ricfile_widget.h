#ifndef RICFILE_WIDGET_H
#define RICFILE_WIDGET_H

#include <QWidget>
#include <QImage>
#include <QGraphicsScene>


#include "src/riclib/ricfile.h"
#include "src/GUI/ricObjectModel.h"


class ricfile_widget: public QWidget{
	Q_OBJECT
	
	private:
		class Ui_Form *ui;
		QImage image;
		QGraphicsScene scene;
		ricfile graphics;
		ricModel model;
		
		bool edited;
		bool original;
	
	public:
		explicit ricfile_widget( QString filename = "", QWidget *parent = 0 );
		~ricfile_widget();
	
	
	public:
		void open_file( QString filename );
		void reset();
		void save_file( QString filename );
		void update_preview();
		bool replaceable();
};

#endif