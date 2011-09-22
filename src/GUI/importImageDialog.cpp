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
	-	Display "open" dialog when the dialog shows and reject if the user cancels
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

static double round_sym( double r ){
	return ( r > 0.0 ) ? floor( r + 0.5 ) : ceil( r - 0.5 );
}

/*
	Exports "canavs" to png and saves it at the location specified in "filepath"
*/
void importImageDialog::export_canvas( nxtCanvas* canvas, QString filepath ){
	if( !canvas || filepath.isEmpty() )
		return;
	
	QImage img( canvas->get_width(), canvas->get_height(), QImage::Format_Mono );
	//TODO: check which format should be used, QImage::Format_MonoLSB
	//This is MSB, the other is LSB
	//If nxtCanvas also is changed, we could just dump the memory directly, both
	//here and when writing RIC files
	//Or at least dump each line...
	img.fill( 1 );
	for( unsigned int ix=0; ix<canvas->get_width(); ix++ )
		for( unsigned int iy=0; iy<canvas->get_height(); iy++ )
			if( canvas->get_pixel( ix, iy ) )
				img.setPixel( ix, canvas->get_height()-iy-1, 0 );	//set black
	
	img.save( filepath, "PNG" );
}



importImageDialog::importImageDialog( QWidget *parent ): QDialog( parent ), ui(new Ui_import_image_dialog){
	ui->setupUi(this);
	
	//Initialize members
	org_image = NULL;
	scaled_image = NULL;
	bitmap = NULL;
	bitmap_sub_widget = NULL;
	gray_average = 0;
	
	
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
	connect( ui->desaluration_level, SIGNAL( valueChanged(int) ), this, SLOT( create_bitmap() ) );
	connect( ui->sbx_adp_size, SIGNAL( valueChanged(int) ), this, SLOT( create_bitmap() ) );
	connect( ui->sbx_adp_c, SIGNAL( valueChanged(int) ), this, SLOT( create_bitmap() ) );
	connect( ui->sbx_adp_max, SIGNAL( valueChanged(int) ), this, SLOT( create_bitmap() ) );
	connect( ui->sbx_adp_min, SIGNAL( valueChanged(int) ), this, SLOT( create_bitmap() ) );
	connect( ui->tabs_method, SIGNAL( currentChanged(int) ), this, SLOT( create_bitmap() ) );
	
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
	if( scaled_image )
		delete scaled_image;
	if( bitmap )
		delete bitmap;
}


