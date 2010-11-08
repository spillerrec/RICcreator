/*
	ricPixel class
	
	A ricPixel object contains the color of a pixel and how it interacts with a underlaying pixel.
	The information stored is as following:
		1 bit color, which reprecent a display's on/off state
		1 bit alpha which enables transparentcy
		The possibility to invert the color???	TODO: Check the neccesarily of this...
		
		4 different inteactions with an underlaying ricPixel: (will only have effect if the ricPixel is not transparent)
			Copy: the underlaying ricPixel will be ignored completely
			AND: an AND operation will be done on the two pixels and the result will be the new pixels color.
			OR: same as AND, except that an OR operation will be done
			XOR: same as AND, except that an XOR operation will be done
			
*/

const char PIXEL_COLOR_WHITE = 0;
const char PIXEL_COLOR_BLACK = 1;

const char PIXEL_SOLID = 0;
const char PIXEL_TRANSPARENT = 1 << 1;

const char PIXEL_NO_INVERT = 0;
const char PIXEL_INVERT = 1 << 2;

const char PIXEL_MERGE_COPY = 0;
const char PIXEL_MERGE_AND = 1 << 3;
const char PIXEL_MERGE_OR = 2 << 3;
const char PIXEL_MERGE_XOR = 3 << 3;


class ricPixel{
	private:
		unsigned char bin;
	
	public:
		ricPixel(){ bin = 0; }
		
		char GetColor(){ return (bin & 0x01); }
		char GetAlpha(){ return (bin & 0x02); }
		char GetInvert(){ return (bin & 0x04); }
		char GetMerge(){ return (bin & 0x18); }
		
		void SetColor(char color){ bin = (bin & 0xFE) | color; }
		void SetAlpha(char alpha){ bin = (bin & 0xFD) | alpha; }
		void SetInvert(char invert){ bin = (bin & 0xFB) | invert; }
		void SetMerge(char merge){ bin = (bin & 0xE7) | merge; }
		
		void Merge(ricPixel* top);	//Places another rixPixel on top of this one and merges the result into this ricPixel
};
