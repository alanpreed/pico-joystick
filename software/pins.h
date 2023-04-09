//-----------------------------------------------------------------------------
// Hardware connection definitions for the Pi Pico
//
// Copyright 2023 Alan Reed (areed.me)
//-----------------------------------------------------------------------------

#ifndef __PINS_H__
#define __PINS_H__

//-----------------------------------------------------------------------------
// Public constants
//-----------------------------------------------------------------------------

// Gameport joystick connections
#define JOYSTICK_BUTTON_1_PIN 16 // Gameport pin 2
#define JOYSTICK_BUTTON_2_PIN 17 // Gameport pin 7
#define JOYSTICK_AXIS_X_PIN 26   // Gameport pin 3
#define JOYSTICK_AXIS_Y_PIN 27   // Gameport pin 6

// Pi Pico built-in LED
#define LED_PIN 25

#endif // __PINS_H__
