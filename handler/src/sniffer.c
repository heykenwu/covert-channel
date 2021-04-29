#include <pcap.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/udp.h>
#include "config.h"
#include "header/sniffer.h"

void (*fun_ptr)(char *, size_t);

void sniffer_setup(SNIFFER_INFO *info)
{
    char errbuf[PCAP_ERRBUF_SIZE];
    struct bpf_program fp;
    bpf_u_int32 maskp, netp;

    // Use pcap to get the IP address and subnet mask of the device
    pcap_lookupnet(info->nic_name, &netp, &maskp, errbuf);
    // open device for reading & set the device in promiscuous mode
    if ((info->nic_descr = pcap_open_live(info->nic_name, BUFSIZ, 1, -1, errbuf)) == NULL)
        exit(1);
    // Compile the filter expression
    if ((pcap_compile(info->nic_descr, &fp, info->filter, 0, netp)) == -1)
        exit(1);
    // Load the filter into the capture device
    if (pcap_setfilter(info->nic_descr, &fp) == -1)
        exit(1);
    return;
}

void _packet_handler(u_char *ptr_null, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
    char *ptr = (char *)(packet + UDP_PAYLOAD_OFFSET);
    if ((pkthdr->caplen - UDP_PAYLOAD_OFFSET) <= 0)
        return;
    struct udphdr *udph = (struct udphdr *)(packet + UDP_HEADER_OFFSET);
    fun_ptr((char*)(packet + UDP_PAYLOAD_OFFSET), ntohs(udph->len) - UDP_HEADER_LEN);
}

void sniffer_start(SNIFFER_INFO info, sniffer_callback c)
{
    fun_ptr = c;
    pcap_loop(info.nic_descr, -1, _packet_handler, NULL);
}
