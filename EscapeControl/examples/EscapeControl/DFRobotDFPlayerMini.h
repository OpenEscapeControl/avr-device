/*!
 * @file DFRobotDFPlayerMini.h
 * @brief DFPlayer - An Arduino Mini MP3 Player From DFRobot
 * @n Header file for DFRobot's DFPlayer
 *
 * @copyright	[DFRobot]( http://www.dfrobot.com ), 2016
 * @copyright	GNU Lesser General Public License
 *
 * @author [Angelo](Angelo.qiao@dfrobot.com)
 * @version  V1.0
 * @date  2016-12-07
 */

#include "Arduino.h"

#ifndef DFRobotDFPlayerMini_cpp
#define DFRobotDFPlayerMini_cpp

#define DFPLAYER_SEND_LENGTH 10

//#define _DEBUG

#define Stack_Header 0
#define Stack_Version 1
#define Stack_Length 2
#define Stack_Command 3
#define Stack_ACK 4
#define Stack_Parameter 5
#define Stack_CheckSum 7
#define Stack_End 9

class DFRobotDFPlayerMini
{
    uint8_t _sending[DFPLAYER_SEND_LENGTH] = {0x7E, 0xFF, 06, 00, 01, 00, 00, 00, 00, 0xEF};

    void sendStack();
    void sendStack(uint8_t command);
    void sendStack(uint8_t command, uint16_t argument);
    void sendStack(uint8_t command, uint8_t argumentHigh, uint8_t argumentLow);

    void uint16ToArray(uint16_t value,uint8_t *array);
    uint16_t arrayToUint16(uint8_t *array);
    uint16_t calculateCheckSum(uint8_t *buffer);

public:
    DFRobotDFPlayerMini();
    Stream* _serial;

    bool begin(Stream& stream);
    void volume(uint8_t volume);
    void reset();
    void start();
    void pause();
    void playFolder(uint8_t folderNumber, uint8_t fileNumber);
    void stop();
    void end();

};

#endif
