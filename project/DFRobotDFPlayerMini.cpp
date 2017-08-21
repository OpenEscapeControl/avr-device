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

#include "DFRobotDFPlayerMini.h"

void DFRobotDFPlayerMini::uint16ToArray(uint16_t value, uint8_t *array)
{
    *array = (uint8_t)(value>>8);
    *(array+1) = (uint8_t)(value);
}

uint16_t DFRobotDFPlayerMini::calculateCheckSum(uint8_t *buffer)
{
    uint16_t sum = 0;
    for (int i=Stack_Version; i<Stack_CheckSum; i++) {
        sum += buffer[i];
    }
    return -sum;
}

void DFRobotDFPlayerMini::sendStack()
{
    if(_serial)
        _serial->write(_sending, DFPLAYER_SEND_LENGTH);
}

void DFRobotDFPlayerMini::sendStack(uint8_t command)
{
    sendStack(command, 0);
}

void DFRobotDFPlayerMini::sendStack(uint8_t command, uint16_t argument)
{
    _sending[Stack_Command] = command;
    uint16ToArray(argument, _sending+Stack_Parameter);
    uint16ToArray(calculateCheckSum(_sending), _sending+Stack_CheckSum);
    sendStack();
}

void DFRobotDFPlayerMini::sendStack(uint8_t command, uint8_t argumentHigh, uint8_t argumentLow)
{
    uint16_t buffer = argumentHigh;
    buffer <<= 8;
    sendStack(command, buffer | argumentLow);
}

bool DFRobotDFPlayerMini::begin(Stream &stream)
{
    end();
    _sending[Stack_ACK] = 0x01;
    _serial = &stream;
    reset();
    return(1);
}

uint16_t DFRobotDFPlayerMini::arrayToUint16(uint8_t *array)
{
    uint16_t value = *array;
    value <<=8;
    value += *(array+1);
    return value;
}

void DFRobotDFPlayerMini::volume(uint8_t volume)
{
    sendStack(0x06, volume);
}

void DFRobotDFPlayerMini::reset()
{
    sendStack(0x0C);
}

void DFRobotDFPlayerMini::start()
{
    sendStack(0x0D);
}

void DFRobotDFPlayerMini::pause()
{
    sendStack(0x0E);
}

void DFRobotDFPlayerMini::stop()
{
    sendStack(0x16);
}

void DFRobotDFPlayerMini::end()
{
    if(_serial)
        delete _serial;
    _serial = NULL;
}

void DFRobotDFPlayerMini::playFolder(uint8_t folderNumber, uint8_t fileNumber)
{
    sendStack(0x0F, folderNumber, fileNumber);
}

DFRobotDFPlayerMini::DFRobotDFPlayerMini()
{
    _serial = NULL;
}
