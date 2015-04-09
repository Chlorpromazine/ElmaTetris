#include "mem.h"

BYTE Buff[100];

void MemPatch(BYTE* Dest, BYTE* Src, int Len){
	DWORD Old;

	VirtualProtect(Dest, Len, PAGE_EXECUTE_READWRITE, &Old);
	memcpy(Dest, Src, Len);
	VirtualProtect(Dest, Len, Old, &Old);
}

void MemPatchInt(BYTE* Dest, unsigned int* Src, int Len){
	DWORD Old;

	VirtualProtect(Dest, Len, PAGE_EXECUTE_READWRITE, &Old);
	memcpy(Dest, Src, Len);
	VirtualProtect(Dest, Len, Old, &Old);
}



void CallPatch(BYTE* Dest, BYTE* Src, int Nops){
	Buff[0] = 0xE8;
	*(int*)&Buff[1] = Src - (Dest + 5);

	for (int i = 5; i < Nops + 5; i++){
		Buff[i] = 0x90;
	}
	MemPatch(Dest, Buff, Nops + 5);
}


void JumpPatch(BYTE* Dest, BYTE* Src, int Nops){
	Buff[0] = 0xE9;
	*(int*)&Buff[1] = Src - (Dest + 5);

	for (int i = 5; i < Nops + 5; i++){
		Buff[i] = 0x90;
	}
	MemPatch(Dest, Buff, Nops + 5);
}


