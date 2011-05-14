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
		
	
	public:
		static unsigned long read_multibyte(ifstream* file, unsigned char size);
		static void write_multibyte(ofstream* file, unsigned long number, unsigned char size);
		
	public:
		virtual unsigned int var_type() const = 0;
		virtual unsigned int var_amount() const{ return 1; }
		virtual unsigned int filesize() const = 0;
		virtual void read(ifstream* file) = 0;
		virtual void write(ofstream* file) const = 0;
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
		
		void read_raw( unsigned int raw );
		virtual void read(ifstream* file);
		virtual void write(ofstream* file) const;
		unsigned int value() const;
		
		operator unsigned int() const{ return value(); }
		nxtVarRicWord* operator=( int newValue ){
			number = newValue;
			return this;
		}
};



class ricfile::nxtVarRicCopyoptions: public ricfile::nxtVarRicWord{
	public:
		bool invert;
		char merge;
		bool clear;
		bool clear_except_status;
		bool fill_shape;
		bool polyline;
	
	//Merge types
	public:
		static const char MERGE_COPY = 0;
		static const char MERGE_AND = 1;
		static const char MERGE_OR = 2;
		static const char MERGE_XOR = 3;
	
	private:
		void set_properties(unsigned int raw);
	
	public:
		
		unsigned int var_type() const{ return TYPE_COPYOPTIONS; }
		
		void invert_switch(){
			if( invert )
				invert = false;
			else
				invert = true;
		}
		
		nxtVarRicCopyoptions( ricObject *container ): ricfile::nxtVarRicWord( container ){
			set_properties( value() );
		}
		
		void copy( const nxtVarRicCopyoptions* original );
		
		unsigned int filesize() const{ return 2; }
		virtual void read(ifstream* file){
			set_properties( read_multibyte( file, 2 ) );
		}
		
		virtual void write(ofstream* file) const;
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
