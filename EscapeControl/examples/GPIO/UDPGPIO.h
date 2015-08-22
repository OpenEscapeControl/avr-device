#ifndef UDPGPIO_H
#define UDPGPIO_H

#include "ard_env.h"
#include "uart.h"
#include "proto_gpio.h"
#include <inttypes.h>

#define GPIO_UPDATE_PINS_MAX 100
extern uint8_t gpio_update_pins_n;
extern uint8_t gpio_update_pins[GPIO_UPDATE_PINS_MAX];

// returns 1 on success
bool processCommandUDPGPIO(uint8_t* data, uint8_t len);
void UDPGPIOIteration();
void sendPowerOnMessage();

#endif
