#include "peripherals.h"

pGPIO::pGPIO() { status = 0; }

pGPIO::pGPIO(GPIO_TypeDef* pin_bank, uint8_t pin_num) {
  bank   = pin_bank;
  pin    = pin_num;
  status = 0;
}

#if    defined(VVC_F0) || defined(VVC_F3)

pGPIO::pGPIO(GPIO_TypeDef* pin_bank, uint8_t pin_num,
             uint8_t mode, uint8_t type,
             uint8_t speed, uint8_t pupdr) {
  bank           =   pin_bank;
  pin            =   pin_num;
  bank->MODER   &= ~(0x3 << (pin * 2));
  bank->MODER   |=  (mode << (pin * 2));
  bank->OTYPER  &= ~(0x1 << pin);
  bank->OTYPER  |=  (type << pin);
  bank->OSPEEDR &= ~(0x3 << (pin * 2));
  bank->OSPEEDR |=  (speed << (pin * 2));
  bank->PUPDR   &= ~(0x3 << (pin * 2));
  bank->PUPDR   |=  (pupdr << (pin * 2));
  status         =   1;
}

#elif  VVC_F1

pGPIO::pGPIO(GPIO_TypeDef* pin_bank, uint8_t pin_num,
             uint8_t mode, uint8_t cnf) {
  bank   = pin_bank;
  pin    = pin_num;
  if (pin < 8) {
    bank->CRL &= ~(0x0F << (pin * 4));
    bank->CRL |=  (mode << (pin * 4));
    bank->CRL |=  (cnf << ((pin * 4) + 2));
  }
  else {
    bank->CRH &= ~(0x0F << ((pin - 8) * 4));
    bank->CRH |=  (mode << ((pin - 8) * 4));
    bank->CRH |=  (cnf << (((pin - 8) * 4) + 2));
  }
  status = 1;
}

#endif

pLED::pLED() : pGPIO() {}

#if    defined(VVC_F0) || defined(VVC_F3)

pLED::pLED(GPIO_TypeDef* pin_bank, int pin_num) :
      pGPIO(pin_bank, pin_num, 0x01, 0x00, 0x00, 0x00) {}

#elif  VVC_F1

pLED::pLED(GPIO_TypeDef* pin_bank, int pin_num) :
      pGPIO(pin_bank, pin_num, 0x02, 0x00) {}

#endif

void pLED::on() {
  bank->ODR |=  (1 << pin);
}

void pLED::off() {
  bank->ODR &= ~(1 << pin);
}

void pLED::toggle() {
  bank->ODR ^=  (1 << pin);
}
