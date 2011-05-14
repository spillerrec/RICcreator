#ifndef COPYOPTIONS_VALUE
#define COPYOPTIONS_VALUE

#include <QWidget>
#include "../../riclib/nxtVariable.h"


class copyoptions_value: public QWidget{
	Q_OBJECT
	
	private:
		class Ui_copyoptions *ui;
		ricfile::nxtVarRicCopyoptions* copyoptions;
		
	
	private:
		void read();
	
	public:
		explicit copyoptions_value( ricfile::nxtVarRicCopyoptions* value_object, QWidget* parent=NULL );
		void change_value_object( ricfile::nxtVarRicCopyoptions* new_value_object );
	
	
	private slots:
		void write();
	
	signals:
		void value_changed();
};

#endif