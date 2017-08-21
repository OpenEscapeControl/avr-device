#include "DFPlayer.h"

DFRobotDFPlayerMini players[DFPLAYER_N];

// proto: CMD PLAYER_ID
//         0     1
// if not stated otherwise

bool processCommandDFPlayer(uint8_t* data, uint8_t len)
{
    if(data[1] >= DFPLAYER_N)
        return(1);
    if(data[0] == CMD_DFPLAYER_BEGIN && len >= 4)
    {
        // proto: CMD PLAYER_ID PIN_RX PIN_TX
        //         0      1        2      3
        SoftwareSerial* sws = new SoftwareSerial(data[2], data[3]);
        sws->begin(9600);
        players[data[1]].begin(*sws);
        return(1);
    }
    else if(data[0] == CMD_DFPLAYER_PAUSE && len >= 2)
    {
        players[data[1]].pause();
        return(1);
    }
    else if(data[0] == CMD_DFPLAYER_PLAYFOLDER && len >= 4)
    {
        // proto: CMD PLAYER_ID FOLDER FILE
        //         0     1         2    3
        players[data[1]].playFolder(data[2], data[3]);
        return(1);
    }
    else if(data[0] == CMD_DFPLAYER_START && len >= 2)
    {
        players[data[1]].start();
        return(1);
    }
    else if(data[0] == CMD_DFPLAYER_STOP && len >= 2)
    {
        players[data[1]].stop();
        return(1);
    }
    else if(data[0] == CMD_DFPLAYER_VOLUME && len >= 3)
    {
        // proto: CMD PLAYER_ID VOLUME
        //         0       1       2
        players[data[1]].volume(data[2]);
        return(1);
    }
    else if(data[0] == CMD_DFPLAYER_END && len >= 2)
    {
        players[data[1]].end();
    }
    return(0);
}
