//
// Created by abrooke on 17/12/15.
//

#include <stdio.h>
#include "ether.h"

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