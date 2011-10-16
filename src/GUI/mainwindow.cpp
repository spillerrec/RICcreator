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

//For central widget
#include <QTabBar>
#include <QStackedWidget>

#include "../riclib/nxtCanvas.h"
#include "openRicfile.h"
#include "ricfileEditor.h"
#include "importImageDialog.h"



#include <QFileInfo>


MainWindow::MainWindow( QStringList filenames, QWidget *parent) :
		QMainWindow(parent),
		ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	
	openRicfile::set_parent( this ); //Set editor parent
	current_editor = NULL;

	connect( ui->action_Exit, SIGNAL(triggered()), this, SLOT( close() ) );
	connect( ui->action_Open, SIGNAL(triggered()), this, SLOT( open_file() ) );
	connect( ui->action_Save, SIGNAL(triggered()), this, SLOT( save_file() ) );
	connect( ui->action_Save_as, SIGNAL(triggered()), this, SLOT( save_file_as() ) );
	connect( ui->action_export_bitmap, SIGNAL(triggered()), this, SLOT( export_file() ) );
	connect( ui->action_export_header, SIGNAL(triggered()), this, SLOT( export_header() ) );
	connect( ui->action_New, SIGNAL(triggered()), this, SLOT( new_file() ) );
	connect( ui->action_Close, SIGNAL(triggered()), this, SLOT( close_tab() ) );
	connect( ui->actionRICcreator_Help, SIGNAL(triggered()), this, SLOT( show_help() ) );
	connect( ui->action_About, SIGNAL(triggered()), this, SLOT( show_about() ) );
	connect( ui->action_fullscreen, SIGNAL(toggled(bool)), this, SLOT( enter_fullscreen(bool) ) );
	
	//Change editor actions
	connect( ui->actionSimple, SIGNAL(triggered()), this, SLOT( change_to_simple_editor() ) );
	connect( ui->actionFull, SIGNAL(triggered()), this, SLOT( change_to_advanced_editor() ) );
	connect( ui->actionFont, SIGNAL(triggered()), this, SLOT( change_to_font_editor() ) );
	
	
	//Setup tab bar
	tab_bar = new QTabBar( this );
	tab_bar->setTabsClosable( true );
	tab_bar->setMovable( true );
	centralWidget()->layout()->addWidget( tab_bar );
	
	//Connect tab bar
	connect( tab_bar, SIGNAL(tabCloseRequested(int)), this, SLOT( close_tab(int) ) );
	connect( tab_bar, SIGNAL(currentChanged(int)), this, SLOT( tab_changed(int) ) );
	connect( tab_bar, SIGNAL(tabMoved(int,int)), this, SLOT( tab_moved(int,int) ) );
	
	
	perferences.load();
	setAcceptDrops( true );
	
	//Open files, or create a new
	if( filenames.isEmpty() )
		new_file();
	else
		foreach( QString file, filenames )
			open_file( file );
}

MainWindow::~MainWindow(){
	perferences.save();
	delete ui;
}


//*************** TAB BAR ***************//

void MainWindow::tab_changed( int index ){
	if( files.size() > index && index > -1 )
		change_file( files[index] );
}


void MainWindow::tab_moved( int from, int to ){
	files.move( from, to );
}


bool MainWindow::close_tab(){
	return close_tab( tab_bar->currentIndex() );
}


