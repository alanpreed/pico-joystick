//-----------------------------------------------------------------------------
// Module for handling USB HID report generation
//
// Copyright 2023 Alan Reed (areed.me)
//-----------------------------------------------------------------------------

#include "usb_hid.h"

#include <math.h>

#include "joystick.h"
#include "pico/time.h"
#include "tusb.h"

//-----------------------------------------------------------------------------
// Private variables
//-----------------------------------------------------------------------------
static joystick_state_t joystick = {0, 0, 0, 0};

static struct repeating_timer hid_report_timer;
static bool send_hid_report = false;

//-----------------------------------------------------------------------------
// Private functions
//-----------------------------------------------------------------------------
static bool hid_report_timer_callback(struct repeating_timer *t) {
  send_hid_report = true;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
void usb_init(void) {
  add_repeating_timer_ms(USB_HID_POLL_INTERVAL_MS, &hid_report_timer_callback, NULL, &hid_report_timer);
}

void usb_task(void) {
  if (send_hid_report) {
    send_hid_report = false;

    joystick_read(&joystick);

    if (tud_hid_ready()) {
      hid_joystick_report_t report = {
          joystick_rescale_axis(joystick.x_axis),
          joystick_rescale_axis(joystick.y_axis),
          joystick.button_1 | joystick.button_2 << 1};

      tud_hid_report(0, &report, sizeof(report));
    }
  }
}
