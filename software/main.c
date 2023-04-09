//-----------------------------------------------------------------------------
// Raspberry Pi Pico analogue gameport joystick adapter
//
// Copyright 2023 Alan Reed (areed.me)
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#include "pico/time.h"
#include "tusb.h"
#include "joystick.h"
#include "usb_hid.h"

//-----------------------------------------------------------------------------
// Private constants
//-----------------------------------------------------------------------------

#define DEBUG_PRINT_INTERVAL_MS 1000

//-----------------------------------------------------------------------------
// Private variables
//-----------------------------------------------------------------------------

static joystick_state_t joystick = {0, 0, 0, 0};
static struct repeating_timer debug_print_timer;
static bool debug_print_output = false;

//-----------------------------------------------------------------------------
// Private functions
//-----------------------------------------------------------------------------

static bool debug_print_timer_callback(struct repeating_timer *t) {
  debug_print_output = true;
}

static void debug_print_init(void) {
  add_repeating_timer_ms(DEBUG_PRINT_INTERVAL_MS, &debug_print_timer_callback, NULL, &debug_print_timer);
}

static void debug_print_task(void) {
  if (debug_print_output) {
    debug_print_output = false;
    joystick_read(&joystick);

    printf("Raw joystick values: X: %f Y: %f B1: %d B2: %d\n",
           joystick.x_axis, joystick.y_axis, joystick.button_1, joystick.button_2);

    printf("Rescaled joystick axes: X: %d Y: %d \n",
           joystick_rescale_axis(joystick.x_axis), joystick_rescale_axis(joystick.y_axis));
  }
}

//-----------------------------------------------------------------------------
// Main entry point
//-----------------------------------------------------------------------------

int main(void) {
  stdio_init_all();
  tusb_init();
  joystick_init();
  usb_init();
  debug_print_init();

  printf("Raspberry Pi Pico gameport joystick USB adapter\n");
  printf("Copyright 2023 Alan Reed (areed.me)\n");
  printf("\n");

  while (1) {
    tud_task();
    usb_task();
    debug_print_task();
  }

  return 0;
}
