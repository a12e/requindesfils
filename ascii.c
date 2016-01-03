//
// Created by abrooke on 21/12/15.
//

#include <ctype.h>
#include <stdio.h>
#include "ascii.h"

#define ASCII_MAX_COLS 80

void print_ascii(unsigned char *str, size_t len) {
    size_t lines = 0;
    while(1) {
        for(int cols = 0; cols < ASCII_MAX_COLS; cols++) {
            if(lines * ASCII_MAX_COLS + cols >= len)
                return;
            if(isprint(str[lines * ASCII_MAX_COLS + cols])) {
                putchar(str[lines * ASCII_MAX_COLS + cols]);
            }
            else {
                putchar(' ');
            }
        }
        lines++;
    }
}
