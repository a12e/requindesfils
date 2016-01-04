#ifndef REQUINDESFILS_PRINT_H
#define REQUINDESFILS_PRINT_H

#include <stdio.h>

extern int verbosity;

#define printf1(format, ...) if(verbosity == 1) printf(format, __VA_ARGS__);
#define printf2(format, ...) if(verbosity == 2) printf(format, __VA_ARGS__);
#define printf3(format, ...) if(verbosity == 3) printf(format, __VA_ARGS__);

#define putchar1(c) if(verbosity == 1) putchar(c);
#define putchar2(c) if(verbosity == 2) putchar(c);
#define putchar3(c) if(verbosity == 3) putchar(c);

#define print1(str) if(verbosity == 1) printf(str);
#define print2(str) if(verbosity == 2) printf(str);
#define print3(str) if(verbosity == 3) printf(str);

#endif //REQUINDESFILS_PRINT_H
