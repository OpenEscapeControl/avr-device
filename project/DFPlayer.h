#ifndef DFPLAYER_H
#define DFPLAYER_H

#include "proto_dfplayer.h"
#include <inttypes.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#define DFPLAYER_N 10

// returns 1 on success
bool processCommandDFPlayer(uint8_t* data, uint8_t len);

#endif
