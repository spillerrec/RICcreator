
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
	
	image.LineOut(9,1, 1,7);
//	image.RectOut(2,2, 2,2);
	
	image.OutputCanvas();
	
	return 0;
}
