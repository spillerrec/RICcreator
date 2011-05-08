#ifndef RIC_VALUE_H
#define RIC_VALUE_H

#include <QWidget>
#include "../riclib/nxtVariable.h"


class ric_value: public QWidget{
	Q_OBJECT
	
	private:
		class Ui_ric_value_select *ui;
		ricfile::nxtVarRicWord* ricword;
		
	
	private:
		void read();
		void write();
		void refresh_mode();
	
	public:
		explicit ric_value( QWidget* parent, QString text, ricfile::nxtVarRicWord* value_object, QString tooltip = "" );
	
	
	private slots:
		void update(){
			
		}
		void update_mode();
	
	signals:
		void value_changed();
};

#endif