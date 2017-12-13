#include <Arduino.h>
#include "../TM1637/TM1637Display.h"
#include "UDPTM1637.h"
#include "UDPProcessor.h"
#include <inttypes.h>

TM1637Display displays[MAX_TM1637_N];

bool processCommandTM1637(uint8_t* data, uint8_t len)
{
    if(len < 2 || data[1] >= MAX_TM1637_N)
        return(0);
    if(data[0] == CMD_TM1637_INIT)
    {
        // proto: tm_n clk_pin dio_pin
        uint8_t id = data[1];
        uint8_t clk_n = data[2];
        uint8_t dio_n = data[3];
        displays[id].init(clk_n, dio_n);

//        UDPProcessor::tx(len);

        return(1);
    }
    else if(data[0] == CMD_TM1637_DISPLAYDEC)
    {
        // proto: tm_n digit1 digit2 digit3 digit4
        uint8_t id = data[1];
        displays[id].setBrightness(0x0f);

        uint8_t digits[4];
        for(int i = 0; i < 4; i++)
            digits[i] = displays[id].encodeDigit(data[2 + i]);

        displays[id].setSegments(digits);
//        UDPProcessor::tx(len);

        return(1);
    }
    return(0);
}
