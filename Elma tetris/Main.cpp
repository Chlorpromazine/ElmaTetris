#include <Windows.h>
#include <iostream>
#include <string>
#include <time.h>

//debug output
#include "guicon.h"

//for elma
#include "define.h"
#include "tetris.h"
#include "objectHack.h"
#include "mem.h"

//tetris
#include "game.h"

using namespace std;
int run = 2;
int checkLev = 0;
bool enteredLev = 0;
bool inLev = 0;
bool newGame = 0;

clock_t lastTime = clock();
clock_t timer;

DWORD WINAPI mainLoop(void*)
{
	
	while (run == 2){

		//0 = menu, 1 = in normal level, replay = 2, 3 = editor level
		switch (currentScreen)
		{
		case 0:
			//reset if exited level.
			enteredLev = 1;
			inLev = 0;
			//objects::obj.reload = 1;
			break;
		case 1:
		case 3:
			
			//prepare objects and verification of correct level
			if (enteredLev && inLev == 0)
			{
				if (!memcmp(comparedLev, (char*)0x00485674, 10) ) 
				{
					cout << "Entered correct level" << endl;
					if (objects::obj.reload) objects::obj.clearVals();
					inLev = 1;
					newGame = 1;
				}
				else objects::obj.reload = 1;
				
				enteredLev = 0;
				
			}

			
			if (newGame)
			{
				//Pieces mPieces2;

				// Board
				//Board mBoard(&mPieces2);

				// Game
				//Game mGame(&mBoard, &mPieces2);

				newGame = 0;
			}
			
			//start tetris
			if (inLev)
			{
				timer = clock();                            
				                                             

				if (timer - lastTime > 16.6f)  //60fps
				{
					tetris::tet.mainTetris();
					
					lastTime = timer;   
				}
				
			}

			break;
		case 2:
			break;
		}
		Sleep(1);
	}
	
	return 0;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		RedirectIOToConsole();
		JumpPatch((BYTE*)0x00408DEF, (BYTE*)&objInject, 1);
		run = 2;
		CreateThread(NULL, 0, mainLoop, NULL, 0L, NULL);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		run = 1;
		
		break;
	}
	return TRUE;
}