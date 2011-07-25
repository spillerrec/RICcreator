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

#ifndef IMPORTIMAGEDIALOG_H
#define IMPORTIMAGEDIALOG_H

#include <QDialog>
#include <QString>

class nxtCanvas;
class nxtCanvasWidget;

class importImageDialog: public QDialog{
	Q_OBJECT
	
	private:
		class Ui_import_image_dialog *ui;
	
	private:
		nxtCanvas *bitmap;
		nxtCanvasWidget *bitmap_sub_widget;
		QImage *org_image;
		QImage *gray_image;
	
	public:
		explicit importImageDialog( QWidget *parent );
		~importImageDialog();
		
		nxtCanvas* get_canvas();
		static void export_canvas( nxtCanvas* canvas, QString filepath );
	
	private:
		void create_gray_image();
		void create_bitmap();
	
	private slots:
		bool change_image();
		void desaluration_method_changed();
		void desaluration_level_changed( int new_level );
		void width_changed( int value );
		void height_changed( int value );
		
		void action_ok();
};

#endif