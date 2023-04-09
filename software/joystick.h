//-----------------------------------------------------------------------------
// Interrupt-driven module for monitoring analogue joystick state
//
// Copyright 2023 Alan Reed (areed.me)
//-----------------------------------------------------------------------------

#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include "stdint.h"
#include "stdbool.h"

//-----------------------------------------------------------------------------
// Public constants
//-----------------------------------------------------------------------------

#define JOYSTICK_AXIS_CENTRE_RESISTANCE 55000

//-----------------------------------------------------------------------------
// Public types
//-----------------------------------------------------------------------------

typedef struct {
  bool button_1;
  bool button_2;
  float x_axis;
  float y_axis;
} joystick_state_t;

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

// Initialise the joystick module and begin monitoring state
void joystick_init();

// Populate a struct with the current state of the joystick
void joystick_read(joystick_state_t *state_buffer);

// Convert a joystick axis value to an 8-bit integer
int8_t joystick_rescale_axis(float value);

#endif // __JOYSTICK_H__
