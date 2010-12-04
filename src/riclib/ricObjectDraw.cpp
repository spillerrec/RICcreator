/*

*/

#include "ricObject.h"
#include "nxtCanvas.h"
#include <iostream>





void ricfile::ricOpCopyBits::draw(nxtCanvas* canvas){
	
	ricOpSprite* sprite = pRIC->GetSprite( SpriteID );
	if( sprite == 0 )
		return;
	
	for(int ix=0; ix<width; ix++)
		for(int iy=0; iy<height; iy++){
			if( sprite->pixel( ix+posX, iy+posY ) )
				canvas->PointOut( ix+relX, iy+relY );
		}
}

void ricfile::ricOpPixel::draw(nxtCanvas* canvas){
	canvas->PointOut(posX, posY);
}

void ricfile::ricOpLine::draw(nxtCanvas* canvas){
	canvas->LineOut(startX, startY, endX, endY);
}

void ricfile::ricOpRectangle::draw(nxtCanvas* canvas){
	canvas->RectOut(posX, posY, width, height);
}

void ricfile::ricOpCicle::draw(nxtCanvas* canvas){
	std::cout << "ricOpCicle::draw() not yet implemented!\n";
}

void ricfile::ricOpNumber::draw(nxtCanvas* canvas){
	std::cout << "ricOpNumber::draw() not yet implemented!\n";
}

