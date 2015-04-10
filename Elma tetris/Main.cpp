#include <Windows.h>
#include <iostream>
#include <string>
#include <time.h>

//debug output
#include "guicon.h"

//for elma
#include "define.h"
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

long delayDown = 0;

bool checkExit = 1;

clock_t lastTime = clock();
clock_t timer;

static vector<bool> currKeyStates;
static vector<bool> prevKeyStates;

void keyDown(int key1)
{
	if (GetAsyncKeyState(key1) < 0)
		currKeyStates[key1] = 1;
	else currKeyStates[key1] = 0;
}
void exitLev(int exitBool);

void hotKeys(Game &mGame,Board &mBoard)
{

		//up
	keyDown(VK_UP);
	if (prevKeyStates[VK_UP] != currKeyStates[VK_UP])
	{
		if (currKeyStates[VK_UP])
			if (mBoard.IsPossibleMovement(mGame.mPosX, mGame.mPosY, mGame.mPiece, (mGame.mRotation + 1) % 4))
				mGame.mRotation = (mGame.mRotation + 1) % 4;


	}prevKeyStates[VK_UP] = currKeyStates[VK_UP];

	//down
	if (GetAsyncKeyState(VK_DOWN) < 0 && delayDown > 100)
	{

		if (mBoard.IsPossibleMovement(mGame.mPosX, mGame.mPosY + 1, mGame.mPiece, mGame.mRotation))
			mGame.mPosY++;
		delayDown = 0;
	}
	

	//left
	keyDown(VK_LEFT);
	if (prevKeyStates[VK_LEFT] != currKeyStates[VK_LEFT])
	{
		if (currKeyStates[VK_LEFT])
			if (mBoard.IsPossibleMovement(mGame.mPosX + 1, mGame.mPosY, mGame.mPiece, mGame.mRotation))
				mGame.mPosX++;
			
			

	}prevKeyStates[VK_LEFT] = currKeyStates[VK_LEFT];

	//right
	keyDown(VK_RIGHT);
	if (prevKeyStates[VK_RIGHT] != currKeyStates[VK_RIGHT])
	{
		if (currKeyStates[VK_RIGHT])
			if (mBoard.IsPossibleMovement(mGame.mPosX - 1, mGame.mPosY, mGame.mPiece, mGame.mRotation))
				mGame.mPosX--;
			
		

	}prevKeyStates[VK_RIGHT] = currKeyStates[VK_RIGHT];

	//c
	keyDown(0x43);
	if (prevKeyStates[0x43] != currKeyStates[0x43])
	{
		if (currKeyStates[0x43])
		{
			while (mBoard.IsPossibleMovement(mGame.mPosX, mGame.mPosY, mGame.mPiece, mGame.mRotation)) { mGame.mPosY++; }

			mBoard.StorePiece(mGame.mPosX, mGame.mPosY - 1, mGame.mPiece, mGame.mRotation);

			mBoard.DeletePossibleLines();

			if (mBoard.IsGameOver())
			{
				exitLev(0);
				checkExit = 0;
				cout << "game over" << endl;
			}

			mGame.CreateNewPiece();
		}


	}prevKeyStates[0x43] = currKeyStates[0x43];



	//remove dll from elma (DEL)
	if (GetAsyncKeyState(VK_DELETE))
	{
		run = 1;
	}

}

void exitLev(int exitBool)
{
	if (!exitBool)
	{
		BYTE* Dest = (BYTE*)0x0041F63B;
		BYTE Buff[5] = { NOP, NOP, NOP, NOP, NOP };
		MemPatch(Dest, Buff, 5);
	}
	else
	{
		JumpPatch((BYTE*)0x0041F63B, (BYTE*)0x0041F1D4, 0);
	}
	exitBool = !exitBool;
}

DWORD WINAPI mainLoop(void*)
{
	for (int i = 0; i < 255; i++)
	{
		currKeyStates.push_back(0);
		prevKeyStates.push_back(0);
	}

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
		{
			if (enteredLev){
				//prepare objects and verification of correct level
				if (inLev == 0)
				{
					if (!memcmp(comparedLev, (char*)0x00485674, 10))
					{
						cout << "Entered correct level" << endl;
						if (objects::obj.reload) objects::obj.clearVals();
						inLev = 1;
						newGame = 1;
					}
					else objects::obj.reload = 1;

					enteredLev = 0;

				}

				Pieces mPieces;

				// Board
				Board mBoard(&mPieces);

				// Game
				Game mGame(&mBoard, &mPieces);

				newGame = 0;

				long mTime1 = clock();
				//start tetris
				while (inLev)
				{
					delayDown++;
					timer = clock();
					hotKeys(mGame,mBoard);
					
					if (timer - lastTime > 16.6f)  //60fps
					{
						
						unsigned long mTime2 = clock();
						mBoard.clearApples();
						mGame.DrawScene();
						if ((mTime2 - mTime1) > WAIT_TIME)
						{
							
							if (mBoard.IsPossibleMovement(mGame.mPosX, mGame.mPosY + 1, mGame.mPiece, mGame.mRotation))
							{
								mGame.mPosY++;
							}
							else
							{
								mBoard.StorePiece(mGame.mPosX, mGame.mPosY, mGame.mPiece, mGame.mRotation);

								mBoard.DeletePossibleLines();

								if (mBoard.IsGameOver())
								{
									
										exitLev(0);
										checkExit = 0;
										cout << "game over" << endl;
									
									
								}
								
								mGame.CreateNewPiece();
							}
							
							mTime1 = clock();
						}

						
						lastTime = timer;
						
					}
					if (currentScreen != 1 && currentScreen != 3) 
					{
						inLev = 0; 
						exitLev(1);
						checkExit = 1;
					}
					Sleep(1);
				}
			}
			break;
		}
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