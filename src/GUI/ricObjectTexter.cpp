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


#include "ricObjectTexter.h"
#include "../riclib/nxtVariable.h"
#include "../riclib/nxtVarRicWord.h"
#include <QObject>

unsigned int object_amount(){ return 11; }

QString ricObjectTexter::object_name( ricObject::object_op opcode ){
	switch( opcode ){
		case ricObject::RIC_OP_OPTIONS:	return QObject::tr( "Options", "ricObject" );
		case ricObject::RIC_OP_SPRITE:	return QObject::tr( "Sprite", "ricObject" );
		case ricObject::RIC_OP_VARMAP:	return QObject::tr( "VarMap", "ricObject" );
		case ricObject::RIC_OP_COPYBITS:	return QObject::tr( "CopyBits", "ricObject" );
		case ricObject::RIC_OP_PIXEL:	return QObject::tr( "Pixel", "ricObject" );
		case ricObject::RIC_OP_LINE:	return QObject::tr( "Line", "ricObject" );
		case ricObject::RIC_OP_RECTANGLE:	return QObject::tr( "Rectangle", "ricObject" );
		case ricObject::RIC_OP_CICLE:	return QObject::tr( "Circle", "ricObject" );
		case ricObject::RIC_OP_NUMBER:	return QObject::tr( "Number", "ricObject" );
		case ricObject::RIC_OP_ELLIPSE:	return QObject::tr( "Ellipse", "ricObject" );
		case ricObject::RIC_OP_POLYGON:	return QObject::tr( "Polygon", "ricObject" );
		default:	return QObject::tr( "Unknown element", "ricObject" );
	}
}


QString ricObjectTexter::object_description( ricObject::object_op opcode ){
	switch( opcode ){
		case ricObject::RIC_OP_OPTIONS:	return QObject::tr( "Meta-data?", "ricObject" );
		case ricObject::RIC_OP_SPRITE:	return QObject::tr( "Contains an image that can be displayed with CopyBits", "ricObject" );
		case ricObject::RIC_OP_VARMAP:	return QObject::tr( "Simple function implementation which can be used to scale paramters", "ricObject" );
		case ricObject::RIC_OP_COPYBITS:	return QObject::tr( "Copy a Sprite to the screen", "ricObject" );
		case ricObject::RIC_OP_PIXEL:	return QObject::tr( "Draws a pixel", "ricObject" );
		case ricObject::RIC_OP_LINE:	return QObject::tr( "Draws a line", "ricObject" );
		case ricObject::RIC_OP_RECTANGLE:	return QObject::tr( "Draws a rectangle", "ricObject" );
		case ricObject::RIC_OP_CICLE:	return QObject::tr( "Draws a circle", "ricObject" );
		case ricObject::RIC_OP_NUMBER:	return QObject::tr( "Draws a number", "ricObject" );
		case ricObject::RIC_OP_ELLIPSE:	return QObject::tr( "Draws a ellipse", "ricObject" );
		case ricObject::RIC_OP_POLYGON:	return QObject::tr( "Draws a polygon", "ricObject" );
		default:	return QObject::tr( "Unknown element", "ricObject" );
	}
}


unsigned int ricObjectTexter::object_property_amount( ricObject::object_op opcode ){
	switch( opcode ){
		case ricObject::RIC_OP_OPTIONS:	return 3;
		case ricObject::RIC_OP_SPRITE:	return 2;
		case ricObject::RIC_OP_VARMAP:	return 2;
		case ricObject::RIC_OP_COPYBITS:	return 4;
		case ricObject::RIC_OP_PIXEL:	return 3;
		case ricObject::RIC_OP_LINE:	return 3;
		case ricObject::RIC_OP_RECTANGLE:	return 2;
		case ricObject::RIC_OP_CICLE:	return 3;
		case ricObject::RIC_OP_NUMBER:	return 3;
		case ricObject::RIC_OP_ELLIPSE:	return 4;
		case ricObject::RIC_OP_POLYGON:	return 2;
		default:	return 0;
	}
}


