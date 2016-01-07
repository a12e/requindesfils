#ifndef REQUINDESFILS_IP_H
#define REQUINDESFILS_IP_H

void print_ip_addr(int32_t ip);
void print_ips_from_last_header_v1(); // Prints src IP and and dest IP if verbosity is the lowest
void handle_ip(const unsigned char *bytes);

#endif //REQUINDESFILS_IP_H
