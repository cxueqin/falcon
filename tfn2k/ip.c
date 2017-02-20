/* 
* Tribe FloodNet - 2k edition 
* by Mixter <mixter@newyorkoffice.com> 
* 
* ip.c - low level IP functions 
* 
* This program is distributed for educational purposes and without any 
* explicit or implicit warranty; in no event shall the author or 
* contributors be liable for any direct, indirect or incidental damages 
* arising in any way out of the use of this software. 
* 
*/ 

#include "tribe.h" 
#include "ip.h" 

unsigned int 
resolve (char *host) 
{ 
   struct hostent *he; 
   struct sa tmp; 
   if (isip (host)){ 
       char *pchar; 
     for(pchar=host;;pchar++) 
         if(((*pchar)!='.')&&((*pchar<'0')||(*pchar >'9'))){
         
             *pchar='\0'; 
             break; 
             } 
     return (inet_addr (host)); 
       } 
   he = gethostbyname (host); 
   if (he) 
     { 
       memcpy ((caddr_t) & tmp.add, he->h_addr, he->h_length); 
     } 
   else 
     return (0); 
   return (tmp.add); 
} 

char * 
ntoa (u32 in) 
{ 
   struct in_addr ad; 
   ad.s_addr = in; 
   return (inet_ntoa (ad)); 
} 

int 
isip (char *ip) 
{ 
   int a, b, c, d; 
   sscanf (ip, "%d.%d.%d.%d", &a, &b, &c, &d); 
   if (a < 0) 
     return 0; 
   if (a > 255) 
     return 0; 
   if (b < 0) 
     return 0; 
   if (b > 255) 
     return 0; 
   if (c < 0) 
     return 0; 
   if (c > 255) 
     return 0; 
   if (d < 0) 
     return 0; 
   if (d > 255) 
     return 0; 
   return 1; 
} 
u16 
cksum (u16 * buf, int nwords) 
{ 
   unsigned int sum; 
   for (sum = 0; nwords > 0; nwords--) 
     sum += *buf++; 
   sum = (sum >> 16) + (sum & 0xffff); 
   sum += (sum >> 16); 
   return ~sum; 
} 
/* 
unsigned short cksum(u16 *data,int len) 
{ 
         unsigned int sum=0; 
         __asm__ __volatile__( 
     &, nbsp;    "pushl %%ecx\r\n\t" 
         "pushl %%edx\r\n\t" 
                 "shrl $1,%%ecx\r\n\t" 
                 "jnc 1f\r\n\t" 
                 "movzbw (%%edx,%%ecx,2),%%ax\r\n\t" 
                 "1:jcxz 4f\r\n\t" 
                 "shrl $1,%%ecx\r\n\t" 
                 "jnc 2f\r\n\t" 
                 "addw (%%edx,%%ecx,4),%%ax\r\n\t" 
                 "2:jcxz 4f\r\n\t" 
                 "3:addl -4(%%edx,%%ecx,4),%%eax\r\n\t" 
                 "adcl $0,%%eax\r\n\t" 
                 "loop 3b\r\n\t" 
                 "4:shldl $16,%%eax,%%ecx\r\n\t" 
                 "addw %%cx,%%ax\r\n\t" 
                 "adcw $0,%%ax\r\n\t" 
                 "notw %%ax\r\n\t" 
         "popl %%edx\r\n\t" 
         "popl %%ecx\r\n\t" 
                 :"=a"(sum):"a"(sum),"c"(len),"d"(data):"memory"); 
         return ((unsigned short)sum); 
}*/ 
unsigned short 
ip_sum (addr, len) 
      unsigned short *addr; 
      int len; 
{ 
   int nleft = len; 
   unsigned short *w = addr; 
   int sum = 0; 
   unsigned short answer = 0; 

   while (nleft > 1) 
     { 

       //printf("%x--",ntohs(*w));
       sum += ntohs(*w++); 
       nleft -= 2; 
     } 
   if (nleft == 1) 
     { 
       *(unsigned char *) (&answer) = *(unsigned char *) w; 
       sum += answer; 
       printf("oh,I found one byte(%x)--",answer);
     } 
   sum = (sum >> 16) + (sum & 0xffff); 

   sum += (sum >> 16);  
   answer = ~sum;  
   //printf("chsum is %x--over\n",htons(answer));
   return (htons(answer));  
   /* modified by wu_huashan@21cn.com,minifw.luohuedu.net */
} 
unsigned int 
ip_sum2 (addr, len) 
      unsigned short *addr; 
      int len; 
{ 
   int nleft = len; 
   unsigned short *w = addr; 
   int sum = 0; 
   unsigned short answer = 0; 

   while (nleft > 1) 
     { 

       //printf("%x--",ntohs(*w));
       sum += ntohs(*w++); 
       nleft -= 2; 
     } 
   if (nleft == 1) 
     { 
       *(unsigned char *) (&answer) = *(unsigned char *) w; 
       sum += answer; 
       printf("oh,I found one byte(%x)--",answer);
     } 
/*   sum = (sum >> 16) + (sum & 0xffff); */

/*   sum += (sum >> 16);  */
/*   answer = ~sum;  */
/*   printf("sum is %x--over\n",answer);*/
/*   return (htons(answer));  */
  /* modified by wu_huashan@21cn.com,minifw.luohuedu.net */
  //printf("sum is %x,len is %d\n",sum,len);
 return(sum);
} 