bool MainWindow::close_tab( int tab ){
	if( tab < 0 || files.count() <= tab )
		return false;
	
	openRicfile*const file = files[tab];
	
	if( file->file_edited() ){
		//Create message box
		QMessageBox msg_box;
		if( file->is_original() )
			msg_box.setText( tr( "This file has not been saved." ) );
		else
			msg_box.setText( tr( "\"" ) + QFileInfo( file->file_name ).fileName() + tr( "\" has been modified" ) );
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
	
	//Remove the file and tab
	files.removeAt( tab );
	delete file;
	tab_bar->removeTab( tab );
	
	//If no tabs are back, open a new empty file
	if( files.size() < 1 )
		new_file();
	
	return true;
}

void MainWindow::update_tab(){
	int tab = tab_bar->currentIndex();
	if( tab >= 0 ){
		openRicfile *const file = files[tab];
		
		//Show edited
		tab_bar->setTabTextColor( tab, QColor( file->edited ? Qt::red : Qt::black ) );
		
		//Set text
		QString text = QFileInfo( file->file_name ).fileName();
		if( file->edited )
			text += "*";
		tab_bar->setTabText( tab, text );
	}
}


openRicfile::file_editor MainWindow::select_editor( ricfile &file ){
	if( openRicfile::viewer_supported( file, openRicfile::font_mode ) )
		return openRicfile::font_mode;
	
	if( openRicfile::viewer_supported( file, openRicfile::simple_mode ) )
		return openRicfile::simple_mode;
	
	return openRicfile::advanced_mode;
}

void MainWindow::change_editor( ricfileEditor *new_editor ){
	if( new_editor == current_editor || !new_editor )
		return;	//Don't do this if it already is the correct editor (or it is missing)
	
	qDebug( "editor changed" );
	
	//Remove the old editor and toolbar
	QToolBar *toolbar = NULL;
	if( current_editor ){
		centralWidget()->layout()->removeWidget( current_editor );
		current_editor->hide();
		
		toolbar = current_editor->editor_toolbar();
	}
	if( toolbar )
		removeToolBar( toolbar );
	
	
	
	//Update settings
	current_editor = new_editor;
	
	
	//Add the new editor and toolbar
	centralWidget()->layout()->addWidget( current_editor );
	current_editor->show();
	
	toolbar = current_editor->editor_toolbar();
	if( toolbar )
		addToolBar( toolbar );
}

void MainWindow::change_to_editor( openRicfile::file_editor editor_type ){
	//Get current file
	openRicfile *const file = get_current_ricfile();
	if( !file )
		return;
	
	if( file->editor == editor_type )
		return; //Already correct editor
	
	//Change editor if possible
	if( openRicfile::viewer_supported( file->ric(), editor_type ) ){
		file->editor = editor_type;
		change_file( file );
	}
	else
		QMessageBox::warning(
				this,
				tr( "Can't change editor" ),
				tr( "The selected editor can't edit this file" )
			);
}

void MainWindow::change_to_simple_editor(){
	change_to_editor( openRicfile::simple_mode );
}
void MainWindow::change_to_advanced_editor(){
	change_to_editor( openRicfile::advanced_mode );
}
void MainWindow::change_to_font_editor(){
	change_to_editor( openRicfile::font_mode );
}



void MainWindow::change_file( openRicfile *file ){
	ricfileEditor* viewer = file->get_viewer();
	change_editor( viewer );
	viewer->change_file( file );
}

void MainWindow::enter_fullscreen( bool action ){
	if( action )
		showFullScreen();
	else
		showNormal();
}

void MainWindow::closeEvent( QCloseEvent *event ){
	//Close all open files
	while( tab_bar->count() > 1 ){	//We need to do last tab manually
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


openRicfile *const MainWindow::get_current_ricfile() const{
	int tab = tab_bar->currentIndex();
	if( tab >= 0 && files.size() > tab )
		return files[tab];//(ricfile_widget*) ui->tabWidget->widget( tab );
	else
		return NULL;
	
}




//*************** File operations ***************//

void MainWindow::show_loader_error( QString filepath, QString operation, nxtIO::LoaderError code ){
	QString filename = QFileInfo( filepath ).fileName();
	QString error;
	
	//Determine message
	switch( code ){
		case nxtIO::LDR_UNDEFINEDERROR:
		case nxtIO::LDR_ENDOFFILE:
				error = QString( tr( "%1 is either corrupted or not a valid file" ) ).arg( filename );
			break;
			
		case nxtIO::LDR_FILENOTFOUND:
				error = QString( tr( "%1 could not be opened for %2" ) ).arg( filename ).arg( operation );
			break;
		
		default:
				error = QString( tr( "A LoaderError %1 orcurred while %2 %3" ) ).arg( code ).arg( operation ).arg( filename );
	}
	
	//Show warning
	QMessageBox::warning(
			this,
			QString( tr( "Error while %1 %2" ) ).arg( operation ).arg( filename ),
			error
		);
}

void MainWindow::open_file(){
	QString filename = QFileDialog::getOpenFileName(this, tr("Open RIC file"), perferences.get_last_path(), tr("RIC files (*.ric)") );
	open_file( filename );
}

void MainWindow::open_file( QString filename ){
	if( !filename.isEmpty() ){
		//Get filename without path
		QString name = QFileInfo( filename ).fileName();
		
		//TODO: check filetype
		
		//If the current file is empty and uneditet, replace it
		openRicfile*const file = get_current_ricfile();
		if( file ){
			if( file->replaceable() ){
				//Read file
				nxtIO::LoaderError result = file->ric().readfile( filename.toLocal8Bit().data() );
				
				if( result == nxtIO::LDR_SUCCESS ){
					perferences.new_file( filename );
					
					file->source = openRicfile::pc_file;
					file->file_name = filename;
					file->editor = select_editor( file->ric() );
					update_tab();
					change_file( file );
				}
				else	//Couldn't read the file
					show_loader_error( filename, "reading", result );
				
				return;
			}	
		}
		
		//Couldn't replace, create a new
		ricfile &newfile = *new ricfile;
		nxtIO::LoaderError result = newfile.readfile( filename.toLocal8Bit().data() );
		
		if( result == nxtIO::LDR_SUCCESS ){
			//Add the file
			openRicfile*const file = new openRicfile( newfile, "New file", openRicfile::pc_file, select_editor( newfile ) );
			perferences.new_file( filename );
			file->file_name = filename;
			
			files.append( file );
			tab_bar->setCurrentIndex( tab_bar->addTab( "" ) );
			update_tab();
			change_file( file );
		}
		else{	//Couldn't read the file
			delete &newfile;
			show_loader_error( filename, "reading", result );
		}
	}
}


bool MainWindow::save_file(){
	openRicfile*const file = get_current_ricfile();
	if( file ){
		if( file->file_edited() ){
			if( file->is_original() )
				return save_file_as();
			else{
				nxtIO::LoaderError result = file->ric().writefile( file->file_name.toLocal8Bit().data() );
				if( result == nxtIO::LDR_SUCCESS ){
					perferences.new_file( file->file_name );
					file->edited = false;
					update_tab();
					
					return true;
				}
				else
					show_loader_error( file->file_name, "writing", result );
				
				return false;
			}
		}
	}
	return false;
}


bool MainWindow::save_file_as(){
	openRicfile*const file = get_current_ricfile();
	if( file ){
		QString filename = QFileDialog::getSaveFileName( this, tr("Save RIC file"), perferences.get_last_path(), tr("RIC files (*.ric)") );
		if( !filename.isEmpty() ){
			//A filename was choosen, attempt to save
			nxtIO::LoaderError result = file->ric().writefile( filename.toLocal8Bit().data() );
			if( result == nxtIO::LDR_SUCCESS ){
				//Saving went fine, update
				perferences.new_file( filename );
				file->file_name = filename;
				file->edited = false;
				update_tab();
				
				return true;
			}
			else
				show_loader_error( file->file_name, "writing", result );
			
			return false;
		}
	}
	return false;	//File not saved
}


void MainWindow::export_file(){
	openRicfile*const file = get_current_ricfile();
	if( file ){
		QString filename = QFileDialog::getSaveFileName(this, tr("Export to png"), perferences.get_last_path(), tr("Portable network graphics (*.png)") );
		nxtCanvas image;
		image.set_auto_resize( true );
		file->ric().Draw( &image );
		importImageDialog::export_canvas( &image, filename );
	}
}


void MainWindow::export_header(){
	openRicfile*const file = get_current_ricfile();
	if( file ){
		QString filename = QFileDialog::getSaveFileName(this, tr("Export as C header"), perferences.get_last_path(), tr("Header file (*.h)") );
		QString var_name = QFileInfo(file->file_name).baseName();
		var_name.replace( " ", "_" );
		if( var_name[0].isDigit() )
			var_name[0] = QChar( '_' );
		file->ric().write_header_file( filename.toLocal8Bit().data(), var_name.toLocal8Bit().data() );
	}
}


void MainWindow::new_file(){
	ricfile &newfile = *new ricfile;
	
	openRicfile*const file = new openRicfile( newfile, "New file", openRicfile::original, select_editor( newfile ) );
	files.append( file );
	change_file( file );
	int position = tab_bar->addTab( "New file" );
	tab_bar->setCurrentIndex( position );
}



//*************** Dialogs ***************//

void MainWindow::show_about(){
	about_window.show();
}


void MainWindow::show_help(){
	QDesktopServices::openUrl( QUrl( "http://riccreator.sourceforge.net/docs.html" ) );
}

