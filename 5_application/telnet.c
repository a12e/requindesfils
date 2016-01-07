#define TELCMDS
#define TELOPTS
#include <stdint.h>
#include "telnet.h"
#include "../print.h"
#include "ascii.h"

void handle_telnet(const unsigned char *bytes, uint16_t frame_len) {
    printf1("TELNET  %u bytes\n", frame_len);

    const unsigned char *end = bytes + frame_len;
    print_hex(bytes, frame_len);

    while(bytes < end) {
        if(*bytes & 0xFF) {
            bytes++;
            uint8_t command = *bytes++;
            printf3("        cmd %s (%u)", TELCMD(command), command);
            switch(command) {
                case DO:
                case DONT:
                case WONT:
                case WILL: {
                    uint8_t option = *bytes++;
                    printf(": %s (%u)", TELOPT(option), option);
                    break;
                }
                default:
                    print3("unknown command");
                    break;
            }
            putchar3('\n');
            if(command == SE)
                break; // end of options
        }

        bytes++;
    }
}
