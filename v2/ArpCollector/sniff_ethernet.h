#ifndef SNIFF_ETHERNET_H
#define SNIFF_ETHERNET_H

#include <pcap.h>
#include <netinet/ether.h>

#define SIZE_ETHERNET 14

/* Ethernet header */
struct sniff_ethernet {
    const struct ether_addr ether_dhost; /* Destination host address */
    const struct ether_addr ether_shost; /* Source host address */
    u_short ether_type;                  /* IP? ARP? RARP? etc */
};

#endif // SNIFF_ETHERNET_H
