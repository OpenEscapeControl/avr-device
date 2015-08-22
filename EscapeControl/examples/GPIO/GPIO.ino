#include "UDPProcessor.h"
#include "UDPGPIO.h"

UDPProcessor p;

void cbc(unsigned port, unsigned char ip[], const char *data, unsigned len)
{
    if(processCommandUDPGPIO((uint8_t*) data, len) == 1)
        return;
}

void setup()
{
    int res = p.initialize();
    if(res)
        for(;;) {}
    else
        p.setCallBack(&cbc);

    sendPowerOnMessage();
}

void loop()
{
    p.update();
    UDPGPIOIteration();
}
