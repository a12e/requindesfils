//
// Created by abrooke on 17/12/15.
//

#ifndef REQUINDESFILS_ETHER_H
#define REQUINDESFILS_ETHER_H

#include <net/ethernet.h>

void print_ether_address(u_int8_t ether_addr[ETH_ALEN]);
void print_ether_type(u_int16_t type);

#endif //REQUINDESFILS_ETHER_H
