#include <netinet/tcp.h>
#include <netinet/in.h>
#include "tcp.h"
#include "../print.h"
#include "../5_application/http.h"

extern void handle_telnet(const unsigned char *bytes, uint16_t frame_len);

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

void handle_tcp(const unsigned char *bytes, uint16_t segment_len) {
    struct tcphdr *tcp_hdr = (struct tcphdr *) bytes;
    tcp_hdr->th_sport = ntohs(tcp_hdr->th_sport);
    tcp_hdr->th_dport = ntohs(tcp_hdr->th_dport);
    printf2("TCP     %u -> %u, [", tcp_hdr->th_sport, tcp_hdr->th_dport);
    print_flags(tcp_hdr->th_flags);
    print2("], ");
    printf2("seq %u, ack %u, win %u\n", ntohl(tcp_hdr->seq), ntohl(tcp_hdr->ack_seq), ntohs(tcp_hdr->window));

    int data_offset = 4 * tcp_hdr->th_off;
    const unsigned char *end = bytes + data_offset;
    bytes += sizeof(struct tcphdr);

    while(bytes < end) {
        uint8_t kind = *bytes++;

        uint8_t len = 0;
        if(kind != 0 && kind != 1)
            len = *bytes++;

        printf3("        option %u: ", kind);
        switch(kind) {
            case 0: print3("end of options"); break;
            case 1: print3("no operation (NOP)"); break;
            case 2: printf3("MSS %u", (*(uint32_t*) bytes)); break;
            case 3: print3("window scale"); break;
            case 4: print3("SACK permited"); break;
            case 5: print3("SACK"); break;
            case 8: print3("timestamps"); break;
            default: print3("unknown"); break;
        }
        print3("\n");

        // advance by the size of the option read
        if(kind != 0 && kind != 1)
            bytes += len - 2;
    }

    if(tcp_hdr->th_sport == 80 || tcp_hdr->th_dport == 80) {
        handle_http((const char *) bytes);
    }
    else if(tcp_hdr->th_sport == 23 || tcp_hdr->th_dport == 23) {
        handle_telnet(bytes, segment_len - data_offset);
    }
    else {
        printf1("???     Unknown TCP application with ports %u -> %u\n", tcp_hdr->th_sport, tcp_hdr->th_dport);
    }
}
