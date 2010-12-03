/*
	variable types used on the NXT
*/


#ifndef NXTVARIABLE_H
#define NXTVARIABLE_H

#include <fstream>
using namespace std;


class nxtVariable{
	protected:
		unsigned long read_multibyte(ifstream* file, unsigned char size);
		void write_multibyte(ofstream* file, unsigned long number, unsigned char size);
		
	public:
		virtual void read(ifstream* file) = 0;
		virtual void write(ofstream* file) = 0;
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
		
		operator int(){ return variable; }
		nxtVarWord* operator=( int newValue ){
			variable = newValue;
			return this;
		}
};



class nxtVarRicWord: public nxtVariable{
	private:
		unsigned int number;
		unsigned char type;
		unsigned char VarMapID;
		
	public:
		nxtVarRicWord(){
			number = 0;
			type = 0;
			VarMapID = 0;
		}
		virtual void read(ifstream* file){
			number = read_multibyte( file, 2 );
		}
		virtual void write(ofstream* file){
			write_multibyte( file, number, 2 );
		}
		unsigned int value();
		
		operator int(){ return value(); }
		nxtVarRicWord* operator=( int newValue ){
			number = newValue;
			return this;
		}
};


#endif
