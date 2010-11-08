#include "ricPixel.h"

void ricPixel::Merge(ricPixel* top){
	if(top == 0)
		return;
	
	//Don't do anything if the pixel to merge is transparent
	if( top->GetAlpha() == PIXEL_TRANSPARENT )
		return;
	
	//TODO: simplify this pixel so it is easier to merge
	
	//Now we are interested in two behaviours, if this pixel is solid then do a normal merge.
		//If this pixel is transparent, merge the properties so they are intact when doing a second merge
	if( GetAlpha() == PIXEL_SOLID ){
		switch( top->GetMerge() ){
			case PIXEL_MERGE_COPY:
					SetColor( top->GetColor() );
					SetAlpha( top->GetAlpha() );
					SetInvert( top->GetInvert() );
					SetMerge( top->GetMerge() );
				break;
				
			case PIXEL_MERGE_AND:
					if( (GetColor() == PIXEL_COLOR_BLACK) && (top->GetColor() == PIXEL_COLOR_BLACK) )
						SetColor( PIXEL_COLOR_BLACK );
					else
						SetColor( PIXEL_COLOR_WHITE );
				break;
			
			case PIXEL_MERGE_OR:
					if( (GetColor() == PIXEL_COLOR_BLACK) || (top->GetColor() == PIXEL_COLOR_BLACK) )
						SetColor( PIXEL_COLOR_BLACK );
					else
						SetColor( PIXEL_COLOR_WHITE );
				break;
			
			case PIXEL_MERGE_XOR:
					if( GetColor() == PIXEL_COLOR_BLACK ){
						if( top->GetColor() == PIXEL_COLOR_WHITE )
							SetColor( PIXEL_COLOR_BLACK );
						else
							SetColor( PIXEL_COLOR_WHITE );
					}
					else
						if( top->GetColor() == PIXEL_COLOR_BLACK )
							SetColor( PIXEL_COLOR_BLACK );
						else
							SetColor( PIXEL_COLOR_WHITE );
				break;
		}
	}
	else{
		//TODO:
	}
}

void ricPixel::SetAll(ricPixel* newpixel){
	SetColor( newpixel->GetColor() );
	SetAlpha( newpixel->GetAlpha() );
	SetInvert( newpixel->GetInvert() );
	SetMerge( newpixel->GetMerge() );
}

