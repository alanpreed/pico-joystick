//-----------------------------------------------------------------------------
// Interrupt-driven module for monitoring analogue joystick state
//
// Copyright 2023 Alan Reed (areed.me)
//-----------------------------------------------------------------------------

#include "joystick.h"

#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "pico/stdlib.h"
#include "pins.h"

//-----------------------------------------------------------------------------
// Private constants
//-----------------------------------------------------------------------------

// Buttons idle high so interrupt on a falling edge
#define BUTTON_PRESS_EVENT GPIO_IRQ_EDGE_FALL
#define BUTTON_RELEASE_EVENT GPIO_IRQ_EDGE_RISE

// Joystick axis ADC constants
#define NUM_AXES 2
#define ADC_CLOCK_DIV 65535      // Reduce sample rate to once every (1 + ADC_CLOCK_DIV) cycles
#define ADC_INPUT_PIN_OFFSET 26  // ADC inputs are numbered from 0-4, but connected on pins 26-29
#define AXIS_X_ADC_INPUT (JOYSTICK_AXIS_X_PIN - ADC_INPUT_PIN_OFFSET)
#define AXIS_Y_ADC_INPUT (JOYSTICK_AXIS_Y_PIN - ADC_INPUT_PIN_OFFSET)

// ADC - joystick resistor conversion values
#define VOLTAGE_REFERENCE 3.3f                              // Reference voltage used by ADC, and used to drive voltage divider
#define ADC_VOLTS_PER_BIT (VOLTAGE_REFERENCE / (1 << 12));  // 12-bit ADC, assume ADC reference of 3.3V
#define RESISTOR_FIXED_OHMS 10000                           // Value of the fixed resistor part of the voltage divider

//-----------------------------------------------------------------------------
// Private variables
//-----------------------------------------------------------------------------

static joystick_state_t state = {false, false, 0, 0};

//-----------------------------------------------------------------------------
// Private functions
//-----------------------------------------------------------------------------

// Analogue joystick axes are variable resistors, connected to the ADC as part of a voltage divider.
// This function converts the ADC value back into the resistance set by the stick.
inline float convert_adc_value_to_resistance(uint16_t value) {
  float voltage = value * ADC_VOLTS_PER_BIT;
  float resistance = RESISTOR_FIXED_OHMS * ((VOLTAGE_REFERENCE / voltage) - 1);
  return resistance;
}

// Joystick interrupts
void button_1_irq() {
  if (gpio_get_irq_event_mask(JOYSTICK_BUTTON_1_PIN) & BUTTON_PRESS_EVENT) {
    gpio_acknowledge_irq(JOYSTICK_BUTTON_1_PIN, BUTTON_PRESS_EVENT);
    state.button_1 = true;
  } else if (gpio_get_irq_event_mask(JOYSTICK_BUTTON_1_PIN) & BUTTON_RELEASE_EVENT) {
    gpio_acknowledge_irq(JOYSTICK_BUTTON_1_PIN, BUTTON_RELEASE_EVENT);
    state.button_1 = false;
  }
}

void button_2_irq() {
  if (gpio_get_irq_event_mask(JOYSTICK_BUTTON_2_PIN) & BUTTON_PRESS_EVENT) {
    gpio_acknowledge_irq(JOYSTICK_BUTTON_2_PIN, BUTTON_PRESS_EVENT);
    state.button_2 = true;
  } else if (gpio_get_irq_event_mask(JOYSTICK_BUTTON_2_PIN) & BUTTON_RELEASE_EVENT) {
    gpio_acknowledge_irq(JOYSTICK_BUTTON_2_PIN, BUTTON_RELEASE_EVENT);
    state.button_2 = false;
  }
}

void adc_irq() {
  uint16_t val_x = adc_fifo_get();
  uint16_t val_y = adc_fifo_get();

  state.x_axis = convert_adc_value_to_resistance(val_x);
  state.y_axis = convert_adc_value_to_resistance(val_y);
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
void joystick_init() {
  // Button setup
  gpio_init(JOYSTICK_BUTTON_1_PIN);
  gpio_init(JOYSTICK_BUTTON_2_PIN);

  gpio_set_dir(JOYSTICK_BUTTON_1_PIN, GPIO_IN);
  gpio_set_dir(JOYSTICK_BUTTON_2_PIN, GPIO_IN);

  gpio_pull_up(JOYSTICK_BUTTON_1_PIN);
  gpio_pull_up(JOYSTICK_BUTTON_2_PIN);

  gpio_add_raw_irq_handler(JOYSTICK_BUTTON_1_PIN, &button_1_irq);
  gpio_add_raw_irq_handler(JOYSTICK_BUTTON_2_PIN, &button_2_irq);

  gpio_set_irq_enabled(JOYSTICK_BUTTON_1_PIN, BUTTON_PRESS_EVENT | BUTTON_RELEASE_EVENT, true);
  gpio_set_irq_enabled(JOYSTICK_BUTTON_2_PIN, BUTTON_PRESS_EVENT | BUTTON_RELEASE_EVENT, true);
  irq_set_enabled(IO_IRQ_BANK0, true);

  // Axis setup
  adc_init();
  adc_gpio_init(JOYSTICK_AXIS_X_PIN);
  adc_gpio_init(JOYSTICK_AXIS_Y_PIN);

  // Start with X axis, round robin sampling of both X and Y
  adc_select_input(AXIS_X_ADC_INPUT);
  adc_set_round_robin(1 << AXIS_X_ADC_INPUT | 1 << AXIS_Y_ADC_INPUT);
  adc_fifo_setup(true, false, NUM_AXES, false, false);

  // Add delay between ADC samples to prevent FIFO overun
  adc_set_clkdiv(ADC_CLOCK_DIV);

  // Interrupt raised every two ADC readings, allowing X and Y to update at the same time
  irq_set_exclusive_handler(ADC_IRQ_FIFO, &adc_irq);
  adc_irq_set_enabled(true);
  irq_set_enabled(ADC_IRQ_FIFO, true);
  adc_run(true);
}

void joystick_read(joystick_state_t *state_buffer) {
  memcpy(state_buffer, &state, sizeof(state));
}

int8_t joystick_rescale_axis(float value) {
  float scaling = ((255.0 / (2.0 * (float)JOYSTICK_AXIS_CENTRE_RESISTANCE)));
  float axis = (value - (float)JOYSTICK_AXIS_CENTRE_RESISTANCE) * scaling;
  return (int8_t)round(axis);
}
