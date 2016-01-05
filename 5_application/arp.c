#include <net/if_arp.h>
#include <netinet/in.h>
#include "arp.h"
#include "../print.h"
#include "../2_link/ethernet.h"
#include "../3_network/ip.h"

void handle_arp(const unsigned char *bytes) {
    struct arphdr *arp_hdr = (struct arphdr *)bytes;
    arp_hdr->ar_op = ntohs(arp_hdr->ar_op);
    print1("ARP     ");

    switch(arp_hdr->ar_op) {
        case ARPOP_REQUEST: print1("request"); break;
        case ARPOP_REPLY: print1("reply"); break;
        case ARPOP_RREQUEST: print1("r-request"); break;
        case ARPOP_RREPLY: print1("r-reply"); break;
        default: printf1("opcode %u", arp_hdr->ar_op); break;
    }

    bytes += sizeof(struct arphdr);

    if(arp_hdr->ar_hln == 6 && arp_hdr->ar_pln == 4) {
        print1(", ");
        print_ether_address((u_int8_t *) bytes);
        bytes += 6;
        print1(" (");
        print_ip_addr(*(int32_t *) bytes);
        bytes += 4;
        print1(") -> ");
        print_ether_address((u_int8_t *) bytes);
        bytes += 6;
        print1(" (");
        print_ip_addr(*(int32_t *) bytes);
        bytes += 4;
        print1(")");
    }
    print1("\n");
}
