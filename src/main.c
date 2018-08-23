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
static void led_task(void *args) {
  int delay_ms = *(int*)args;

  while (1) {
    // Toggle the LED.
    LED_BANK->ODR ^= (1 << LED_PIN);
    // Delay for a second-ish.
    vTaskDelay(pdMS_TO_TICKS(delay_ms));
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
    // Set the LED pin to push-pull output, 2MHz.
    LED_BANK->MODER   &= ~(3 << (LED_PIN*2));
    LED_BANK->MODER   |=  (1 << (LED_PIN*2));
    LED_BANK->OTYPER  &= ~(1 << LED_PIN);
    LED_BANK->OSPEEDR &= ~(3 << (LED_PIN*2));
    LED_BANK->PUPDR   &= ~(1 << LED_PIN);
    LED_BANK->ODR     |=  (1 << LED_PIN);
  #elif VVC_F1
    // Enable the GPIOB clock.
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    // Enable the on-board LED pin, B12. TODO: make a method.
    // TODO: Make these work with the LED pin macro.
    #if LED_PIN > 7
      LED_BANK->CRH   &= ~(7 << ((LED_PIN-8)*4));
      // 2MHz push-pull output.
      LED_BANK->CRH   |=  (2 << ((LED_PIN-8)*4));
    #else
      LED_BANK->CRL   &= ~(7 << (LED_PIN*4));
      // 2MHz push-pull output.
      LED_BANK->CRL   |=  (2 << (LED_PIN*3));
    #endif
    // Turn off to start.
    // (The LED on these boards turns off with 1, on with 0)
    LED_BANK->ODR     |=  (1 << LED_PIN);
  #elif VVC_F3
    // Enable the GPIOA clock.
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    // Enable the GPIOB clock.
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    // Set the LED pin to push-pull output, 2MHz.
    LED_BANK->MODER   &= ~(3 << (LED_PIN*2));
    LED_BANK->MODER   |=  (1 << (LED_PIN*2));
    LED_BANK->OTYPER  &= ~(1 << LED_PIN);
    LED_BANK->OSPEEDR &= ~(3 << (LED_PIN*2));
    LED_BANK->PUPDR   &= ~(1 << LED_PIN);
    LED_BANK->ODR     |=  (1 << LED_PIN);
  #elif VVC_L0
    // Enable the GPIOA clock.
    RCC->IOPENR   |= RCC_IOPENR_IOPAEN;
    // Enable the GPIOB clock.
    RCC->IOPENR   |= RCC_IOPENR_IOPBEN;
    // TODO: On-board LED?
  #endif

  // Create the LED tasks.
  xTaskCreate(led_task, "LED_blink_1", 128, (void*)&led_delay_1, configMAX_PRIORITIES-1, NULL);
  xTaskCreate(led_task, "LED_blink_2", 128, (void*)&led_delay_2, configMAX_PRIORITIES-1, NULL);
  // Start the scheduler.
  vTaskStartScheduler();

  // This should never be reached; the FreeRTOS scheduler
  // should be in charge of the program's execution after starting.
  while (1) {}
  return 0;
}
