#include "UDPGPIO.h"
#include "UDPProcessor.h"
#include "proto_ping.h"

uint8_t gpio_update_pins_n = 0;
uint8_t gpio_update_pins[GPIO_UPDATE_PINS_MAX];
bool need_gpio_all = 1;
unsigned long last_ms[GPIO_UPDATE_PINS_MAX];
uint8_t last_state[GPIO_UPDATE_PINS_MAX];

void sendPowerOnMessage()
{
    *(UDPProcessor::getUDPPtr()) = CMD_POWERON;
    UDPProcessor::tx(1);
}

void sendACK()
{
    *(UDPProcessor::getUDPPtr()) = CMD_ACK;
    UDPProcessor::tx(1);
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
    else if(data[0] == CMD_GPIO_DIGITALREAD && len >= 2)
    {
        uint8_t res = digitalRead(data[1]);
        *(UDPProcessor::getUDPPtr()) = CMD_GPIO_DIGITALREAD_RESPONSE;
        *(UDPProcessor::getUDPPtr() + 1) = data[1];
        *(UDPProcessor::getUDPPtr() + 2) = res;
        UDPProcessor::tx(3);
        return(1);
    }
    else if(data[0] == CMD_GPIO_ANALOGREAD && len >= 2)
    {
        int res = analogRead(data[1]);
        *(UDPProcessor::getUDPPtr()) = CMD_GPIO_ANALOGREAD_RESPONSE;
        *(UDPProcessor::getUDPPtr() + 1) = data[1];
        *(UDPProcessor::getUDPPtr() + 2) = res / 4;
        UDPProcessor::tx(3);
        return(1);
    }
    else if(data[0] == CMD_GPIO_UPDATE_LIST && len >= 2)
    {
        if(data[1] >= GPIO_UPDATE_PINS_MAX)
            return(1);
        gpio_update_pins_n = data[1];
        for(uint8_t i = 0; i < gpio_update_pins_n; i++)
        {
            pinMode(data[2 + i], INPUT_PULLUP);
            gpio_update_pins[i] = data[2 + i];
            last_ms[i] = 0;
            last_state[i] = -1;
        }
        return(1);
    }
    else if(data[0] == CMD_GPIO_REFRESH)
    {
        need_gpio_all = 1;
        return(1);
    }
    else if(data[0] == CMD_GPIO_SEND_DIGITAL_CHANGE_INITIAL && len >= 1)
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
    
    for(unsigned i = 0; i < gpio_update_pins_n; i++)
    {
        uint8_t newState = digitalRead(gpio_update_pins[i]);
        unsigned long newMs = millis();

        if(newState != last_state[i] || need_gpio_all)
        {
            unsigned long differenceMs = newMs - last_ms[i];
            last_ms[i] = newMs;
            last_state[i] = newState;
            uint8_t* data = UDPProcessor::getUDPPtr();
            data[0] = CMD_GPIO_SEND_DIGITAL_CHANGE;
            data[1] = gpio_update_pins[i];
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
    need_gpio_all = false;
}
