#include <stdio.h>
#include <netinet/in.h>
#include "ethernet.h"
#include "../3_network/ip.h"

void print_ether_address(u_int8_t ether_addr[ETH_ALEN]) {
    printf("%02x:%02x:%02x:%02x:%02x:%02x",
           ether_addr[0], ether_addr[1], ether_addr[2],
           ether_addr[3], ether_addr[4], ether_addr[5]);
}

void print_ether_type(u_int16_t type) {
    switch(type) {
        case ETHERTYPE_PUP:
            printf("PUP");
            break;          /* Xerox PUP */
        case ETHERTYPE_SPRITE:
            printf("SPRITE");
            break;        /* Sprite */
        case ETHERTYPE_IP:
            printf("IP");
            break;        /* IP */
        case ETHERTYPE_ARP:
            printf("ARP");
            break;        /* Address resolution */
        case ETHERTYPE_REVARP:
            printf("REVARP");
            break;        /* Reverse ARP */
        case ETHERTYPE_AT:
            printf("AT");
            break;        /* AppleTalk protocol */
        case ETHERTYPE_AARP:
            printf("AARP");
            break;        /* AppleTalk ARP */
        case ETHERTYPE_VLAN:
            printf("VLAN");
            break;        /* IEEE 802.1Q VLAN tagging */
        case ETHERTYPE_IPX:
            printf("IPX");
            break;        /* IPX */
        case ETHERTYPE_IPV6:
            printf("IPV6");
            break;        /* IP protocol version 6 */
        case ETHERTYPE_LOOPBACK:
            printf("LOOPBACK");
            break;        /* used to test interfaces */
        default:
            printf("UNKNOWN (0x%x)", type);
            break;
    }
}

void handle_ethernet(const unsigned char *bytes) {
    struct ether_header *eth_hdr = (struct ether_header *)bytes;
    eth_hdr->ether_type = ntohs(eth_hdr->ether_type);
    printf("ETHER   src="); print_ether_address(eth_hdr->ether_shost);
    printf(" dest="); print_ether_address(eth_hdr->ether_dhost);
    printf(" type="); print_ether_type(eth_hdr->ether_type);
    printf("\n");

    bytes += sizeof(struct ether_header);

    switch(eth_hdr->ether_type) {
        case ETHERTYPE_IP: return handle_ip(bytes);
        default: return;
    }
}