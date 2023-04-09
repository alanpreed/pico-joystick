//-----------------------------------------------------------------------------
// Module for handling USB HID report generation
//
// Copyright 2023 Alan Reed (areed.me)
//-----------------------------------------------------------------------------

#ifndef __USB_HID_H__
#define __USB_HID_H__

#include <stdint.h>

//-----------------------------------------------------------------------------
// Public constants
//-----------------------------------------------------------------------------

#define USB_HID_POLL_INTERVAL_MS 10

//-----------------------------------------------------------------------------
// Public types
//-----------------------------------------------------------------------------

// HID joystick protocol report, matching descriptor in usb_descriptors.h
typedef struct TU_ATTR_PACKED
{
  int8_t  x;         // 8-bit X axis data (-128 to 127)
  int8_t  y;         // 8-bit Y axis data (-128 to 127)
  uint8_t buttons;   // 2-bit button mask plus 6 bits of padding
}hid_joystick_report_t;


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

// Initialises a timer for requesting the HID reports
void usb_init(void);

// Task that generates a HID report for the joystick at the requested interval
void usb_task(void);

#endif  // __USB_HID_H__
