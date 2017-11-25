#include "UDPProcessor.h"

char UDPProcessor::buf[MAX_LEN];

UDPProcessor::UDPProcessor(const uint8_t physical_device)
{
    device = physical_device;
    buf[0] = 0;
}

int UDPProcessor::begin()
{
    Serial.begin(115200);
    return(0);
}

void UDPProcessor::update()
{
    int len = 0;
    while (Serial.available() > 0)
        buf[len++] = Serial.read();
    rxCallback(len, buf);
}

uint8_t* UDPProcessor::getUDPPtr()
{
    return((uint8_t*) buf);
}

void UDPProcessor::tx(int size)
{
    buf[size + 1] = 0;
    Serial.write((unsigned char*) buf, size);
}
