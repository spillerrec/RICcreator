/*

*/

#include "ricObject.h"
#include "nxtCanvas.h"
#include <iostream>





void ricfile::ricOpCopyBits::draw(nxtCanvas* canvas){
	copyoptions options( CopyOptions );
	copyoptions options_rev( CopyOptions );
	options_rev.invert_switch();
	
	ricOpSprite* sprite = pRIC->GetSprite( SpriteID );
	if( sprite == 0 )
		return;
	
	for(unsigned int ix=0; ix<width; ix++)
		for(unsigned int iy=0; iy<height; iy++){
			if( sprite->pixel( ix+posX, iy+posY ) )
				canvas->PointOut( ix+relX, iy+relY, &options );
			else
				canvas->PointOut( ix+relX, iy+relY, &options_rev );
		}
}

void ricfile::ricOpPixel::draw(nxtCanvas* canvas){
	copyoptions options( CopyOptions );
	
	canvas->PointOut(posX, posY, &options);
}

void ricfile::ricOpLine::draw(nxtCanvas* canvas){
	copyoptions options( CopyOptions );
	
	canvas->LineOut(startX, startY, endX, endY, &options);
}

void ricfile::ricOpRectangle::draw(nxtCanvas* canvas){
	copyoptions options( CopyOptions );
	
	canvas->RectOut(posX, posY, width, height, &options);
}

void ricfile::ricOpCicle::draw(nxtCanvas* canvas){
	std::cout << "ricOpCicle::draw() not yet implemented!\n";
}

void ricfile::ricOpNumber::draw(nxtCanvas* canvas){
	std::cout << "ricOpNumber::draw() not yet implemented!\n";
}

