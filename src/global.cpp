#include "global.h"

// (Core system clock speed; initial value depends on the chip.)
volatile uint32_t core_clock_hz = 8000000;

// LED delay timings.
const int led_delay_1 = 1111;
const int led_delay_2 = 500;

// LED objects.
pLED board_led;
