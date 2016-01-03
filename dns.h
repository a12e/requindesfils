//
// Created by abrooke on 17/12/15.
//

#ifndef REQUINDESFILS_DNS_H
#define REQUINDESFILS_DNS_H

#include <endian.h>

// https://opensource.apple.com/source/ChatServer/ChatServer-260/jabberd2/resolver/dns.c
struct dnshdr {
    unsigned        id :16;         /* query identification number */
#if BYTE_ORDER == BIG_ENDIAN
    /* fields in third byte */
    unsigned        qr: 1;          /* response flag */
    unsigned        opcode: 4;      /* purpose of message */
    unsigned        aa: 1;          /* authoritive answer */
    unsigned        tc: 1;          /* truncated message */
    unsigned        rd: 1;          /* recursion desired */
    /* fields in fourth byte */
    unsigned        ra: 1;          /* recursion available */
    unsigned        unused :3;      /* unused bits (MBZ as of 4.9.3a3) */
    unsigned        rcode :4;       /* response code */
#endif
#if BYTE_ORDER == LITTLE_ENDIAN
    /* fields in third byte */
    unsigned        rd :1;          /* recursion desired */
    unsigned        tc :1;          /* truncated message */
    unsigned        aa :1;          /* authoritive answer */
    unsigned        opcode :4;      /* purpose of message */
    unsigned        qr :1;          /* response flag */
    /* fields in fourth byte */
    unsigned        rcode :4;       /* response code */
    unsigned        unused :3;      /* unused bits (MBZ as of 4.9.3a3) */
    unsigned        ra :1;          /* recursion available */
#endif
    /* remaining bytes */
    unsigned        qdcount :16;    /* number of question entries */
    unsigned        ancount :16;    /* number of answer entries */
    unsigned        nscount :16;    /* number of authority entries */
    unsigned        arcount :16;    /* number of resource entries */
};


#endif //REQUINDESFILS_DNS_H
