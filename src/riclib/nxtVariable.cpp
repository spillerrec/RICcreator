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
	file->write(data, size); //Write it to stream
	
	delete[] data;
}



unsigned int ricfile::nxtVarRicWord::value() const{
	if( extended ){
		unsigned int par_val = object->parent()->GetParameter( parameter );
		if( VarMapID ){
			//TODO: return VarMap[VarMapID] value at x=value
			return object->parent()->get_varmap_value( VarMapID, object, par_val );
		}
		else
			return par_val;
	}
	else
		return number;
}


void ricfile::nxtVarRicWord::read(ifstream* file){
	int raw = read_multibyte( file, 2 );
	if( raw & 0x1000 ){
		extended = true;
		number = 0;
		parameter = raw & 0x00FF;
		VarMapID = (raw & 0x0F00) >> 8;
	}
	else{
		extended = false;
		number = raw & 0x0FFF;
		parameter = 0;
		VarMapID = 0;
	}
}
void ricfile::nxtVarRicWord::write(ofstream* file) const{
	if( is_extended() ){
		unsigned int raw = 0x1000 + parameter + (VarMapID << 8);
		write_multibyte( file, raw, 2 );
	}
	else
		write_multibyte( file, number, 2 );
}



