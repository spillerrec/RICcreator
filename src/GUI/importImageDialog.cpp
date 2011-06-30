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


/*	TODO:
	-	Add adaptive threeshoulding
	-	Display "open" dialog when the dialog shows and reject if the user cancels
	-	Copy the canvas
	-	Add perferences
	
	Future TODOs:
	-	Add more advanced image filters
	-	Add help?
*/


#include "ui_importImageDialog.h"
#include "importImageDialog.h"

#include "../riclib/nxtCanvas.h"
#include "nxtCanvasWidget.h"
#include "nxtCanvasWidgetContainer.h"

#include <QImage>
#include <QFileDialog>
#include <QSizePolicy>
#include <math.h>


importImageDialog::importImageDialog( QWidget *parent ): QDialog( parent ), ui(new Ui_import_image_dialog){
	ui->setupUi(this);
	
	//Initialize members
	org_image = NULL;
	gray_image = NULL;
	bitmap = NULL;
	bitmap_sub_widget = NULL;
	
	
	//Add nxtCanvasWidgetContainer to the dialog
	bitmap = new nxtCanvas;
	bitmap_sub_widget = new nxtCanvasWidget( this );
	bitmap_sub_widget->change_object( bitmap );
	bitmap_sub_widget->set_tool( nxtCanvasWidget::TOOL_SELECTION );
	
	nxtCanvasWidgetContainer *bitmap_widget = new nxtCanvasWidgetContainer( bitmap_sub_widget, true, (QWidget*)this );
	ui->main_layout->insertWidget( 1, (QWidget*)bitmap_widget );
	
	//Image widgets
	connect( ui->change_image, SIGNAL( clicked() ), this, SLOT( change_image() ) );
	
	//Desaluration widgets
	connect( ui->convert_desaturation, SIGNAL( toggled(bool) ), this, SLOT( desaluration_method_changed() ) );
	connect( ui->convert_gray, SIGNAL( toggled(bool) ), this, SLOT( desaluration_method_changed() ) );
	connect( ui->convert_luminosity, SIGNAL( toggled(bool) ), this, SLOT( desaluration_method_changed() ) );
	connect( ui->desaluration_level, SIGNAL( valueChanged(int) ), this, SLOT( desaluration_level_changed(int) ) );
	
	//DialogBoxButtons
	connect( ui->dialog_buttons, SIGNAL( rejected() ), this, SLOT( reject() ) );
	connect( ui->dialog_buttons, SIGNAL( accepted() ), this, SLOT( action_ok() ) );
	
	//Image scaling spinboxes
	connect( ui->scale_height, SIGNAL( valueChanged(int) ), this, SLOT( height_changed(int) ) );
	connect( ui->scale_width, SIGNAL( valueChanged(int) ), this, SLOT( width_changed(int) ) );
	
	
	//Now, open a image
	//change_image();	//TODO: wrong place?
	//TODO: reject dialog if user quits
}


importImageDialog::~importImageDialog(){
	delete ui;
	
	if( org_image )
		delete org_image;
	if( gray_image )
		delete gray_image;
	if( bitmap )
		delete bitmap;
}


