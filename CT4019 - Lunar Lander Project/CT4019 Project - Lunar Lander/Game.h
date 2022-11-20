/////////////////////////////////////////////////////////////////////////
// File: Game.h
// Author: Iain Farlow
// Date Created: 02/12/2019
// Brief: This file contains the gamestate of thwe game, as well as the variabales required for the console buffer
//		  variabales required for the console buffer.
/////////////////////////////////////////////////////////////////////////

#ifndef GAME_H
#define GAME_H

#include <Windows.h>
#include "GameObjects.h"

class Game
{
public:
	Game();
	void Initialise();
	void Update(float deltaTime);
	void Draw();

	bool exitGame = false;

private:
	enum GAME_STATE
	{
		SPLASH,
		MENU,
		PLAY,
		OPTIONS,
		GAME_OVER
	};

	enum DIFFICULTY
	{
		EASY,
		MEDIUM,
		HARD
	};

	SMALL_RECT windowSize{ 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };

	COORD bufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };

	COORD characterBufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	COORD characterPosition = { 0 , 0 };
	SMALL_RECT consoleWriteArea = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };

	CHAR_INFO consoleBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];

	HANDLE wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE rHnd = GetStdHandle(STD_INPUT_HANDLE);

	GAME_STATE currentGameState = SPLASH;
	DIFFICULTY currentDifficulty = EASY;
	Background background;
	Player player;
};

#endif // !GAME_H