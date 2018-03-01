#ifndef UDPTM1637_H
#define UDPTM1637_H

#include "proto_tm1637.h"

#define MAX_TM1637_N 2

bool processCommandTM1637(uint8_t* data, uint8_t len);

#endif
