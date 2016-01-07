#include <arpa/inet.h>
#include <string.h>
#include "bootp.h"
#include "../print.h"
#include "../2_link/ethernet.h"
#include "../3_network/ip.h"

void handle_bootp(const unsigned char *bytes) {
    struct bootphdr *bootp_hdr = (struct bootphdr *)bytes;
    printf1("BOOTP   %s ", bootp_hdr->bp_op == BOOTPREQUEST ? "request" : (bootp_hdr->bp_op == BOOTPREPLY ? "reply" : "unknown"));
    printf1("tid 0x%x", ntohl(bootp_hdr->bp_xid));
    if(bootp_hdr->bp_ciaddr.s_addr != 0) printf1(", client %s", inet_ntoa(bootp_hdr->bp_ciaddr));
    if(bootp_hdr->bp_htype == 1 && bootp_hdr->bp_hlen == 6 && bootp_hdr->bp_chaddr[0] != 0 && bootp_hdr->bp_chaddr[1] != 0) {
        print1(" ("); print_ether_address1(bootp_hdr->bp_chaddr); print1(")");
    }
    if(bootp_hdr->bp_yiaddr.s_addr != 0) printf1(", your %s", inet_ntoa(bootp_hdr->bp_yiaddr));
    if(bootp_hdr->bp_siaddr.s_addr != 0) printf1(", server %s", inet_ntoa(bootp_hdr->bp_siaddr));
    if(bootp_hdr->bp_giaddr.s_addr != 0) printf1(", gate %s", inet_ntoa(bootp_hdr->bp_giaddr));
    putchar1('\n');

    u_int8_t *pvendor = bootp_hdr->bp_vend;
    const u_int8_t magic_cookie[] = VM_RFC1048;

    if(memcmp(pvendor, magic_cookie, 4) == 0) {
        pvendor += 4;
        printf3("        magic cookie 0x%02x%02x%02x%02x\n", magic_cookie[0], magic_cookie[1], magic_cookie[2], magic_cookie[3]);

        while(1) {
            u_int8_t option = *pvendor++;
            u_int8_t len = *pvendor++;

            if(option != 0) printf3("        option %u(%u): ", option, len);

            switch (option) {
                case TAG_PAD:
                    break;
                case TAG_SUBNET_MASK:
                    print3("Subnet mask: "); print_ip_addr3(*(int32_t *)pvendor);
                    break;
                case TAG_TIME_OFFSET:
                    break;
                case TAG_GATEWAY:
                    print3("Gateway: "); print_ip_addr3(*(int32_t *)pvendor);
                    break;
                case TAG_TIME_SERVER:
                    break;
                case TAG_NAME_SERVER:
                    break;
                case TAG_DOMAIN_SERVER:
                    break;
                case TAG_LOG_SERVER:
                    break;
                case TAG_COOKIE_SERVER:
                    break;
                case TAG_LPR_SERVER:
                    break;
                case TAG_IMPRESS_SERVER:
                    break;
                case TAG_RLP_SERVER:
                    break;
                case TAG_HOSTNAME:
                    break;
                case TAG_BOOTSIZE:
                    break;
                case TAG_END:
                    print3("end of options\n");
                    return;
                case TAG_REQUESTED_IP:
                    print3("Requested IP: "); print_ip_addr3(*(int32_t *)pvendor);
                    break;
                case TAG_IP_LEASE:
                    printf3("IP lease time: %us", ntohl(*(u_int32_t *)pvendor))
                    break;
                case TAG_OPT_OVERLOAD:
                    break;
                case TAG_TFTP_SERVER:
                    break;
                case TAG_BOOTFILENAME:
                    break;
                case TAG_DHCP_MESSAGE: {
                    u_int8_t dhcp_message = *pvendor;
                    switch (dhcp_message) {
                        case DHCPDISCOVER:
                            print3("DHCP Discover");
                            break;
                        case DHCPOFFER:
                            print3("DHCP Offer");
                            break;
                        case DHCPREQUEST:
                            print3("DHCP Request");
                            break;
                        case DHCPDECLINE:
                            print3("DHCP Decline");
                            break;
                        case DHCPACK:
                            print3("DHCP Ack");
                            break;
                        case DHCPNAK:
                            print3("DHCP N-Ack");
                            break;
                        case DHCPRELEASE:
                            print3("DHCP Release");
                            break;
                        case DHCPINFORM:
                            print3("DHCP Inform");
                            break;
                        default:
                            break;
                    }
                    break;
                }
                case TAG_SERVER_ID:
                    print3("DHCP server identifier: "); print_ip_addr3(*(int32_t *)pvendor);
                    break;
                case TAG_PARM_REQUEST:
                    print3("Parameter request list");
                    break;
                case TAG_MESSAGE:
                    break;
                case TAG_MAX_MSG_SIZE:
                    break;
                case TAG_RENEWAL_TIME:
                    printf3("Renewal time: %us", ntohl(*(u_int32_t*)pvendor));
                    break;
                case TAG_REBIND_TIME:
                    printf3("Rebinding time: %us", ntohl(*(u_int32_t*)pvendor));
                    break;
                case TAG_VENDOR_CLASS:
                    break;
                case TAG_CLIENT_ID:
                    print3("Client identifier: "); print_ether_address3(pvendor + 1);
                    break;
                default:
                    break;
            }

            if(option != 0) putchar3('\n');
            pvendor += len;
        }
    }
}
