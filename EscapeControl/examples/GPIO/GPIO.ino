#include <stdlib.h>
#include <stdio.h>
#include "UDPProcessor.h"
#include "uart.h"
#include "ard_env.h"
#include "UDPGPIO.h"

UDPProcessor p;

void cbc(unsigned port, unsigned char ip[], const char *data, unsigned len)
{
    if(processCommandUDPGPIO((uint8_t*) data, len) == 1)
        return;
}

void init()
{
    ard_env_initialize();

    int res = p.initialize();
    if(res)
        for(;;) {}
    else
        p.setCallBack(&cbc);

    sendPowerOnMessage();
}

int main()
{
    init();
    for(;;)
    {
        p.update();
        UDPGPIOIteration();
    }
}
