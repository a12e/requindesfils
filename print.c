#include "print.h"

void putchar1(int c) {
    if(verbosity >= 1 && c != '\r') putchar(c);
}

void putchar2(int c) {
    if(verbosity >= 2 && c != '\r') putchar(c);
}

void putchar3(int c) {
    if(verbosity >= 3 && c != '\r') putchar(c);
}

void print1(const char *str) {
    if(verbosity >= 1) {printf(str);}
}

void print2(const char *str) {
    if(verbosity >= 2) {printf(str);}
}

void print3(const char *str) {
    if(verbosity >= 3) {printf(str);}
}
