/* AES CryptAPI - interface to use AES algorithms for       *
 * character buffer / binary / text encryption.             *
 * Written Nov 99 by Mixter <mixter@newyorkoffice.com>      *
 * and based on the NIST standard definitions for           *
 * AES cryptography source code.                            */

#ifndef _AES_H
#define _AES_H

void base64_in (unsigned char *, char *, int);
void base64_out (char *, unsigned char *, int);

typedef unsigned char u1byte;	/* an 8 bit unsigned character type */
typedef unsigned short u2byte;	/* a 16 bit unsigned integer type   */
typedef unsigned int u4byte;	/* a 32 bit unsigned integer type   */
typedef signed char s1byte;	/* an 8 bit signed character type   */
typedef signed short s2byte;	/* a 16 bit signed integer type     */
typedef signed int s4byte;	/* a 32 bit signed integer type     */

#ifdef  __cplusplus
extern "C"
{
  u4byte *aes_setkey (char *password);
  u1byte *encode (u1byte * inbuf, int len);
  u1byte *decode (u1byte * inbuf, int len);
  u1byte *encode64 (u1byte * inbuf, u1byte * outbuf, int len);
  u1byte *decode64 (u1byte * inbuf, u1byte * outbuf, int len);
  char **cipher_name (void);
  u4byte *set_key (const u4byte in_key[], const u4byte key_len);
  void aes_encrypt (const u4byte in_blk[4], u4byte out_blk[4]);
  void decrypt (const u4byte in_blk[4], u4byte out_blk[4]);
};
#else
u4byte *aes_setkey (char *password);
u1byte *encode (u1byte * inbuf, int len);
u1byte *decode (u1byte * inbuf, int len);
u1byte *encode64 (u1byte * inbuf, u1byte * outbuf, int len);
u1byte *decode64 (u1byte * inbuf, u1byte * outbuf, int len);
char **cipher_name (void);
u4byte *set_key (const u4byte in_key[], const u4byte key_len);
void aes_encrypt (const u4byte in_blk[4], u4byte out_blk[4]);
void decrypt (const u4byte in_blk[4], u4byte out_blk[4]);
#endif

#ifndef _MSC_VER
#define rotr(x,n)   (((x) >> ((int)(n))) | ((x) << (32 - (int)(n))))
#define rotl(x,n)   (((x) << ((int)(n))) | ((x) >> (32 - (int)(n))))
#else
#include <stdlib.h>
#pragma intrinsic(_lrotr,_lrotl)
#define rotr(x,n)   _lrotr(x,n)
#define rotl(x,n)   _lrotl(x,n)
#endif
#define bswap(x)    ((rotl(x, 8) & 0x00ff00ff) | (rotr(x, 8) & 0xff00ff00))
#define byte(x,n)   ((u1byte)((x) >> (8 * n)))
#ifdef  BYTE_SWAP
#define io_swap(x)  bswap(x)
#else
#define io_swap(x)  (x)
#endif
#ifdef  WORD_SWAP
#define get_block(x)                            \
    ((u4byte*)(x))[0] = io_swap(in_blk[3]);     \
    ((u4byte*)(x))[1] = io_swap(in_blk[2]);     \
    ((u4byte*)(x))[2] = io_swap(in_blk[1]);     \
    ((u4byte*)(x))[3] = io_swap(in_blk[0])
#define put_block(x)                            \
    out_blk[3] = io_swap(((u4byte*)(x))[0]);    \
    out_blk[2] = io_swap(((u4byte*)(x))[1]);    \
    out_blk[1] = io_swap(((u4byte*)(x))[2]);    \
    out_blk[0] = io_swap(((u4byte*)(x))[3])
#define get_key(x,len)                          \
    ((u4byte*)(x))[4] = ((u4byte*)(x))[5] =     \
    ((u4byte*)(x))[6] = ((u4byte*)(x))[7] = 0;  \
    switch((((len) + 63) / 64)) {               \
    case 2:                                     \
    ((u4byte*)(x))[0] = io_swap(in_key[3]);     \
    ((u4byte*)(x))[1] = io_swap(in_key[2]);     \
    ((u4byte*)(x))[2] = io_swap(in_key[1]);     \
    ((u4byte*)(x))[3] = io_swap(in_key[0]);     \
    break;                                      \
    case 3:                                     \
    ((u4byte*)(x))[0] = io_swap(in_key[5]);     \
    ((u4byte*)(x))[1] = io_swap(in_key[4]);     \
    ((u4byte*)(x))[2] = io_swap(in_key[3]);     \
    ((u4byte*)(x))[3] = io_swap(in_key[2]);     \
    ((u4byte*)(x))[4] = io_swap(in_key[1]);     \
    ((u4byte*)(x))[5] = io_swap(in_key[0]);     \
    break;                                      \
    case 4:                                     \
    ((u4byte*)(x))[0] = io_swap(in_key[7]);     \
    ((u4byte*)(x))[1] = io_swap(in_key[6]);     \
    ((u4byte*)(x))[2] = io_swap(in_key[5]);     \
    ((u4byte*)(x))[3] = io_swap(in_key[4]);     \
    ((u4byte*)(x))[4] = io_swap(in_key[3]);     \
    ((u4byte*)(x))[5] = io_swap(in_key[2]);     \
    ((u4byte*)(x))[6] = io_swap(in_key[1]);     \
    ((u4byte*)(x))[7] = io_swap(in_key[0]);     \
    }
#else
#define get_block(x)                            \
    ((u4byte*)(x))[0] = io_swap(in_blk[0]);     \
    ((u4byte*)(x))[1] = io_swap(in_blk[1]);     \
    ((u4byte*)(x))[2] = io_swap(in_blk[2]);     \
    ((u4byte*)(x))[3] = io_swap(in_blk[3])
#define put_block(x)                            \
    out_blk[0] = io_swap(((u4byte*)(x))[0]);    \
    out_blk[1] = io_swap(((u4byte*)(x))[1]);    \
    out_blk[2] = io_swap(((u4byte*)(x))[2]);    \
    out_blk[3] = io_swap(((u4byte*)(x))[3])
#define get_key(x,len)                          \
    ((u4byte*)(x))[4] = ((u4byte*)(x))[5] =     \
    ((u4byte*)(x))[6] = ((u4byte*)(x))[7] = 0;  \
    switch((((len) + 63) / 64)) {               \
    case 4:                                     \
    ((u4byte*)(x))[6] = io_swap(in_key[6]);     \
    ((u4byte*)(x))[7] = io_swap(in_key[7]);     \
    case 3:                                     \
    ((u4byte*)(x))[4] = io_swap(in_key[4]);     \
    ((u4byte*)(x))[5] = io_swap(in_key[5]);     \
    case 2:                                     \
    ((u4byte*)(x))[0] = io_swap(in_key[0]);     \
    ((u4byte*)(x))[1] = io_swap(in_key[1]);     \
    ((u4byte*)(x))[2] = io_swap(in_key[2]);     \
    ((u4byte*)(x))[3] = io_swap(in_key[3]);     \
    }
#endif

#ifdef  BLOCK_SWAP
#define BYTE_SWAP
#define WORD_SWAP
#endif

#endif
