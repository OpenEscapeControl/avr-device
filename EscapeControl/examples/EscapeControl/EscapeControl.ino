#include "UDPProcessor.h"
#include "CapSense.h"
#include "UDPGPIO.h"
#include "UDPCapSense.h"
#include "OneWire.h"

// Set physical_device to 1
// Slot is always 1
UDPProcessor p(1);

void rxCallback(unsigned len, const char *data)
{
    if(processCommandUDPGPIO((uint8_t*) data, len) == 1)
        return;
    if(processCommandCapSense((uint8_t*) data, len) == 1)
        return;
    if(processCommandOneWire((uint8_t*) data, len) == 1)
        return;
}

void setup()
{
    p.begin();
    delay(100);
    sendPowerOnMessage();
}

void loop()
{
    p.update();
    
    UDPGPIOIteration();
}
