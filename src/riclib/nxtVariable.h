/*
	variable types used on the NXT
*/


#ifndef NXTVARIABLE_H
#define NXTVARIABLE_H

#include <fstream>
using namespace std;

#include "nxtConstants.h"

class nxtVariable{
	protected:
		unsigned long read_multibyte(ifstream* file, unsigned char size);
		void write_multibyte(ofstream* file, unsigned long number, unsigned char size);
		
	public:
		virtual void read(ifstream* file) = 0;
		virtual void write(ofstream* file) = 0;
};



class copyoptions: public nxtVariable{
	public:
		bool invert;
		char merge;
		bool clear;
		bool clear_except_status;
		bool fill_shape;
		bool polyline;
	
	public:
		static const char MERGE_COPY = 0;
		static const char MERGE_AND = 1;
		static const char MERGE_OR = 2;
		static const char MERGE_XOR = 3;
	
	private:
		void set_properties(unsigned int raw){
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
	
	public:
		copyoptions(){
			invert = false;
			clear = false;
			clear_except_status = false;
			fill_shape = false;
			polyline = false;
			merge = MERGE_COPY;
		}
		
		copyoptions( unsigned int raw ){
			copyoptions();
			set_properties( raw );
		}
		
		void invert_switch(){
			if( invert )
				invert = false;
			else
				invert = true;
		}
		
		copyoptions( copyoptions* original ){
			copyoptions();
			if( original != 0 ){
				invert = original->invert;
				clear = original->clear;
				clear_except_status = original->clear_except_status;
				fill_shape = original->fill_shape;
				polyline = original->polyline;
				merge = original->merge;
			}
		}
		
		virtual void read(ifstream* file){
			set_properties( read_multibyte( file, 2 ) );
		}
		
		virtual void write(ofstream* file){
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
};


class nxtVarWord: public nxtVariable{
	private:
		unsigned int variable;
		
	public:
		virtual void read(ifstream* file){
			variable = read_multibyte( file, 2 );
		}
		virtual void write(ofstream* file){
			write_multibyte( file, variable, 2 );
		}
		unsigned int value(){ return variable; }
		
		operator unsigned int(){ return variable; }
		nxtVarWord* operator=( int newValue ){
			variable = newValue;
			return this;
		}
};

#include "ricfile.h"

class ricfile::nxtVarRicWord: public nxtVariable{
	private:
		unsigned int number;
		bool extended;
		unsigned char parameter;
		unsigned char VarMapID;
		ricObject *object;
		
	public:
		nxtVarRicWord( ricObject *container ){
			number = 0;
			parameter = 0;
			VarMapID = 0;
			extended = false;
			object = container;
		}
		
		bool is_extended() const{ return extended; }
		unsigned int get_number() const{ return number; }
		unsigned char get_parameter() const{ return parameter; }
		unsigned char get_varmap() const{ return VarMapID; }
		
		virtual void read(ifstream* file){
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
		virtual void write(ofstream* file){
			write_multibyte( file, number, 2 );
		}
		unsigned int value() const;
		
		operator unsigned int(){ return value(); }
		nxtVarRicWord* operator=( int newValue ){
			number = newValue;
			return this;
		}
};


#endif
