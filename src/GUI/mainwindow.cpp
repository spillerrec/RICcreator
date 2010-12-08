#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>

#include <QtGui/QApplication>

#include <QPoint>
#include <QPixmap>

#include <QString>
#include <QFileDialog>

#include "src/riclib/nxtCanvas.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
	image(100,64,QImage::Format_Mono),
	model( &graphics )
{
    ui->setupUi(this);
//	connect( ui->value_a, SIGNAL(valueChanged(double)), this, SLOT(on_equation_changed()) );	//Example

	connect( ui->action_Exit, SIGNAL(triggered()), this, SLOT( exit() ) );
	connect( ui->action_Open, SIGNAL(triggered()), this, SLOT( open_file() ) );
	connect( ui->action_Save, SIGNAL(triggered()), this, SLOT( save_file() ) );
	connect( ui->action_New, SIGNAL(triggered()), this, SLOT( new_file() ) );
	
	
	ui->canvas->setScene(&scene);
	ui->canvas->setSceneRect(0,0, 100, 64 );
	ui->canvas->scale( 2,2 );
	
	
	ui->listView->setModel( &model );
//	ui->treeView->setModel( &model );
	
	update_preview();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::exit(){
	QApplication::exit(0);
}


void MainWindow::open_file(){
	QString filename = QFileDialog::getOpenFileName(this, tr("Open RIC file"), "", tr("RIC files (*.ric)") );
	
	if( !filename.isNull() ){
		graphics.readfile( filename.toLocal8Bit().data() );
	
		emit update_preview();
		model.update();
	}
}

void MainWindow::save_file(){
	
	QString filename = QFileDialog::getSaveFileName( this, tr("Save RIC file"), "", tr("RIC files (*.ric)") );
	
	if( !filename.isNull() )
		graphics.writefile( filename.toLocal8Bit().data() );
	
}

void MainWindow::new_file(){
	graphics.Reset();
	
	emit update_preview();
	model.update();
}


void MainWindow::update_preview(){
	ui->listView->update( model.index( 0 ) );
	
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




