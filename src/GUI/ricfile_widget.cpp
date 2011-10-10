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


#include <QMessageBox>
#include <QToolBar>

#include "openRicfile.h"

#include "../riclib/ricObject.h"
#include "ricObjectModel.h"
#include "ricobjectview/ricobject_container.h"

#include "nxtCanvasWidgetContainer.h"

class ricObject;

ricfile_widget::ricfile_widget( QWidget *parent ):
		ricfileEditor( parent ),
		ui(new Ui_Form),
		canvas( this ),
		model( NULL ),
		parameters( NULL, this )
{
	ui->setupUi(this);
	
	ricobjectview = new ricobject_container( ui->properties_box );
	
	connect( &parameters, SIGNAL(dataChanged( QModelIndex, QModelIndex )), this, SLOT( update_preview() ) );
	connect( ricobjectview, SIGNAL( object_changed() ), this, SLOT( update_preview() ) );
	connect( ricobjectview, SIGNAL( object_changed() ), this, SLOT( update_model() ) );
	connect( ricobjectview, SIGNAL( object_changed() ), this, SLOT( file_changed() ) );
	
	connect( ui->move_up, SIGNAL( clicked( bool ) ), this, SLOT( move_object_up() ) );
	connect( ui->move_down, SIGNAL( clicked( bool ) ), this, SLOT( move_object_down() ) );
	connect( ui->remove_item, SIGNAL( clicked( bool ) ), this, SLOT( remove_object() ) );
	
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
	
	
	//Create the toolbar
	toolbar = new QToolBar( "Add object", this );
	
	QAction *temp = new QAction( "Options", this ); //Icon can be added as first parameter
	connect( temp, SIGNAL( triggered(bool) ), this, SLOT( add_options() ) );
	toolbar->addAction( temp );
	
	temp = new QAction( "Sprite", this );
	connect( temp, SIGNAL( triggered(bool) ), this, SLOT( add_sprite() ) );
	toolbar->addAction( temp );
	
	temp = new QAction( "Copybits", this );
	connect( temp, SIGNAL( triggered(bool) ), this, SLOT( add_copybits() ) );
	toolbar->addAction( temp );
	
	temp = new QAction( "VarMap", this );
	connect( temp, SIGNAL( triggered(bool) ), this, SLOT( add_varmap() ) );
	toolbar->addAction( temp );
	
	temp = new QAction( "Pixel", this );
	connect( temp, SIGNAL( triggered(bool) ), this, SLOT( add_pixel() ) );
	toolbar->addAction( temp );
	
	temp = new QAction( "Line", this );
	connect( temp, SIGNAL( triggered(bool) ), this, SLOT( add_line() ) );
	toolbar->addAction( temp );
	
	temp = new QAction( "Rectangle", this );
	connect( temp, SIGNAL( triggered(bool) ), this, SLOT( add_rectangle() ) );
	toolbar->addAction( temp );
	
	temp = new QAction( "Circle", this );
	connect( temp, SIGNAL( triggered(bool) ), this, SLOT( add_circle() ) );
	toolbar->addAction( temp );
	
	temp = new QAction( "Number", this );
	connect( temp, SIGNAL( triggered(bool) ), this, SLOT( add_number() ) );
	toolbar->addAction( temp );
	
	temp = new QAction( "Ellipse", this );
	connect( temp, SIGNAL( triggered(bool) ), this, SLOT( add_ellipse() ) );
	toolbar->addAction( temp );
	
	temp = new QAction( "Polygon", this );
	connect( temp, SIGNAL( triggered(bool) ), this, SLOT( add_polygon() ) );
	toolbar->addAction( temp );
}

void ricfile_widget::change_file( openRicfile *new_file ){
	file = new_file;
	model.reset_model();
	model.change_file( &file->ric() );
	parameters.change_file( &file->ric() );
	update_model();
	update_preview();

	//TODO: store selection position
	ricfile_selection_model->select( QItemSelection( model.index( 0, 0 ), model.index( 0, 2 ) ), QItemSelectionModel::Select );
	update_selection();
}

QToolBar* ricfile_widget::editor_toolbar(){
	return toolbar;
}


void ricfile_widget::update_model(){
	model.update();
}

ricfile_widget::~ricfile_widget(){ delete ui; }

