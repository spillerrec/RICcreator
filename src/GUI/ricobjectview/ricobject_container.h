#ifndef RICOBJECT_CONTAINER
#define RICOBJECT_CONTAINER

#include <QStackedLayout>
#include "../../riclib/ricObject.h"

#include "ricobject_description.h"

class ricobject_container: public QStackedLayout{
	Q_OBJECT
	
	private:
		ricobject_description* ric_description;
	
	public:
		ricobject_container( QWidget *parent = 0 );
		
		void view_object( ricfile::ricObject* object );
	
	signals:
		void object_changed();
};

#endif
