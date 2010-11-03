#include <vector>
//#include "ricObject.h"
class ricObject;

class ricfile{
	private:
		std::vector<ricObject*> objects;
		
	public:
		int readfile(char* filename);
		int writefile(char* filename);
};