void importImageDialog::create_gray_image(){
	if( !org_image )
		return;
	
	//Create and scale the image
	if( gray_image )
		delete gray_image;
	gray_image = new QImage( org_image->scaled( ui->scale_width->value(), ui->scale_height->value() ) );
	
	//Get conversion method
	int method = 0;
	if( ui->convert_gray->isChecked() )
		method = 1;
	else if( ui->convert_luminosity->isChecked() )
		method = 2;
	else if( ui->convert_desaturation->isChecked() )
		method = 3;
	else
		qDebug( "importImageDialog::create_gray_image(): invalid conversion method!!!" );
	
	//Convert to grayscale while calculating average
	int width = gray_image->width();
	int height = gray_image->height();
	unsigned long average = 0;
	for( int iy = 0; iy < height; iy++ ){
		QRgb *data = (QRgb*)gray_image->scanLine( iy );
		unsigned long horizontial_average = 0;
		
		for( int ix = 0; ix < width; ix++ ){
			int gray;
			//Select conversion method
			switch( method ){
				case 1:	//Grayscale
						gray = qGray( *data );
					break;
					
				case 2:	//Luminanace
						gray = sqrt( 0.241*qRed(*data)*qRed(*data) + 0.691*qGreen(*data)*qGreen(*data) + 0.068*qBlue(*data)*qBlue(*data) );
					break;
				
				case 3:	//Desalurate
						QColor temp = *data;
						gray = temp.value();
					break;
			}
			
			//Use white background for alpha
			gray += (255-gray) * (255-qAlpha( *data )) / 255;
		//	gray -= gray * (255-qAlpha( *data )) / 255;	//Black background
			
			*data = qRgba( gray, gray, gray, 255 );	//Write gray value
			data++;	//Go to next element
			
			//Calculate average
			horizontial_average += gray;
		}
		average += horizontial_average / width;
	}
	average = average / height;
	
	//Display image
	ui->image->setPixmap( QPixmap::fromImage( *gray_image ) );
	
	ui->desaluration_level->setValue( average );
	
	create_bitmap();	//TODO: unnesesarry?
}


void importImageDialog::create_bitmap(){
	if( !gray_image )
		return;
	
	bitmap->create( gray_image->width(), gray_image->height() );
	
	int width = gray_image->width();
	int height = gray_image->height();
	int threeshoul = ui->desaluration_level->value();
	
	for( int iy = 0; iy < height; iy++ ){
		QRgb *data = (QRgb*)gray_image->scanLine( height-iy-1 );
		
		for( int ix = 0; ix < width; ix++ ){
			if( qBlue( *data ) < threeshoul )
				bitmap->PointOut( ix, iy );
			data++;
		}
	}
	
	bitmap_sub_widget->select_all();
	bitmap_sub_widget->update();
}


bool importImageDialog::change_image(){
	if( org_image )
		delete org_image;
	
	QString filename = QFileDialog::getOpenFileName(this, tr("Open image file"), "", tr("Images (*.bmp *gif *.jpg *.jpeg *.png *.tif)") );
	if( filename.isEmpty() )
		return false;
	//TODO: open at default location
	
	org_image = new QImage( QImage( filename ).convertToFormat( QImage::Format_ARGB32 ) );
	if( org_image->isNull() ){
		//TODO: show error
		return change_image();	//Starts this dialog again, but exits this one
	}
	
	//Update text field
	ui->label_size->setText( tr( "Original: " ) + QString::number(org_image->width()) + "x" + QString::number(org_image->height()) );
	
	//Change width, this updates height and gray image
	width_changed( org_image->width() );
	
	//TODO: reset bitmap_sub_widget
	
	return true;
}


void importImageDialog::desaluration_method_changed(){
	create_gray_image();
}


void importImageDialog::desaluration_level_changed( int new_level ){
	create_bitmap();
}


void importImageDialog::width_changed( int value ){
	if( !org_image )
		return;
	
	if( value <= 0 )	//Don't handle invalid values
		return;
	
	int height = round( org_image->height() * (double)value/org_image->width() );
	if( ui->scale_height->value() != height )
		ui->scale_height->setValue( height );
	else
		create_gray_image();
}


void importImageDialog::height_changed( int value ){
	if( !org_image )
		return;
	
	if( value <= 0 )	//Don't handle invalid values
		return;
	
	int width = round( org_image->width() * (double)value/org_image->height() );
	if( ui->scale_width->value() != width )
		ui->scale_width->setValue( width );
	else
		create_gray_image();
}


void importImageDialog::action_ok(){
	//TODO: check selction
	accept();
}

nxtCanvas* importImageDialog::get_canvas(){
	//TODO: copy the selected area
	return NULL;
}

