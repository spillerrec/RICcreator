
#ifndef RICFILE_H
#define RICFILE_H

#include <vector>
//#include "nxtCanvas.h"
class nxtCanvas;

class ricfile{
	private:
		unsigned int parameter[256];
	
	public:
		void SetParameter(unsigned char parameter, unsigned int value){ this->parameter[parameter] = value; }
		unsigned int GetParameter(unsigned char parameter){ return this->parameter[parameter]; }
		class nxtVarRicWord;
		
		class ricObject;
		class ricOpOptions;
		class ricOpSprite;
		class ricOpVarMap;
		class ricOpCopyBits;
		class ricOpPixel;
		class ricOpLine;
		class ricOpRectangle;
		class ricOpCicle;
		class ricOpNumber;
	
	private:
		std::vector<ricObject*> objects;
		
	public:
		int readfile(char* filename);
		int writefile(char* filename);
		
		void ResetParameters(){
			for(int i=0; i<256; i++)
				parameter[i] = 0;
		}
		
		void Draw(nxtCanvas* canvas, unsigned int width, unsigned int height);
		
		ricOpSprite* GetSprite( char SpriteID, unsigned int currListID = -1 );
		
		ricfile(){
			ResetParameters();
		}
};

#endif

