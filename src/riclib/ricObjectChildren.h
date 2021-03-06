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
#ifndef RICOBJECTCHILDREN_H
#define RICOBJECTCHILDREN_H

#include "nxtVariable.h"
#include "nxtVarRicWord.h"
#include "nxtVarRicCopyOptions.h"
#include "nxtCanvas.h"
#include "pointArray.h"

#include "ricObject.h"



class ricOpOptions: public ricObject{
	public:
		nxtVarWord options;
		nxtVarRicWord width;
		nxtVarRicWord height;
		
	public:
		object_op object_type() const{ return RIC_OP_OPTIONS; }
		nxtIO::LoaderError ricscript_read( nxtIO* file );
		
		ricOpOptions( ricfile *container ): 
				ricObject( container, 3 ), 
				width( this ), 
				height( this )
		{
			vars[0] = &options;
			vars[1] = &width;
			vars[2] = &height;
		}
		
		bool is_ricfont() const{ return options == 32769; }
		void set_ricfont(){ options = 32769; }
};


class ricOpSprite: public ricObject{
	public:
		ricVarId sprite_ID;
		nxtCanvas sprite_data;
		
	public:
		object_op object_type() const{ return RIC_OP_SPRITE; }
		nxtIO::LoaderError ricscript_read( nxtIO* file );
		
		ricOpSprite( ricfile *container ): ricObject( container, 2 ), sprite_ID( this ){
			vars[0] = &sprite_ID;
			vars[1] = &sprite_data;
			
			sprite_ID.autoassign_id();
		}
		
		unsigned int get_ID() const{ return sprite_ID; }
};


class ricOpVarMap: public ricObject{
	public:
		ricVarId VarMapID;
		pointArray VarMap;
		
	public:
		object_op object_type() const{ return RIC_OP_VARMAP; }
		nxtIO::LoaderError ricscript_read( nxtIO* file );
		
		ricOpVarMap( ricfile *container ):
				ricObject( container, 2 ),
				VarMapID( this ),
				VarMap( true )
		{
			vars[0] = &VarMapID;
			vars[1] = &VarMap;
			
			VarMapID.autoassign_id();
		}
		
		unsigned int value( unsigned char x ){ return VarMap.value( x ); }
		unsigned int get_ID() const{ return VarMapID; }
		
};

class ricOpCopyBits: public ricObject{
	public:
		nxtVarRicCopyoptions CopyOptions;
		nxtVarRicWord SpriteID;
		ricvarRect src;
		ricvarPoint dest;
		
	public:
		object_op object_type() const{ return RIC_OP_COPYBITS; }
		void draw(nxtCanvas* canvas) const;
		nxtIO::LoaderError ricscript_read( nxtIO* file );
		
		ricOpCopyBits( ricfile *container ): 
				ricObject( container, 4 ), 
				CopyOptions( this ), 
				SpriteID( this, true ), 
				src( this ), 
				dest( this )
		{
			vars[0] = &CopyOptions;
			vars[1] = &SpriteID;
			vars[2] = &src;
			vars[3] = &dest;
			CopyOptions.enabled_fill_shape = true;
			
			find_sprite();
			fit_sprite();
		}
		
		void find_sprite();
		void fit_sprite();
};



class ricOpPixel: public ricObject{
	public:
		nxtVarRicCopyoptions CopyOptions;
		ricvarPoint pos;
		nxtVarRicWord value;
		
	public:
		object_op object_type() const{ return RIC_OP_PIXEL; }
		void draw(nxtCanvas* canvas) const;
		nxtIO::LoaderError ricscript_read( nxtIO* file );
		
		ricOpPixel( ricfile *container ): 
				ricObject( container, 3 ), 
				CopyOptions( this ), 
				pos( this ), 
				value( this )
			{
			pos.X.set_normal( 10 );
			pos.Y.set_normal( 10 );
			vars[0] = &CopyOptions;
			vars[1] = &pos;
			vars[2] = &value;
		}
		
};


class ricOpLine: public ricObject{
	public:
		nxtVarRicCopyoptions CopyOptions;
		ricvarPoint start;
		ricvarPoint end;
		
	public:
		object_op object_type() const{ return RIC_OP_LINE; }
		void draw(nxtCanvas* canvas) const;
		nxtIO::LoaderError ricscript_read( nxtIO* file );
		
