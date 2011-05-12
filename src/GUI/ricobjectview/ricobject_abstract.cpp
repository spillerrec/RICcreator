#include "ricobject_abstract.h"

ricobject_abstract::ricobject_abstract( QWidget *parent ): QWidget( parent ), layout( QBoxLayout::TopToBottom, parent ){
	setLayout( &layout );
}



