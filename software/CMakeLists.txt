#-----------------------------------------------------------------------------
# Raspberry Pi Pico analogue gameport joystick adapter
#
# Copyright 2023 Alan Reed (areed.me)
#-----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
project(pico-joystick C CXX ASM)
pico_sdk_init()

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/main.c
        ${CMAKE_CURRENT_LIST_DIR}/usb_callbacks.c
        ${CMAKE_CURRENT_LIST_DIR}/usb_hid.c
        ${CMAKE_CURRENT_LIST_DIR}/joystick.c
        )

# Make sure TinyUSB can find tusb_config.h
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})

# Extra libraries:
# pico_stdlib    (common PicoSDK functions)
# hardware_adc   (PicoSDK ADC support)
# tinyusb_device (USB device support)
target_link_libraries(${PROJECT_NAME} PUBLIC pico_stdlib hardware_adc tinyusb_device)

# Generate additional build output, including a uf2 file
pico_add_extra_outputs(${PROJECT_NAME})

# Enable printf output via UART0
pico_enable_stdio_uart(${PROJECT_NAME} 1)
