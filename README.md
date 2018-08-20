# Overview

This is an example FreeRTOS application for boards using the cheap and widely-available STM32F103C8 microcontroller. Depending on your board, the LED might be on pin C13 (or something else) instead of B12.

I had some trouble finding FreeRTOS examples that didn't use proprietary development platforms or external libraries. So I decided to cobble together a simple example that only uses the GCC toolchain and some device-specific header files provided by ST.

This barebones example just creates a single task which toggles a GPIO pin every second. It is intended to be a simple introduction to bootstrapping an STM32 FreeRTOS application using free tools, so corrections and suggestions are always appreciated.

# Boards

Currently, only the STM32F103C8 is supported; boards with these chips are very cheap, and they have a lot of features. But I would like to add support for chips from other STM32 lines. ST provides affordable 'Nucleo' development boards, so those would probably be a good starting point.
