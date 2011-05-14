#ifndef RICOBJECT_CICLE
#define RICOBJECT_CICLE

#include "../../riclib/ricObject.h"
#include "ricobject_abstract.h"

class ricobject_circle: public ricobject_abstract{
	Q_OBJECT
	
	private:
		//The objects
		copyoptions_value options;
		ric_value posx;
		ric_value posy;
		ric_value radius;
	
	public:
		ricobject_circle( QWidget *parent = 0 );
		
		bool change_object( ricfile::ricObject* new_object );
};

#endif
