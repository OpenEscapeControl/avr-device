#include "UDPProcessor.h"
#include "UDPGPIO.h"

UDPProcessor p(49931, 49900, 0x01);

void rxCallback(unsigned len, const char *data)
{
    if(processCommandUDPGPIO((uint8_t*) data, len) == 1)
        return;
}

void setup()
{
    p.begin();
    
    sendPowerOnMessage();
}

void loop()
{
    p.update();
    
    UDPGPIOIteration();
}
