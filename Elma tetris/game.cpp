/*****************************************************************************************
/* File: Game.cpp
/* Desc: General class for the game
/*
/* gametuto.com - Javier López López (javilop.com)
/*
/*****************************************************************************************
/*
/* Creative Commons - Attribution 3.0 Unported
/* You are free:
/*	to Share — to copy, distribute and transmit the work
/*	to Remix — to adapt the work
/*
/* Under the following conditions:
/* Attribution. You must attribute the work in the manner specified by the author or licensor
/* (but not in any way that suggests that they endorse you or your use of the work).
/*
/*****************************************************************************************/

// ------ Includes -----
#ifndef LINUX
#include <windows.h>
#endif
#include "Game.h"


/*
======================================
Init
======================================
*/
Game::Game(Board *pBoard, Pieces *pPieces)
{

	// Get the pointer to the Board and Pieces classes
	mBoard = pBoard;
	mPieces = pPieces;
	

	// Game initialization
	InitGame();
}


/*
======================================
Get a random int between to integers

Parameters:
>> pA: First number
>> pB: Second number
======================================
*/
int Game::GetRand(int pA, int pB)
{
	return rand() % (pB - pA + 1) + pA;
}


/*
======================================
Initial parameters of the game
======================================
*/
void Game::InitGame()
{
	// Init random numbers
	srand((unsigned int)time(NULL));

	// First piece
	mPiece = GetRand(0, 6);
	mRotation = GetRand(0, 3);
	mPosX = (BOARD_WIDTH / 2) + mPieces->GetXInitialPosition(mPiece, mRotation);
	mPosY = mPieces->GetYInitialPosition(mPiece, mRotation);

	//  Next piece
	mNextPiece = GetRand(0, 6);
	mNextRotation = GetRand(0, 3);
	mNextPosX = BOARD_WIDTH + 5;
	mNextPosY = 5;
}


/*
======================================
Create a random piece
======================================
*/
void Game::CreateNewPiece()
{
	// The new piece
	mPiece = mNextPiece;
	mRotation = mNextRotation;
	mPosX = (BOARD_WIDTH / 2) + mPieces->GetXInitialPosition(mPiece, mRotation);
	mPosY = mPieces->GetYInitialPosition(mPiece, mRotation);

	// Random next piece
	mNextPiece = GetRand(0, 6);
	mNextRotation = GetRand(0, 3);
}


/*
======================================
Draw piece

Parameters:

>> pX:		Horizontal position in blocks
>> pY:		Vertical position in blocks
>> pPiece:	Piece to draw
>> pRotation:	1 of the 4 possible rotations
======================================
*/
void Game::DrawPiece(int pX, int pY, int pPiece, int pRotation)
{
	for (int i = 0; i < PIECE_BLOCKS; i++)
	{
		for (int j = 0; j < PIECE_BLOCKS; j++)
		{
			if (mPieces->GetBlockType(pPiece, pRotation, j, i) != 0)
				objects::obj.objectActive(200 - ((pY+j) * 10) + pX+i,1);
			
		}
	}
	
}

void Game::DrawPiece2(int pX, int pY, int pPiece, int pRotation)
{
	for (int i = 0; i < PIECE_BLOCKS; i++)
	{
		for (int j = 0; j < PIECE_BLOCKS; j++)
		{
			if (mPieces->GetBlockType(pPiece, pRotation, j, i) != 0)
				objects::obj.objectActive(200+((i-1)*3+(j-1)),1);

		}
	}

}


/*
======================================
Draw board

Draw the two lines that delimit the board
======================================
*/
void Game::DrawBoard()
{
	
}


/*
======================================
Draw scene

Draw all the objects of the scene
======================================
*/
void Game::DrawScene()
{
	DrawBoard();													// Draw the delimitation lines and blocks stored in the board
	DrawPiece(mPosX, mPosY, mPiece, mRotation);					// Draw the playing piece
	DrawPiece2(0, 0, mNextPiece, mNextRotation);	// Draw the next piece
}
