#ifndef tetris_h
#define tetris_h

#include "objectHack.h"

class tetris
{
private:
	int time;
	int i;
public:
	static tetris tet;
	tetris();

	void mainTetris();
};


#endif