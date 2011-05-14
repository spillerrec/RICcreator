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

void ricfile::nxtVarRicWord::read_raw( unsigned int raw ){
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

void ricfile::nxtVarRicWord::read(ifstream* file){
	read_raw( read_multibyte( file, 2 ) );
}
void ricfile::nxtVarRicWord::write(ofstream* file) const{
	if( is_extended() ){
		unsigned int raw = 0x1000 + parameter + (VarMapID << 8);
		write_multibyte( file, raw, 2 );
	}
	else
		write_multibyte( file, number, 2 );
}


void ricfile::nxtVarRicCopyoptions::copy( const nxtVarRicCopyoptions* original ){
	if( original != 0 ){
		invert = original->invert;
		clear = original->clear;
		clear_except_status = original->clear_except_status;
		fill_shape = original->fill_shape;
		polyline = original->polyline;
		merge = original->merge;
	}
}



void ricfile::nxtVarRicCopyoptions::set_properties(unsigned int raw){
	read_raw( raw );	//For nxtVarRicWord
	
	if( raw & DRAW_OPT_CLEAR_BACKGROUND )
		clear = true;
	else
		clear = false;
	
	if( raw & DRAW_OPT_CLEAR_EXCEPT_STATUS )
		clear_except_status = true;
	else
		clear_except_status = false;
	
	if( raw & DRAW_OPT_NOT )
		invert = true;
	else
		invert = false;
	
	if( raw & DRAW_OPT_FILL_SHAPE )
		fill_shape = true;
	else
		fill_shape = false;
	
	if( raw & DRAW_OPT_POLYGON_POLYLINE )
		polyline = true;
	else
		polyline = false;
	
	unsigned int merge_raw = raw & DRAW_OPT_XOR;
	switch( merge_raw ){
		case DRAW_OPT_AND: merge = MERGE_AND; break;
		case DRAW_OPT_OR: merge = MERGE_OR; break;
		case DRAW_OPT_XOR: merge = MERGE_XOR; break;
		default: merge = MERGE_COPY;
	}
}

void ricfile::nxtVarRicCopyoptions::write(ofstream* file) const{
	unsigned int raw = 0;
	
	if( clear )
		raw = raw | DRAW_OPT_CLEAR_BACKGROUND;
	
	if( clear_except_status )
		raw = raw | DRAW_OPT_CLEAR_EXCEPT_STATUS;
	
	if( invert )
		raw = raw | DRAW_OPT_NOT;
	
	switch( merge ){
		case MERGE_AND: raw = raw | DRAW_OPT_AND; break;
		case MERGE_OR: raw = raw | DRAW_OPT_OR; break;
		case MERGE_XOR: raw = raw | DRAW_OPT_XOR; break;
	}
	
	if( fill_shape )
		raw = raw | DRAW_OPT_FILL_SHAPE;
	
	if( polyline )
		raw = raw | DRAW_OPT_POLYGON_POLYLINE;
	
	write_multibyte( file, raw, 2 );
}


