#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include "ip.h"
#include "../4_transport/udp.h"
#include "../4_transport/tcp.h"

void print_ip_addr(int ip) {
    int bytes[4];
    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;
    printf("%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);
}

void handle_ip(const unsigned char *bytes) {
    struct iphdr *ip_hdr = (struct iphdr *)bytes;
    printf("IP      source="); print_ip_addr(ip_hdr->saddr);
    printf(" dest="); print_ip_addr(ip_hdr->daddr);
    printf(" version=%u ihl=%u tos=%u len=%u ", ip_hdr->version, ip_hdr->ihl, ip_hdr->tos, ntohs(ip_hdr->tot_len));
    printf("id=%u frag_off=%u ttl=%u ", ip_hdr->id, ip_hdr->frag_off, ip_hdr->ttl);
    printf("protocol=Ox%x checksum=%u ", ip_hdr->protocol, ip_hdr->check);

    const u_char *ip_hdr_end = bytes + 4 * ip_hdr->ihl;

    printf(" options=");
    while(bytes < ip_hdr_end) {
        bytes++;
    }
    printf("\n");

    if(ip_hdr->protocol == 0x11) {
        handle_udp(bytes);
    }
    else if(ip_hdr->protocol == 0x06) {
        handle_tcp(bytes);
    }
    else {
        printf("???     unsupported transport protocol 0x%x\n", ip_hdr->protocol);
    }
}
