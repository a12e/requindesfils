#ifndef REQUINDESFILS_PRINT_H
#define REQUINDESFILS_PRINT_H

#include <stdio.h>

extern int verbosity;

#define printf1(format, ...) if(verbosity == 1) printf(format, __VA_ARGS__)
#define printf2(format, ...) if(verbosity == 2) printf(format, __VA_ARGS__)
#define printf3(format, ...) if(verbosity == 3) printf(format, __VA_ARGS__)

#endif //REQUINDESFILS_PRINT_H
