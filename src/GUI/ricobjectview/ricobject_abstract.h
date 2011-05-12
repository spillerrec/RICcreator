#ifndef RICOBJECT_ABSTRACT
#define RICOBJECT_ABSTRACT

#include <QBoxLayout>

#include "../../riclib/ricObject.h"
#include "../ric_value.h"

class ricobject_abstract: public QWidget{
	Q_OBJECT
	
	public:
		ricobject_abstract( QWidget *parent = 0 );
		
		QBoxLayout layout;
		
		virtual bool change_object( ricfile::ricObject* new_object ) = 0;
		
	signals:
		void changed();
};

#endif
