#include "tetris.h"

tetris tetris::tet;

//constructor
tetris::tetris()
{
	time = 0;
	i = 0;
}

void tetris::mainTetris()
{
	time++;

	if (time > 5)
	{
		if (i > 20) i = 0;
		for (int j = 0; j < 10; j++)
			objects::obj.objectActive((i*10)+j,1);
		i++;
		time = 0;
	}
}