#include <netinet/in.h>
#include "dns.h"
#include "../print.h"

void handle_dns(const unsigned char *bytes) {
    struct dnshdr *dns_hdr = (struct dnshdr *)bytes;
    printf("DNS     id=%u #questions=%u #answers=%u #authorities=%u resources=%u\n",
           ntohs(dns_hdr->id), ntohs(dns_hdr->qdcount), ntohs(dns_hdr->ancount),
           ntohs(dns_hdr->nscount), ntohs(dns_hdr->arcount));

    for(int i = 0; i < ntohs(dns_hdr->qdcount); i++) {

    }


}
