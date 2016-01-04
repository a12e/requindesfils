#include <ctype.h>
#include <netinet/udp.h>
#include "udp.h"
#include "../print.h"
#include "../5_application/bootp.h"
#include "../5_application/dhcp.h"
#include "../5_application/dns.h"

void handle_udp(const unsigned char *bytes) {
    struct udphdr *udp_hdr = (struct udphdr *)bytes;
    u_short source = ntohs(udp_hdr->source), dest = ntohs(udp_hdr->dest);
    printf("UDP     srcport=%u destport=%u len=%u sum=%u\n",
           source, dest, ntohs(udp_hdr->len), ntohs(udp_hdr->check));

    bytes += sizeof(struct udphdr);

    if(source == 53 || dest == 53) {
        handle_dns(bytes);
    }
    else if(source == IPPORT_BOOTPS || dest == IPPORT_BOOTPS) {
        handle_dhcp(bytes);
    }
    else if(source == 137 || dest == 137) {
        printf("NETBIOS \n");
    }
    else {
        printf("???     unsupported UDP protocol\n");
    }
}
