#include "nxtCanvas.h"

ricPixel* nxtCanvas::GetPixel(unsigned int X, unsigned int Y){
	if( (X >= width) || (Y >= height) )
		return 0;
	
	return &map[ X + Y*width ];
}
