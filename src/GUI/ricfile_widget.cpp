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
	
	connect( &parameters, SIGNAL(dataChanged( QModelIndex, QModelIndex )), this, SLOT( update_preview() ) );
	
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
	if( connect( ricfile_selection_model, SIGNAL(selectionChanged( QItemSelection, QItemSelection )), this, SLOT( update_selection() ) ) ){
	
		ui->properties_box->setTitle( "Connection made" );
	}
	
	//Some quick testcode
	ricfile::nxtVarRicWord* ricword = new ricfile::nxtVarRicWord( NULL );
	//ricword->set_extended( 3, 24 );
	ricword->set_normal( 200 );
	new ric_value( ui->properties_box, "test", ricword, "something" );
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
				switch( object->object_type() ){
					case ricfile::ricObject::RIC_OP_OPTIONS: ui->properties_box->setTitle( "Options" ); break;
					case ricfile::ricObject::RIC_OP_SPRITE: ui->properties_box->setTitle( "Sprite" ); break;
					case ricfile::ricObject::RIC_OP_VARMAP: ui->properties_box->setTitle( "VarMap" ); break;
					case ricfile::ricObject::RIC_OP_COPYBITS: ui->properties_box->setTitle( "CopyBits" ); break;
					case ricfile::ricObject::RIC_OP_PIXEL: ui->properties_box->setTitle( "Pixel" ); break;
					case ricfile::ricObject::RIC_OP_LINE: ui->properties_box->setTitle( "Line" ); break;
					case ricfile::ricObject::RIC_OP_RECTANGLE: ui->properties_box->setTitle( "Rectangle" ); break;
					case ricfile::ricObject::RIC_OP_CICLE: ui->properties_box->setTitle( "Circle" ); break;
					case ricfile::ricObject::RIC_OP_NUMBER: ui->properties_box->setTitle( "Number" ); break;
					case ricfile::ricObject::RIC_OP_ELLIPSE: ui->properties_box->setTitle( "Ellipse" ); break;
					case ricfile::ricObject::RIC_OP_POLYGON: ui->properties_box->setTitle( "Polygon" ); break;
					default: ui->properties_box->setTitle( "Unknown element" ); break;
				}
				
				return;
			}
			
			ui->properties_box->setTitle( "Wrong index amount" );
			return;
		}
		ui->properties_box->setTitle( "Wrong amount of indexes" );
		return;
	}
	
	
	ui->properties_box->setTitle( "No Selection" );
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
		
		model.update();
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
	
	model.update();
	emit update_preview();
}