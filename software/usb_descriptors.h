//-----------------------------------------------------------------------------
// USB descriptor datastructures required by TinyUSB
// 
// Copyright 2023 Alan Reed (areed.me)
//-----------------------------------------------------------------------------

#ifndef __USB_DESCRIPTORS_H__
#define __USB_DESCRIPTORS_H__

#include "tusb.h"
#include "usb_hid.h"

//-----------------------------------------------------------------------------
// Device descriptor constants
//-----------------------------------------------------------------------------

#define USB_VID   0x7EED
#define USB_PID   0x0001

//-----------------------------------------------------------------------------
// Device descriptor
//-----------------------------------------------------------------------------

tusb_desc_device_t const desc_device =
{
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200, // Supports up to USB 2.0
    .bDeviceClass       = 0x00,   // HID device class defined in Interface descriptor
    .bDeviceSubClass    = 0x00,   // No boot interface subclass
    .bDeviceProtocol    = 0x00,   // No boot interface protocol
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor           = USB_VID,
    .idProduct          = USB_PID,
    .bcdDevice          = 0x0100, // Device release number 1.0

    .iManufacturer      = 0x01,   // String descriptor indices
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,

    .bNumConfigurations = 0x01    // Only 1 configuration descriptor
};


//-----------------------------------------------------------------------------
// HID report descriptor
//-----------------------------------------------------------------------------

// Custom HID report descriptor, for a joystick with 2 axes and 2 buttons,
// based upon the example templates in TinyUSB's hid_device.h.
// Should match report struct definition in usb_hid.h
#define TUD_HID_REPORT_DESC_JOYSTICK(...)                                                 \
    HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),                                               \
        HID_USAGE(HID_USAGE_DESKTOP_JOYSTICK),                                            \
        HID_COLLECTION(HID_COLLECTION_APPLICATION), /* Report ID if any */                \
        __VA_ARGS__                                                                       \
        /* 8 bits each for X and Y axes, minimum value -128 (0x80), maximum 127 (0x7f) */ \
        HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),                                           \
        HID_USAGE(HID_USAGE_DESKTOP_X),                                                   \
        HID_USAGE(HID_USAGE_DESKTOP_Y),                                                   \
        HID_LOGICAL_MIN(0x80),                                                            \
        HID_LOGICAL_MAX(0x7f),                                                            \
        HID_REPORT_COUNT(2),                                                              \
        HID_REPORT_SIZE(8),                                                               \
        HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),                                \
        /* 2 bit button map */                                                            \
        HID_USAGE_PAGE(HID_USAGE_PAGE_BUTTON),                                            \
        HID_USAGE_MIN(1),                                                                 \
        HID_USAGE_MAX(2),                                                                 \
        HID_LOGICAL_MIN(0),                                                               \
        HID_LOGICAL_MAX(1),                                                               \
        HID_REPORT_COUNT(2),                                                              \
        HID_REPORT_SIZE(1),                                                               \
        HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),                                \
        /* 6 bit padding to bring up to a whole byte */                                   \
        HID_REPORT_COUNT(1),                                                              \
        HID_REPORT_SIZE(6),                                                               \
        HID_INPUT(HID_CONSTANT),                                                          \
        HID_COLLECTION_END

uint8_t const desc_hid_report[] = {
  TUD_HID_REPORT_DESC_JOYSTICK()
};


//-----------------------------------------------------------------------------
// Configuration descriptor constants
//-----------------------------------------------------------------------------

#define CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)
#define POWER_CONSUMPTION_MA 100

// Bit 0..3 The endpoint number
// Bit 4..6 Reserved, reset to zero
// Bit 7 Direction, ignored for Control endpoints
// 0 - OUT endpoint
// 1 - IN endpoint
#define ENDPOINT_IN_ADDRESS  01 | 1 << 7   // Endpoint 1, direction IN
#define STRING_DESCRIPTOR_INDEX 0

// Interface descriptor numbers
enum
{
  INTERFACE_NUM_HID,
  INTERFACE_NUM_TOTAL
};

//-----------------------------------------------------------------------------
// Configuration descriptor
//-----------------------------------------------------------------------------

uint8_t const desc_configuration[] = {
  TUD_CONFIG_DESCRIPTOR(1,                        // Configuration number
                        INTERFACE_NUM_TOTAL,      // Total interface count
                        STRING_DESCRIPTOR_INDEX,  // String descriptor index
                        CONFIG_TOTAL_LEN,         // Config descriptor length
                        0,                        // Attributes bitfield
                        POWER_CONSUMPTION_MA),    // Power consumption

  // Interface descriptor, HID descriptor, and endpoint descriptor are all generated by this macro
  TUD_HID_DESCRIPTOR(INTERFACE_NUM_HID,         // Interface number
                     STRING_DESCRIPTOR_INDEX,   // String descriptor index,
                     HID_ITF_PROTOCOL_NONE,     // Protocol
                     sizeof(desc_hid_report),   // Report descriptor length
                     ENDPOINT_IN_ADDRESS,       // Endpoint In address
                     CFG_TUD_HID_EP_BUFSIZE,    // Size of HID endpoint
                     USB_HID_POLL_INTERVAL_MS)  // Polling interval
};

//-----------------------------------------------------------------------------
// String descriptor
//-----------------------------------------------------------------------------

char const* string_descriptors [] =
{
  (const char[]) { 0x09, 0x08 }, // 0: Supported language is UK English (0x0809)
  "AReed",                       // 1: Manufacturer name
  "PicoJoystick",                // 2: Product name
  "00001",                       // 3: Serial number
};

#endif // __USB_DESCRIPTORS_H__
