#ifndef REQUINDESFILS_ETHER_H
#define REQUINDESFILS_ETHER_H

#include <net/ethernet.h>

void print_ether_address(u_int8_t ether_addr[ETH_ALEN]);
void print_ether_type(u_int16_t type);

void handle_ethernet(const unsigned char *bytes);

#endif //REQUINDESFILS_ETHER_H
