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

#include "ricfile_widget.h"
#include "ui_ricfile.h"

#include <QPoint>
#include <QPixmap>
#include <QModelIndex>


#include "../riclib/nxtCanvas.h"
#include "ric_value.h"
#include "ricObjectModel.h"
//#include "ricobject.h"
#include "../riclib/ricObject.h"


ricfile_widget::ricfile_widget( QString filename, QWidget *parent ): QWidget(parent), ui(new Ui_Form), image(100,64,QImage::Format_Mono), model( &graphics ), parameters( &graphics, this ){
	ui->setupUi(this);
	edited = false;
	
	ricobjectview = new ricobject_container( ui->properties_box );
	
	connect( &parameters, SIGNAL(dataChanged( QModelIndex, QModelIndex )), this, SLOT( update_preview() ) );
	connect( ricobjectview, SIGNAL( object_changed() ), this, SLOT( update_preview() ) );
	connect( ricobjectview, SIGNAL( object_changed() ), this, SLOT( update_model() ) );
	connect( ricobjectview, SIGNAL( object_changed() ), this, SLOT( file_changed() ) );
	
	//Setup treeview
	ui->treeView->setModel( &model );
	ui->treeView->setColumnWidth( 0, 120 );
	ui->treeView->setColumnWidth( 1, 80 );
	ui->treeView->setColumnWidth( 2, 40 );
	ricfile_selection_model = ui->treeView->selectionModel();
	
	//Setup other views
	ui->parameter_view->setModel( &parameters );
	ui->canvas->setScene(&scene);
	ui->canvas->setSceneRect(0,0, 100, 64 );
	ui->canvas->scale( 2,2 );
	
	open_file( filename );	//Try to open an existing file
	
	
	//ricfile_object* editthing = new ricfile_object( ui->properties_box );
	//update_selection();
	connect( ricfile_selection_model, SIGNAL(selectionChanged( QItemSelection, QItemSelection )), this, SLOT( update_selection() ) );
	
	
}

void ricfile_widget::update_model(){
	model.update();
}
void ricfile_widget::file_changed(){
	edited = true;
}

ricfile_widget::~ricfile_widget(){ delete ui; }

void ricfile_widget::update_selection(){
	if( ricfile_selection_model->hasSelection() ){
		const QModelIndexList indexes = ricfile_selection_model->selection().indexes();
		if( indexes.size() >= 1 ){
			QModelIndex current_index = indexes[0];
			
			//Convert it to a ricObject
			ricfile::ricObject* object = model.ricobject_at_index( current_index );
			if( object ){
				ricobjectview->view_object( object );
				
				return;
			}
			
			ui->properties_box->setTitle( "Wrong index amount" );
			return;
		}
		ui->properties_box->setTitle( "Wrong amount of indexes" );
		return;
	}
}

bool ricfile_widget::replaceable() const{
	return current_file.isEmpty() && !edited;
}

bool ricfile_widget::file_edited() const{
	return edited;
}

bool ricfile_widget::is_original() const{
	return current_file.isEmpty();
}


void ricfile_widget::update_preview(){
	ui->treeView->update( model.index( 0,0 ) );
	
	nxtCanvas image2;
	image2.create(100,64);
	
	graphics.Draw(&image2, 100, 64);
	
	
	image.fill(1);
	for(int ix=0; ix<100; ix++)
		for(int iy=0; iy<64; iy++){
			QPoint position( ix, 63-iy );
			
			if( image2.get_pixel( ix, iy ) )
				image.setPixel( position, 0 );
			else
				image.setPixel( position, 1 );
		}
	
	
	
	
	scene.addPixmap( QPixmap::fromImage( image ) );
	
	ui->canvas->show();
}



void ricfile_widget::open_file( QString filename ){
	if( !filename.isEmpty() ){
		graphics.readfile( filename.toLocal8Bit().data() );
		current_file = filename;
		
		model.reset_model();
		parameters.update();
		emit update_preview();
	}
}


void ricfile_widget::save_file(){
	save_file( current_file );
}
void ricfile_widget::save_file( QString filename ){
	if( !filename.isEmpty() ){
		graphics.writefile( filename.toLocal8Bit().data() );
		edited = false;
	}
}

void ricfile_widget::reset(){
	graphics.Reset();
	edited = true;
	
	model.reset_model();
	emit update_preview();
}

bool ricfile_widget::add_object( unsigned int object_type ){
	if( graphics.add_ric_object( object_type ) ){
		model.reset_model();
		emit update_preview();
		
		ricfile_selection_model->select( model.index( graphics.object_amount()-1, 0 ), QItemSelectionModel::Select );
		
		return true;
	}
	else
		return false;
}