void ricfile_widget::update_selection(){
	if( ricfile_selection_model->hasSelection() ){
		const QModelIndexList indexes = ricfile_selection_model->selection().indexes();
		if( indexes.size() >= 1 ){
			QModelIndex current_index = indexes[0];
			
			//Convert it to a ricObject
			ricObject* object = model.ricobject_at_index( current_index );
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
	else
		ricobjectview->view_object( NULL );
}


void ricfile_widget::update_preview(){
	ui->treeView->update( model.index( 0,0 ) );
	
	if( file )
		file->ric().Draw(&drawing_canvas, 100, 64);
	
	canvas.update();
}


bool ricfile_widget::add_object( unsigned int object_type ){
	if( file && file->ric().add_ric_object( object_type ) ){
		file_changed();
		model.reset_model();
		emit update_preview();
		
		ricfile_selection_model->select( QItemSelection( model.index( file->ric().object_amount()-1, 0 ), model.index( file->ric().object_amount()-1, 2 ) ), QItemSelectionModel::Select );
		
		return true;
	}
	else
		return false;
}

void ricfile_widget::move_object_up(){
	if( file && ricfile_selection_model->hasSelection() ){
		const QModelIndexList indexes = ricfile_selection_model->selection().indexes();
		if( indexes.size() >= 1 ){
			QModelIndex current_index = indexes[0];
			
			//Get index
			unsigned int index = file->ric().object_index( model.ricobject_at_index( current_index ) );
			
			if( index > 0 ){	//Only move up if it can
				file->ric().move_object( index, index-1 );
				
				file_changed();
				model.reset_model();
				emit update_preview();
				
				//Reset selection
				ricfile_selection_model->select( QItemSelection( model.index( index-1, 0 ), model.index( index-1, 2 ) ), QItemSelectionModel::Select );
			}
		}
	}
}
void ricfile_widget::move_object_down(){
	if( file && ricfile_selection_model->hasSelection() ){
		const QModelIndexList indexes = ricfile_selection_model->selection().indexes();
		if( indexes.size() >= 1 ){
			QModelIndex current_index = indexes[0];
			
			//Get index
			unsigned int index = file->ric().object_index( model.ricobject_at_index( current_index ) );
			
			if( index < file->ric().object_amount()-1 ){	//Only move up if it can
				file->ric().move_object( index, index+1 );
				
				file_changed();
				model.reset_model();
				emit update_preview();
				
				//Reset selection
				ricfile_selection_model->select( QItemSelection( model.index( index+1, 0 ), model.index( index+1, 2 ) ), QItemSelectionModel::Select );
			}
		}
	}
}
void ricfile_widget::remove_object(){
	if( file && ricfile_selection_model->hasSelection() ){
		const QModelIndexList indexes = ricfile_selection_model->selection().indexes();
		if( indexes.size() >= 1 ){
			QModelIndex current_index = indexes[0];
			
			//Get index
			unsigned int index = file->ric().object_index( model.ricobject_at_index( current_index ) );
			file->ric().remove_object( index );
			
			//Update the view
			file_changed();
			model.reset_model();
			emit update_preview();
			
			//Set the selection on the next element
			if( file->ric().object_amount() > 0 ){
				if( index >= file->ric().object_amount() )
					index = file->ric().object_amount()-1;
				ricfile_selection_model->select( QItemSelection( model.index( index, 0 ), model.index( index, 2 ) ), QItemSelectionModel::Select );
			}
			else
				update_selection();	//If there are no objects back, make sure it shows nothing
		}
	}
}


void ricfile_widget::add_options()	{ add_object( ricObject::RIC_OP_OPTIONS ); }
void ricfile_widget::add_sprite()	{ add_object( ricObject::RIC_OP_SPRITE ); }
void ricfile_widget::add_copybits()	{ add_object( ricObject::RIC_OP_COPYBITS ); }
void ricfile_widget::add_varmap()	{ add_object( ricObject::RIC_OP_VARMAP ); }
void ricfile_widget::add_pixel()	{ add_object( ricObject::RIC_OP_PIXEL ); }
void ricfile_widget::add_line()	{ add_object( ricObject::RIC_OP_LINE ); }
void ricfile_widget::add_rectangle()	{ add_object( ricObject::RIC_OP_RECTANGLE ); }
void ricfile_widget::add_circle()	{ add_object( ricObject::RIC_OP_CICLE ); }
void ricfile_widget::add_number()	{ add_object( ricObject::RIC_OP_NUMBER ); }
void ricfile_widget::add_ellipse()	{ add_object( ricObject::RIC_OP_ELLIPSE ); }
void ricfile_widget::add_polygon()	{ add_object( ricObject::RIC_OP_POLYGON ); }


