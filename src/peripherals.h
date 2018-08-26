#ifndef _VVC_PERIPHS_H
#define _VVC_PERIPHS_H

/* Forward declarations for peripheral/device classes. */
class pGPIO;
class pLED;

#include "global.h"

/* Peripheral/device class declarations. */
/*
 * Base class for a GPIO pin.
 */
class pGPIO {
public:
  pGPIO();
  pGPIO(GPIO_TypeDef* pin_bank, uint8_t pin_num);
  #if    defined(VVC_F0) || defined(VVC_F3)
    pGPIO(GPIO_TypeDef* pin_bank, uint8_t pin_num,
          uint8_t mode, uint8_t type,
          uint8_t speed, uint8_t pupdr);
  #elif  VVC_F1
    pGPIO(GPIO_TypeDef* pin_bank, uint8_t pin_num,
          uint8_t mode, uint8_t cnf);
  #endif
protected:
  GPIO_TypeDef* bank;
  uint8_t       status;
  uint8_t       pin;
  #if defined(VVC_F0) || defined(VVC_F3)
    uint8_t     mode;
    uint8_t     type;
    uint8_t     speed;
    uint8_t     pupdr;
  #elif  VVC_F1
    uint8_t     mode;
    uint8_t     cnf;
  #endif
private:
};

/*
 * It's slightly pointless to have an 'LED' class, since
 * it's just a wrapper for a push-pull output. Whatever.
 */
class pLED : public pGPIO {
public:
  pLED();
  pLED(GPIO_TypeDef* pin_bank, int pin_num);
  void on();
  void off();
  void toggle();
protected:
private:
};

#endif
