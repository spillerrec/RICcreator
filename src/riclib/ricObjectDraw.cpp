/*

*/

#include "ricObject.h"
#include "nxtCanvas.h"
#include <iostream>





void ricOpCopyBits::draw(nxtCanvas* canvas){
	std::cout << "ricOpCopyBits::draw() not yet implemented!\n";
}

void ricOpPixel::draw(nxtCanvas* canvas){
	canvas->PointOut(posX, posY);
}

void ricOpLine::draw(nxtCanvas* canvas){
	canvas->LineOut(startX, startY, endX, endY);
}

void ricOpRectangle::draw(nxtCanvas* canvas){
	canvas->RectOut(posX, posY, width, height);
}

void ricOpCicle::draw(nxtCanvas* canvas){
	std::cout << "ricOpCicle::draw() not yet implemented!\n";
}

void ricOpNumber::draw(nxtCanvas* canvas){
	std::cout << "ricOpNumber::draw() not yet implemented!\n";
}

