/* AES CryptAPI - implementation of AES functions for       *
 * character buffer / binary / text encryption.             *
 * Written Nov 99 by Mixter <mixter@newyorkoffice.com>      */

#include "aes.h"
#include <string.h>
u4byte *
aes_setkey (char *password)
{
  u4byte keylen = strlen (password) * 8;
  u4byte key[strlen (password) / 4];
  memcpy (key, password, strlen (password));
  return (set_key (key, keylen));
}

u1byte *
encode (u1byte * inbuf, int len)
{
  u4byte i, in_blk[4], out_blk[4];

  if (len % 16 != 0)
    len += 16 - (len % 16);

  for (i = 0; i < len - 15; i += 16)
    {
      memcpy (in_blk, inbuf + i, 16);
      aes_encrypt (in_blk, out_blk);
      memcpy (inbuf + i, out_blk, 16);
    }

  return inbuf;
}

u1byte *
decode (u1byte * inbuf, int len)
{
  u4byte i, in_blk[4], out_blk[4];

  if (len % 16 != 0)
    len += 16 - (len % 16);

  for (i = 0; i < len - 15; i += 16)
    {
      memcpy (in_blk, inbuf + i, 16);
      decrypt (in_blk, out_blk);
      memcpy (inbuf + i, out_blk, 16);
    }

  return inbuf;
}

u1byte *
encode64 (u1byte * inbuf, u1byte * outbuf, int len)
{
  if (!len)
    return inbuf;
  encode (inbuf, len);
 if (len>16)  
    base64_in (inbuf, (char *) outbuf, (len + 16));
 else
  base64_in (inbuf, (char *) outbuf, (len + 15));
  /*    modify by wu_huashan@21cn.com, minifw.luohuedu.net  */
 

  return outbuf;
}

u1byte *
decode64 (u1byte * inbuf, u1byte * outbuf, int len)
{
  if (!len)
    return inbuf;
  base64_out ((char *) inbuf, outbuf, len);
  decode (outbuf, len);
  return outbuf;
}
