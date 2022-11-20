/////////////////////////////////////////////////////////////////////////
// File: Constants.h
// Author: Iain Farlow
// Date Created: 02/12/2019
// Brief: This file contains the constants for the game. Such as keys and rates.
/////////////////////////////////////////////////////////////////////////

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Windows.h>

#define SCREEN_WIDTH 150
#define SCREEN_HEIGHT 40
#define FRAME_RATE 5

const int KEY_ESK = VK_ESCAPE;
const int KEY_ENTER = VK_RETURN;
const int KEY_W = 'W';
const int KEY_A = 'A';
const int KEY_S = 'S';
const int KEY_D = 'D';
const int KEY_1 = '1';
const int KEY_2 = '2';
const int KEY_3 = '3';
const int KEY_4 = '4';
const int KEY_5 = '5';

const float ACCELERATION_RATE = 0.5f;
const float DECLERATION_RATE = 0.4f;
const float ASSENT_FUEL_RATE = 10.0f;
const float MANEUVERING_FUEL_RATE = 5.0f;

#endif //!CONSTANTS_H