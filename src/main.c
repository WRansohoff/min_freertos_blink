#include "main.h"

/**
 * Define some system interrupt handlers to call 'port.c' FreeRTOS methods.
 */
extern void vPortSVCHandler(void) __attribute__((naked));
extern void xPortPendSVHandler(void) __attribute__((naked));
extern void xPortSysTickHandler(void);

void SVC_handler(void) {
	vPortSVCHandler();
}

void pending_SV_handler(void) {
	xPortPendSVHandler();
}

void SysTick_handler(void) {
	xPortSysTickHandler();
} 

/**
 * 'Blink LED' task.
 */
static void led_task(void *args __attribute__((unused))) {
  while (1) {
    // Toggle the LED.
    GPIOB->ODR ^= GPIO_ODR_ODR12;
    // Delay for a second.
    vTaskDelay(pdMS_TO_TICKS(1000));
  };
}

/**
 * Main program.
 */
int main(void) {
  // Initial clock setup.
  setup_clocks();

  #ifdef VVC_F0
    // Enable the GPIOA clock.
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    // Enable the GPIOB clock.
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    // TODO: On-board LED?
  #elif VVC_F1
    // Enable the GPIOB clock.
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    // Enable the on-board LED pin, B12. TODO: make a method.
    GPIOB->CRH   &= ~(GPIO_CRH_MODE12 |
                      GPIO_CRH_CNF12);
    // 2MHz push-pull output.
    GPIOB->CRH   |=  (GPIO_CRH_MODE12_1);
    // Turn off to start.
    // (The LED on these boards turns off with 1, on with 0)
    GPIOB->ODR   |=  (GPIO_ODR_ODR12);
  #elif VVC_L0
    // Enable the GPIOA clock.
    RCC->IOPENR   |= RCC_IOPENR_IOPAEN;
    // Enable the GPIOB clock.
    RCC->IOPENR   |= RCC_IOPENR_IOPBEN;
    // TODO: On-board LED?
  #endif

  // Create the LED task.
  xTaskCreate(led_task, "LED_blink", 128, NULL, configMAX_PRIORITIES-1, NULL);
  // Start the scheduler.
  vTaskStartScheduler();

  // This should never be reached; the FreeRTOS scheduler
  // should be in charge of the program's execution after starting.
  while (1) {}
  return 0;
}
