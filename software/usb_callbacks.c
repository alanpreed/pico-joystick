//-----------------------------------------------------------------------------
// USB callback function definitions required by TinyUSB
//
// Copyright 2023 Alan Reed (areed.me)
//-----------------------------------------------------------------------------

#include "tusb.h"
#include "usb_descriptors.h"

//-----------------------------------------------------------------------------
// Private constants
//-----------------------------------------------------------------------------

#define USB_STRING_DESCRIPTOR_MAX_LEN 31

//-----------------------------------------------------------------------------
// Private variables
//-----------------------------------------------------------------------------

// Holding array for UTF-16 reformatted string descriptors.
// Extra entry stores length and type header
static uint16_t string_descriptor_utf16[USB_STRING_DESCRIPTOR_MAX_LEN + 1];

//-----------------------------------------------------------------------------
// Mandatory HID callbacks declared in TinyUSB's hid_device.h
//-----------------------------------------------------------------------------

// Invoked when the device receives GET HID REPORT DESCRIPTOR
// Should return a pointer to the HID report descriptor
// (contents must exist long enough for transfer to complete)
uint8_t const* tud_hid_descriptor_report_cb(uint8_t instance) {
  return desc_hid_report;
}

// Invoked when the device receives a GET_REPORT control request
// Should fill the buffer report's content and return its length.
// Returning zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
  return 0;
}

// Invoked when the device receives a SET_REPORT control request or
// receives data on an OUT endpoint (Report ID = 0, Type = 0)
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {}

//-----------------------------------------------------------------------------
// Mandatory descriptor callbacks declared in TinyUSB's usbd.h
//-----------------------------------------------------------------------------

// Invoked when the device receives GET DEVICE DESCRIPTOR
// Should return a pointer to the device descriptor
uint8_t const* tud_descriptor_device_cb(void) {
  return (uint8_t const*)&desc_device;
}

// Invoked when the device receives GET CONFIGURATION DESCRIPTOR
// Should return a pointer to the configuration descriptor
// (contents must exist long enough for the transfer to complete)
uint8_t const* tud_descriptor_configuration_cb(uint8_t index) {
  return desc_configuration;
}

// Invoked when the device receives GET STRING DESCRIPTOR request
// Should return a pointer to the string descriptor
// (contents must exist long enough for the transfer to complete)
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
  if (!(index < sizeof(string_descriptors) / sizeof(string_descriptors[0]))) {
    // Requested string descriptor out of range
    return NULL;
  }

  uint8_t descriptor_length_bytes = 2;
  const char* descriptor = string_descriptors[index];

  if (index == 0) {
    // 2-byte language ID is not encoded as UTF-16, so copy this directly
    memcpy(&string_descriptor_utf16[1], descriptor, 2);
  } else {
    // Make sure descriptor fits in buffer
    uint8_t string_length = (uint8_t)strlen(descriptor);
    if (string_length > USB_STRING_DESCRIPTOR_MAX_LEN) {
      string_length = USB_STRING_DESCRIPTOR_MAX_LEN;
    }

    // Convert ASCII string into UTF-16
    for (uint8_t i = 0; i < string_length; i++) {
      string_descriptor_utf16[1 + i] = descriptor[i];
      descriptor_length_bytes += 2;
    }
  }

  // First byte is the descriptor length in bytes (including header)
  // Second byte is the descriptor type (0x03 for string descriptor)
  string_descriptor_utf16[0] = (uint16_t)((TUSB_DESC_STRING << 8) | descriptor_length_bytes);

  return string_descriptor_utf16;
}
