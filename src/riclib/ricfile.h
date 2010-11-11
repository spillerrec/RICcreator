
#ifndef RICFILE_H
#define RICFILE_H

#include <vector>
//#include "ricObject.h"
class ricObject;
#include "nxtCanvas.h"

class ricfile{
	private:
		std::vector<ricObject*> objects;
		unsigned int parameter[256];
		
	public:
		int readfile(char* filename);
		int writefile(char* filename);
		
		void ResetParameters(){
			for(int i=0; i<256; i++)
				parameter[i] = 0;
		}
		void SetParameter(unsigned char parameter, unsigned int value){ this->parameter[parameter] = value; }
		
		void Draw(nxtCanvas* canvas, unsigned int width, unsigned int height);
		
		ricfile(){
			ResetParameters();
		}
};

#endif

