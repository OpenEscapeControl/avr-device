#ifndef UART_URP_PROCESSOR
#define UART_URP_PROCESSOR

#include "Arduino.h"
#include <inttypes.h>

void rxCallback(unsigned len, const char *data);

#define MAX_LEN 1000

class UDPProcessor
{
private:
    static PROGMEM char const hostname_[];
    static char buf[MAX_LEN];
    int device;

public:
    UDPProcessor(const uint8_t physical_device);
    static int begin();
    static void update();
    static uint8_t* getUDPPtr();
    static void tx(int size);
};

#endif // UART_UDP_PROCESSOR
