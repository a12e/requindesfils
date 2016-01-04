#include <stdio.h>
#include <netinet/tcp.h>
#include "tcp.h"

void handle_tcp(const unsigned char *bytes) {
    struct tcphdr *tcp_hdr = (struct tcphdr *) bytes;
    printf("TCP  srcport=%u destport=%u seq=%u ack=%u offset=%u \n",
           tcp_hdr->th_sport, tcp_hdr->th_dport, tcp_hdr->seq, tcp_hdr->seq, tcp_hdr->th_off);

}
