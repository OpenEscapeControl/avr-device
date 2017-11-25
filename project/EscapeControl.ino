#include "UDPProcessor.h"
#include "CapSense.h"
#include "UDPKeypad.h"
#include "UDPGPIO.h"
#include "UDPCapSense.h"
#include "OneWire.h"
#include "SoftwareSerial.h"
#include "DFPlayer.h"
#include "device_n.h"

// Set physical_device to device_n
// Slot is always 1
UDPProcessor p(device_n);

void rxCallback(unsigned len, const char *data)
{
    if(processCommandUDPGPIO((uint8_t*) data, len) == 1)
        return;
    if(processCommandCapSense((uint8_t*) data, len) == 1)
        return;
    if(processCommandOneWire((uint8_t*) data, len) == 1)
        return;
    if(processCommandDFPlayer((uint8_t*) data, len) == 1)
        return;
    if(processCommandUDPKeypad((uint8_t*) data, len) == 1)
        return;

    // return unknown command to the sender
    UDPProcessor::tx(len);
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
