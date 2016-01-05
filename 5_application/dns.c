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
            //printf1("(>>%d)", offset);
            print_label(base, base + offset);
            total += 2;
            total -= 1; // to cancel the normal total + 1 at the end
            break;
        }
        else {
            //printf1("(%u)", len);
            total += 1 + len;
            for (int i = 0; i < len; i++) {
                putchar1(*(bytes++));
            }
            putchar1('.');
        }
    }
    return total + 1;
}

void print_type(uint16_t type) {
    switch(type) {
        case 1: print1("A"); break;
        case 2: print1("NS"); break;
        case 5: print1("CNAME"); break;
        case 6: print1("SOA"); break;
        case 12: print1("PTR"); break;
        case 15: print1("MX"); break;
        case 16: print1("TXT"); break;
        case 28: print1("AAAA"); break;
        default: printf1("UNKNOWN(%u (%02X %02X))", type, ((uint8_t*)&type)[1], ((uint8_t*)&type)[0]); break;
    }
}

void print_class(uint16_t class) {
    switch(class) {
        case 1: print1("IN"); break;
        default: printf1("UNKNOWN(%u (%02X %02X))", class, ((uint8_t*)&class)[1], ((uint8_t*)&class)[0]); break;
    }
}

void handle_dns(const unsigned char *bytes) {
    struct dnshdr *dns_hdr = (struct dnshdr *)bytes;
    printf("DNS     #0x%x: %u questions, %u answers, %u authorities, %u resources\n",
           ntohs(dns_hdr->id), ntohs(dns_hdr->qdcount), ntohs(dns_hdr->ancount),
           ntohs(dns_hdr->nscount), ntohs(dns_hdr->arcount));

    bytes += sizeof(struct dnshdr);

    for(int i = 0; i < ntohs(dns_hdr->qdcount); i++) {
        print1("-QUESTN ");
        bytes += print_label((const unsigned char *)dns_hdr, bytes);
        putchar1(' ');
        print_type(ntohs(*(uint16_t *) bytes));
        bytes += 2;
        putchar1(' ');
        print_class(ntohs(*(uint16_t *) bytes));
        bytes += 2;
        putchar1('\n');
    }

    for(int i = 0; i < ntohs(dns_hdr->ancount); i++) {
        print1("-ANSWER ");
        bytes += print_label((const unsigned char *)dns_hdr, bytes);
        putchar1(' ');

        uint16_t type = ntohs(*(uint16_t *) bytes);
        print_type(type);
        bytes += 2;
        putchar1(' ');

        uint16_t class = ntohs(*(uint16_t *) bytes);
        print_class(class);
        bytes += 2;

        printf1(", ttl %u  ", ntohl(*(uint32_t*) bytes));
        bytes += 4;

        uint16_t rdlength = ntohs(*(uint16_t*) bytes);
        bytes += 2;

        if(type == 1 && class == 1) {
            print_ip_addr(*(int32_t*) bytes);
        }
        bytes += rdlength;

        print1("\n");
    }
}
