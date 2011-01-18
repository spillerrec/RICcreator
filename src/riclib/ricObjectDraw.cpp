/*

*/

#include "ricObject.h"
#include "nxtCanvas.h"





void ricfile::ricOpCopyBits::draw(nxtCanvas* canvas) const{
	copyoptions options( CopyOptions );
	copyoptions options_rev( CopyOptions );
	options_rev.invert_switch();
	
	ricOpSprite* sprite = (ricOpSprite*)pRIC->object_at_ID( SpriteID, RIC_OP_SPRITE, (ricOpSprite*)this );
	if( sprite == 0 )
		return;
	
	for(unsigned int ix=0; ix<src.width; ix++)
		for(unsigned int iy=0; iy<src.height; iy++){
			if( sprite->pixel( ix+src.pos.X, iy+src.pos.Y ) )
				canvas->PointOut( ix+dest.X, iy+dest.Y, &options, false );
			else
				canvas->PointOut( ix+dest.X, iy+dest.Y, &options_rev, false );
		}
}

void ricfile::ricOpPixel::draw(nxtCanvas* canvas) const{
	copyoptions options( CopyOptions );
	
	canvas->PointOut(pos.X, pos.Y, &options, false);
}

void ricfile::ricOpLine::draw(nxtCanvas* canvas) const{
	copyoptions options( CopyOptions );
	
	canvas->LineOut(start.X, start.Y, end.X, end.Y, &options, false);
}

void ricfile::ricOpRectangle::draw(nxtCanvas* canvas) const{
	copyoptions options( CopyOptions );
	
	canvas->RectOut(rect.pos.X, rect.pos.Y, rect.width, rect.height, &options, false);
}

void ricfile::ricOpCicle::draw(nxtCanvas* canvas) const{
	copyoptions options( CopyOptions );
	
	canvas->CircleOut(pos.X, pos.Y, radius, &options, false);
}

void ricfile::ricOpNumber::draw(nxtCanvas* canvas) const{
	copyoptions options( CopyOptions );
	
	canvas->NumberOut(pos.X, pos.Y, number, &options, false);
}


void ricfile::ricOpEllipse::draw(nxtCanvas* canvas) const{
	copyoptions options( CopyOptions );
	
	canvas->EllipseOut(pos.X, pos.Y, radius_x, radius_y, &options, false);
}


void ricfile::ricOpPolygon::draw(nxtCanvas* canvas) const{
	copyoptions options( CopyOptions );
	
	canvas->PolyOut(&points, &options, false);
}

