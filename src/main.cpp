
#include "riclib/ricfile.h"
#include "riclib/nxtCanvas.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[]){
	ricfile graphics;
	graphics.readfile( "test.ric" );
	graphics.writefile( "copy.ric" );
	
	cout << "\n\n";
	nxtCanvas image;
	
	image.create(10,10);
	image.create(10,10);
	
	ricPixel* point = image.GetPixel( 3,3 );
	
	cout << "Color: " << (int)point->GetColor() << "\n";
	cout << "Alpha: " << (int)point->GetAlpha() << "\n";
	cout << "Invert: " << (int)point->GetInvert() << "\n";
	cout << "Merge: " << (int)point->GetMerge() << "\n";
	cout << "\n";
	
	point->SetColor( PIXEL_COLOR_BLACK );
	point->SetAlpha( PIXEL_TRANSPARENT );
	point->SetInvert( PIXEL_INVERT );
	point->SetMerge( PIXEL_MERGE_OR );
	
	cout << "Color: " << (int)point->GetColor() << "\n";
	cout << "Alpha: " << (int)point->GetAlpha() << "\n";
	cout << "Invert: " << (int)point->GetInvert() << "\n";
	cout << "Merge: " << (int)point->GetMerge() << "\n";
	cout << "\n";
	
	return 0;
}
