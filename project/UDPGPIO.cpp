#include "UDPGPIO.h"
#include "UDPProcessor.h"
#include "proto_ping.h"
#include <strings.h>

uint8_t gpio_subscribers[GPIO_UPDATE_MAX_PIN];
unsigned long gpio_last_ms[GPIO_UPDATE_PINS_MAX];
uint8_t gpio_last_state[GPIO_UPDATE_PINS_MAX];

void GPIOInit()
{
    bzero(gpio_subscribers, sizeof(gpio_subscribers));
    bzero(gpio_last_ms, sizeof(gpio_last_ms));
    bzero(gpio_last_state, sizeof(gpio_last_state));
    bzero(gpio_updated, sizeof(gpio_updated));
    need_gpio_all = 0;
}

void sendPowerOnMessage()
{
    *(UDPProcessor::getUDPPtr()) = CMD_POWERON;
    UDPProcessor::tx(1);
}

void GPIOUpdate(int i)
{
    uint8_t newState = digitalRead(i);
    unsigned long newMs = millis();

    if(newState != gpio_last_state[i])
    {
        unsigned long differenceMs = newMs - gpio_last_ms[i];
        gpio_last_ms[i] = newMs;
        gpio_last_state[i] = newState;
        uint8_t* data = UDPProcessor::getUDPPtr();
        data[0] = CMD_GPIO_SEND_DIGITAL_CHANGE;
        data[1] = i;
        data[2] = newState;
        uint32_t_uint8_t_gpio uu;
        uu.intval = differenceMs;
        for(unsigned i = 0; i < sizeof(uint32_t); i++)
        {
            data[3 + i] = uu.charval[i];
        }
        UDPProcessor::tx(3 + sizeof(uint32_t));
    }

}

void sendACK()
{
    uint8_t data;
    uint8_t* ptr = UDPProcessor::getUDPPtr();

    data = *ptr;
    *ptr = CMD_ACK;
    UDPProcessor::tx(1);
    *ptr = data;
}

bool processCommandUDPGPIO(uint8_t* data, uint8_t len)
{
    if(data[0] == CMD_GPIO_PINMODE && len >= 3)
    {
        pinMode(data[1], data[2]);
        return(1);
    }
    else if(data[0] == CMD_GPIO_DIGITALWRITE && len >= 3)
    {
        digitalWrite(data[1], data[2]);
        return(1);
    }
    else if(data[0] == CMD_GPIO_TONE && len >= 3)
    {
        uint16_t_uint8_t_gpio uu;
        uu.charval[0] = data[2];
        uu.charval[1] = data[3];
        tone(data[1], uu.intval);
        return(1);
    }
    else if(data[0] == CMD_GPIO_NOTONE && len >= 2)
    {
        noTone(data[1]);
        return(1);
    }
    else if(data[0] == CMD_GPIO_DIGITALREAD && len >= 2)
    {
        uint8_t pin = data[1];
        uint8_t res = digitalRead(pin);
        *(UDPProcessor::getUDPPtr()) = CMD_GPIO_DIGITALREAD_RESPONSE;
        *(UDPProcessor::getUDPPtr() + 1) = pin;
        *(UDPProcessor::getUDPPtr() + 2) = res;
        UDPProcessor::tx(3);
        return(1);
    }
    else if(data[0] == CMD_GPIO_ANALOGREAD && len >= 2)
    {
        uint8_t pin = data[1];
        int res = analogRead(pin);
        *(UDPProcessor::getUDPPtr()) = CMD_GPIO_ANALOGREAD_RESPONSE;
        *(UDPProcessor::getUDPPtr() + 1) = pin;
        *(UDPProcessor::getUDPPtr() + 2) = res / 4;
        UDPProcessor::tx(3);
        return(1);
    }
    else if(data[0] == CMD_GPIO_SUBSCRIBE && len >= 2)
    {
        uint8_t pin = data[1];
        if(pin > GPIO_UPDATE_MAX_PIN)
            return(1);
        if(gpio_subscribers[pin] < 255)
            gpio_subscribers[pin]++;

        if(gpio_subscribers[pin] == 1)
        {
            pinMode(pin, INPUT_PULLUP);
            gpio_last_ms[pin] = 0;
            gpio_last_state[pin] = -1;
        }

        GPIOUpdate(pin);

        return(1);
    }
    else if(data[0] == CMD_GPIO_UNSUBSCRIBE && len >= 2)
    {
        uint8_t pin = data[1];
        if(pin > GPIO_UPDATE_MAX_PIN)
            return(1);
        if(gpio_subscribers[pin] > 0)
            gpio_subscribers[pin]--;
        return(1);
    }
    else if(data[0] == CMD_GPIO_REFRESH)
    {
        need_gpio_all = 1;
        return(1);
    }
    else if(data[0] == CMD_PING_REQ)
    {
        *(UDPProcessor::getUDPPtr()) = CMD_PING_RESP;
        UDPProcessor::tx(1);
        return(1);
    }
    else if(data[0] == CMD_GPIO_ANALOGWRITE)
    {
        analogWrite(data[1], data[2]);
        return(1);
    }
    return(0);
}

void UDPGPIOIteration()
{
    for(unsigned i = 0; i < GPIO_UPDATE_MAX_PIN; i++)
    {
        if(gpio_subscribers[i] == 0)
            continue;
        GPIOUpdate(i);
    }
}
