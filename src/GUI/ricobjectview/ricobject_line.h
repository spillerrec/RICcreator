#ifndef RICOBJECT_LINE
#define RICOBJECT_LINE

#include "../../riclib/ricObject.h"
#include "ricobject_abstract.h"

class ricobject_line: public ricobject_abstract{
	Q_OBJECT
	
	private:
		//The objects
		copyoptions_value options;
		ric_value posx1;
		ric_value posy1;
		ric_value posx2;
		ric_value posy2;
	
	public:
		ricobject_line( QWidget *parent = 0 );
		
		bool change_object( ricfile::ricObject* new_object );
};

#endif
