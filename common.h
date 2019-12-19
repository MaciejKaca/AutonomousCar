#ifndef BUTTONS_H
#define BUTTONS_H

#include <cstdint>

const uint8_t GAMEPAD_REFRESH_TIME = 1; //Time in microseconds

const uint8_t EXIT_BY_BUTTON = 1;
const uint8_t EXIT_BY_MISSING_GAMEPAD = 2;

const bool BUTTON_DOWN = true;
const bool BUTTON_UP = false;

const uint8_t NUMBER_OF_BUTTONS = 10;
const uint8_t NUMBER_OF_AXIS = 7;

const uint8_t XBOX_BUTTON = 8;


#endif // BUTTONS_H
