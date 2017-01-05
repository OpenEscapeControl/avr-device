#ifndef PROTO_GPIO_H
#define PROTO_GPIO_H

#include <ctype.h>
#include <inttypes.h>

#define CMD_GPIO_PINMODE			0xC0
#define CMD_GPIO_DIGITALWRITE			0xC1
#define CMD_GPIO_DIGITALREAD			0xC2
#define CMD_GPIO_ANALOGREAD			0xC3
#define CMD_GPIO_DIGITALREAD_RESPONSE		0xC4
#define CMD_GPIO_ANALOGREAD_RESPONSE		0xC5
#define CMD_GPIO_SEND_DIGITAL_CHANGE		0xCF
#define CMD_GPIO_ANALOGWRITE			0xC7
#define CMD_GPIO_SUBSCRIBE			0xC8
#define CMD_GPIO_UNSUBSCRIBE			0xCC
#define CMD_GPIO_REFRESH			0xC9
#define CMD_GPIO_TONE				0xCA
#define CMD_GPIO_NOTONE				0xCB

typedef union
{
    uint32_t intval;
    uint8_t charval[sizeof(uint32_t)];
} uint32_t_uint8_t_gpio;

typedef union
{
    uint16_t intval;
    uint8_t charval[sizeof(uint16_t)];
} uint16_t_uint8_t_gpio;

#endif
