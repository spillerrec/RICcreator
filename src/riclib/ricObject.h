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
	ricObject is an abstract base class for RIC file opcodes.
	
	The following methods must be implemented:
		object_type(...)	return the opcode ID
		draw(...)	draw the opcode to a nxtCanvas
	All nxtVars must be added to the vars array and the size must be correct!
*/
#ifndef RICOBJECT_H
#define RICOBJECT_H

class ricfile;
class nxtVariable;
class nxtCanvas;

#include "nxtIO.h"

class ricObject{
	//Type constants
	public:
		enum object_op{
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
		ricObject( ricfile *container, unsigned int var_amount );
		~ricObject(){ delete vars; }
		
		nxtIO::LoaderError read( nxtIO* file );
		nxtIO::LoaderError write( nxtIO* file ) const;
		unsigned int filesize() const;
		
		virtual object_op object_type() const = 0;
		virtual void draw(nxtCanvas* canvas) const{ return; }
		ricfile* parent() const{ return pRIC; }
		
		nxtVariable* get_setting( unsigned int index );
		unsigned int setting_amount() const{ return var_count; }
		
		//RICScript
		virtual nxtIO::LoaderError ricscript_read( nxtIO* file ) = 0;
};



#endif

