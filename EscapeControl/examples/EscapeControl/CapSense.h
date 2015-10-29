#ifndef CAPSENSE_H
#define CAPSENSE_H

#include "Arduino.h"

long CapSense(uint8_t sendPin, uint8_t receivePin, uint8_t samples, uint8_t ms_timeout);

#endif
