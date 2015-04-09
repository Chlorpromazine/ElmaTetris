#include "objectHack.h"

objects objects::obj;

objects::objects() {

	//object currently selected
	curObject = 0;

	//current lev check
	for (int i = 0; i < 8; i++)
		oldLev[i] = 0;

	reload = 1;

}

vector<int> objectAddr;
vector<int> tempObjAddr;
int tmpAddr;

void objects::changeObject(int curObj)
{

	if (objectAddr.size() > curObj)
	{
		//read current object type;
		BYTE* Dest = (BYTE*)(objectAddr[curObject] + 32);
		BYTE type = *(BYTE*)Dest;

		//augment value and copy ( 0 < type < 5 ) or crash
		//1 = flower, 2 = apple, 3 = killer, 4 = start (only 1 start location or it crashes)
		type++;
		if (type > 4) type = 1;

		BYTE Buff[1] = { type };
		MemPatch(Dest, Buff, 1);

	}
}

void objects::objectActive(int curObj)
{
	if (objectAddr.size() > curObj)
	{
		//read current object type;
		BYTE* Dest = (BYTE*)(objectAddr[curObj] + 44);
		BYTE type = *(BYTE*)Dest;

		//1 active, 0 non active
		type = !type;

		BYTE Buff[1] = { type };
		MemPatch(Dest, Buff, 1);

	}
}


//function which takes the addresses of the dynamically stored objects 
int addr_00408DFA = 0x00408DFA;
int correctLev;


void __declspec(naked) objInject()
{

	__asm{pusha}

	correctLev = memcmp(comparedLev, (char*)0x00485674, 10);
	
	__asm{popa}

	__asm{
			
		fld qword ptr[esi + 8]
		fchs
		mov tmpAddr, esi
		mov[esi + 0x10], eax
		fsub qword ptr[edi + 0x20]

	}
		
	if (!correctLev)
		tempObjAddr.push_back(tmpAddr);
			
		
	__asm{
		jmp addr_00408DFA
	}
	
}

void objects::nextObject()
{

	if (objectAddr.size())
	{
		//last object is always null, don't know what it is so I'm just removing it.
		if (curObject < objectAddr.size() - 1)
			curObject++;
		else
			curObject = 0;

		//debug
		cout << "Current Object: " << curObject;
		cout << "\tType: ";
		switch (*(BYTE*)(objectAddr[curObject] + 32))
		{
		case 1:
			cout << "Flower";
			break;
		case 2:
			cout << "Apple";
			break;
		case 3:
			cout << "Killer";
			break;
		case 4:
			cout << "Start";
			break;
		default:
			cout << "Unknown";
			break;
		}
		cout << endl;
	}
}

void objects::moveObject(int dir)
{
	if (objectAddr.size())
	{
		pos position = getLocation();

		//movement
		if (dir == 0) position.y += 1;
		else if (dir == 1) position.y -= 1;
		else if (dir == 2) position.x -= 1;
		else               position.x += 1;

		newLocation(position);
	}
}


objects::pos objects::getLocation()
{
	int data;
	pos position;

	int addr = objectAddr[curObject] + 16;

	//take the current position from the memory
	position.x = (*(int*)addr);
	position.y = (*(int*)(addr + 4));
	return position;
}

void objects::newLocation(pos newPos)
{
	//copy object location x
	BYTE* Dest = (BYTE*)(objectAddr[curObject] + 16);
	MemPatchInt(Dest, &newPos.x, 4);

	//copy object location y
	Dest += 4;
	MemPatchInt(Dest, &newPos.y, 4);

}

void objects::clearVals()
{
	
		objectAddr = tempObjAddr;
		cout << objectAddr.size() << endl;
		tempObjAddr.clear();
		reload = 0;
}


