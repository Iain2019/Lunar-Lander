/////////////////////////////////////////////////////////////////////////
// File: Utility.h
// Author: Iain Farlow
// Date Created: 02/12/2019
// Brief: This file contains the unility functions of the game.
//		  Thi allows for functions to be called that can be applied to multiple situations.
/////////////////////////////////////////////////////////////////////////

#ifndef UTILITY_H
#define UTILITY_H

#include <Windows.h>
#include <string>
#include "Constants.h"

static void WriteImageToBuffer(CHAR_INFO* consoleBuffer, const int CharsToPrint[], const int coloursToPrint[], const int imageHeight, const int imageWidth, int imageXpos, int imageYpos)
{
	for (int y = 0; y < imageHeight; ++y)
	{
		for (int x = 0; x < imageWidth; ++x)
		{
			consoleBuffer[(imageXpos + x) + SCREEN_WIDTH * (imageYpos + y)].Char.AsciiChar = CharsToPrint[x + imageWidth * y];
			if (coloursToPrint)
			{
				consoleBuffer[(imageXpos + x) + SCREEN_WIDTH * (imageYpos + y)].Attributes = coloursToPrint[x + imageWidth * y];
			}
			else
			{
				consoleBuffer[(imageXpos + x) + SCREEN_WIDTH * (imageYpos + y)].Attributes = 7;
			}
		}
	}
}

static void ClearScreen(CHAR_INFO* consoleBuffer)
{
	for (int i = 0; i < (SCREEN_WIDTH * SCREEN_HEIGHT); ++i)
	{
		consoleBuffer[i].Char.AsciiChar = 0;
		consoleBuffer[i].Attributes = 0;
	}
}

static void WriteTextToBuffer(CHAR_INFO* consoleBuffer, std::string stringToPrint, int textXpos, int textYpos)
{
	for (int x = 0; x < stringToPrint.length(); ++x)
	{
		consoleBuffer[(textXpos + x) + SCREEN_WIDTH * textYpos].Char.AsciiChar = stringToPrint[x];
		consoleBuffer[(textXpos + x) + SCREEN_WIDTH * textYpos].Attributes = 0xf;
	}
}

static int ClampInt(int intToClamp, int lowerLimit, int upperLimit)
{
	if (intToClamp <= lowerLimit)
	{
		return lowerLimit;
	}
	else if (intToClamp >= upperLimit)
	{
		return upperLimit;
	}
	else
	{
		return intToClamp;
	}
}

static float ClampFloat(float floatToClamp, float lowerLimit, float upperLimit)
{
	if (floatToClamp <= lowerLimit)
	{
		return lowerLimit;
	}
	else if (floatToClamp >= upperLimit)
	{
		return upperLimit;
	}
	else
	{
		return floatToClamp;
	}
}   

#endif //!UTILITY_H