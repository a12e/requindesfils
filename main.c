#include <stdio.h>
#include <pcap/pcap.h>
#include <stdlib.h>
#include <getopt.h>
#include "2_link/ethernet.h"
#include "3_network/ip.h"

int verbosity = 1;
char errbuf[PCAP_ERRBUF_SIZE];

void abort_pcap(const char *error) {
    perror(error);
    fprintf(stderr, "%s\n", errbuf);
    exit(EXIT_FAILURE);
}

void packet_handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    static int packet_count = 0;

    printf("--(");
    int number_len = printf("%d)", packet_count++);
    for(int i = 3 + number_len; i <= 80; i++) putchar('-');
    putchar('\n');

    handle_ethernet(bytes);
}

int main(int argc, char **argv) {
    char *device = NULL; // when capturing live
    char *capture_file = NULL; // when reading capture file
    pcap_t *capture;
    bpf_u_int32 ip;
    bpf_u_int32 mask = PCAP_NETMASK_UNKNOWN;
    const char *filter = "";
    struct bpf_program bpf_program;

    int option;
    while((option = getopt(argc, argv, "hi:o:f:v:")) != -1) {
        switch(option) {
            case 'h':
                fprintf(stderr, "usage: %s [-v verbosity] [-f filter] [-i interface]|[-o capture_file]\n", argv[0]);
                exit(EXIT_SUCCESS);
            case 'i':
                device = optarg;
                break;
            case 'o':
                capture_file = optarg;
                break;
            case 'f':
                filter = optarg;
                printf("using \"%s\" as BPF filter\n", filter);
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

    if(device != NULL && capture_file != NULL) {
        fprintf(stderr, "error: you must choose between live capture or capture file\n");
        exit(EXIT_FAILURE);
    }

    if(capture_file == NULL) {
        if(device == NULL && (device = pcap_lookupdev(errbuf)) == NULL) {
            abort_pcap("pcap_lookupdev");
        }
        printf("using device %s for capture (", device);

        if(pcap_lookupnet(device, &ip, &mask, errbuf) != 0) {
            abort_pcap("pcap_lookupnet");
        }

        print_ip_addr(ip); printf(" ^ "); print_ip_addr(mask); printf(")\n");

        if((capture = pcap_open_live(device, 1500, 0, 100, errbuf)) == NULL) {
            abort_pcap("pcap_open_live");
        }
    }
    else {
        if((capture = pcap_open_offline(capture_file, errbuf)) == NULL) {
            abort_pcap("pcap_open_offline");
        }
    }

    if(pcap_compile(capture, &bpf_program, filter, 0, mask) != 0) {
        abort_pcap("pcap_compile");
    }

    if(pcap_setfilter(capture, &bpf_program) != 0) {
        abort_pcap("pcap_setfilter");
    }

    pcap_freecode(&bpf_program);

    pcap_loop(capture, -1, &packet_handler, NULL);

    return EXIT_SUCCESS;
}