unsigned int importImageDialog::make_gray( QImage *image ){
	if( !image )
		return 0;
	
	//Convert to grayscale while calculating average
	int width = image->width();
	int height = image->height();
	unsigned long average = 0;
	for( int iy = 0; iy < height; iy++ ){
		QRgb *data = (QRgb*)image->scanLine( iy );
		unsigned long horizontial_average = 0;
		
		for( int ix = 0; ix < width; ix++ ){
			int gray = qGray( *data );
			
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
	
	return average;
}

void importImageDialog::create_scaled_image(){
	if( !org_image )
		return;
	
	//Create and scale the image
	if( scaled_image )
		delete scaled_image;
	scaled_image = new QImage( org_image->scaled( ui->scale_width->value(), ui->scale_height->value() ) );
	
	
	//Display image
	ui->image->setPixmap( QPixmap::fromImage( *scaled_image ) );
	
	create_bitmap();
}


void importImageDialog::create_bitmap(){
	if( !scaled_image )
		return;
	
	bitmap->create( scaled_image->width(), scaled_image->height() );
	
	int width = scaled_image->width();
	int height = scaled_image->height();
	
	
	if( ui->tabs_method->currentIndex() == 1 ){
		//Use adaptive threeshoulding
		int size = ui->sbx_adp_size->value();
		int c = ui->sbx_adp_c->value();
		int min = ui->sbx_adp_min->value();
		int max = ui->sbx_adp_max->value();
		
		//Prevent anything weird...
		if( min > max ){
			min = max;
			ui->sbx_adp_min->setValue( min );
			return; //This should be recalled since the spinbox changed.
		}
		
		if( size < 3 ){
			size = 3;
			qDebug( "Size is too small!!!" );
		}
		
		//Get offsets from center point
		int ur_corner = size / 2;
		int ll_corner = size - ur_corner;
		
		ur_corner--;
		ll_corner--;
		
		for( int iy = 0; iy < height; iy++ ){
			QRgb *data = (QRgb*)scaled_image->scanLine( height-iy-1 );
			
			for( int ix = 0; ix < width; ix++ ){
				
				//TODO: Calculate threeshould
				//Get rectangle size
				int left, top, right, bottom;
				left = top = ur_corner;
				right = bottom = ll_corner;
				if( ix - left < 0 )
					left = ix;
				if( iy - top < 0 )
					top = iy;
				if( ix + right >= width )
					right = width - ix;
				if( iy + bottom >= height )
					bottom = height - iy;
				
				//Collect data
				int threeshould = 0;
				for( int jy = -top; jy <= bottom; jy++ ){
					QRgb *local = data-left - jy*width;
					for( int jx = -left; jx <= right; jx++ ){
						threeshould += qBlue( *local );
						local++;
					}
				}
				
				//Calculate average
				threeshould /= (left+right)*(top+bottom);
				if( threeshould < min )
					threeshould = min;
				if( threeshould > max )
					threeshould = max;
				
				//Apply threeshould and move on
				if( qBlue( *data ) < threeshould - c )
					bitmap->PointOut( ix, iy );
				data++;
			}
		}
		
	}
	else{
		//Use global threeshould
		int threeshoul = ui->desaluration_level->value();
		
		for( int iy = 0; iy < height; iy++ ){
			QRgb *data = (QRgb*)scaled_image->scanLine( height-iy-1 );
			
			for( int ix = 0; ix < width; ix++ ){
				if( qBlue( *data ) < threeshoul )
					bitmap->PointOut( ix, iy );
				data++;
			}
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
	
	//Convert image to grayscale
	if( scaled_image )
		delete scaled_image;
	scaled_image = NULL;
	gray_average = make_gray( org_image );
	ui->desaluration_level->setValue( gray_average );
	
	//Update text field
	ui->label_size->setText( tr( "Original: " ) + QString::number(org_image->width()) + "x" + QString::number(org_image->height()) );
	
	//Change width, this updates height and gray image
	int width = org_image->width();
	if( width > 200 )	//Autoscale if enormious
		width = 200;
	width_changed( width );	//Updates the height and scales the image
	
	//TODO: reset bitmap_sub_widget
	
	return true;
}




void importImageDialog::width_changed( int value ){
	if( !org_image )
		return;
	
	if( value <= 0 )	//Don't handle invalid values
		return;
	
	int height = round_sym( org_image->height() * (double)value/org_image->width() );
	if( ui->scale_height->value() != height )
		ui->scale_height->setValue( height );
	else
		create_scaled_image();
}


void importImageDialog::height_changed( int value ){
	if( !org_image )
		return;
	
	if( value <= 0 )	//Don't handle invalid values
		return;
	
	int width = round_sym( org_image->width() * (double)value/org_image->height() );
	if( ui->scale_width->value() != width )
		ui->scale_width->setValue( width );
	else
		create_scaled_image();
}


void importImageDialog::action_ok(){
	//TODO: check selction
	accept();
}

nxtCanvas* importImageDialog::get_canvas(){
	QRect area = bitmap_sub_widget->get_selection();
	if( !area.isNull() ){
		nxtCanvas *copied = new nxtCanvas;
		copied->create( area.width(), area.height() );
		copied->copy_canvas( bitmap, area.x(), area.y(), area.width(), area.height(), 0,0 );
		return copied;
	}
	
	return NULL;
}

