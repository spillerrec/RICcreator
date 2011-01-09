#include "ricfile_widget.h"
#include "ui_ricfile.h"

#include <QPoint>
#include <QPixmap>


#include "src/riclib/nxtCanvas.h"

ricfile_widget::ricfile_widget( QString filename, QWidget *parent ): QWidget(parent), ui(new Ui_Form), image(100,64,QImage::Format_Mono), model( &graphics ), parameters( &graphics, this ){
	ui->setupUi(this);
	edited = false;
	
	connect( &parameters, SIGNAL(dataChanged( QModelIndex, QModelIndex )), this, SLOT( update_preview() ) );
	
	//Setup treeview
	ui->treeView->setModel( &model );
	ui->treeView->setColumnWidth( 0, 120 );
	ui->treeView->setColumnWidth( 1, 80 );
	ui->treeView->setColumnWidth( 2, 40 );
	
	//Setup other views
	ui->parameter_view->setModel( &parameters );
	ui->canvas->setScene(&scene);
	ui->canvas->setSceneRect(0,0, 100, 64 );
	ui->canvas->scale( 2,2 );
	
	open_file( filename );	//Try to open an existing file
}

ricfile_widget::~ricfile_widget(){ delete ui; }


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