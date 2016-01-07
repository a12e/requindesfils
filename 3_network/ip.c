#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include "ip.h"
#include "../4_transport/udp.h"
#include "../4_transport/tcp.h"
#include "../print.h"
#include "../4_transport/icmp.h"

void print_ip_addr(int32_t ip) {
    int32_t bytes[4];
    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;
    printf("%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);
}

static struct iphdr *ip_hdr;

void print_ips_from_last_header_v1() {
    if(verbosity == 1) {
        printf("[");
        print_ip_addr(ip_hdr->saddr);
        printf(" -> ");
        print_ip_addr(ip_hdr->daddr);
        printf("] ");
    }
}

void handle_ip(const unsigned char *bytes) {
    ip_hdr = (struct iphdr *)bytes;

    printf2("IPv%u    ", ip_hdr->version); print_ip_addr2(ip_hdr->saddr);
    print2(" -> "); print_ip_addr2(ip_hdr->daddr);
    printf3("ihl %u, tos %u, ", ip_hdr->ihl, ip_hdr->tos);
    uint16_t len = ntohs(ip_hdr->tot_len);
    printf2("len %u, ", len);
    printf2("id %u, ", ip_hdr->id);
    printf3("foff %u, ttl %u, ", ip_hdr->frag_off, ip_hdr->ttl);
    printf3("proto Ox%x, ", ip_hdr->protocol);
    printf3("chksum 0x%x ", ip_hdr->check);

    const u_char *ip_hdr_end = bytes + 4 * ip_hdr->ihl;

    while(bytes < ip_hdr_end) {
        bytes++;
    }
    print2("\n");

    if(ip_hdr->protocol == 0x01) {
        handle_icmp(bytes);
    }
    else if(ip_hdr->protocol == 0x11) {
        handle_udp(bytes);
    }
    else if(ip_hdr->protocol == 0x06) {
        handle_tcp(bytes, len - sizeof(struct iphdr));
    }
    else {
        printf("???     unsupported transport protocol 0x%x\n", ip_hdr->protocol);
    }
}
