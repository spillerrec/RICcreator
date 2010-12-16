/*

*/

#include "ricObject.h"
#include "nxtCanvas.h"





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
				canvas->PointOut( ix+relX, iy+relY, &options, false );
			else
				canvas->PointOut( ix+relX, iy+relY, &options_rev, false );
		}
}

void ricfile::ricOpPixel::draw(nxtCanvas* canvas){
	copyoptions options( CopyOptions );
	
	canvas->PointOut(posX, posY, &options, false);
}

void ricfile::ricOpLine::draw(nxtCanvas* canvas){
	copyoptions options( CopyOptions );
	
	canvas->LineOut(startX, startY, endX, endY, &options, false);
}

void ricfile::ricOpRectangle::draw(nxtCanvas* canvas){
	copyoptions options( CopyOptions );
	
	canvas->RectOut(posX, posY, width, height, &options, false);
}

void ricfile::ricOpCicle::draw(nxtCanvas* canvas){
	copyoptions options( CopyOptions );
	
	canvas->CircleOut(posX, posY, radius, &options, false);
}

void ricfile::ricOpNumber::draw(nxtCanvas* canvas){
	copyoptions options( CopyOptions );
	
	canvas->NumberOut(posX, posY, number, &options, false);
}

