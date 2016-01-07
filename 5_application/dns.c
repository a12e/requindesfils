#include <netinet/in.h>
#include "dns.h"
#include "../print.h"
#include "../3_network/ip.h"

int print_label(const unsigned char *base, const unsigned char *bytes) {
    int total = 0;
    int len;
    while((len = *(bytes++)) != 0) {
        if(len & 0b11000000) {
            int offset = (len & 0b00111111) << 8 | *(bytes++);
            //printf3("(@%d)", offset);
            print_label(base, base + offset);
            total += 2;
            total -= 1; // to cancel the normal total + 1 at the end
            break;
        }
        else {
            //printf3("(%u)", len);
            total += 1 + len;
            for (int i = 0; i < len; i++) {
                putchar3(*(bytes++));
            }
            putchar3('.');
        }
    }
    return total + 1;
}

void print_type(uint16_t type) {
    switch(type) {
        case 1: print3("A"); break;
        case 2: print3("NS"); break;
        case 5: print3("CNAME"); break;
        case 6: print3("SOA"); break;
        case 12: print3("PTR"); break;
        case 15: print3("MX"); break;
        case 16: print3("TXT"); break;
        case 28: print3("AAAA"); break;
        default: printf3("UNKNOWN(%u (%02X %02X))", type, ((uint8_t*)&type)[1], ((uint8_t*)&type)[0]); break;
    }
}

void print_class(uint16_t class) {
    switch(class) {
        case 1: print3("IN"); break;
        default: printf3("UNKNOWN(%u (%02X %02X))", class, ((uint8_t*)&class)[1], ((uint8_t*)&class)[0]); break;
    }
}

void handle_dns(const unsigned char *bytes) {
    struct dnshdr *dns_hdr = (struct dnshdr *)bytes;
    print1("DNS     ");
    print_ips_from_last_header_v1();
    printf1("#0x%x: %u questions, %u answers, %u authorities, %u resources\n",
           ntohs(dns_hdr->id), ntohs(dns_hdr->qdcount), ntohs(dns_hdr->ancount),
           ntohs(dns_hdr->nscount), ntohs(dns_hdr->arcount));

    bytes += sizeof(struct dnshdr);

    for(int i = 0; i < ntohs(dns_hdr->qdcount); i++) {
        print3("        question ");
        bytes += print_label((const unsigned char *)dns_hdr, bytes);
        putchar3(' ');
        print_type(ntohs(*(uint16_t *) bytes));
        bytes += 2;
        putchar3(' ');
        print_class(ntohs(*(uint16_t *) bytes));
        bytes += 2;
        putchar3('\n');
    }

    for(int i = 0; i < ntohs(dns_hdr->ancount); i++) {
        print3("        answer ");
        bytes += print_label((const unsigned char *)dns_hdr, bytes);
        putchar3(' ');

        uint16_t type = ntohs(*(uint16_t *) bytes);
        print_type(type);
        bytes += 2;
        putchar3(' ');

        uint16_t class = ntohs(*(uint16_t *) bytes);
        print_class(class);
        bytes += 2;

        printf3(", ttl %u  ", ntohl(*(uint32_t*) bytes));
        bytes += 4;

        uint16_t rdlength = ntohs(*(uint16_t*) bytes);
        bytes += 2;

        if(type == 1 && class == 1) {
            print_ip_addr3(*(int32_t*) bytes);
        }
        bytes += rdlength;

        print3("\n");
    }
}
