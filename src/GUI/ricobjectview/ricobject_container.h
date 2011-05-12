#ifndef RICOBJECT_CONTAINER
#define RICOBJECT_CONTAINER

#include <QStackedLayout>
#include "../../riclib/ricObject.h"

#include "ricobject_description.h"
#include "ricobject_point.h"
#include "ricobject_line.h"
#include "ricobject_rect.h"
#include "ricobject_circle.h"
#include "ricobject_ellipse.h"

class ricobject_container: public QStackedLayout{
	Q_OBJECT
	
	private:
		ricobject_description* ric_description;
		ricobject_point* ric_point;
		ricobject_line* ric_line;
		ricobject_rect* ric_rect;
		ricobject_circle* ric_circle;
		ricobject_ellipse* ric_ellipse;
	
	public:
		ricobject_container( QWidget *parent = 0 );
		
		void view_object( ricfile::ricObject* object );
	
	signals:
		void object_changed();
};

#endif
