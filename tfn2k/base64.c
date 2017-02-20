/* base64 encoding/decoding functions by Mixter */

const char b64[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char ascii[256] =
{
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
  52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
  64, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
  15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
  64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
  41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};

void
base64_in (unsigned char *buf, char *obuf, int len)
{
  int i;
  for (i = 0; i < len - 2; i += 3)
    {
      *obuf++ = b64[(buf[i] >> 2) & 0x3F];
      *obuf++ = b64[((buf[i] & 0x3) << 4 | ((int) (buf[i + 1] & 0xF0) >> 4))];
      *obuf++ = b64[((buf[i + 1] & 0xF) << 2) | ((int) (buf[i + 2] & 0xC0) >> 6)];
      *obuf++ = b64[buf[i + 2] & 0x3F];
    }
  if (i < len)
    {
      *obuf++ = b64[(buf[i] >> 2) & 0x3F];
      if (i == (len - 1))
	{
	  *obuf++ = b64[((buf[i] & 0x3) << 4)];
//        *obuf++ = '=';
	}
      else
	{
	  *obuf++ = b64[((buf[i] & 0x3) << 4 | ((int) (buf[i + 1] & 0xf0) >> 4))];
	  *obuf++ = b64[((buf[i + 1] & 0xf) << 2)];
	}
//      *obuf++ = '=';
    }
  *obuf++ = '\0';
}

void
base64_out (char *buf, unsigned char *obuf, int len)
{
  int nprbytes;
  char *p = buf;
  while (ascii[(int) *(p++)] <= 63);

  nprbytes = len - 1;

  while (nprbytes > 4 && *buf != '\0')
    {
      *(obuf++) = (ascii[(int) *buf] << 2 | ascii[(int) buf[1]] >> 4);
      *(obuf++) = (ascii[(int) buf[1]] << 4 | ascii[(int) buf[2]] >> 2);
      *(obuf++) = (ascii[(int) buf[2]] << 6 | ascii[(int) buf[3]]);
      buf += 4;
      nprbytes -= 4;
    }
  if (nprbytes > 1)
    *(obuf++) =
      (ascii[(int) *buf] << 2 | ascii[(int) buf[1]] >> 4);
  if (nprbytes > 2)
    *(obuf++) =
      (ascii[(int) buf[1]] << 4 | ascii[(int) buf[2]] >> 2);
  if (nprbytes > 3)
    *(obuf++) =
      (ascii[(int) buf[2]] << 6 | ascii[(int) buf[3]]);
  *(obuf)++ = '\0';
}
