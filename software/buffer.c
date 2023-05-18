//-----------------------------------------------------------------------------
// Basic circular buffer for storing and averaging ADC readings
//
// Copyright 2023 Alan Reed (areed.me)
//-----------------------------------------------------------------------------

#include "buffer.h"

#include "hardware/sync.h"
#include "stdlib.h"

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

void buffer_init(buffer_t *buffer) {
  buffer->write_index = 0;

  for (int i = 0; i < BUFFER_SIZE; i++) {
    buffer->values[i] = 0;
  }
}

void buffer_write(buffer_t *buffer, float value) {
  uint32_t interrupt_status = save_and_disable_interrupts();

  buffer->values[buffer->write_index % BUFFER_SIZE] = value;
  buffer->write_index++;

  restore_interrupts(interrupt_status);
}

float buffer_average(buffer_t *buffer) {
  float sum = 0;

  uint32_t interrupt_status = save_and_disable_interrupts();
  for (int i = 0; i < BUFFER_SIZE; i++) {
    sum += buffer->values[i];
  }
  restore_interrupts(interrupt_status);

  return sum / BUFFER_SIZE;
}
