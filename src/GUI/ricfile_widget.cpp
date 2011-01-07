#include "ricfile_widget.h"
#include "ui_ricfile.h"

#include <QPoint>
#include <QPixmap>

#include <QString>

#include "src/riclib/nxtCanvas.h"

ricfile_widget::ricfile_widget( QString filename, QWidget *parent ): QWidget(parent), ui(new Ui_Form), image(100,64,QImage::Format_Mono), model( &graphics ){
    ui->setupUi(this);
	
	//Add stuff
	//tabCloseRequested
	
	ui->canvas->setScene(&scene);
	ui->canvas->setSceneRect(0,0, 100, 64 );
	ui->canvas->scale( 2,2 );
	
	
	ui->treeView->setModel( &model );
	if( !filename.isEmpty() ){
		open_file( filename );
		original = false;
	}
	else
		original = true;
	
	edited = false;
	update_preview();
}

ricfile_widget::~ricfile_widget(){ delete ui; }


bool ricfile_widget::replaceable(){
	if( original && !edited )
		return true;
	else
		return false;
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
		
		model.update();
		emit update_preview();
	}
}

void ricfile_widget::save_file( QString filename ){
	if( !filename.isEmpty() )
		graphics.writefile( filename.toLocal8Bit().data() );
}

void ricfile_widget::reset(){
	graphics.Reset();
}