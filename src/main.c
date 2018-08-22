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
 * 'Blink LED' task 1.
 */
static void led_task1(void *args __attribute__((unused))) {
  while (1) {
    // Toggle the LED.
    LED_BANK->ODR ^= LED_ODR;
    // Delay for a second-ish.
    vTaskDelay(pdMS_TO_TICKS(1111));
  };
}

/**
 * 'Blink LED' task 2.
 */
static void led_task2(void *args __attribute__((unused))) {
  while (1) {
    // Toggle the LED.
    LED_BANK->ODR ^= LED_ODR;
    // Delay for a second-ish.
    vTaskDelay(pdMS_TO_TICKS(789));
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
    // TODO: Make these work with the LED pin macro.
    LED_BANK->CRH   &= ~(GPIO_CRH_MODE12 |
                         GPIO_CRH_CNF12);
    // 2MHz push-pull output.
    LED_BANK->CRH   |=  (GPIO_CRH_MODE12_1);
    // Turn off to start.
    // (The LED on these boards turns off with 1, on with 0)
    LED_BANK->ODR   |=  (LED_ODR);
  #elif VVC_F3
    // Enable the GPIOA clock.
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    // Enable the GPIOB clock.
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    // Set the LED pin to push-pull output, 2MHz.
    // TODO: Make these work with the LED pin macro.
    LED_BANK->MODER   &= ~(GPIO_MODER_MODER10);
    LED_BANK->MODER   |=  (1 << GPIO_MODER_MODER10_Pos);
    LED_BANK->OTYPER  &= ~(GPIO_OTYPER_OT_10);
    LED_BANK->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR10);
    LED_BANK->PUPDR   &= ~(GPIO_PUPDR_PUPDR10);
    LED_BANK->ODR     |=  (LED_ODR);
  #elif VVC_L0
    // Enable the GPIOA clock.
    RCC->IOPENR   |= RCC_IOPENR_IOPAEN;
    // Enable the GPIOB clock.
    RCC->IOPENR   |= RCC_IOPENR_IOPBEN;
    // TODO: On-board LED?
  #endif

  // Create the LED tasks.
  xTaskCreate(led_task1, "LED_blink_1", 128, NULL, configMAX_PRIORITIES-1, NULL);
  xTaskCreate(led_task2, "LED_blink_2", 128, NULL, configMAX_PRIORITIES-1, NULL);
  // Start the scheduler.
  vTaskStartScheduler();

  // This should never be reached; the FreeRTOS scheduler
  // should be in charge of the program's execution after starting.
  while (1) {}
  return 0;
}
