#define TELCMDS
#define TELOPTS
#include <stdint.h>
#include <netinet/in.h>
#include "telnet.h"
#include "../print.h"
#include "../3_network/ip.h"

void handle_telnet(const unsigned char *bytes, uint16_t frame_len) {
    print1("TELNET  ");
    print_ips_from_last_header_v1();
    printf1("%u bytes\n", frame_len);

    const unsigned char *end = bytes + frame_len;

    while(bytes < end) {
        if(*bytes & 0xFF) {
            bytes++;
            uint8_t command = *bytes++;
            printf3("        cmd %s (%u): ", TELCMD_OK(command) ? TELCMD(command) : "CMD?", command);
            switch(command) {
                case DO:
                case DONT:
                case WONT:
                case WILL: {
                    uint8_t option = *bytes++;
                    printf3("%s (%u)", TELOPT(option), option);
                    break;
                }
                case SB: {
                    uint8_t suboption = *bytes++;
                    printf3("%s (%u)", TELOPT(suboption), suboption);
                    switch(suboption) {
                        case TELOPT_TSPEED:
                            printf3(" = %u", *bytes++);
                            break;
                        case TELOPT_NAWS:
                            printf3(" = %u x %u", ntohs(*(uint16_t*)&bytes[0]), ntohs(*(uint16_t*)&bytes[2]));
                            bytes += 4;
                            break;
                        default:
                            break;
                    }
                    break;
                }
                case SE:
                    print3("end of suboptions");
                    break;
                default:
                    print3("unknown command");
                    break;
            }
            putchar3('\n');
            if(command == SE)
                break; // end of options
        }
        else {
            bytes++;
        }
    }
}
