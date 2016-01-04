#include <arpa/inet.h>
#include "dhcp.h"
#include "bootp.h"
#include "../print.h"

void handle_dhcp(const unsigned char *bytes) {
    struct bootphdr *bootp_hdr = (struct bootphdr *)bytes;
    printf("BOOTP   %s", bootp_hdr->bp_op == BOOTPREQUEST ? "request" : (bootp_hdr->bp_op == BOOTPREPLY ? "reply" : "unknown"));
    printf(" tid=%u ", ntohs(bootp_hdr->bp_xid));
    printf("client=%s, %s ", inet_ntoa(bootp_hdr->bp_ciaddr), bootp_hdr->bp_chaddr);
    printf("your=%s ", inet_ntoa(bootp_hdr->bp_yiaddr));
    printf("server=%s, %s ", inet_ntoa(bootp_hdr->bp_siaddr), bootp_hdr->bp_sname);
    printf("gate=%s ", inet_ntoa(bootp_hdr->bp_giaddr));
    printf("file=%s\n", bootp_hdr->bp_file);
}
