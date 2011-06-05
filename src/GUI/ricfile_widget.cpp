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

#include <QModelIndex>
#include <QItemSelection>
#include <QItemSelectionModel>

#include "ricObjectModel.h"
#include "../riclib/ricObject.h"
#include "ricobjectview/ricobject_container.h"

#include "nxtCanvasWidgetContainer.h"


ricfile_widget::ricfile_widget( QString filename, QWidget *parent ):
		QWidget(parent),
		ui(new Ui_Form),
		canvas( this ),
		model( &graphics ),
		parameters( &graphics, this )
{
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
	connect( ricfile_selection_model, SIGNAL(selectionChanged( QItemSelection, QItemSelection )), this, SLOT( update_selection() ) );
	
	
	//Setup other views
	ui->parameter_view->setModel( &parameters );
	
	//Setup nxtCanvasWidget
	drawing_canvas.create(100,64);
	canvas.zoom( 2 );
	canvas.change_canvas( &drawing_canvas );
	
	nxtCanvasWidgetContainer *container = new nxtCanvasWidgetContainer( &canvas, false, this );
	container->setMinimumSize( QSize( 202, 130 ) );
	container->setMaximumSize( QSize( 202, 130 ) );
	ui->horizontalLayout_2->insertWidget( 0, (QWidget*)container );	//Add to layout
	
	open_file( filename );	//Try to open an existing file
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
	
	graphics.Draw(&drawing_canvas, 100, 64);
	
	canvas.update();
}



void ricfile_widget::open_file( QString filename ){
	if( !filename.isEmpty() ){
		graphics.readfile( filename.toLocal8Bit().data() );
		current_file = filename;
		
		model.reset_model();
		parameters.update();
		emit update_preview();
		
		ricfile_selection_model->select( QItemSelection( model.index( 0, 0 ), model.index( 0, 2 ) ), QItemSelectionModel::Select );
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
		
		ricfile_selection_model->select( QItemSelection( model.index( graphics.object_amount()-1, 0 ), model.index( graphics.object_amount()-1, 2 ) ), QItemSelectionModel::Select );
		
		return true;
	}
	else
		return false;
}

