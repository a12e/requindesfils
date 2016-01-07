#include <string.h>
#include "http.h"
#include "../print.h"

void handle_http(const char *bytes) {
    print1("HTTP    ");

    if(strncmp("HTTP", bytes, 4) == 0
       || strncmp("GET", bytes, 3) == 0
       || strncmp("POST", bytes, 4) == 0
       || strncmp("PUT", bytes, 4) == 0
       || strncmp("DELETE", bytes, 6) == 0
       || strncmp("HEAD", bytes, 4) == 0) {

        while(*bytes != '\n')
            putchar1(*bytes++);

        putchar('\n');
        bytes++;

        while(strncmp("\r\n", bytes, 2) != 0) { // print headers (<==> until we have double \n)
            print3("        ");

            while(*bytes != '\n')
                putchar3(*bytes++);

            putchar3('\n');
            bytes++;
        }
    }
    else {
        print1("[... HTTP Content...]\n");
    }
}
