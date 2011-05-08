/*
	variable types used on the NXT
*/


#ifndef NXTVARIABLE_H
#define NXTVARIABLE_H

#include <fstream>
using namespace std;

#include "nxtConstants.h"

class nxtVariable{
	public:
		static const unsigned int TYPE_SBYTE = 0;
		static const unsigned int TYPE_UBYTE = 1;
		static const unsigned int TYPE_UWORD = 2;
		static const unsigned int TYPE_SWORD = 3;
		static const unsigned int TYPE_ULONG = 4;
		static const unsigned int TYPE_SLONG = 5;
		static const unsigned int TYPE_FLOAT = 6;
		
		static const unsigned int TYPE_RIC_WORD = 32;
		static const unsigned int TYPE_RIC_POINT = 33;
		static const unsigned int TYPE_RIC_RECT = 34;
		static const unsigned int TYPE_POINT_ARRAY = 35;
		static const unsigned int TYPE_COPYOPTIONS = 36;
		
	
	protected:
		unsigned long read_multibyte(ifstream* file, unsigned char size) const;
		void write_multibyte(ofstream* file, unsigned long number, unsigned char size) const;
		
	public:
		virtual unsigned int var_type() const = 0;
		virtual unsigned int var_amount() const{ return 1; }
		virtual unsigned int filesize() const = 0;
		virtual void read(ifstream* file) = 0;
		virtual void write(ofstream* file) const = 0;
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
		
		unsigned int var_type() const{ return TYPE_COPYOPTIONS; }
		
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
		
		unsigned int filesize() const{ return 2; }
		virtual void read(ifstream* file){
			set_properties( read_multibyte( file, 2 ) );
		}
		
		virtual void write(ofstream* file) const{
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
		unsigned int filesize() const{ return 2; }
		unsigned int var_type() const{ return TYPE_UWORD; }
		virtual void read(ifstream* file){
			variable = read_multibyte( file, 2 );
		}
		virtual void write(ofstream* file) const{
			write_multibyte( file, variable, 2 );
		}
		unsigned int value(){ return variable; }
		
		operator unsigned int() const{ return variable; }
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
		unsigned int filesize() const{ return 2; }
		unsigned int var_type() const{ return TYPE_RIC_WORD; }
		
		bool is_extended() const{ return extended; }
		unsigned int get_number() const{ return number; }
		unsigned char get_parameter() const{ return parameter; }
		unsigned char get_varmap() const{ return VarMapID; }
		
		bool set_extended( unsigned char map, unsigned char para ){
			if( map < 16 ){
				extended = true;
				VarMapID = map;
				parameter = para;
				
				return true;
			}
			
			return false;
		}
		bool set_normal( unsigned int new_value ){
			if( new_value <= 0x0FFF ){
				extended = false;
				number = new_value;
				
				return true;
			}
			
			return false;
		}
		
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
		virtual void write(ofstream* file) const{
			write_multibyte( file, number, 2 );
		}
		unsigned int value() const;
		
		operator unsigned int() const{ return value(); }
		nxtVarRicWord* operator=( int newValue ){
			number = newValue;
			return this;
		}
};



		struct point{
			nxtVarWord X;
			nxtVarWord Y;
		};

class ricfile::ricvarPoint: public nxtVariable{
	public:
		nxtVarRicWord X;
		nxtVarRicWord Y;
		
		ricvarPoint( ricObject *container ):
				X( container ),
				Y( container )
			{ }
		
		unsigned int filesize() const{ return 2; }
		unsigned int var_type() const{ return TYPE_RIC_POINT; }
		unsigned int var_amount() const{ return 2; }
		void read(ifstream* file){
			X.read( file );
			Y.read( file );
		}
		void write(ofstream* file) const{
			X.write( file );
			Y.write( file );
		}
};

class ricfile::ricvarRect: public nxtVariable{
	public:
		ricvarPoint pos;
		nxtVarRicWord width;
		nxtVarRicWord height;
		
		ricvarRect( ricObject *container ):
				pos( container ),
				width( container ),
				height( container )
			{ }
		
		unsigned int filesize() const{ return 2; }
		unsigned int var_type() const{ return TYPE_RIC_RECT; }
		unsigned int var_amount() const{ return 4; }
		void read(ifstream* file){
			pos.read( file );
			width.read( file );
			height.read( file );
		}
		void write(ofstream* file) const{
			pos.write( file );
			width.write( file );
			height.write( file );
		}
};


#endif
