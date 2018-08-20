#ifndef _VVC_GLOBAL_H
#define _VVC_GLOBAL_H

#include <stdio.h>

// Core includes.
#ifdef VVC_F0
  #include "stm32f0xx.h"
#elif VVC_F1
  #include "stm32f1xx.h"
#elif VVC_L0
  #include "stm32l0xx.h"
#endif

// FreeRTOS includes.
#include "FreeRTOS.h"
#include "task.h"

// ----------------------
// Global variables and defines.
// (Core system clock speed; initial value depends on the chip.)
static volatile uint32_t core_clock_hz;

#endif
