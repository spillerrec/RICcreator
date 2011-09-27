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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>

#include <QtGui/QApplication>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>

//For Drag-and-drop
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QUrl>

#include "ricfile_widget.h"

MainWindow::MainWindow( QStringList filenames, QWidget *parent) :
		QMainWindow(parent),
		ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect( ui->action_Exit, SIGNAL(triggered()), this, SLOT( close() ) );
	connect( ui->action_Open, SIGNAL(triggered()), this, SLOT( open_file() ) );
	connect( ui->action_Save, SIGNAL(triggered()), this, SLOT( save_file() ) );
	connect( ui->action_Save_as, SIGNAL(triggered()), this, SLOT( save_file_as() ) );
	connect( ui->action_export_bitmap, SIGNAL(triggered()), this, SLOT( export_file() ) );
	connect( ui->action_New, SIGNAL(triggered()), this, SLOT( new_file() ) );
	connect( ui->action_Close, SIGNAL(triggered()), this, SLOT( close_tab() ) );
	connect( ui->actionRICcreator_Help, SIGNAL(triggered()), this, SLOT( show_help() ) );
	connect( ui->action_About, SIGNAL(triggered()), this, SLOT( show_about() ) );
	connect( ui->action_fullscreen, SIGNAL(toggled(bool)), this, SLOT( enter_fullscreen(bool) ) );
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
	connect( ui->action_new_polygon, SIGNAL(triggered()), this, SLOT( add_polyline() ) );
	
	if( filenames.isEmpty() )
		new_file();
	else
		foreach( QString file, filenames )
			open_file( file );
	
	perferences.load();
	setAcceptDrops( true );
}

MainWindow::~MainWindow(){
	perferences.save();
	delete ui;
}

void MainWindow::enter_fullscreen( bool action ){
	if( action )
		showFullScreen();
	else
		showNormal();
}

void MainWindow::closeEvent( QCloseEvent *event ){
	//Close all open files
	while( ui->tabWidget->count() > 1 ){	//We need to do last tab manually
		if( !close_tab() ){
			event->ignore();
			return;
		}
	}
	if( !close_tab() ){
		event->ignore();
		return;
	}
	
	//Window is ready to close
	event->accept();
}

void MainWindow::dragEnterEvent( QDragEnterEvent *event ){
	if( event->mimeData()->hasUrls() )
		event->acceptProposedAction();
		//TODO: only accept if it includes files we want?
}
void MainWindow::dropEvent( QDropEvent *event ){
	if( event->mimeData()->hasUrls() ){
		event->setDropAction( Qt::CopyAction );
		
		foreach( QUrl url, event->mimeData()->urls() )
			open_file( url.toLocalFile() );
		
		event->accept();
	}
}


ricfile_widget* MainWindow::get_current_ricfile() const{
	int tab = ui->tabWidget->currentIndex();
	if( tab >= 0 )
		return (ricfile_widget*) ui->tabWidget->widget( tab );
	else
		return NULL;
	
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
		
		//TODO: check filetype
		
		ricfile_widget* file = get_current_ricfile();
		if( file ){
			if( file->replaceable() ){
				file->open_file( filename );
				perferences.new_file( filename );
				
				ui->tabWidget->setTabText( ui->tabWidget->currentIndex(), name );
				return;
			}	
		}
		
		int position = ui->tabWidget->addTab(new ricfile_widget( filename ), name);
		ui->tabWidget->setCurrentIndex( position );
	}
}

bool MainWindow::save_file(){
	ricfile_widget* file = get_current_ricfile();
	if( file ){
		if( file->file_edited() ){
			if( file->is_original() )
				return save_file_as();
			else
				return file->save_file();
		}
	}
	return false;
}

bool MainWindow::save_file_as(){
	ricfile_widget* file = get_current_ricfile();
	if( file ){
		QString filename = QFileDialog::getSaveFileName( this, tr("Save RIC file"), perferences.get_last_path(), tr("RIC files (*.ric)") );
		if( !filename.isEmpty() ){
			file->save_file( filename );
			ui->tabWidget->setTabText( ui->tabWidget->currentIndex(), path_to_filename( filename ) );
			
			return true;
		}
	}
	return false;	//File not saved
}

void MainWindow::export_file(){
	ricfile_widget* file = get_current_ricfile();
	if( file )
		file->export_file();
}

void MainWindow::new_file(){
	int position = ui->tabWidget->addTab(new ricfile_widget(), tr("new file"));
	ui->tabWidget->setCurrentIndex( position );
}


bool MainWindow::close_tab(){
	return close_tab( ui->tabWidget->currentIndex() );
}
void MainWindow::show_about(){
	about_window.show();
}
void MainWindow::show_help(){
	QDesktopServices::openUrl( QUrl( "http://riccreator.sourceforge.net/docs.html" ) );
}

bool MainWindow::close_tab( int tab ){
	ricfile_widget* file = (ricfile_widget*)ui->tabWidget->widget( tab );
	if( file ){
		if( file->file_edited() ){
			//Create message box
			QMessageBox msg_box;
			if( file->is_original() )
				msg_box.setText( tr( "This file has not been saved." ) );
			else
				msg_box.setText( tr( "\"" ) + path_to_filename( file->get_filename() ) + tr( "\" has been modified" ) );
			msg_box.setInformativeText( tr( "Do you want to save it before closing?" ) );
			msg_box.setStandardButtons( QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel );
			msg_box.setDefaultButton(QMessageBox::Save);
			msg_box.setIcon( QMessageBox::Question );
			
			//Display and handle message box
			switch( msg_box.exec() ){
				case QMessageBox::Save: if( !save_file() ) return false; break;	//return if save failed
				case QMessageBox::Discard: break;
				case QMessageBox::Cancel: return false;
				default: qDebug( "MainWindow::close_tab(int): no button in msg_box clicked!" ); return false;
			}
		}
		
		//Remove the tab, but there seems to be some mess with the documentation...
		delete file;
		//	ui->tabWidget->removeTab( tab );
		
		//If no tabs are back, open a new empty file
		if( ui->tabWidget->count() < 1 )
			new_file();
		
		return true;
	}
	else
		return false;
}

void MainWindow::add_object( unsigned int object_type ){
	ricfile_widget* file = get_current_ricfile();
	if( file )
		file->add_object( object_type );
}


#include "../riclib/ricObject.h"

void MainWindow::add_options()	{ add_object( ricObject::RIC_OP_OPTIONS ); }
void MainWindow::add_sprite()	{ add_object( ricObject::RIC_OP_SPRITE ); }
void MainWindow::add_copybits()	{ add_object( ricObject::RIC_OP_COPYBITS ); }
void MainWindow::add_varmap()	{ add_object( ricObject::RIC_OP_VARMAP ); }
void MainWindow::add_pixel()	{ add_object( ricObject::RIC_OP_PIXEL ); }
void MainWindow::add_line()	{ add_object( ricObject::RIC_OP_LINE ); }
void MainWindow::add_rectangle()	{ add_object( ricObject::RIC_OP_RECTANGLE ); }
void MainWindow::add_circle()	{ add_object( ricObject::RIC_OP_CICLE ); }
void MainWindow::add_number()	{ add_object( ricObject::RIC_OP_NUMBER ); }
void MainWindow::add_ellipse()	{ add_object( ricObject::RIC_OP_ELLIPSE ); }
void MainWindow::add_polyline()	{ add_object( ricObject::RIC_OP_POLYGON ); }


