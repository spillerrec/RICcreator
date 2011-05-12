#ifndef RICOBJECT_RECT
#define RICOBJECT_RECT

#include "../../riclib/ricObject.h"
#include "ricobject_abstract.h"

class ricobject_rect: public ricobject_abstract{
	Q_OBJECT
	
	private:
		//The objects
	//	ric_value options;
		ric_value posx;
		ric_value posy;
		ric_value width;
		ric_value height;
	
	public:
		ricobject_rect( QWidget *parent = 0 );
		
		bool change_object( ricfile::ricObject* new_object );
};

#endif
