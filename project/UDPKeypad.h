#ifndef UDPKEYPAD_H
#define UDPKEYPAD_H

#include "Keypad.h"
#include "proto_keypad.h"

#define MAX_KEYPAD_N 2

bool processCommandUDPKeypad(uint8_t* data, uint8_t len);

#endif