QString ricObjectTexter::object_property_name( ricObject::object_op opcode, unsigned int index ){
	switch( opcode ){
		case ricObject::RIC_OP_OPTIONS:
			switch( index ){
				case 0: return QObject::tr( "Options", "ricObject property" );
				case 1: return QObject::tr( "Width", "ricObject property" );
				case 2: return QObject::tr( "Height", "ricObject property" );
			} break;
		case ricObject::RIC_OP_SPRITE:
			switch( index ){
				case 0: return QObject::tr( "ID", "ricObject property" );
				case 1: return QObject::tr( "Sprite", "ricObject property" );
			} break;
		case ricObject::RIC_OP_VARMAP:
			switch( index ){
				case 0: return QObject::tr( "ID", "ricObject property" );
				case 1: return QObject::tr( "Values", "ricObject property" );
			} break;
		case ricObject::RIC_OP_COPYBITS:
			switch( index ){
				case 0: return QObject::tr( "Copy options", "ricObject property" );
				case 1: return QObject::tr( "Sprite ID", "ricObject property" );
				case 2: return QObject::tr( "Source area", "ricObject property" );
				case 3: return QObject::tr( "Destination", "ricObject property" );
			} break;
		case ricObject::RIC_OP_PIXEL:
			switch( index ){
				case 0: return QObject::tr( "Copy options", "ricObject property" );
				case 1: return QObject::tr( "Position", "ricObject property" );
				case 2: return QObject::tr( "Value", "ricObject property" );
			} break;
		case ricObject::RIC_OP_LINE:
			switch( index ){
				case 0: return QObject::tr( "Copy options", "ricObject property" );
				case 1: return QObject::tr( "Point 1", "ricObject property" );
				case 2: return QObject::tr( "Point 2", "ricObject property" );
			} break;
		case ricObject::RIC_OP_RECTANGLE:
			switch( index ){
				case 0: return QObject::tr( "Copy options", "ricObject property" );
				case 1: return QObject::tr( "Rectangle", "ricObject property" );
			} break;
		case ricObject::RIC_OP_CICLE:
			switch( index ){
				case 0: return QObject::tr( "Copy options", "ricObject property" );
				case 1: return QObject::tr( "Center", "ricObject property" );
				case 2: return QObject::tr( "Radius", "ricObject property" );
			} break;
		case ricObject::RIC_OP_NUMBER:
			switch( index ){
				case 0: return QObject::tr( "Copy options", "ricObject property" );
				case 1: return QObject::tr( "Position", "ricObject property" );
				case 2: return QObject::tr( "Number", "ricObject property" );
			} break;
		case ricObject::RIC_OP_ELLIPSE:
			switch( index ){
				case 0: return QObject::tr( "Copy options", "ricObject property" );
				case 1: return QObject::tr( "Center", "ricObject property" );
				case 2: return QObject::tr( "Radius X", "ricObject property" );
				case 3: return QObject::tr( "Radius Y", "ricObject property" );
			} break;
		case ricObject::RIC_OP_POLYGON:
			switch( index ){
				case 0: return QObject::tr( "Copy options", "ricObject property" );
				case 1: return QObject::tr( "Points", "ricObject property" );
			} break;
	}

	return QObject::tr( "Unknown property", "ricObject property" );
}


