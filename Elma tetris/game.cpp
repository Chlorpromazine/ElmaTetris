#include "game.h"

int Game::GetRand(int pA, int pB)
{
	return rand() % (pB - pA + 1) + pA;
}

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

//edit
void Game::DrawPiece(int pX, int pY, int pPiece, int pRotation)
{

}

void Game::DrawScene()
{
	DrawPiece(mPosX, mPosY, mPiece, mRotation);                    // Draw the playing piece
	DrawPiece(mNextPosX, mNextPosY, mNextPiece, mNextRotation);    // Draw the next piece
}