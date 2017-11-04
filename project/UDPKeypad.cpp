#include "UDPKeypad.h"
#include "UDPProcessor.h"
#include <inttypes.h>

Keypad keypads[MAX_KEYPAD_N];
uint8_t row_pins[MAX_KEYPAD_N][KEYPAD_MAX_S];
uint8_t col_pins[MAX_KEYPAD_N][KEYPAD_MAX_S];

bool processCommandUDPKeypad(uint8_t* data, uint8_t len)
{
    if(len < 2 || data[1] >= MAX_KEYPAD_N)
        return(0);
    if(data[0] == CMD_KEYPAD_SET && len >= 3)
    {
        // proto: keypad_n row_n col_n 10xrow_pins    10xcol_pins
        //          1        2    3    45678910111213 14
        uint8_t id = data[1];
        uint8_t row_n = data[2];
        uint8_t col_n = data[3];
        if(row_n > KEYPAD_MAX_S) row_n = KEYPAD_MAX_S;
        if(col_n > KEYPAD_MAX_S) col_n = KEYPAD_MAX_S;
        for(uint8_t i = 0; i < row_n; i++) row_pins[id][i] = data[4 + i];
        for(uint8_t i = 0; i < col_n; i++) col_pins[id][i] = data[14 + i];
        keypads[id] = Keypad(row_pins[id], col_pins[id], row_n, col_n);
    }
    else if(data[0] == CMD_KEYPAD_REQUEST && len >= 2)
    {
        data[0] = CMD_KEYPAD_RESPONSE;
        Keypad* kp = &(keypads[data[1]]);

        uint8_t list_n = 0;

        if(kp->getKeys())
        {
            for(uint8_t i = 0; i < LIST_MAX; i++)
            {
                if(kp->key[i].kstate == PRESSED || kp->key[i].kstate == HOLD)
                {
                    data[3 + list_n] = kp->key[i].kcode;
                    list_n++;
                }
            }
        }

        data[2] = list_n;
        UDPProcessor::tx(3 + list_n);

        return(1);
    }
    return(0);
}
