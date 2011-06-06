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


#include "ricObjectChildren.h"
#include "nxtCanvas.h"
#include "ricfile.h"





void ricOpCopyBits::draw(nxtCanvas* canvas) const{
	ricOpSprite* sprite = (ricOpSprite*)pRIC->object_at_ID( SpriteID, RIC_OP_SPRITE, (ricOpSprite*)this );
	if( sprite == 0 )
		return;
	
	canvas->copy_canvas( &sprite->sprite_data, (unsigned int)src.pos.X, (unsigned int)src.pos.Y, (unsigned int)src.width, (unsigned int)src.height, (int)dest.X, (int)dest.Y, &CopyOptions, false );
}

void ricOpPixel::draw(nxtCanvas* canvas) const{
	canvas->PointOut(pos.X, pos.Y, &CopyOptions, false);
}

void ricOpLine::draw(nxtCanvas* canvas) const{
	canvas->LineOut(start.X, start.Y, end.X, end.Y, &CopyOptions, false);
}

void ricOpRectangle::draw(nxtCanvas* canvas) const{
	canvas->RectOut(rect.pos.X, rect.pos.Y, rect.width, rect.height, &CopyOptions, false);
}

void ricOpCicle::draw(nxtCanvas* canvas) const{
	canvas->CircleOut(pos.X, pos.Y, radius, &CopyOptions, false);
}

void ricOpNumber::draw(nxtCanvas* canvas) const{
	canvas->NumberOut(pos.X, pos.Y, number, &CopyOptions, false);
}


void ricOpEllipse::draw(nxtCanvas* canvas) const{
	canvas->EllipseOut(pos.X, pos.Y, radius_x, radius_y, &CopyOptions, false);
}


void ricOpPolygon::draw(nxtCanvas* canvas) const{
	canvas->PolyOut(&points, &CopyOptions, false);
}

