#include <iostream>
/*
 * http://www.tcpdump.org/pcap.html
*/

#include <arpa/inet.h>
#include <netinet/ether.h>
#include <net/ethernet.h>
#include <pcap.h>
#include "sniff_ethernet.h"
#include "sniff_ip.h"
#include "sniff_tcp.h"

#define ERROR -1;

using namespace std;

int main()
{
    cout << "Starting!" << endl;

    pcap_t *handle;                         /* Session handle */
    char *dev;                              /* The device to sniff on */
    char errbuf[PCAP_ERRBUF_SIZE];          /* Error string */
    struct bpf_program fp;                  /* The compiled filter */
    char filter_exp[] = "ip";               /* The filter expression */
    bpf_u_int32 mask;                       /* Our netmask */
    bpf_u_int32 net;                        /* Our IP */
    struct pcap_pkthdr *header;              /* The header that pcap gives us */
    const u_char *packet;                   /* The actual packet */
    const struct sniff_ethernet *ethernet;  /* The ethernet header */
    const struct sniff_ip *ip;              /* The IP header */
    const struct sniff_tcp *tcp;            /* The TCP header */

    /* Define the device */
    dev = pcap_lookupdev(errbuf);
    if (dev == NULL) {
        fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
        return ERROR;
    }

    /* Find the properties for the device */
    if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuf);
        net = 0;
        mask = 0;
    }

    /* Open the session in promiscuous mode */
    handle = pcap_open_live(dev, BUFSIZ, 1, 0, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        return ERROR;
    }

    /* Compile and apply the filter */
    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return ERROR;
    }

    if (pcap_setfilter(handle, &fp) == -1) {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return ERROR;
    }

    u_int size_ip;
    u_int size_tcp;

    while (pcap_next_ex(handle, &header, &packet) >= 0) {
        /* Extract ETH header */
        if (header->caplen < SIZE_ETHERNET) {
            printf("* Invalid ETH header length: %u bytes\n", header->caplen);
            printf("* Likely no packet recieved before timeout!");
            return ERROR;
        }
        ethernet = (struct sniff_ethernet*)(packet);

        /* Extract IP header */
        ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
        size_ip = IP_HL(ip) * 4;
        if (size_ip < SIZE_IP) {
            printf("   * Invalid IP header length: %u bytes\n", size_ip);
            return ERROR;
        }


        printf("\n\n");

        printf("src MAC: %s\n", ether_ntoa(&ethernet->ether_shost));
        printf("dst MAC: %s\n", ether_ntoa(&ethernet->ether_dhost));

        printf("src address: %s\n", inet_ntoa(ip->ip_src));
        printf("dst address: %s\n", inet_ntoa(ip->ip_dst));


        /* Extract TCP header */
        if (ip->ip_p == IPPROTO_TCP) {
            tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
            size_tcp = TH_OFF(tcp) * 4;
            if (size_tcp < SIZE_TCP) {
                printf("   * Invalid TCP header length: %u bytes\n", size_tcp);
                return ERROR;
            }

            printf("src port: %d\n", ntohs(tcp->th_sport));
            printf("dst port: %d \n", ntohs(tcp->th_dport));
        }
        /* Print protocol info */
        else {
            string type;
            switch (ip->ip_p) {
                case IPPROTO_ICMP: type = "ICMP"; break;
                case IPPROTO_IGMP: type = "IGMP"; break;
                case IPPROTO_UDP: type = "UDP"; break;
                default: type = to_string(ip->ip_p);
            }
            printf("Other (unsupported) protocol: %s\n", type.c_str());
        }
    }

    /* And close the session */
    pcap_close(handle);

    return 0;
}
