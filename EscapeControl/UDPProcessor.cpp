#include "UDPProcessor.h"

void rxCallback(unsigned len, const char* buf);

// SHOULD NOT BE < 400
uint8_t Ethernet::buffer[BUFSIZE];

uint8_t UDPProcessor::remote_mac_[] = {0xAA, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t UDPProcessor::remote_ip_[] = {192, 168, 100, 1};
unsigned UDPProcessor::local_tx_port_ = 59001;

uint8_t UDPProcessor::mac_[] = {0xAA, 0x00, 0x00, 0x00, 0x01, 0x00};
unsigned UDPProcessor::local_rx_port_ = 49900;
unsigned UDPProcessor::remote_port_ = 0;

void rxCallback1(unsigned port, unsigned char ip[], const char *data, unsigned len)
{
    rxCallback(len, data);
}


UDPProcessor::UDPProcessor(uint8_t mac_last)
{
    remote_port_ = 49930 + mac_last;
    mac_[5] = mac_last;
}

int UDPProcessor::begin()
{
    if(!ether.begin(sizeof Ethernet::buffer, mac_))
    {
        return(-1);
    }

    dhcpInit();

    setCallBack(rxCallback1);

    return(0);
}

void UDPProcessor::tx(const char *msg, int size)
{
    ether.sendUdp(msg, size, local_tx_port_, remote_ip_, remote_port_, remote_mac_);
}

void UDPProcessor::tx(const char *cstr)
{
    tx(cstr, strlen(cstr) + 1);
}

void UDPProcessor::setCallBack(UdpServerCallback callback)
{
    ether.udpServerListenOnPort(callback, local_rx_port_);
}

void UDPProcessor::update()
{
    ether.packetLoop(ether.packetReceive());
}

uint8_t *UDPProcessor::getUDPPtr()
{
    return(ether.getUDPPointer());
}

void UDPProcessor::tx(int size)
{
    ether.sendUDPPTR(size, local_tx_port_, remote_ip_, remote_port_, remote_mac_);
}

void UDPProcessor::dhcpInit()
{
    for(;;)
        if(ether.dhcpSetup(hostname_))
            return;
}
