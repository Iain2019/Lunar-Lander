/////////////////////////////////////////////////////////////////////////
// File: Source.cpp
// Author: Iain Farlow
// Date Created: 22/11/2019
// Brief: This file contains the main game loop. 
//		  This includes the callingof the functions within Game.cpp.
//        These are all contained within a while loop and are restricted 
//        to five frames per second by a clock
/////////////////////////////////////////////////////////////////////////

#include <Windows.h>
#include <chrono>
#include "GameObjects.h"
#include "Game.h"

typedef std::chrono::steady_clock::time_point Time;
typedef std::chrono::high_resolution_clock HighResClock;
typedef std::chrono::duration<float> TimeDiff;

int main()
{
	Game gameInstance;

	gameInstance.Initialise();

	float deltaTime = 0.0f;
	Time currentFrameTime = HighResClock::now();
	Time previousFrameTime = HighResClock::now();

	while (!gameInstance.exitGame)
	{
		currentFrameTime = HighResClock::now();
		TimeDiff diff = currentFrameTime - previousFrameTime;
		deltaTime = diff.count();

		if (deltaTime >= (1.0f / FRAME_RATE))
		{
			previousFrameTime = currentFrameTime;

			gameInstance.Update(deltaTime);
		}

		gameInstance.Draw();
	}
	return 0;
}