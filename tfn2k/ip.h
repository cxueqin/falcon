/*
 * Tribe FloodNet - 2k edition
 * by Mixter <mixter@newyorkoffice.com>
 *
 * ip.h - low level IP definitions
 *
 * This program is distributed for educational purposes and without any
 * explicit or implicit warranty; in no event shall the author or
 * contributors be liable for any direct, indirect or incidental damages
 * arising in any way out of the use of this software.
 *
 */

#ifndef _IP_H
#define _IP_H
#include <sys/socket.h>

#ifndef INADDR_ANY
#define INADDR_ANY              ((unsigned) 0x00000000)
#endif

#ifndef IP_HDRINCL
#define IP_HDRINCL 3
#endif
#ifndef PF_INET
#define PF_INET 2
#endif
#ifndef AF_INET
#define AF_INET PF_INET
#endif

typedef char s8;
typedef unsigned char u8;
typedef short int s16;
typedef unsigned short int u16;
typedef int s32;
typedef unsigned int u32;

#define ICMP_ECHOREPLY	0
#define ICMP_ECHO	8

#ifndef htons

#if __BYTE_ORDER == __BIG_ENDIAN
#define ntohl(x)        (x)
#define ntohs(x)        (x)
#define htonl(x)        (x)
#define htons(x)        (x)
#else
unsigned long int htonl (unsigned int hostlong);
unsigned short int htons (unsigned short int hostshort);
unsigned long int ntohl (unsigned  int netlong);
unsigned short int ntohs (unsigned short int netshort);
#endif

#endif

#define IP 0
#define ICMP 1
#define IGMP 2
#define TCP 6
#define UDP 17
#define RAW 255

struct sa
  {
    u16 fam, dp;
    u32 add;
    u8 zero[8];
  };

struct su
  {
    u16 fam;
    char path[108];
  };

struct ip
  {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    u8 ihl:4, ver:4;
#else
    u8 ver:4, ihl:4;
#endif
    u8 tos;
    u16 tl, id, off;
    u8 ttl, pro;
    u16 sum;
    u32 src, dst;
  };

struct tcp
  {
    u16 src, dst;
    u32 seq, ack;
#if __BYTE_ORDER == __LITTLE_ENDIAN
    u8 x2:4, off:4;
#else
    u8 off:4, x2:4;
#endif
    u8 flg;			/* flag1 | flag2 */
#define FIN  0x01
#define SYN  0x02
#define RST  0x04
#define PUSH 0x08
#define ACK  0x10
#define URG  0x20
    u16 win, sum, urp;
  };

struct udp
  {
    u16 src, dst, len, sum;
  };

struct icmp
  {
    u8 type, code;
    u16 sum;
    u16 id, seq;
  };

struct phdr
  {
    unsigned  int sip, dip;
    char  zero;
    char proto;
    unsigned short len;
  };

char *inet_ntoa (struct in_addr);
unsigned int inet_addr (const char *cp);
u16 cksum (u16 *, int);
unsigned short ip_sum (unsigned short *, int);
unsigned int ip_sum2 (unsigned short *, int);
char *ntoa (u32);
int isip (char *);
unsigned int resolve (char *);

#endif
