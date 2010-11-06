/*
	RIC image type

	Each
*/
#include "Brush.h"


class ricImage{
	private:
		unsigned int width;
		unsigned int height;
		
	public:
		char* map;
		
		ricImage(){
			width = 0;
			height = 0;
			map = NULL;
		}
		
		void create(unsigned int width, unsigned int height){
			this->width = width;
			this->height = height;
			
			if(map)
				delete[] map;
			map = new char [width*height];
		}
		
		~ricImage(){
			if(map)
				delete[] map;
		}
};
