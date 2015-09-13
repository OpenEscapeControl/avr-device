#ifndef UDPPROCESSOR_H
#define UDPPROCESSOR_H

#include "EtherCard.h"
#include "udp_config.h"

class UDPProcessor
{
private:
    static const unsigned IP_BYTES = 4;

    static PROGMEM char const hostname_[];
    static uint8_t mac_[MAC_BYTES];
    static uint8_t remote_mac_[MAC_BYTES];
    static unsigned local_tx_port_;

    static uint8_t remote_ip_[IP_BYTES];
    static unsigned remote_port_;

    static unsigned local_rx_port_;

    static uint16_t buffer_counter_;

    static void dhcpInit();
public:
    UDPProcessor(const uint8_t physical_device);
    static int begin();
    static void tx(const char* msg, int size);
    static void tx(const char* cstr);
    static void setCallBack(UdpServerCallback callback);
    static void update();
    static uint8_t* getUDPPtr();
    static void tx(int size);
};

#endif
