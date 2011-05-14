#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>

#include <QtGui/QApplication>
#include <QString>
#include <QFileDialog>

#include "ricfile_widget.h"

MainWindow::MainWindow( QString filenames, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//	connect( ui->value_a, SIGNAL(valueChanged(double)), this, SLOT(on_equation_changed()) );	//Example

	connect( ui->action_Exit, SIGNAL(triggered()), this, SLOT( exit() ) );
	connect( ui->action_Open, SIGNAL(triggered()), this, SLOT( open_file() ) );
	connect( ui->action_Save, SIGNAL(triggered()), this, SLOT( save_file() ) );
	connect( ui->action_Save_as, SIGNAL(triggered()), this, SLOT( save_file_as() ) );
	connect( ui->action_New, SIGNAL(triggered()), this, SLOT( new_file() ) );
	connect( ui->action_Close, SIGNAL(triggered()), this, SLOT( close_tab() ) );
	connect( ui->action_About, SIGNAL(triggered()), this, SLOT( show_about() ) );
	connect( ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT( close_tab(int) ) );
	
	//Add the New Object menu command signals
	connect( ui->action_new_options, SIGNAL(triggered()), this, SLOT( add_options() ) );
	connect( ui->action_new_sprite, SIGNAL(triggered()), this, SLOT( add_sprite() ) );
	connect( ui->action_new_copybits, SIGNAL(triggered()), this, SLOT( add_copybits() ) );
	connect( ui->action_new_varmap, SIGNAL(triggered()), this, SLOT( add_varmap() ) );
	connect( ui->action_new_pixel, SIGNAL(triggered()), this, SLOT( add_pixel() ) );
	connect( ui->action_new_line, SIGNAL(triggered()), this, SLOT( add_line() ) );
	connect( ui->action_new_rectangle, SIGNAL(triggered()), this, SLOT( add_rectangle() ) );
	connect( ui->action_new_circle, SIGNAL(triggered()), this, SLOT( add_circle() ) );
	connect( ui->action_new_number, SIGNAL(triggered()), this, SLOT( add_number() ) );
	connect( ui->action_new_ellipse, SIGNAL(triggered()), this, SLOT( add_ellipse() ) );
	connect( ui->action_new_polyline, SIGNAL(triggered()), this, SLOT( add_polyline() ) );
	
	if( filenames.isEmpty() )
		new_file();
	else
		open_file( filenames );
	
	perferences.load();
}

MainWindow::~MainWindow(){
	perferences.save();
    delete ui;
}

void MainWindow::exit(){
	QApplication::exit(0);
}


void MainWindow::open_file(){
	QString filename = QFileDialog::getOpenFileName(this, tr("Open RIC file"), perferences.get_last_path(), tr("RIC files (*.ric)") );
	open_file( filename );
}

QString path_to_filename( QString path ){
	int name_start = path.lastIndexOf( "/" );
	if( name_start > -1 )
		path.remove( 0, name_start+1 );
	else{
		name_start = path.lastIndexOf( "\\" );
		if( name_start > -1 )
			path.remove( 0, name_start+1 );
	}
	
	return path;
}

void MainWindow::open_file( QString filename ){
	if( !filename.isEmpty() ){
		//Get filename without path
		QString name = path_to_filename( filename );
		
		int tab = ui->tabWidget->currentIndex();
		if( tab >= 0 ){
			ricfile_widget* file = (ricfile_widget*) ui->tabWidget->widget( tab );
			if( file->replaceable() ){
				file->open_file( filename );
				perferences.new_file( filename );
				
				ui->tabWidget->setTabText( tab, name );
				return;
			}	
		}
		
		int position = ui->tabWidget->addTab(new ricfile_widget( filename ), name);
		ui->tabWidget->setCurrentIndex( position );
	}
}

void MainWindow::save_file(){
	int tab = ui->tabWidget->currentIndex();
	if( tab >= 0 ){
		ricfile_widget* file = (ricfile_widget*) ui->tabWidget->widget( tab );
		
		if( file->file_edited() ){
			if( file->is_original() ){
				save_file_as();
			}
			else{
				file->save_file();
			}
		}
	}
}

void MainWindow::save_file_as(){
	int tab = ui->tabWidget->currentIndex();
	if( tab >= 0 ){
		ricfile_widget* file = (ricfile_widget*) ui->tabWidget->widget( tab );
		
		QString filename = QFileDialog::getSaveFileName( this, tr("Save RIC file"), perferences.get_last_path(), tr("RIC files (*.ric)") );
		if( !filename.isEmpty() ){
			file->save_file( filename );
			
			ui->tabWidget->setTabText( tab, path_to_filename( filename ) );
		}
	}
}

void MainWindow::new_file(){
	int position = ui->tabWidget->addTab(new ricfile_widget(), tr("new file"));
	ui->tabWidget->setCurrentIndex( position );
}


void MainWindow::close_tab(){
	close_tab( ui->tabWidget->currentIndex() );
}
void MainWindow::show_about(){
	about_window.show();
}

void MainWindow::close_tab( int tab ){
	//Make sure it doesn't close the last tab
	if( ui->tabWidget->count() <= 1 )
		return;
	
	//Remove the tab, but there seems to be some mess with the documentation...
	ricfile_widget* file = (ricfile_widget*) ui->tabWidget->widget( tab );
	delete file;
//	ui->tabWidget->removeTab( tab );
}

void MainWindow::add_object( unsigned int object_type ){
	int tab = ui->tabWidget->currentIndex();
	if( tab >= 0 ){
		ricfile_widget* file = (ricfile_widget*) ui->tabWidget->widget( tab );
		
		file->add_object( object_type );
	}
}

void MainWindow::add_options()	{ add_object( ricfile::ricObject::RIC_OP_OPTIONS ); }
void MainWindow::add_sprite()	{ add_object( ricfile::ricObject::RIC_OP_SPRITE ); }
void MainWindow::add_copybits()	{ add_object( ricfile::ricObject::RIC_OP_COPYBITS ); }
void MainWindow::add_varmap()	{ add_object( ricfile::ricObject::RIC_OP_VARMAP ); }
void MainWindow::add_pixel()	{ add_object( ricfile::ricObject::RIC_OP_PIXEL ); }
void MainWindow::add_line()	{ add_object( ricfile::ricObject::RIC_OP_LINE ); }
void MainWindow::add_rectangle()	{ add_object( ricfile::ricObject::RIC_OP_RECTANGLE ); }
void MainWindow::add_circle()	{ add_object( ricfile::ricObject::RIC_OP_CICLE ); }
void MainWindow::add_number()	{ add_object( ricfile::ricObject::RIC_OP_NUMBER ); }
void MainWindow::add_ellipse()	{ add_object( ricfile::ricObject::RIC_OP_ELLIPSE ); }
void MainWindow::add_polyline()	{ add_object( ricfile::ricObject::RIC_OP_POLYGON ); }


