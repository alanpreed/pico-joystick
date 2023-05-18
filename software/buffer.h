//-----------------------------------------------------------------------------
// Basic circular buffer for storing and averaging ADC readings
//
// Copyright 2023 Alan Reed (areed.me)
//-----------------------------------------------------------------------------

#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "stdbool.h"
#include "stdint.h"

//-----------------------------------------------------------------------------
// Public constants
//-----------------------------------------------------------------------------

#define BUFFER_SIZE 10

//-----------------------------------------------------------------------------
// Public types
//-----------------------------------------------------------------------------

typedef struct {
  uint32_t write_index;
  float values[BUFFER_SIZE];
} buffer_t;

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

// Set the buffer entries to defined  values
void buffer_init(buffer_t *buffer);

// Writes the provided value to the oldest slot in the buffer
void buffer_write(buffer_t *buffer, float value);

// Calculate the average of all entries in the buffer
float buffer_average(buffer_t *buffer);

#endif  // __BUFFER_H__
