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


#ifndef RICFILE_H
#define RICFILE_H

#include <vector>
class nxtCanvas;
class ricObject;

class ricfile{
	private:
		unsigned int parameter[256];
	
	public:
		void SetParameter(unsigned char parameter, unsigned int value){ this->parameter[parameter] = value; }
		unsigned int GetParameter(unsigned char parameter) const{ return this->parameter[parameter]; }
		
		ricObject* add_ric_object( unsigned int type );
	
	private:
		std::vector<ricObject*> objects;
		
	public:
		int readfile(const char* filename);
		int writefile(const char* filename);
		
		void ResetParameters(){
			for(int i=0; i<256; i++)
				parameter[i] = 0;
		}
		
		unsigned int object_amount(){ return objects.size(); }
		ricObject* get_object( unsigned int index ){
			if( index < object_amount() )
				return objects[ index ];
			else
				return 0;
		}
		unsigned int object_index( ricObject* obj_wanted ) const;
		
		
		void Reset();
		
		void Draw(nxtCanvas* canvas, unsigned int width, unsigned int height);
		
		
		static const unsigned int INVALID_INDEX = (unsigned int)0 - 1;
		ricObject* object_at_ID( unsigned char ID, unsigned int type, unsigned int from_index = INVALID_INDEX ) const;
		ricObject* object_at_ID( unsigned char ID, unsigned int type, ricObject* from_object ) const;
		unsigned int get_varmap_value( unsigned char varmapID, ricObject* from_object, unsigned int x ) const;
		
		ricfile(){
			Reset();
		}
};

#endif

