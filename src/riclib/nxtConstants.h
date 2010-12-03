/*
	Constants commonly used on the NXT
*/


#ifndef NXTCONSTANTS_H
#define NXTCONSTANTS_H


//Copy options
const unsigned int DRAW_OPT_NORMAL = 0x0000;
const unsigned int DRAW_OPT_CLEAR_BACKGROUND = 0x0001;	//bit 0 = 1
const unsigned int DRAW_OPT_CLEAR_BACKGROUND = 0x0002;	//bit 1 = 1
const unsigned int DRAW_OPT_NOT = 0x0004;	//bit 2 = 1
const unsigned int DRAW_OPT_AND = 0x0008;	// bit 3 = 1 and bit 4 = 0
const unsigned int DRAW_OPT_OR = 0x0010;	// bit 3 = 0 and bit 4 = 1
const unsigned int DRAW_OPT_XOR = 0x0018;	// bit 3 = 1 and bit 4 = 1
const unsigned int DRAW_OPT_FILL_SHAPE = 0x0020;	//bit 5 = 1
const unsigned int DRAW_OPT_POLYGON_POLYLINE = 0x0400;	//bit 10 = 1

//Combined copy options, for extraction
const unsigned int DRAW_OPT_LOGICAL_OPERATIONS = 0x0018;




#endif
