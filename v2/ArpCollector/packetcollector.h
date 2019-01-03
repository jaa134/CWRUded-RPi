#ifndef PACKETCOLLECTOR_H
#define PACKETCOLLECTOR_H

/*
 * http://www.tcpdump.org/pcap.html
*/

#include <stdlib.h>
#include <arpa/inet.h>
#include <pcap.h>
#include <chrono>
#include <unordered_map>

#define ERROR -1;
#define ETHH_LEN 14
#define ETH_ALEN 6

class PacketCollector
{

    /* Ethernet header */
    struct sniff_ethernet {
        u_int8_t ether_dhost[ETH_ALEN]; /* Destination host address */
        u_int8_t ether_shost[ETH_ALEN]; /* Source host address */
        u_short ether_type;             /* IP? ARP? RARP? etc */
    };

public:
    PacketCollector();
    int start();
    void request_stop();

private:
    int collect();
    std::string toMAC(const u_int8_t addr[]);
    void printDevices();
    long long getCurrentTime();

    bool is_stop_requested;
    std::unordered_map<std::string, long long> devices;
};

#endif // PACKETCOLLECTOR_H
