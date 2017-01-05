#ifndef UDPGPIO_H
#define UDPGPIO_H

#include "proto_gpio.h"
#include <inttypes.h>

#define GPIO_UPDATE_MAX_PIN 70

// returns 1 on success
bool processCommandUDPGPIO(uint8_t* data, uint8_t len);
void UDPGPIOIteration();
void sendPowerOnMessage();
void sendACK();
void GPIOInit();

#endif
