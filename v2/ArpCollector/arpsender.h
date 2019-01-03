#ifndef ARPSENDER_H
#define ARPSENDER_H

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <arpa/inet.h>
#include <chrono>
#include <thread>

#define ETH2_HEADER_LEN 14
#define HW_TYPE 1
#define MAC_LENGTH 6
#define IPV4_LENGTH 4
#define ARP_REQUEST 0x01
#define BUF_SIZE 60

#define debug(x...) printf(x);printf("\n");
#define info(x...) printf(x);printf("\n");
#define warn(x...) printf(x);printf("\n");
#define err(x...) printf(x);printf("\n");

class ArpSender
{

    struct arp_header {
        unsigned short hardware_type;
        unsigned short protocol_type;
        unsigned char hardware_len;
        unsigned char protocol_len;
        unsigned short opcode;
        unsigned char sender_mac[MAC_LENGTH];
        unsigned char sender_ip[IPV4_LENGTH];
        unsigned char target_mac[MAC_LENGTH];
        unsigned char target_ip[IPV4_LENGTH];
    };

public:
    ArpSender();
    int start();

private:
    int test_arping(const char *ifname);
    int get_if_info(const char *ifname, uint32_t *ip, char *mac, int *ifindex);
    int get_if_ip4(int fd, const char *ifname, uint32_t *ip);
    int int_ip4(struct sockaddr *addr, uint32_t *ip);
    int send_arp(int fd, int ifindex, char *mac, uint32_t src_ip);
    int bind_arp(int ifindex, int *fd);

};

#endif // ARPSENDER_H
