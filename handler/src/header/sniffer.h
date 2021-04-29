#ifndef SNIFFER_H
#define SNIFFER_H

#include <pcap.h>

#define UDP_PAYLOAD_OFFSET 42
#define UDP_HEADER_OFFSET 34
#define UDP_HEADER_LEN 8

typedef void(*sniffer_callback)(char * msg, size_t msg_len);

typedef struct {
    char nic_name[20];
    char filter[100];
    pcap_t *nic_descr;
} SNIFFER_INFO;

typedef struct {
    
} PACKET_INFO;

void sniffer_setup(SNIFFER_INFO * info);
void sniffer_start(SNIFFER_INFO info, sniffer_callback c);
void _packet_handler(u_char *ptr_null, const struct pcap_pkthdr *pkthdr, const u_char *packet);

#endif