		ricOpLine( ricfile *container ): 
				ricObject( container, 3 ), 
				CopyOptions( this ), 
				start( this ),
				end( this )
			{
			start.X.set_normal( 10 );
			start.Y.set_normal( 10 );
			end.X.set_normal( 20 );
			end.Y.set_normal( 20 );
			vars[0] = &CopyOptions;
			vars[1] = &start;
			vars[2] = &end;
		}
		
};


class ricOpRectangle: public ricObject{
	public:
		nxtVarRicCopyoptions CopyOptions;
		ricvarRect rect;
		
	public:
		object_op object_type() const{ return RIC_OP_RECTANGLE; }
		void draw(nxtCanvas* canvas) const;
		nxtIO::LoaderError ricscript_read( nxtIO* file );
		
		ricOpRectangle( ricfile *container ): 
				ricObject( container, 2 ),
				CopyOptions( this ),
				rect( this )
			{
			rect.pos.X.set_normal( 10 );
			rect.pos.Y.set_normal( 10 );
			rect.width.set_normal( 10 );
			rect.height.set_normal( 5 );
			CopyOptions.enabled_fill_shape = true;
			vars[0] = &CopyOptions;
			vars[1] = &rect;
		}
		
};


class ricOpCircle: public ricObject{
	public:
		nxtVarRicCopyoptions CopyOptions;
		ricvarPoint pos;
		nxtVarRicWord radius;
		
	public:
		object_op object_type() const{ return RIC_OP_CICLE; }
		void draw(nxtCanvas* canvas) const;
		nxtIO::LoaderError ricscript_read( nxtIO* file );
		
		ricOpCircle( ricfile *container ): 
				ricObject( container, 3 ), 
				CopyOptions( this ),
				pos( this ),
				radius( this )
			{
			pos.X.set_normal( 25 );
			pos.Y.set_normal( 25 );
			radius.set_normal( 7 );
			CopyOptions.enabled_fill_shape = true;
			vars[0] = &CopyOptions;
			vars[1] = &pos;
			vars[2] = &radius;
		}
		
};


class ricOpNumber: public ricObject{
	public:
		nxtVarRicCopyoptions CopyOptions;
		ricvarPoint pos;
		nxtVarRicWord number;
		
	public:
		object_op object_type() const{ return RIC_OP_NUMBER; }
		void draw(nxtCanvas* canvas) const;
		nxtIO::LoaderError ricscript_read( nxtIO* file );
		
		ricOpNumber( ricfile *container ): 
				ricObject( container, 3 ),
				CopyOptions( this ),
				pos( this ),
				number( this )
			{
			vars[0] = &CopyOptions;
			vars[1] = &pos;
			vars[2] = &number;
		}
		
};


class ricOpEllipse: public ricObject{
	public:
		nxtVarRicCopyoptions CopyOptions;
		ricvarPoint pos;
		nxtVarRicWord radius_x;
		nxtVarRicWord radius_y;
		
	public:
		object_op object_type() const{ return RIC_OP_ELLIPSE; }
		void draw(nxtCanvas* canvas) const;
		nxtIO::LoaderError ricscript_read( nxtIO* file );
		
		ricOpEllipse( ricfile *container ): 
				ricObject( container, 4 ), 
				CopyOptions( this ), 
				pos( this ), 
				radius_x( this ),
				radius_y( this )
			{
			pos.X.set_normal( 25 );
			pos.Y.set_normal( 25 );
			radius_x.set_normal( 14 );
			radius_y.set_normal( 7 );
			CopyOptions.enabled_fill_shape = true;
			vars[0] = &CopyOptions;
			vars[1] = &pos;
			vars[2] = &radius_x;
			vars[3] = &radius_y;
		}
		
};


class ricOpPolygon: public ricObject{
	public:
		nxtVarRicCopyoptions CopyOptions;
		pointArray points;
	
	public:
		object_op object_type() const{ return RIC_OP_POLYGON; }
		void draw(nxtCanvas* canvas) const;
		nxtIO::LoaderError ricscript_read( nxtIO* file );
		
		ricOpPolygon( ricfile *container ):
				ricObject( container, 2 ),
				CopyOptions( this )
		{
			CopyOptions.enabled_fill_shape = true;
			CopyOptions.enabled_polyline = true;
			vars[0] = &CopyOptions;
			vars[1] = &points;
		}
};


#endif

