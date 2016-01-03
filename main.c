#include <stdio.h>
#include <pcap/pcap.h>
#include <stdlib.h>
#include <net/ethernet.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <ctype.h>
#include "ether.h"
#include "dns.h"
#include "bootp.h"
#include "ascii.h"


char errbuf[PCAP_ERRBUF_SIZE];

void abort_pcap(const char *error) {
    perror(error);
    fprintf(stderr, "%s\n", errbuf);
    exit(EXIT_FAILURE);
}

void print_ip_addr(int ip) {
    int bytes[4];
    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;
    printf("%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);
}

void packet_handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    printf("-\n");

    struct ether_header *eth_hdr = (struct ether_header *)bytes;
    printf("ETHER   src="); print_ether_address(eth_hdr->ether_shost);
    printf(" dest="); print_ether_address(eth_hdr->ether_dhost);
    printf(" type="); print_ether_type(ntohs(eth_hdr->ether_type));
    printf("\n");

    bytes += sizeof(struct ether_header);

    struct iphdr *ip_hdr = (struct iphdr *)bytes;
    printf("IP      source="); print_ip_addr(ip_hdr->saddr);
    printf(" dest="); print_ip_addr(ip_hdr->daddr);
    printf(" version=%u ihl=%u tos=%u len=%u ", ip_hdr->version, ip_hdr->ihl, ip_hdr->tos, ntohs(ip_hdr->tot_len));
    printf("id=%u frag_off=%u ttl=%u ", ip_hdr->id, ip_hdr->frag_off, ip_hdr->ttl);
    printf("protocol=Ox%x checksum=%u ", ip_hdr->protocol, ip_hdr->check);

    const u_char *ip_hdr_end = bytes + 4 * ip_hdr->ihl;

    printf(" options=");
    while(bytes < ip_hdr_end) {
        bytes++;
    }
    printf("\n");

    if(ip_hdr->protocol == 0x11) {
        struct udphdr *udp_hdr = (struct udphdr *)bytes;
        u_short source = ntohs(udp_hdr->source), dest = ntohs(udp_hdr->dest);
        printf("UDP     srcport=%u destport=%u len=%u sum=%u\n",
                source, dest, ntohs(udp_hdr->len), ntohs(udp_hdr->check));

        bytes += sizeof(struct udphdr);

        if(source == 53 || dest == 53) {
            struct dnshdr *dns_hdr = (struct dnshdr *)bytes;
            printf("DNS     id=%u #questions=%u #answers=%u #authorities=%u resources=%u\n",
                   ntohs(dns_hdr->id), ntohs(dns_hdr->qdcount), ntohs(dns_hdr->ancount),
                   ntohs(dns_hdr->nscount), ntohs(dns_hdr->arcount));

            for(int i = 0; i < ntohs(dns_hdr->qdcount); i+) {

            }

            unsigned char *dns_payload = bytes + sizeof(struct dnshdr);
            print_ascii(dns_payload, ntohs(udp_hdr->len) - sizeof(struct dnshdr));

            printf("name = ");
            while(isalnum(*dns_payload)) {
                putchar(*dns_payload);
                dns_payload++;
            }

            union {
                unsigned short ui;
                char b[2];
            } type;
            type.b[1] = *dns_payload++;
            type.b[0] = *dns_payload++;

            printf(" type = %u", type.ui);

        }
        else if(source == IPPORT_BOOTPS || dest == IPPORT_BOOTPS) {
            struct bootphdr *bootp_hdr = (struct bootphdr *)bytes;
            printf("BOOTP   %s", bootp_hdr->bp_op == BOOTPREQUEST ? "request" : (bootp_hdr->bp_op == BOOTPREPLY ? "reply" : "unknown"));
            printf(" tid=%u ", ntohs(bootp_hdr->bp_xid));
            printf("client=%s, %s ", inet_ntoa(bootp_hdr->bp_ciaddr), bootp_hdr->bp_chaddr);
            printf("your=%s ", inet_ntoa(bootp_hdr->bp_yiaddr));
            printf("server=%s, %s ", inet_ntoa(bootp_hdr->bp_siaddr), bootp_hdr->bp_sname);
            printf("gate=%s ", inet_ntoa(bootp_hdr->bp_giaddr));
            printf("file=%s\n", bootp_hdr->bp_file);
        }
        else if(source == 137 || dest == 137) {
            printf("NETBIOS \n");
        }
        else {
            printf("???     unsupported UDP protocol\n");
        }
    }
    else if(ip_hdr->protocol == 0x06) {
        struct tcphdr *tcp_hdr = (struct tcphdr *) bytes;
        printf("TCP  srcport=%u destport=%u seq=%u ack=%u offset=%u \n",
               tcp_hdr->th_sport, tcp_hdr->th_dport, tcp_hdr->seq, tcp_hdr->seq, tcp_hdr->th_off);


    }
    else {
        printf("???     unsupported transport protocol 0x%x\n", ip_hdr->protocol);
    }
}

int main(int argc, char **argv) {
    char *device = NULL;
    pcap_t *capture;
    bpf_u_int32 ip;
    bpf_u_int32 mask;
    const char *filter = "udp";
    struct bpf_program bpf_program;

    int option;
    while((option = getopt(argc, argv, "i:o:f:v:")) != -1) {
        switch(option) {
            case 'i':
                device = optarg;
                break;
            case 'o': break;
            case 'f':
                filter = optarg;
                break;
            case 'v': break;
            default: break;
        }
    }

    if(device == NULL && (device = pcap_lookupdev(errbuf)) == NULL) {
        abort_pcap("pcap_lookupdev");
    }
    printf("using device %s for capture\n", device);

    if(pcap_lookupnet(device, &ip, &mask, errbuf) != 0) {
        abort_pcap("pcap_lookupnet");
    }

    if((capture = pcap_open_live(device, 1500, 0, 100, errbuf)) == NULL) {
        abort_pcap("pcap_open_live");
    }

    if(pcap_compile(capture, &bpf_program, filter, 0, mask) != 0) {
        abort_pcap("pcap_compile");
    }

    if(pcap_setfilter(capture, &bpf_program) != 0) {
        abort_pcap("pcap_setfilter");
    }

    pcap_loop(capture, -1, &packet_handler, NULL);

    return EXIT_SUCCESS;
}