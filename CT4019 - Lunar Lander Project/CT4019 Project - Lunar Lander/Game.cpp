/////////////////////////////////////////////////////////////////////////
// File: Game.cpp
// Author: Iain Farlow
// Date Created: 22/11/2019
// Brief: This file contains the functionality of the game.
//		  This include the update function, that is called in Source.cpp
//        as well as the the initilize and draw functions.
/////////////////////////////////////////////////////////////////////////



#include "Game.h"
#include "Utility.h"
#include "Constants.h"
#include "GameObjects.h"
#include <fstream>
#include <chrono>
#include <iostream>

Game::Game() 
{

}

void Game::Initialise()
{
	SetConsoleTitle("Lunar Lander");

	SetConsoleScreenBufferSize(wHnd, bufferSize);

	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	background.bkgrndRand = std::rand() % 2;
}

void Game::Update(float deltaTime)
{
	exitGame = false;

	switch (currentGameState)
	{
		case SPLASH:
		{
			static Splash splash;
			splash.duration += deltaTime;

			if (splash.duration < 3.0f)
			{
				WriteImageToBuffer(consoleBuffer, splash.CHARACTERS, splash.COLOURS, splash.HEIGHT, splash.WIDTH,
					(SCREEN_WIDTH / 2) - (splash.WIDTH / 2), (SCREEN_HEIGHT / 2) - (splash.HEIGHT / 2));
			}
			else
			{
				splash.duration = 0.0f;
				currentGameState = MENU;
			}

			break;
		}

		case MENU:
		{
			ClearScreen(consoleBuffer);

			//open save data
			player.TakeHighScoreData.open("HighScoreVales.txt");
			player.TakeHighScoreData >> player.highScore;
			player.TakeHighScoreData.close();
			player.TakeHighTimeData.open("HighTimeValues.txt");
			player.TakeHighTimeData >> player.highTime;
			player.TakeHighTimeData.close();
			player.TakeHighFuelData.open("HighFuelValues.txt");
			player.TakeHighFuelData >> player.highfuel;
			player.TakeHighFuelData.close();


			WriteTextToBuffer(consoleBuffer, "1. Play", (SCREEN_WIDTH / 2) - 12, (SCREEN_HEIGHT / 2) - 2);
			WriteTextToBuffer(consoleBuffer, "2. Options", (SCREEN_WIDTH / 2) - 12, (SCREEN_HEIGHT / 2) - 1);
			WriteTextToBuffer(consoleBuffer, "Highscore: " + std::to_string(player.highScore), (SCREEN_WIDTH / 2) - 12, (SCREEN_HEIGHT / 2));
			WriteTextToBuffer(consoleBuffer, "Fuel Remaining: " + std::to_string(player.highfuel) + "    Time Taken: " + std::to_string(player.highTime), (SCREEN_WIDTH / 2) - 12, (SCREEN_HEIGHT / 2) + 1);
			WriteTextToBuffer(consoleBuffer, "3. Quit", (SCREEN_WIDTH / 2) - 12, (SCREEN_HEIGHT / 2) + 2);

			if (GetAsyncKeyState(KEY_1))
			{
				currentGameState = PLAY;
			}
			else if (GetAsyncKeyState(KEY_2))
			{
				currentGameState = OPTIONS;
			}
			else if (GetAsyncKeyState(KEY_3))
			{
				exitGame = true;
			}

			break;
		}

		case PLAY:
		{
			//difficulty
			switch (currentDifficulty)
			{
				case EASY:
				{
					player.reFuelAmount = 50;
					player.difMultiplier = 1;
					break;
				}
				case MEDIUM:
					player.reFuelAmount = 40;
					player.difMultiplier = 2;
					break;
				case HARD:
					player.reFuelAmount = 30;
					player.difMultiplier = 3;
					break;
				default:
					break;
			}
			player.gameTimer += 0.2f;
			if (!player.hasCrashed)
			{
				if (GetAsyncKeyState(KEY_ESK))
				{
					exitGame = true;
				}
				if (GetAsyncKeyState(KEY_W) && player.fuelAmount > 0.0f)
				{
					player.movingUp = true;
					player.isAccelerating = true;
				}
				//movmenet fuel
				if (GetAsyncKeyState(KEY_A) && player.fuelAmount > 0.0f)
				{
					player.movingLeft = true;
					player.fuelAmount -= (MANEUVERING_FUEL_RATE * deltaTime);
					--player.Xpos;
				}
				if (GetAsyncKeyState(KEY_D) && player.fuelAmount > 0.0f)
				{
					player.movingRight = true;
					player.fuelAmount -= (MANEUVERING_FUEL_RATE * deltaTime);
					++player.Xpos;
				}

				if (player.isAccelerating)
				{
					//assent fuel
					player.landerAcceleration += (ACCELERATION_RATE * deltaTime);
					player.fuelAmount -= (ASSENT_FUEL_RATE * deltaTime);
				}
				else
				{
					player.landerAcceleration -= (DECLERATION_RATE * deltaTime);
				}

				player.isAccelerating = false;

				player.landerAcceleration = ClampFloat(player.landerAcceleration, 0.0f, 1.5f);

				if (player.landerAcceleration >= 1.0f)
				{
					player.Ypos--;
				}
				else if (player.landerAcceleration < 0.5f)
				{
					player.Ypos++;
				}

				player.Xpos = ClampInt(player.Xpos, 0, (SCREEN_WIDTH - player.WIDTH));
				player.Ypos = ClampInt(player.Ypos, 0, (SCREEN_HEIGHT - player.HEIGHT));

				//random background
				if (background.bkgrndRand == 0)
				{
					background.bottomLeftChar = background.CHARACTERS[player.Xpos + SCREEN_WIDTH * (player.Ypos + (player.HEIGHT - 1))];
					background.bottomRightChar = background.CHARACTERS[(player.Xpos + (player.WIDTH - 1)) + SCREEN_WIDTH * (player.Ypos + (player.HEIGHT - 1))];
					background.scoreToAdd = background.CHARACTERS[(player.Xpos + (player.WIDTH - 1)) + SCREEN_WIDTH * (player.Ypos + (player.HEIGHT))];
				}
				if (background.bkgrndRand == 1)
				{
					background.bottomLeftChar = background.CHARACTERSTWO[player.Xpos + SCREEN_WIDTH * (player.Ypos + (player.HEIGHT - 1))];
					background.bottomRightChar = background.CHARACTERSTWO[(player.Xpos + (player.WIDTH - 1)) + SCREEN_WIDTH * (player.Ypos + (player.HEIGHT - 1))];
					background.scoreToAdd = background.CHARACTERSTWO[(player.Xpos + (player.WIDTH - 1)) + SCREEN_WIDTH * (player.Ypos + (player.HEIGHT))];
				}
				

				if (background.bottomLeftChar == '_' && background.bottomRightChar == '_')
				{
					//lander detection
					player.landerAcceleration = 0.9f;
					if (background.scoreToAdd == '2' && player.landedOnce)
					{
						player.score += (2 * player.difMultiplier);
						player.fuelAmount += player.reFuelAmount;
						player.landedOnce = false;
					}
					if (background.scoreToAdd == '3' && player.landedOnce)
					{
						player.score += (3 * player.difMultiplier);
						player.fuelAmount += player.reFuelAmount;
						player.landedOnce = false;
					}
					if (background.scoreToAdd == '4' && player.landedOnce)
					{
						player.score += (4 * player.difMultiplier);
						player.fuelAmount += player.reFuelAmount;
						player.landedOnce = false;
					}
					if (background.scoreToAdd == '5' && player.landedOnce)
					{
						player.score += (5 * player.difMultiplier);
						player.fuelAmount += player.reFuelAmount;
						player.landedOnce = false;
					}
				}
				else if (background.bottomLeftChar != ' ' || background.bottomRightChar != ' ')
				{
					player.hasCrashed = true;
					currentGameState = GAME_OVER;
				}
				else 
				{
					player.landedOnce = true;
				}
			}

			ClearScreen(consoleBuffer);

			if (background.bkgrndRand == 0)
			{
				WriteImageToBuffer(consoleBuffer, background.CHARACTERS, nullptr, SCREEN_HEIGHT, SCREEN_WIDTH, 0, 0);
			}
			if (background.bkgrndRand == 1)
			{
				WriteImageToBuffer(consoleBuffer, background.CHARACTERSTWO, nullptr, SCREEN_HEIGHT, SCREEN_WIDTH, 0, 0);
			}

			//write boosters
			WriteImageToBuffer(consoleBuffer, player.CHARACTERS, player.COLOURS, player.HEIGHT, player.WIDTH, player.Xpos, player.Ypos);
			if (player.movingRight)
			{
				WriteImageToBuffer(consoleBuffer, player.MANEUVERINGBOOSTERSL, player.MANEUVERINGBOOSTERSLCOLOURL, player.MANBOOSTERSHEIGHT, player.MANBOOSTERSWIDTH, player.Xpos - 2, player.Ypos + 1);
				player.movingRight = false;
			}
			if (player.movingLeft)
			{
				WriteImageToBuffer(consoleBuffer, player.MANEUVERINGBOOSTERSR, player.MANEUVERINGBOOSTERSRCOLOURR, player.MANBOOSTERSHEIGHT, player.MANBOOSTERSWIDTH, player.Xpos + 2, player.Ypos + 1);
				player.movingLeft = false;
			}
			if (player.movingUp)
			{
				WriteImageToBuffer(consoleBuffer, player.BOOSTERS, player.BOOSTERSLCOLOUR, player.BOOSTERSHEIGHT, player.BOOSTERSWIDTH, player.Xpos, player.Ypos + 3);
				player.movingUp = false;
			}

			WriteTextToBuffer(consoleBuffer, "Score: " + std::to_string(player.score), 1, 0);
			WriteTextToBuffer(consoleBuffer, "Time: " + std::to_string(player.gameTimer), 1, 1);
			WriteTextToBuffer(consoleBuffer, "Fuel: " + std::to_string(player.fuelAmount), 1, 2);

			break;
		}

		case OPTIONS:
		{
			ClearScreen(consoleBuffer);

			WriteTextToBuffer(consoleBuffer, "1. SET GAMEMODE TO EASY", (SCREEN_WIDTH / 2) - 7, (SCREEN_HEIGHT / 2) - 2);
			WriteTextToBuffer(consoleBuffer, "2. SET GAMEMODE TO MEDIUM", (SCREEN_WIDTH / 2) - 7, (SCREEN_HEIGHT / 2) - 1);
			WriteTextToBuffer(consoleBuffer, "3. SET GAMEMODE TO HARD", (SCREEN_WIDTH / 2) - 7, (SCREEN_HEIGHT / 2));
			WriteTextToBuffer(consoleBuffer, "4. RESET HIGHSCORE", (SCREEN_WIDTH / 2) - 7, (SCREEN_HEIGHT / 2 + 1));
			WriteTextToBuffer(consoleBuffer, "5. BACK", (SCREEN_WIDTH / 2) - 7, (SCREEN_HEIGHT / 2) + 2);

			if (GetAsyncKeyState(KEY_1))
			{
				currentDifficulty = EASY;
			}
			else if (GetAsyncKeyState(KEY_2))
			{
				currentDifficulty = MEDIUM;
			}
			else if (GetAsyncKeyState(KEY_3))
			{
				currentDifficulty = HARD;
			}
			else if (GetAsyncKeyState(KEY_4))
			{
				player.GiveHighScoreData.open("HighScoreVales.txt");
				player.GiveHighScoreData << 0;
				player.GiveHighScoreData.close();
				player.GiveHighTimeData.open("HighTimeValues.txt");
				player.GiveHighTimeData << 0;
				player.GiveHighTimeData.close();
				player.GiveHighFuelData.open("HighFuelValues.txt");
				player.GiveHighFuelData << 0;
				player.GiveHighFuelData.close();
			}
			else if (GetAsyncKeyState(KEY_5))
			{
				currentGameState = MENU;
			}
			break;
		}
		case GAME_OVER:
		{
			//writes explosion
			WriteImageToBuffer(consoleBuffer, player.EXPLOSIONFIRST, player.EXPLOSIONCOLOURSFIRST, player.EXPLOSIONHEIGHT, player.EXPLOSIONWIDTH,
				(player.Xpos - 2), (player.Ypos - 2));
			player.gameTime = player.gameTimer;
			if (player.gameTime % 2 == 0)
			{
				WriteImageToBuffer(consoleBuffer, player.EXPLOSIONFIRST, player.EXPLOSIONCOLOURSSECOND, player.EXPLOSIONHEIGHT, player.EXPLOSIONWIDTH,
					(player.Xpos - 2), (player.Ypos - 2));
			}
			WriteTextToBuffer(consoleBuffer, "HOUSTON, WE HAVE A PROBLEM!", (SCREEN_WIDTH / 2) - 14, (SCREEN_HEIGHT / 2));
			player.gameTimer += 0.2f;
			if (GetAsyncKeyState(KEY_ESK))
			{
				exitGame = true;
			}
			if (GetAsyncKeyState(KEY_ENTER))
			{
				player.Reset();
				background.Reset();
				currentGameState = MENU;
			}
			else if (player.hasCrashed)
			{
				if (player.score > player.highScore) 
				{
					player.highScore = player.score;
					player.highTime = player.gameTimer;
					player.highfuel = player.fuelAmount;

					//save data
					player.GiveHighScoreData.open("HighScoreVales.txt");
					player.GiveHighScoreData << player.highScore;
					player.GiveHighScoreData.close();
					player.GiveHighTimeData.open("HighTimeValues.txt");
					player.GiveHighTimeData << player.highTime;
					player.GiveHighTimeData.close();
					player.GiveHighFuelData.open("HighFuelValues.txt");
					player.GiveHighFuelData << player.highfuel;
					player.GiveHighFuelData.close();

					//display data
					WriteTextToBuffer(consoleBuffer, "NEW HIGH SCORE!! " + std::to_string(player.score), (SCREEN_WIDTH / 2) - 14, 1);
					WriteTextToBuffer(consoleBuffer, "TIME TAKEN!! " + std::to_string(player.gameTimer), (SCREEN_WIDTH / 2) - 14, 2);
					WriteTextToBuffer(consoleBuffer, "FUEL REMAINING!! " + std::to_string(player.fuelAmount), (SCREEN_WIDTH / 2) - 14, 3);
				}
			}

			break;
		}
		default:
		{
			break;
		}
	}
}

void Game::Draw()
{
	WriteConsoleOutputA(wHnd, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
}