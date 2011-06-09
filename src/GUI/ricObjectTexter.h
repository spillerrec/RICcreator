/*
	This file is part of RICcreator.

	RICcreator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RICcreator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RICcreator.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
	ricObjectTexter provides human-readable names/descriptions for ricObject types and properties.
	Use this class whenever possible as it will simplify translations. All members are static.
	
	Provides strings for:
		names for ricObject opcodes
		descriptions for ricObject opcodes
		names for ricObject properties
		descriptions for ricObject properties
	
	Provides info about:
		amount of ricObject opcodes (implemented)
		amount of ricObject properties (implemented)
*/

#ifndef RICOBJECTTEXTER_H
#define RICOBJECTTEXTER_H

#include <QString>
#include "../riclib/ricObject.h"

class nxtVariable;

class ricObjectTexter{
	public:
		static ricObject* object( ricObject::object_op opcode );
		static unsigned int object_amount();
		static QString object_name( ricObject::object_op opcode );
		static QString object_description( ricObject::object_op opcode );
		
		static unsigned int object_property_amount( ricObject::object_op opcode );
		static QString object_property_name( ricObject::object_op opcode, unsigned int index );
		static QString object_property_description( ricObject::object_op opcode, unsigned int index );
		
		static QString nxtVarToStr( nxtVariable* var );
	
};

#endif