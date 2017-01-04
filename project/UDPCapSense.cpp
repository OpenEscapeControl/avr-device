#include "CapSense.h"
#include "UDPCapSense.h"
#include "UDPProcessor.h"

bool processCommandCapSense(uint8_t* data, uint8_t len)
{
    if(data[0] == CMD_CAPSENSE_DO && len >= 5)
    {
        uint8_t* dst = UDPProcessor::getUDPPtr();
        uint32_t res = CapSense(data[1], data[2], data[3], data[3]);
        dst[0] = ANS_CAPSENSE_DO;
        dst[1] = data[1];
        dst[2] = data[2];
        dst[3] = res & 0xff;
        dst[4] = (res >> 8) & 0xff;
        dst[5] = (res >> 16) & 0xff;
        dst[6] = (res >> 24) & 0xff;
        UDPProcessor::tx(7);
        return(true);
    }
    return(false);
}
