
#include "riclib/ricfile.h"

int main(int argc, char *argv[]){
	ricfile graphics;
	graphics.readfile( "test.ric" );
	graphics.writefile( "copy.ric" );
	return 0;
}
