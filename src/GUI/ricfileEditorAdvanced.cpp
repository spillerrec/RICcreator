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

#include "ricfileEditorAdvanced.h"
#include "ui_ricfile.h"

#include <QModelIndex>
#include <QItemSelection>
#include <QItemSelectionModel>


#include <QMessageBox>
#include <QToolBar>

#include "openRicfile.h"

#include "../riclib/ricObject.h"
#include "ricObjectContainer.h"

#include "nxtCanvasWidgetContainer.h"


//Create and add a action to the toolbar
//TODO: create icons and add
#define ADD_ACTION( toolbar, text, slot ){ QAction *temp = new QAction( tr( text ), this ); \
	connect( temp, SIGNAL( triggered(bool) ), this, SLOT( slot ) ); \
	toolbar->addAction( temp ); }


ricfileEditorAdvanced::ricfileEditorAdvanced( QWidget *parent ):
		ricfileEditor( parent ),
		ui(new Ui_Form),
		canvas( this ),
		parameters( NULL, this )
{
	ui->setupUi(this);
	
	ricobjectview = new ricObjectContainer( ui->properties_box );
	
	connect( &parameters, SIGNAL(dataChanged( QModelIndex, QModelIndex )), this, SLOT( update_preview() ) );
	connect( ricobjectview, SIGNAL( object_changed() ), this, SLOT( update_preview() ) );
	connect( ricobjectview, SIGNAL( object_changed() ), &model, SLOT( update() ) );
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
	
	ADD_ACTION( toolbar, "Options", add_options() );
	ADD_ACTION( toolbar, "Sprite", add_sprite() );
	ADD_ACTION( toolbar, "Copybits", add_copybits() );
	ADD_ACTION( toolbar, "VarMap", add_varmap() );
	ADD_ACTION( toolbar, "Pixel", add_pixel() );
	ADD_ACTION( toolbar, "Line", add_line() );
	ADD_ACTION( toolbar, "Rectangle", add_rectangle() );
	ADD_ACTION( toolbar, "Circle", add_circle() );
	ADD_ACTION( toolbar, "Number", add_number() );
	ADD_ACTION( toolbar, "Ellipse", add_ellipse() );
	ADD_ACTION( toolbar, "Polygon", add_polygon() );
}

void ricfileEditorAdvanced::change_file( openRicfile *new_file ){
	file = new_file;
	model.change_file( &file->ric() );
	parameters.change_file( &file->ric() );
	update_preview();

	//TODO: store selection position
	ricfile_selection_model->select( QItemSelection( model.index( 0, 0 ), model.index( 0, 2 ) ), QItemSelectionModel::Select );
	update_selection();
}

QToolBar* ricfileEditorAdvanced::editor_toolbar(){
	return toolbar;
}


ricfileEditorAdvanced::~ricfileEditorAdvanced(){ delete ui; }

void ricfileEditorAdvanced::update_selection(){
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


void ricfileEditorAdvanced::update_preview(){
	ui->treeView->update( model.index( 0,0 ) );
	
	if( file )
		file->ric().Draw(&drawing_canvas, 100, 64);
	
	canvas.update();
}


bool ricfileEditorAdvanced::add_object( unsigned int object_type ){
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

void ricfileEditorAdvanced::move_object_up(){
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
void ricfileEditorAdvanced::move_object_down(){
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
void ricfileEditorAdvanced::remove_object(){
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


void ricfileEditorAdvanced::add_options()	{ add_object( ricObject::RIC_OP_OPTIONS ); }
void ricfileEditorAdvanced::add_sprite()	{ add_object( ricObject::RIC_OP_SPRITE ); }
void ricfileEditorAdvanced::add_copybits()	{ add_object( ricObject::RIC_OP_COPYBITS ); }
void ricfileEditorAdvanced::add_varmap()	{ add_object( ricObject::RIC_OP_VARMAP ); }
void ricfileEditorAdvanced::add_pixel()	{ add_object( ricObject::RIC_OP_PIXEL ); }
void ricfileEditorAdvanced::add_line()	{ add_object( ricObject::RIC_OP_LINE ); }
void ricfileEditorAdvanced::add_rectangle()	{ add_object( ricObject::RIC_OP_RECTANGLE ); }
void ricfileEditorAdvanced::add_circle()	{ add_object( ricObject::RIC_OP_CICLE ); }
void ricfileEditorAdvanced::add_number()	{ add_object( ricObject::RIC_OP_NUMBER ); }
void ricfileEditorAdvanced::add_ellipse()	{ add_object( ricObject::RIC_OP_ELLIPSE ); }
void ricfileEditorAdvanced::add_polygon()	{ add_object( ricObject::RIC_OP_POLYGON ); }


