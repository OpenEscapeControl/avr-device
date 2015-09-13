#include "UDPProcessor.h"
#include "UDPGPIO.h"

// Set physical_device to 1
// Slot is always 1
UDPProcessor p(1);

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
