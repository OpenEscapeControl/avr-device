#include "UDPProcessor.h"
#include "ard_env.h"
#include "uart.h"

// SHOULD NOT BE < 400
uint8_t Ethernet::buffer[BUFSIZE];

UDPProcessor::UDPProcessor()
{
}

int UDPProcessor::initialize()
{
    if(!ether.begin(sizeof Ethernet::buffer, mac_))
    {
        uart_puts("ETHER ERROR!\n");
        return(-1);
    }

    dhcpInit();

    uart_puts("ether init ok\n");
    uart_printf("got IP %d.%d.%d.%d\n", ether.myip[0], ether.myip[1], ether.myip[2], ether.myip[3]);

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
