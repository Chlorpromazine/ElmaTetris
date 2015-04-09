#ifndef mem_h
#define mem_h

#include <Windows.h>
#include <iostream>

using namespace std;

void MemPatch(BYTE* Dest, BYTE* Src, int Len);
void MemPatchInt(BYTE* Dest, unsigned int* Src, int Len);
void CallPatch(BYTE* Dest, BYTE* Src, int Nops);
void JumpPatch(BYTE* Dest, BYTE* Src, int Nops);



template<typename T>
void MemPatchTemplate(BYTE* Dest, T Src, int Len){
	DWORD Old;

	VirtualProtect(Dest, Len, PAGE_EXECUTE_READWRITE, &Old);
	memcpy(Dest, Src, Len);
	VirtualProtect(Dest, Len, Old, &Old);
}

#endif