QString ricObjectTexter::object_property_description( ricObject::object_op opcode, unsigned int index ){
	switch( opcode ){
		case ricObject::RIC_OP_OPTIONS:
			switch( index ){
				case 0: return QObject::tr( "Currently only used for descriping RIC fonts", "ricObject property" );
				case 1: return QObject::tr( "Default line-width (vertical text direction)", "ricObject property" );
				case 2: return QObject::tr( "Default line-height", "ricObject property" );
			} break;
		case ricObject::RIC_OP_SPRITE:
			switch( index ){
				case 0: return QObject::tr( "Global indentifier, so it can be used by a CopyBits", "ricObject property" );
				case 1: return QObject::tr( "The image (width will be saved as a multiple of 8)", "ricObject property" );
			} break;
		case ricObject::RIC_OP_VARMAP:
			switch( index ){
				case 0: return QObject::tr( "Global indentifier, so values can reffer to it.", "ricObject property" );
				case 1: return QObject::tr( "A ordered list of points, describing the function.", "ricObject property" );
			} break;
		case ricObject::RIC_OP_COPYBITS:
			switch( index ){
				case 0: return QObject::tr( "The drawing mode which it shall be drawn in", "ricObject property" );
				case 1: return QObject::tr( "The ID of the sprite to use", "ricObject property" );
				case 2: return QObject::tr( "The area on the sprite which shall be copied", "ricObject property" );
				case 3: return QObject::tr( "The place where the area will be copied to.", "ricObject property" );
			} break;
		case ricObject::RIC_OP_PIXEL:
			switch( index ){
				case 0: return QObject::tr( "The drawing mode which it shall be drawn in", "ricObject property" );
				case 1: return QObject::tr( "Position", "ricObject property" );
				case 2: return QObject::tr( "Currently unused", "ricObject property" );
			} break;
		case ricObject::RIC_OP_LINE:
			switch( index ){
				case 0: return QObject::tr( "The drawing mode which it shall be drawn in", "ricObject property" );
				case 1: return QObject::tr( "Draws the line from here", "ricObject property" );
				case 2: return QObject::tr( "Draws the line uptill here", "ricObject property" );
			} break;
		case ricObject::RIC_OP_RECTANGLE:
			switch( index ){
				case 0: return QObject::tr( "The drawing mode which it shall be drawn in", "ricObject property" );
				case 1: return QObject::tr( "Describes the dimentions", "ricObject property" );
			} break;
		case ricObject::RIC_OP_CICLE:
			switch( index ){
				case 0: return QObject::tr( "The drawing mode which it shall be drawn in", "ricObject property" );
				case 1: return QObject::tr( "Position of the midpoint of the circle", "ricObject property" );
				case 2: return QObject::tr( "The distance between the midpoint and the path", "ricObject property" );
			} break;
		case ricObject::RIC_OP_NUMBER:
			switch( index ){
				case 0: return QObject::tr( "The drawing mode which it shall be drawn in", "ricObject property" );
				case 1: return QObject::tr( "Position, will be aligned to multiples of 8 in the y-axis", "ricObject property" );
				case 2: return QObject::tr( "Value to draw", "ricObject property" );
			} break;
		case ricObject::RIC_OP_ELLIPSE:
			switch( index ){
				case 0: return QObject::tr( "The drawing mode which it shall be drawn in", "ricObject property" );
				case 1: return QObject::tr( "Position of the midpoint of the ellipse", "ricObject property" );
				case 2: return QObject::tr( "The distance between the midpoint and the path in X direction.", "ricObject property" );
				case 3: return QObject::tr( "The distance between the midpoint and the path in Y direction.", "ricObject property" );
			} break;
		case ricObject::RIC_OP_POLYGON:
			switch( index ){
				case 0: return QObject::tr( "The drawing mode which it shall be drawn in", "ricObject property" );
				case 1: return QObject::tr( "The points descripting the polygon", "ricObject property" );
			} break;
	}

	return QObject::tr( "Unknown property", "ricObject property" );
}


QString ricObjectTexter::nxtVarToStr( nxtVariable* var ){
	if( var )
		switch( var->var_type() ){
			case nxtVariable::TYPE_UWORD: return QString::number( ((nxtVarWord*)var)->value() );
			
			case nxtVariable::TYPE_RIC_WORD:{
					nxtVarRicWord* variable = (nxtVarRicWord*)var;
					
					if( variable->is_extended() ){
						QString display;
						if( variable->get_varmap() ){
							display = QObject::tr( "V%1 ", "nxtVarToStr" );
							display.arg( QString::number( variable->get_varmap() ) );
						}
						
						display += "P";
						display += QString::number( variable->get_parameter() );
						
						return display;
					}
					else
						return QString::number( variable->value() );
				}
			
			default: return QObject::tr( "unsupported", "nxtVarToStr" );
		}
	else
		return QObject::tr( "error, NULL pointer", "nxtVarToStr" );
}

