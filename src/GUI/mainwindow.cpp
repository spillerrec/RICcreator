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
	connect( ui->action_New, SIGNAL(triggered()), this, SLOT( new_file() ) );
	connect( ui->action_Close, SIGNAL(triggered()), this, SLOT( close_tab() ) );
	connect( ui->action_About, SIGNAL(triggered()), this, SLOT( show_about() ) );
	connect( ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT( close_tab(int) ) );
	
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
				QString filename = QFileDialog::getSaveFileName( this, tr("Save RIC file"), perferences.get_last_path(), tr("RIC files (*.ric)") );
				if( !filename.isEmpty() ){
					file->save_file( filename );
					
					ui->tabWidget->setTabText( tab, path_to_filename( filename ) );
				}
			}
			else{
				file->save_file();
			}
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


