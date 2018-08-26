#ifndef _VVC_GLOBAL_H
#define _VVC_GLOBAL_H

#include <stdio.h>

// Core includes.
#ifdef VVC_F0
  #include "stm32f0xx.h"
#elif VVC_F1
  #include "stm32f1xx.h"
#elif VVC_F3
  #include "stm32f3xx.h"
#elif VVC_L0
  #include "stm32l0xx.h"
#endif

// FreeRTOS includes.
extern "C" {
  #include "FreeRTOS.h"
  #include "task.h"
}

// Project includes.
#include "peripherals.h"

// ----------------------
// Global variables and defines.
// (Platform-specific shims)
#ifdef VVC_F1
  #define LED_BANK GPIOB
  #define LED_PIN  (12)
#else
  #define LED_BANK GPIOA
  #define LED_PIN  (10)
#endif
// (Core system clock speed; initial value depends on the chip.)
extern volatile uint32_t core_clock_hz;

// LED delay timings.
extern const int led_delay_1;
extern const int led_delay_2;

// LED objects.
extern pLED board_led;

#endif
