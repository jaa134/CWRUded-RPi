#include "packetcollector.h"

PacketCollector::PacketCollector() {
    is_stop_requested = false;
}

int PacketCollector::start() {
    if (collect() < 0)
        return ERROR;

    printDevices();
    return 0;
}

int PacketCollector::collect() {
    pcap_t *handle;                         /* Session handle */
    char *dev;                              /* The device to sniff on */
    char errbuf[PCAP_ERRBUF_SIZE];          /* Error string */
    struct bpf_program fp;                  /* The compiled filter */
    bpf_u_int32 mask;                       /* Our netmask */
    bpf_u_int32 net;                        /* Our IP */
    struct pcap_pkthdr *header;             /* The header that pcap gives us */
    const u_char *packet;                   /* The actual packet */
    const struct sniff_ethernet *eth;       /* The ethernet header */

    /* Define the device */
    dev = pcap_lookupdev(errbuf);
    if (dev == nullptr) {
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
    int timeout = 100;
    handle = pcap_open_live(dev, ETHH_LEN, 1, timeout, errbuf);
    if (handle == nullptr) {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        return ERROR;
    }

    /* Compile and apply the filter */
    char filter_exp[] = "tcp or udp or icmp[icmptype] == 0 or arp[6:2] == 2";
    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return ERROR;
    }
    if (pcap_setfilter(handle, &fp) == -1) {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return ERROR;
    }

    /* Capture packets */
    while (!is_stop_requested && pcap_next_ex(handle, &header, &packet) >= 0) {
        long long currentTime = getCurrentTime();
        /* Extract ETH header */
        if (header->len < ETHH_LEN) {
            printf("* Invalid ETH header length: %u bytes\n", header->len);
            if (header->len == 0)
                printf("* Likely no packet received before timeout!\n");
            else
                printf("* Partial packet received!\n");
        }
        else {
            eth = (struct sniff_ethernet*)(packet);
            devices[toMAC(eth->ether_dhost)] = currentTime;
            devices[toMAC(eth->ether_shost)] = currentTime;
        }
    }

    /* And close the session */
    pcap_close(handle);

    return 0;
}

void PacketCollector::request_stop() {
    is_stop_requested = true;
}

std::string PacketCollector::toMAC(const u_int8_t octets[]) {
    char buffer[50];
    sprintf(buffer, "%02x:%02x:%02x:%02x:%02x:%02x",
        (unsigned char)octets[0],
        (unsigned char)octets[1],
        (unsigned char)octets[2],
        (unsigned char)octets[3],
        (unsigned char)octets[4],
        (unsigned char)octets[5]);
    return buffer;
}

long long PacketCollector::getCurrentTime() {
    return std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
}

void PacketCollector::printDevices() {
    printf("\n\n");

    if (devices.size() == 0)
        printf("Found no devices!\n");
    else
        printf("Found %d devices!\n", (int)devices.size());

    for (auto it : devices)
        printf("%s\t%lld\n", it.first.c_str(), it.second);
}
