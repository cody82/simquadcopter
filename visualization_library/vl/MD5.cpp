/*
  Copyright (C) 2008 Michele Bosi

  This file is part of Visualization Library.

  Visualization Library is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Visualization Library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Visualization Library. If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <string.h>

namespace
{
  class Ctx
  {
  public:
    Ctx()
    {
      status[0] = 0x67452301;
      status[1] = 0xefcdab89;
      status[2] = 0x98badcfe;
      status[3] = 0x10325476;
      total[0] = 0;
      total[1] = 0;
      memset(buf, 0, 64);
    }
    unsigned char buf[64];
    unsigned long status[4];                                   
    unsigned long total[2];
  };

  void process(unsigned long status[4], unsigned char block[64])
  {
    unsigned long a = status[0], b = status[1], c = status[2], d = status[3], x[16];

    for (unsigned int i = 0, j = 0; j < 64; i++, j += 4)
      x[i] = ((unsigned long)block[j]) | (((unsigned long)block[j+1]) << 8) | (((unsigned long)block[j+2]) << 16) | (((unsigned long)block[j+3]) << 24);

    #define S(x, n) (((x) << (n)) | ((x & 0xFFFFFFFF) >> (32-(n))))
    #define P(a, b, c, d, x, s, ac) { (a) += (((b) & (c)) | ((~b) & (d))) + (x) + (unsigned long)(ac); (a) = S ((a), (s)); (a) += (b); }
    P(a, b, c, d, x[ 0], 7,  0xd76aa478);
    P(d, a, b, c, x[ 1], 12, 0xe8c7b756);
    P(c, d, a, b, x[ 2], 17, 0x242070db);
    P(b, c, d, a, x[ 3], 22, 0xc1bdceee);
    P(a, b, c, d, x[ 4], 7,  0xf57c0faf);
    P(d, a, b, c, x[ 5], 12, 0x4787c62a);
    P(c, d, a, b, x[ 6], 17, 0xa8304613);
    P(b, c, d, a, x[ 7], 22, 0xfd469501);
    P(a, b, c, d, x[ 8], 7,  0x698098d8);
    P(d, a, b, c, x[ 9], 12, 0x8b44f7af);
    P(c, d, a, b, x[10], 17, 0xffff5bb1);
    P(b, c, d, a, x[11], 22, 0x895cd7be);
    P(a, b, c, d, x[12], 7,  0x6b901122);
    P(d, a, b, c, x[13], 12, 0xfd987193);
    P(c, d, a, b, x[14], 17, 0xa679438e);
    P(b, c, d, a, x[15], 22, 0x49b40821);

    #undef P
    #define P(a, b, c, d, x, s, ac) { (a) += (((b) & (d)) | ((c) & (~d))) + (x) + (unsigned long)(ac); (a) = S ((a), (s)); (a) += (b); }
    P (a, b, c, d, x[ 1], 5,  0xf61e2562);
    P (d, a, b, c, x[ 6], 9,  0xc040b340);
    P (c, d, a, b, x[11], 14, 0x265e5a51);
    P (b, c, d, a, x[ 0], 20, 0xe9b6c7aa);
    P (a, b, c, d, x[ 5], 5,  0xd62f105d);
    P (d, a, b, c, x[10], 9,  0x2441453);
    P (c, d, a, b, x[15], 14, 0xd8a1e681);
    P (b, c, d, a, x[ 4], 20, 0xe7d3fbc8);
    P (a, b, c, d, x[ 9], 5,  0x21e1cde6);
    P (d, a, b, c, x[14], 9,  0xc33707d6);
    P (c, d, a, b, x[ 3], 14, 0xf4d50d87);
    P (b, c, d, a, x[ 8], 20, 0x455a14ed);
    P (a, b, c, d, x[13], 5,  0xa9e3e905);
    P (d, a, b, c, x[ 2], 9,  0xfcefa3f8);
    P (c, d, a, b, x[ 7], 14, 0x676f02d9);
    P (b, c, d, a, x[12], 20, 0x8d2a4c8a);

    #undef P
    #define P(a, b, c, d, x, s, ac) { (a) += ((b) ^ (c) ^ (d)) + (x) + (unsigned long)(ac); (a) = S ((a), (s)); (a) += (b); }
    P(a, b, c, d, x[ 5], 4,  0xfffa3942);
    P(d, a, b, c, x[ 8], 11, 0x8771f681);
    P(c, d, a, b, x[11], 16, 0x6d9d6122);
    P(b, c, d, a, x[14], 23, 0xfde5380c);
    P(a, b, c, d, x[ 1], 4,  0xa4beea44);
    P(d, a, b, c, x[ 4], 11, 0x4bdecfa9);
    P(c, d, a, b, x[ 7], 16, 0xf6bb4b60);
    P(b, c, d, a, x[10], 23, 0xbebfbc70);
    P(a, b, c, d, x[13], 4,  0x289b7ec6);
    P(d, a, b, c, x[ 0], 11, 0xeaa127fa);
    P(c, d, a, b, x[ 3], 16, 0xd4ef3085);
    P(b, c, d, a, x[ 6], 23, 0x4881d05);
    P(a, b, c, d, x[ 9], 4,  0xd9d4d039);
    P(d, a, b, c, x[12], 11, 0xe6db99e5);
    P(c, d, a, b, x[15], 16, 0x1fa27cf8);
    P(b, c, d, a, x[ 2], 23, 0xc4ac5665);

    #undef P
    #define P(a, b, c, d, x, s, ac) { (a) += ((c) ^ ((b) | (~d))) + (x) + (unsigned long)(ac); (a) = S ((a), (s)); (a) += (b); }
    P(a, b, c, d, x[ 0], 6,  0xf4292244);
    P(d, a, b, c, x[ 7], 10, 0x432aff97);
    P(c, d, a, b, x[14], 15, 0xab9423a7);
    P(b, c, d, a, x[ 5], 21, 0xfc93a039);
    P(a, b, c, d, x[12], 6,  0x655b59c3);
    P(d, a, b, c, x[ 3], 10, 0x8f0ccc92);
    P(c, d, a, b, x[10], 15, 0xffeff47d);
    P(b, c, d, a, x[ 1], 21, 0x85845dd1);
    P(a, b, c, d, x[ 8], 6,  0x6fa87e4f);
    P(d, a, b, c, x[15], 10, 0xfe2ce6e0);
    P(c, d, a, b, x[ 6], 15, 0xa3014314);
    P(b, c, d, a, x[13], 21, 0x4e0811a1);
    P(a, b, c, d, x[ 4], 6,  0xf7537e82);
    P(d, a, b, c, x[11], 10, 0xbd3af235);
    P(c, d, a, b, x[ 2], 15, 0x2ad7d2bb);
    P(b, c, d, a, x[ 9], 21, 0xeb86d391);

    status[0] += a;
    status[1] += b;
    status[2] += c;
    status[3] += d;
  }

  void enc(unsigned char *output, unsigned long *input, unsigned int len)
  {
    for (unsigned int i = 0, j = 0; j < len; i++, j += 4) 
    {
      output[j]   = (unsigned char)(input[i] & 255);
      output[j+1] = (unsigned char)((input[i] >> 8)  & 255);
      output[j+2] = (unsigned char)((input[i] >> 16) & 255);
      output[j+3] = (unsigned char)((input[i] >> 24) & 255);
    }
  }

  void update( Ctx *context, unsigned char *input, unsigned int ilen )                    
  {
    unsigned int left, fill;

    left = (unsigned int)((context->total[0] >> 3) & 63);
    if ( (context->total[0] += ((unsigned long)ilen << 3)) < ((unsigned long)ilen << 3) )
    {
      context->total[1]++;
    }
    context->total[1] += ((unsigned long)ilen >> 29);
    fill = 64 - left;

    unsigned int i;
    if (ilen >= fill) 
    {
      memcpy(&context->buf[left], input, fill);
      process(context->status, context->buf);

      for (i = fill; i + 63 < ilen; i += 64)
      {
        process(context->status, &input[i]);
      }

      left = 0;
    }
    else
    {
      i = 0;
    }

    memcpy(&context->buf[left], &input[i], ilen-i);
  }
}

namespace vl
{
  void MD5(const char *string, unsigned char sum[33])
  {
    Ctx context;
    unsigned char digest[16];
    unsigned int len = (int)strlen (string);
   
    update(&context, (unsigned char*)string, len);
    unsigned char bits[8];
    unsigned int index, padLen;

    enc(bits, context.total, 8);

    index = (unsigned int)((context.total[0] >> 3) & 0x3f);
    padLen = (index < 56) ? (56 - index) : (120 - index);

    unsigned char tmp_buf[64];
    memset(tmp_buf,0,sizeof(tmp_buf));
    tmp_buf[0] = 0x80;

    update(&context, tmp_buf, padLen);
    update(&context, bits, 8);

    enc(digest, context.status, 16);

    sprintf ((char*)sum,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", 
      digest[0], digest[1], digest[2],  digest[3],  digest[4],  digest[5],  digest[6],  digest[7],
      digest[8], digest[9], digest[10], digest[11], digest[12], digest[13], digest[14], digest[15] );
  }
}
