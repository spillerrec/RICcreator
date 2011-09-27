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


void ricOpCopyBits::fit_sprite(){
	if( !pRIC )
		return;
	
	ricOpSprite *ric_sprite = (ricOpSprite*)(pRIC->object_at_ID( SpriteID.value(), RIC_OP_SPRITE, this ));
	
	if( ric_sprite ){
		src.pos.X = 0;
		src.pos.Y = 0;
		src.width = ric_sprite->sprite_data.get_width();
		src.height = ric_sprite->sprite_data.get_height();
	}
}

void ricOpCopyBits::find_sprite(){
	if( !pRIC )
		return;
	
	ricOpSprite *ric_sprite = (ricOpSprite*)pRIC->last_object( RIC_OP_SPRITE, this );
	
	if( ric_sprite ){
		SpriteID = ric_sprite->sprite_ID;
	}
}

