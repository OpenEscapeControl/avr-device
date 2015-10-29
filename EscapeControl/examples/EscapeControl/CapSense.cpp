#include "CapSense.h"
#include "Arduino.h"

unsigned long  CS_Timeout_Millis;
unsigned long  total;
uint8_t sBit;   // send pin's ports and bitmask
volatile uint8_t *sReg;
volatile uint8_t *sOut;
uint8_t rBit;    // receive pin's ports and bitmask 
volatile uint8_t *rReg;
volatile uint8_t *rIn;
volatile uint8_t *rOut;

int SenseOneCycle()
{
    cli();
    *sOut &= ~sBit;        // set Send Pin Register low

    *rReg &= ~rBit;        // set receivePin to input
    *rOut &= ~rBit;        // set receivePin Register low to make sure pullups are off

    *rReg |= rBit;         // set pin to OUTPUT - pin is now LOW AND OUTPUT
    *rReg &= ~rBit;        // set pin to INPUT

    *sOut |= sBit;         // set send Pin High
    sei();

    while ( !(*rIn & rBit)  && (total < CS_Timeout_Millis) ) {  // while receive pin is LOW AND total is positive value
	total++;
    }
    
    if (total > CS_Timeout_Millis) {
	return -2;         //  total variable over timeout
    }

    // set receive pin HIGH briefly to charge up fully - because the while loop above will exit when pin is ~ 2.5V
    cli();
    *rOut  |= rBit;        // set receive pin HIGH - turns on pullup
    *rReg |= rBit;         // set pin to OUTPUT - pin is now HIGH AND OUTPUT
    *rReg &= ~rBit;        // set pin to INPUT
    *rOut  &= ~rBit;       // turn off pullup

    *sOut &= ~sBit;        // set send Pin LOW
    sei();

    while ( (*rIn & rBit) && (total < CS_Timeout_Millis) ) {  // while receive pin is HIGH  AND total is less than timeout
	total++;
    }

    if (total >= CS_Timeout_Millis) {
	return -2;     // total variable over timeout
    } else {
	return 1;
    }
}

long CapSense(uint8_t sendPin, uint8_t receivePin, uint8_t samples, uint8_t ms_timeout)
{
    // initialize this instance's variables
    unsigned int loopTimingFactor = 310;		// determined empirically -  a hack

    CS_Timeout_Millis = (ms_timeout * (float)loopTimingFactor * (float)F_CPU) / 16000000;  // floats to deal with large numbers

    // get pin mapping and port for send Pin - from PinMode function in core

    uint8_t sPort, rPort;
    sBit =  digitalPinToBitMask(sendPin);			// get send pin's ports and bitmask
    sPort = digitalPinToPort(sendPin);
    sReg = portModeRegister(sPort);
    sOut = portOutputRegister(sPort);				// get pointer to output register

    rBit = digitalPinToBitMask(receivePin);			// get receive pin's ports and bitmask
    rPort = digitalPinToPort(receivePin);
    rReg = portModeRegister(rPort);
    rIn  = portInputRegister(rPort);
    rOut = portOutputRegister(rPort);

    // get pin mapping and port for receive Pin - from digital pin functions in Wiring.c
    cli();
    *sReg |= sBit;              // set sendpin to OUTPUT
    sei();

    long total = 0;
    if (samples == 0) return 0;


    for (uint8_t i = 0; i < samples; i++) {    // loop for samples parameter - simple lowpass filter
        if (SenseOneCycle() < 0)  return -2;   // variable over timeout
    }

    return(total);
}
