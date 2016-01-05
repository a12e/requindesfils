#include <stdio.h>
#include <pcap/pcap.h>
#include <stdlib.h>
#include <getopt.h>
#include "2_link/ethernet.h"

int verbosity = 1;
char errbuf[PCAP_ERRBUF_SIZE];

void abort_pcap(const char *error) {
    perror(error);
    fprintf(stderr, "%s\n", errbuf);
    exit(EXIT_FAILURE);
}

void packet_handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    printf("--------------------------------------------------------------------------------\n");
    handle_ethernet(bytes);
}

int main(int argc, char **argv) {
    char *device = NULL;
    pcap_t *capture;
    bpf_u_int32 ip;
    bpf_u_int32 mask;
    const char *filter = "";
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
            case 'v':
                verbosity = atoi(optarg);
                if(verbosity < 1 || verbosity > 3) {
                    fprintf(stderr, "invalid verbosity level %d\n", verbosity);
                    exit(EXIT_FAILURE);
                }
                break;
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