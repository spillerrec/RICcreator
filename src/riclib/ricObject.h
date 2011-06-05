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
	ricObject (abstract) class and child classes
	
	ricObject is an abstract base class for RIC file opcodes.
	
	The following methods must be implemented:
		read(...)	Read the opcode contents from a byte array
		write(...)	Write the opcode to an open file
		filesize(...)	return the size of the opcode when written to a file
		object_type(...)	return the opcode ID
		draw(...)	draw the opcode to a nxtCanvas
*/
#ifndef RICOBJECT_H
#define RICOBJECT_H

#include "ricfile.h"

#include <iostream>
#include <fstream>
using namespace std;

class nxtVariable;
class nxtCanvas;

class ricfile::ricObject{
	//Type constants
	public:
		enum object_type{
			RIC_OP_OPTIONS,
			RIC_OP_SPRITE,
			RIC_OP_VARMAP,
			RIC_OP_COPYBITS,
			RIC_OP_PIXEL,
			RIC_OP_LINE,
			RIC_OP_RECTANGLE,
			RIC_OP_CICLE,
			RIC_OP_NUMBER,
			RIC_OP_ELLIPSE,
			RIC_OP_POLYGON
		};
	
	protected:
		ricfile* pRIC;
		unsigned int var_count;
		nxtVariable** vars;
	
	public:
		ricObject( ricfile *container, unsigned int var_amount ){
			pRIC = container;
			var_count = var_amount;
			vars = new nxtVariable* [ var_count ];
		}
		~ricObject(){ delete vars; }
		
		void read(ifstream* file);
		int write(ofstream* file);
		unsigned int filesize() const;
		
		virtual unsigned int object_type() const = 0;
		virtual void draw(nxtCanvas* canvas) const{ return; }
		ricfile* parent() const{ return pRIC; }
		
		virtual nxtVariable& get_setting( unsigned int index ) = 0;
		virtual unsigned int setting_amount() const = 0;
};



#endif

