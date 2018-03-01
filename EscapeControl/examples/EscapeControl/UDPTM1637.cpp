#include <Arduino.h>
#include "TM1637.h"
#include "UDPTM1637.h"
#include "UDPProcessor.h"
#include <inttypes.h>

TM1637 displays[MAX_TM1637_N];

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
        displays[id].set(0);

        return(1);
    }
    else if(data[0] == CMD_TM1637_DISPLAYDEC)
    {
        // proto: tm_n digit1 digit2 digit3 digit4
        uint8_t id = data[1];

        for(int i = 0; i < 4; i++)
            displays[id].display(i, data[2 + i]);

        return(1);
    }
    return(0);
}
