#ifndef RICOBJECT_POINT
#define RICOBJECT_POINT

#include "../../riclib/ricObject.h"
#include "ricobject_abstract.h"

class ricobject_point: public ricobject_abstract{
	Q_OBJECT
	
	private:
		//The objects
	//	ric_value options;
		ric_value posx;
		ric_value posy;
	
	public:
		ricobject_point( QWidget *parent = 0 );
		
		bool change_object( ricfile::ricObject* new_object );
};

#endif
