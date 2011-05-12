#ifndef RICFILE_WIDGET_H
#define RICFILE_WIDGET_H

#include <QWidget>
#include <QString>
#include <QImage>
#include <QGraphicsScene>
#include <QItemSelectionModel>


#include "../riclib/ricfile.h"
#include "ricObjectModel.h"
#include "ricParametersModel.h"
#include "ricobjectview/ricobject_container.h"


class ricfile_widget: public QWidget{
	Q_OBJECT
	
	private:
		class Ui_Form *ui;
		QImage image;
		QGraphicsScene scene;
		ricfile graphics;
		ricModel model;
		ricParametersModel parameters;
		QItemSelectionModel *ricfile_selection_model;
		ricobject_container* ricobjectview;
		
		QString current_file;
		bool edited;
	
	public:
		explicit ricfile_widget( QString filename = "", QWidget *parent = 0 );
		~ricfile_widget();
	
	
	public:
		void open_file( QString filename );
		void reset();
		void save_file();
		void save_file( QString filename );
		bool replaceable() const;
		bool file_edited() const;
		bool is_original() const;
	
	private slots:
		void file_changed();
	
	public slots:
		void update_selection();
		void update_model();
		void update_preview();
};

#endif