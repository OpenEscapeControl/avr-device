#ifndef UDP_CONFIG_H
#define UDP_CONFIG_H
#include "UDPProcessor.h"
uint8_t UDPProcessor::remote_mac_[] = {0xAA, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t UDPProcessor::remote_ip_[] = {192, 168, 100, 1};

unsigned UDPProcessor::remote_port_ = 49931;

uint8_t UDPProcessor::mac_[] = {0xAA, 0x00, 0x00, 0x00, 0x01, 0x01};
unsigned UDPProcessor::local_rx_port_ = 49900;
unsigned UDPProcessor::local_tx_port_ = 59001;
#endif
