#ifndef RICOBJECT_DESCRIPTION
#define RICOBJECT_DESCRIPTION

#include "../../riclib/ricObject.h"
#include "ricobject_abstract.h"

class ricobject_description: public ricobject_abstract{
	Q_OBJECT
	
	private:
		//The objects
	//	ric_value options;
		ric_value width;
		ric_value height;
	
	public:
		ricobject_description( QWidget *parent = 0 );
		
		bool change_object( ricfile::ricObject* new_object );
};

#endif
