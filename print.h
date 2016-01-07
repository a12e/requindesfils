#ifndef REQUINDESFILS_PRINT_H
#define REQUINDESFILS_PRINT_H

#include <stdio.h>
#include <stdint.h>
#include <linux/if_ether.h>

extern int verbosity;

#define printf1(format, ...) if(verbosity >= 1) {printf(format, __VA_ARGS__);}
#define printf2(format, ...) if(verbosity >= 2) {printf(format, __VA_ARGS__);}
#define printf3(format, ...) if(verbosity >= 3) {printf(format, __VA_ARGS__);}

void putchar1(int c);
void putchar2(int c);
void putchar3(int c);

void print1(const char *str);
void print2(const char *str);
void print3(const char *str);

void print_ether_address1(uint8_t ether_addr[ETH_ALEN]);
void print_ether_address2(uint8_t ether_addr[ETH_ALEN]);
void print_ether_address3(uint8_t ether_addr[ETH_ALEN]);

void print_ip_addr1(int32_t ip);
void print_ip_addr2(int32_t ip);
void print_ip_addr3(int32_t ip);

#endif //REQUINDESFILS_PRINT_H
