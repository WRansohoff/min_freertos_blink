# Overview

This is an example FreeRTOS application for boards using some common STM32 microcontrollers. Depending on your board, the LED will probably be on different pins.

I had some trouble finding FreeRTOS examples that didn't use proprietary development platforms or external libraries. So I decided to cobble together a simple example that only uses the GCC toolchain and some device-specific header files provided by the chip's manufacturers.

This barebones example just creates two tasks which toggle a GPIO pin on different timings, to make an on-board LED blink irregularly. I'm hoping it can be a simple introduction to bootstrapping an STM32 FreeRTOS application using a free toolchain, so corrections and suggestions are always appreciated.

# Boards

Currently, the STM32F103C8 and STM32F303K8 are supported. Boards with the STM32F103C8 are very cheap, and they have a lot of features. ST also provides affordable 'Nucleo' development boards with a variety of chips including the STM32F303K8.

I'm also hoping to get this working with the simpler F0 and L0 lines.
