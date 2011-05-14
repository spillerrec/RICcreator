#ifndef RICOBJECT_ELLIPSE
#define RICOBJECT_ELLIPSE

#include "../../riclib/ricObject.h"
#include "ricobject_abstract.h"

class ricobject_ellipse: public ricobject_abstract{
	Q_OBJECT
	
	private:
		//The objects
		copyoptions_value options;
		ric_value posx;
		ric_value posy;
		ric_value radius_x;
		ric_value radius_y;
	
	public:
		ricobject_ellipse( QWidget *parent = 0 );
		
		bool change_object( ricfile::ricObject* new_object );
};

#endif
