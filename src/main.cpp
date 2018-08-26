#include "main.h"

/**
 * 'Blink LED' task.
 */
static void led_task(void *args) {
  int delay_ms = *(int*)args;

  while (1) {
    // Toggle the LED.
    //LED_BANK->ODR ^= (1 << LED_PIN);
    board_led.toggle();
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
    // Enable the on-board LED pin, B12.
    board_led = pLED(LED_BANK, LED_PIN);
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
