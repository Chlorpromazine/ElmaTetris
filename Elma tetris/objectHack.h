#ifndef obj_h
#define obj_h
#include "Mem.h"
#include "windows.h"
#include <vector>
#include <iostream>
#include "define.h"

using namespace std;

void __declspec() objInject();

class objects{

public:
	static objects obj;
	objects();

	struct pos{
		unsigned int x;
		unsigned int y;
	};

	bool reload;


private:
	int curObject;
	char oldLev[8];

public:
	void changeObject(int curObj);
	void objectActive(int curObj, bool active);
	void nextObject();
	void moveObject(int);
	pos getLocation();
	void clearVals();

	void newLocation(pos);

	bool newLev();

	
};

#endif