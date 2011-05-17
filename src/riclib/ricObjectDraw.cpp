/*

*/

#include "ricObject.h"
#include "nxtCanvas.h"





void ricfile::ricOpCopyBits::draw(nxtCanvas* canvas){
	ricOpSprite* sprite = (ricOpSprite*)pRIC->object_at_ID( SpriteID, RIC_OP_SPRITE, (ricOpSprite*)this );
	if( sprite == 0 )
		return;
	
	for(unsigned int ix=0; ix<src.width; ix++)
		for(unsigned int iy=0; iy<src.height; iy++){
			if( sprite->sprite_data.get_pixel( ix+src.pos.X, iy+src.pos.Y ) )
				canvas->PointOut( ix+dest.X, iy+dest.Y, &CopyOptions, false );
			else{
				CopyOptions.invert_switch();
				canvas->PointOut( ix+dest.X, iy+dest.Y, &CopyOptions, false );
				CopyOptions.invert_switch();
			}
		}
}

void ricfile::ricOpPixel::draw(nxtCanvas* canvas){
	canvas->PointOut(pos.X, pos.Y, &CopyOptions, false);
}

void ricfile::ricOpLine::draw(nxtCanvas* canvas){
	canvas->LineOut(start.X, start.Y, end.X, end.Y, &CopyOptions, false);
}

void ricfile::ricOpRectangle::draw(nxtCanvas* canvas){
	canvas->RectOut(rect.pos.X, rect.pos.Y, rect.width, rect.height, &CopyOptions, false);
}

void ricfile::ricOpCicle::draw(nxtCanvas* canvas){
	canvas->CircleOut(pos.X, pos.Y, radius, &CopyOptions, false);
}

void ricfile::ricOpNumber::draw(nxtCanvas* canvas){
	canvas->NumberOut(pos.X, pos.Y, number, &CopyOptions, false);
}


void ricfile::ricOpEllipse::draw(nxtCanvas* canvas){
	canvas->EllipseOut(pos.X, pos.Y, radius_x, radius_y, &CopyOptions, false);
}


void ricfile::ricOpPolygon::draw(nxtCanvas* canvas){
	canvas->PolyOut(&points, &CopyOptions, false);
}

