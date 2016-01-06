#include <netinet/tcp.h>
#include <netinet/in.h>
#include "tcp.h"
#include "../print.h"
#include "../5_application/http.h"

static int flag_count;

void print_flag(const char *name) {
    if(flag_count > 0) {
        printf2(", %s", name);
    }
    else {
        print2(name);
    }
    flag_count++;
}

void print_flags(u_int8_t flags) {
    flag_count = 0;
    if(flags & TH_FIN) print_flag("FIN");
    if(flags & TH_SYN) print_flag("SYN");
    if(flags & TH_RST) print_flag("RST");
    if(flags & TH_PUSH) print_flag("PUSH");
    if(flags & TH_ACK) print_flag("ACK");
    if(flags & TH_URG) print_flag("URG");
}

void handle_tcp(const unsigned char *bytes) {
    struct tcphdr *tcp_hdr = (struct tcphdr *) bytes;
    tcp_hdr->th_sport = ntohs(tcp_hdr->th_sport);
    tcp_hdr->th_dport = ntohs(tcp_hdr->th_dport);
    printf2("TCP     %u -> %u, [", tcp_hdr->th_sport, tcp_hdr->th_dport);
    print_flags(tcp_hdr->th_flags);
    print2("], ");
    printf2("seq %u, ack %u\n", ntohl(tcp_hdr->seq), ntohl(tcp_hdr->ack_seq));

    const unsigned char *end = bytes + 4 * tcp_hdr->th_off;
    bytes += sizeof(struct tcphdr);

    while(bytes < end) {
        uint8_t kind = *bytes++;

        uint8_t len = 0;
        if(kind != 0 && kind != 1)
            len = *bytes++;

        printf3("        option %u ", kind);
        switch(kind) {
            case 0: print3("end of options"); break;
            case 1: print3("no operation (NOP)"); break;
            case 2: printf3("MSS %u", (*(uint32_t*) bytes)); break;
            case 3: print3("window scale"); break;
            case 4: print3("SACK permited"); break;
            case 5: print3("SACK"); break;
            case 8: print3("timestamps"); break;
            default: break;
        }
        print3("\n");

        bytes += len;
    }

    if(tcp_hdr->th_sport == 80 || tcp_hdr->th_dport == 80) {
        handle_http((const char *)bytes);
    }
    else {
        print2("???\n");
    }
}
