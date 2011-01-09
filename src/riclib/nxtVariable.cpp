#include "nxtVariable.h"
#include "ricObject.h"

unsigned long nxtVariable::read_multibyte(ifstream* file, unsigned char size){
	unsigned long read_value = 0;
	unsigned long multiplier = 1;
	
	for(int i=0; i<size; i++){
		read_value += file->get() * multiplier;
		multiplier *= 256;
	}
	
	return read_value;
}


void nxtVariable::write_multibyte(ofstream* file, unsigned long number, unsigned char size){
	//Format it correctly
	char* data = new char [size];
	for(int i=0; i<size; i++){
		data[i] = number % 256;
		number /= 256;
	}
	delete[] data;
	
	file->write(data, size); //Write it to stream
}



unsigned int ricfile::nxtVarRicWord::value() const{
	if( extended ){
		if( VarMapID ){
			//TODO: return VarMap[VarMapID] value at x=value
			return 0;
		}
		else
			return object->parent()->GetParameter( parameter );
	}
	else
		return number;
}

