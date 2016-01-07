#include "print.h"
#include "3_network/ip.h"
#include "2_link/ethernet.h"

void putchar1(int c) {
    if(verbosity >= 1 && c != '\r') putchar(c);
}

void putchar2(int c) {
    if(verbosity >= 2 && c != '\r') putchar(c);
}

void putchar3(int c) {
    if(verbosity >= 3 && c != '\r') putchar(c);
}

void print1(const char *str) {
    if(verbosity >= 1) {printf(str);}
}

void print2(const char *str) {
    if(verbosity >= 2) {printf(str);}
}

void print3(const char *str) {
    if(verbosity >= 3) {printf(str);}
}

void print_ether_address1(uint8_t *ether_addr) {
    if(verbosity >= 1) print_ether_address(ether_addr);
}

void print_ether_address2(uint8_t *ether_addr) {
    if(verbosity >= 2) print_ether_address(ether_addr);
}

void print_ether_address3(uint8_t *ether_addr) {
    if(verbosity >= 3) print_ether_address(ether_addr);
}

void print_ip_addr1(int32_t ip) {
    if(verbosity >= 1) print_ip_addr(ip);
}

void print_ip_addr2(int32_t ip) {
    if(verbosity >= 2) print_ip_addr(ip);
}

void print_ip_addr3(int32_t ip) {
    if(verbosity >= 3) print_ip_addr(ip);
}

