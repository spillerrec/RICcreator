

const bool BRUSH_COLOR_WHITE = 0;
const bool BRUSH_COLOR_BLACK = 1;

const bool BRUSH_SOLID = true;
const bool BRUSH_REMOVE = false;

const bool BRUSH_NO_INVERT = false;
const bool BRUSH_INVERT = true;

const char BRUSH_MERGE_COPY = 0;
const char BRUSH_MERGE_AND = 1;
const char BRUSH_MERGE_OR = 2;
const char BRUSH_MERGE_XOR = 3;


class Brush{
	private:
		bool color;
		bool alpha;
		bool invert;
		char bitmerge;
		
		unsigned char bin;
	
	public:
		Brush(){
			color = BRUSH_COLOR_BLACK;
			alpha = BRUSH_SOLID;
			invert = BRUSH_NO_INVERT;
			bitmerge = BRUSH_MERGE_COPY;
			
			bin = 0;
		}
		
		void SetColor(bool color){ this->color = color; }
		void SetAlpha(bool alpha){ this->alpha = alpha; }
		void SetInvert(bool invert){ this-invert = invert; }
		void SetMerge(char merge){ bitmerge = merge; }
		
		
};
