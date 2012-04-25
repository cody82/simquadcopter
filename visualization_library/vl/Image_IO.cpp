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

#include "vl/Image.hpp"
#include "vl/Say.hpp"
#include "vl/Log.hpp"

using namespace vl;

namespace 
{
  typedef unsigned char TPalette3x256[256*3];
  typedef unsigned char TPalette4x256[256*4];
  typedef unsigned short TPalette16x3x256[256*3];

  unsigned long readDWord(FILE* fin, bool little_endian)
  {
    unsigned char bytes[4];
    fread(bytes, 1, 4, fin);
    if (little_endian)
      return 
        ((unsigned long)bytes[0] << 24) +
        ((unsigned long)bytes[1] << 16) +
        ((unsigned long)bytes[2] <<  8) +
        ((unsigned long)bytes[3] <<  0) ;
    else
      return 
        ((unsigned long)bytes[3] << 24) +
        ((unsigned long)bytes[2] << 16) +
        ((unsigned long)bytes[1] <<  8) +
        ((unsigned long)bytes[0] <<  0) ;
  }

  unsigned short readWord(FILE* fin, bool little_endian)
  {
    unsigned char bytes[2];
    fread(bytes, 1, 2, fin);
    if (little_endian)
      return 
        ((unsigned short)bytes[0] << 8) +
        ((unsigned short)bytes[1] << 0) ;
    else
      return 
        ((unsigned short)bytes[1] << 8) +
        ((unsigned short)bytes[0] << 0) ;
  }

  unsigned char readByte(FILE* fin)
  {
    unsigned char byte=0;
    fread(&byte, 1, 1, fin);
    return byte;
  }

  void writeDWord(unsigned long data, FILE* fout, bool little_endian=true)
  {
    unsigned char byte[4];
    if (little_endian)
    {
      byte[0] = (unsigned char)((data >> 24) & 0xFF);
      byte[1] = (unsigned char)((data >> 16) & 0xFF);
      byte[2] = (unsigned char)((data >>  8) & 0xFF);
      byte[3] = (unsigned char)((data >>  0) & 0xFF);
    }
    else
    {
      byte[3] = (unsigned char)((data >> 24) & 0xFF);
      byte[2] = (unsigned char)((data >> 16) & 0xFF);
      byte[1] = (unsigned char)((data >>  8) & 0xFF);
      byte[0] = (unsigned char)((data >>  0) & 0xFF);
    }
    fwrite(byte, 1, 4, fout);
  }

  void writeWord(unsigned short data, FILE* fout, bool little_endian=true)
  {
    unsigned char byte[2];
    if (little_endian)
    {
      byte[0] = (unsigned char)((data >>  8) & 0xFF);
      byte[1] = (unsigned char)((data >>  0) & 0xFF);
    }
    else
    {
      byte[1] = (unsigned char)((data >>  8) & 0xFF);
      byte[0] = (unsigned char)((data >>  0) & 0xFF);
    }
    fwrite(byte, 1, 2, fout);
  }

  void writeByte(unsigned char byte, FILE* fout)
  {
    fwrite(&byte, 1, 1, fout);
  }

  void internal_RGBToRGBA(void* buf, int w, int h, unsigned char alpha, int bytealign = 1) 
  {
    int xbytes = w*3;
    int pitch = (xbytes / bytealign * bytealign) + ((xbytes % bytealign)? bytealign : 0);

    if(bytealign)
    {

      unsigned char *pxl1 = (unsigned char *)buf;
      unsigned char *pxl2 = (unsigned char *)buf;
      int count = 0;
      for(int i=0; i<pitch*h; i++, count++)
      {
        if (count==pitch)
          count = 0;

        *pxl1 = *pxl2;

        if (count < w*3)
          pxl1 ++;

        pxl2 ++;
      }
    }

	  unsigned char * px32 = (unsigned char*)buf + w * h * 4 - 4;
	  unsigned char * px24 = (unsigned char*)buf + w * h * 3 - 3;
	  for(int i=0; i<w * h; i++) 
    {
		  px32[0] = px24[0];
		  px32[1] = px24[1];
		  px32[2] = px24[2];
		  px32[3] = alpha;
		  px24 -= 3;
		  px32 -= 4;
	  }
  }

  void internal_GrayscaleToRGBA(void* buf, int size, unsigned char alpha) 
  {
	  unsigned char* px32 = (unsigned char*)buf + size * 4 - 4;
	  unsigned char* px8  = (unsigned char*)buf + size * 1 - 1;
	  for(int i=0; i<size; i++) 
    {
		  px32[0] = *px8;
		  px32[1] = *px8;
		  px32[2] = *px8;
		  px32[3] = alpha;
		  px8 -= 1;
		  px32 -= 4;
	  }
  }

  void internal_A1R5G5B5ToRGBA(void* buf, int size, unsigned char alpha)
  {
	  unsigned char* px32 = (unsigned char*)buf + size * 4 - 4;
	  unsigned char* px8  = (unsigned char*)buf + size * 2 - 2;
	  for(int i=0; i<size; i++) 
    {
      unsigned char r = (px8[1] << 1) & ~0x03;
      unsigned char g = ((px8[1] << 6) | (px8[0] >> 2)) & ~0x03;;
      unsigned char b = (px8[0] << 3) & ~0x03;

		  px32[0] = r;
		  px32[1] = g;
		  px32[2] = b;
		  px32[3] = alpha;
		  px8  -= 2;
		  px32 -= 4;
	  }
  }

  void internal_8ToRGBA(const TPalette3x256 & palette, void* buf, int w, int h, unsigned char alpha, int bytealign = 1)
  {

    int xbytes = w;
    int pitch = (xbytes / bytealign * bytealign) + ((xbytes % bytealign)? bytealign : 0);

    if(bytealign)
    {

      unsigned char *pxl1 = (unsigned char *)buf;
      unsigned char *pxl2 = (unsigned char *)buf;
      int count = 0;
      for(int i=0; i<pitch*h; i++, count++)
      {
        if (count==pitch)
          count = 0;

        *pxl1 = *pxl2;

        if (count < w)
          pxl1 ++;

        pxl2 ++;
      }
    }

    unsigned char* px32 = (unsigned char*)buf + w * h * 4 - 4;
	  unsigned char* px8  = (unsigned char*)buf + w * h * 1 - 1;
	  for(int i=0; i<w * h; i++) 
    {
		  px32[0] = palette[*px8*3+0];
		  px32[1] = palette[*px8*3+1];
		  px32[2] = palette[*px8*3+2];
		  px32[3] = alpha;
		  px8  -= 1;
		  px32 -= 4;
	  }
  }

  void internal_8ToRGBA(const TPalette4x256 & palette, void* buf, int w, int h, int bytealign = 1)
  {

    int xbytes = w;
    int pitch = (xbytes / bytealign * bytealign) + ((xbytes % bytealign)? bytealign : 0);

    if(bytealign)
    {

      unsigned char *pxl1 = (unsigned char *)buf;
      unsigned char *pxl2 = (unsigned char *)buf;
      int count = 0;
      for(int i=0; i<pitch*h; i++, count++)
      {
        if (count==pitch)
          count = 0;

        *pxl1 = *pxl2;

        if (count < w)
          pxl1 ++;

        pxl2 ++;
      }
    }

    unsigned char * px32 = (unsigned char*)buf + w * h * 4 - 4;
	  unsigned char * px8  = (unsigned char*)buf + w * h * 1 - 1;
	  for(int i=0; i<w * h; i++) 
    {
		  px32[0] = palette[*px8*4+0];
		  px32[1] = palette[*px8*4+1];
		  px32[2] = palette[*px8*4+2];
		  px32[3] = palette[*px8*4+3];
		  px8  -= 1;
		  px32 -= 4;
	  }
  }

  void internal_swapBytes32(void* buf, int size)
  {
    unsigned char* p = (unsigned char*)buf;
    unsigned char dw[4];
    for(int i=0; i<size; i+=4, p+=4)
    {
      memcpy(dw, p, 4);
      p[0] = dw[3];
      p[3] = dw[0];
      p[1] = dw[2];
      p[2] = dw[1];
    }
  }

  void internal_swapBytes32_BGRA_RGBA(void* buf, int bytecount)
  {
    unsigned char* p = (unsigned char*)buf;
    unsigned char dw[4];
    for(int i=0; i<bytecount; i+=4, p+=4)
    {
      memcpy(dw, p, 4);
      p[0] = dw[2];
      p[2] = dw[0];
    }
  }

  void internal_swapBytes24_BGR_RGB(void* buf, int bytecount)
  {
    unsigned char* p = (unsigned char*)buf;
    unsigned char dw[4];
    int pxl = bytecount / 3;
    for(int i=0; i<pxl; i++, p+=3)
    {
      memcpy(dw, p, 3);
      p[0] = dw[2];
      p[2] = dw[0];
    }
  }

  void internal_fillRGBA32_Alpha(void* buf, int bytecount, unsigned char alpha)
  {
    unsigned char* pxl = (unsigned char*)buf;
    for(int i=0; i<bytecount; i+=4)
    {
      pxl[i+3] = alpha;
    }
  }

  void internal_fillGray8Alpha8_Alpha(void* buf, int bytecount, unsigned char alpha)
  {
    unsigned char* pxl = (unsigned char*)buf;
    for(int i=0; i<bytecount; i+=2)
    {
      pxl[i+1] = alpha;
    }
  }
}

namespace 
{
  typedef struct 
  { 
    char mType[2];
    char mSize[4]; 
    char mReserved1[2]; 
    char mReserved2[2]; 
    char mOffBits[4]; 

    unsigned short Type() const { return *(unsigned short*)mType; }
    unsigned long  Size() const { return *(unsigned long*)mSize; }
    unsigned short Reserved1() const { return *(unsigned short*)mReserved1; }
    unsigned short Reserved2() const { return *(unsigned short*)mReserved2; }
    unsigned long  OffBits() const { return *(unsigned long*)mOffBits; }
  } BitmapFileHeader;

  typedef struct 
  {
    unsigned long Size() { return *(unsigned long*)mSize; }
    long Width() { return *(long*)mWidth; } 
    long Height() { return *(long*)mHeight; } 
    unsigned short Planes() { return *(unsigned short*)mPlanes; } 
    unsigned short BitCount() { return *(unsigned short*)mBitCount; } 
    unsigned long Compression() { return *(unsigned long*)mCompression; } 
    unsigned long SizeImage() { return *(unsigned long*)mSizeImage; } 
    long XPelsPerMeter() { return *(long*)mXPelsPerMeter; } 
    long YPelsPerMeter() { return *(long*)mYPelsPerMeter; } 
    unsigned long ClrUsed() { return *(unsigned long*)mClrUsed; } 
    unsigned long ClrImportant() { return *(unsigned long*)mClrImportant; } 

    char mSize[4];
    char mWidth[4]; 
    char mHeight[4]; 
    char mPlanes[2]; 
    char mBitCount[2]; 
    char  mCompression[4]; 
    char  mSizeImage[4]; 
    char mXPelsPerMeter[4]; 
    char mYPelsPerMeter[4]; 
    char mClrUsed[4]; 
    char mClrImportant[4]; 
  } BitmapInfoHeader;

  const unsigned long BMP_Type_RGB  = 0L;

}

vl::ref<Image> vl::loadBMP( const std::string& path )
{
  ref<Image> img = new Image;

  img->setName(path);

	FILE * fin = fopen( path.c_str(), "rb");
	if (!fin) 
  {
    Log::error( Say("vl::loadBMP: error opening file: '%s'\n") << path );
		return NULL;
	}

  BitmapFileHeader bfh;
  memset(&bfh, 0, sizeof(bfh));
	BitmapInfoHeader bih;
  memset(&bih, 0, sizeof(bih));

  fread(&bfh, 1, sizeof(bfh), fin);
	if (bfh.Type() != 0x4D42) 
  {
		fclose(fin);
    Log::error( Say("The file '%s' is not a BMP file.\n") << path );
		return NULL;
	}

	long long head = ftell(fin);

  fread(&bih, 1, sizeof(bih), fin);

  bool flip = false;
	if ( bih.Height() < 0 )
  {
    long *h = (long*)bih.mHeight;
    *h = -*h;
    flip = true;
  }
	assert( bih.Height() * bih.Width() );

	if ( bih.Compression() != BMP_Type_RGB )
  {
		fclose(fin); fin = NULL;
    Log::error( Say("Compression %n unsupported for %s\n") << (int)bih.Compression() << path );
		return NULL;
	}

	switch( bih.BitCount() ) 
  {

		case 8:
    {
      img->allocate2D(bih.Width(), bih.Height(), 4, IF_RGBA, IT_UNSIGNED_BYTE);

      TPalette4x256 palette;
			memset(palette, 0, 256*4);

 			unsigned char *rgb_quad = (unsigned char *)img->pixels();

			int br;
			fseek(fin, (int)head + bih.Size(), SEEK_SET);
			int palette_bytes = (bih.ClrUsed() ? bih.ClrUsed() : 256)*4;
			br = (int)fread(palette, 1, palette_bytes, fin);
			if(br != palette_bytes) {
				fclose(fin);
        Log::error( Say("File %s truncated: %n < %n.\n") << path << br << palette_bytes );
				return NULL;
			}

			int PadBytes = (4 - (img->width() * 1) % 4) % 4;
			int datasize = (img->width() * 1 + PadBytes) * img->height() ;

      if ( bih.Compression() == BMP_Type_RGB )
      {
			  br = (int)fread(rgb_quad, 1, datasize, fin);
			  if(br != datasize) 
        {
				  fclose(fin);
          Log::error( Say("file %s truncated.\n") << path );
				  return NULL;
			  }
      }

      internal_8ToRGBA( palette, img->pixels(), img->width(), img->height(), 4 );
		} break;

		case 32:
		case 24: 
    {
      img->allocate2D(bih.Width(), bih.Height(), 4, IF_RGBA, IT_UNSIGNED_BYTE);

			fseek(fin, bfh.OffBits(), SEEK_SET);

			int PadBytes = (4 - (img->width() * 3) % 4) % 4;

			int datasize = bih.BitCount() == 32 ? img->width() * img->height() * 4 : (img->width() * 3 + PadBytes) * img->height() ;

			unsigned char * rgb_quad = (unsigned char *)img->pixels(); // (unsigned char*)malloc( width() * height() * 4 );
			memset( rgb_quad, 0xFF, img->width() * img->height() * 4 );
			int rd = (int)fread(rgb_quad, 1, datasize, fin);
			if( rd < datasize ) 
      {
				fclose(fin);
        Log::error( Say("File %s truncated.\n") << path );
				return NULL;
			}

			if (bih.BitCount() == 24)
		    internal_RGBToRGBA(img->pixels(), img->width(), img->height(), 0xFF, 4);

    } break;

		default: {
			fclose(fin); fin = NULL;
      Log::error( Say("BitCount %n unsupported for %s\n") << (int)bih.BitCount() << path );
			return NULL;
		}
	}

	fclose(fin); fin = NULL;
  internal_swapBytes32_BGRA_RGBA(img->pixels(), img->requiredMemory());

  for(int i=0; i<img->requiredMemory(); i+=4)
  {
    unsigned char* px = (unsigned char*)img->pixels() + i;
    px[3] = 0xFF;
  }

  if (flip)
    img->flipVertically();

	return img;
}

namespace 
{
  const unsigned long TGA_NO_IMAGE_DATA = 0;
  const unsigned long TGA_8BIT_UNCOMPRESSED = 1;
  const unsigned long TGA_RGB_UNCOMPRESSED  = 2;
  const unsigned long TGA_GRAYSCALE_UNCOMPRESSED  = 3;
  const unsigned long TGA_8BIT_COMPRESSED   = 9;
  const unsigned long TGA_RGB_COMPRESSED    = 10;
  const unsigned long TGA_GRAYSCALE_COMPRESSED = 11;

  typedef struct
  {
	  unsigned char IdFieldSize; /* at offset 18 it starts, usually 0 */
	  unsigned char HasColMap;   /* 1 for indexed images, 0 otherwise */
	  unsigned char ImageType;   /* see defines above */
	  unsigned char ColMapOrigin[2];
	  unsigned char ColMapCount_lo;
	  unsigned char ColMapCount_hi;
	  unsigned char ColMapEntrySize; /* 16, 24, 32 */
	  unsigned char ImageOrigins[4]; /* lo/hi bytes for x/y origins */
	  unsigned char Width_lo;
	  unsigned char Width_hi;
	  unsigned char Height_lo;
	  unsigned char Height_hi;
	  unsigned char BitsPerPixel;	/* 8/16(?), 16, 24, 32 */
	  unsigned char ImageDescriptor; /* origin | alpha channel bits */
  } STGAHeader;
}

vl::ref<Image> vl::loadTGA( const std::string& path )
{
  ref<Image> img = new Image;

	FILE* fin = fopen(path.c_str(), "rb");

	if (fin == NULL)
	{
    Log::error( Say("vl::loadTGA: can't load TGA file '%s'\n") << path );
		return NULL;
	}

	STGAHeader header;
  memset(&header, 0, sizeof(header));
	fread(&header, 1, sizeof(STGAHeader), fin);

	unsigned int colmap_offset = 18 + header.IdFieldSize;
	unsigned int pixels_offset = colmap_offset +
		(header.ColMapCount_lo + header.ColMapCount_hi*256)*header.ColMapEntrySize/8;
	unsigned int w = header.Width_lo  + header.Width_hi*256;
	unsigned int h = header.Height_lo + header.Height_hi*256;
	unsigned int bpp = header.BitsPerPixel;

  char *type = "";
  switch(header.ImageType)
  {
    case TGA_NO_IMAGE_DATA:          type = "TGA_NO_IMAGE_DATA"; break;
    case TGA_8BIT_UNCOMPRESSED:      type = "TGA_8BIT_UNCOMPRESSED"; break;
    case TGA_RGB_UNCOMPRESSED:       type = "TGA_RGB_UNCOMPRESSED"; break;
    case TGA_GRAYSCALE_UNCOMPRESSED: type = "TGA_GRAYSCALE_UNCOMPRESSED"; break;
    case TGA_8BIT_COMPRESSED:        type = "TGA_8BIT_COMPRESSED"; break;
    case TGA_RGB_COMPRESSED:         type = "TGA_RGB_COMPRESSED"; break;
    case TGA_GRAYSCALE_COMPRESSED:   type = "TGA_GRAYSCALE_COMPRESSED"; break;
  }
  Log::info( Say("TGA %s: w=%n, h=%n, bpp=%n/%n %s\n") << path << w << h << bpp << header.ColMapEntrySize << type);

  if (header.ImageType == TGA_NO_IMAGE_DATA)
    return NULL;

  img->setName(path);

	if (header.ImageType == TGA_RGB_COMPRESSED)
	{
    int pixsize = 0;
    switch(header.BitsPerPixel)
    {
      case 32: pixsize = 4; break;
      case 24: pixsize = 3; break;
      case 16: pixsize = 2; break;
    }

    if (pixsize)
		{
      img->allocate2D(w, h, 4, IF_RGBA, IT_UNSIGNED_BYTE);

      fseek(fin, pixels_offset, SEEK_SET);
      int pixcount = w*h;
      int pix = 0;
      while(pix < pixcount)
      {
        unsigned char header = 0;
        fread(&header, 1, 1, fin);
        if (header >= 128)
        {
          int count = header - 128 + 1;
          unsigned char bgra[4];
          fread(bgra, 1, pixsize, fin);
          while(count--)
          {
            memcpy((unsigned char*)img->pixels() + pix*pixsize, bgra, pixsize);
            pix++;
          }
        }
        else
        {
          int count = header + 1;
          fread((unsigned char*)img->pixels() + pix*pixsize, 1, pixsize*count, fin);
          pix += count;
        }
      }

      switch(header.BitsPerPixel)
      {
        case 24: internal_RGBToRGBA(img->pixels(), img->width(), img->height(), 0xFF); // break;
        case 32: internal_swapBytes32_BGRA_RGBA(img->pixels(), img->requiredMemory()); break;
        case 16: internal_A1R5G5B5ToRGBA(img->pixels(), w*h, 0xFF); break;
      }
      
		}
		else
		{
      Log::error( Say("TGA ERROR: TGA_RGB_COMPRESSED %nbpp not supported.\n") << header.BitsPerPixel );
			fclose(fin);
			return NULL;
		}

	}
  else
	if (header.ImageType == TGA_RGB_UNCOMPRESSED)
	{
    int pixsize = 0;

    switch(header.BitsPerPixel)
    {
      case 32: pixsize = 4; break;
      case 24: pixsize = 3; break;
      case 16: pixsize = 2; break;
    }

    if (pixsize)
		{
      fseek(fin, pixels_offset, SEEK_SET);
      img->allocate2D(w, h, 4, IF_RGBA, IT_UNSIGNED_BYTE);
      fread(img->pixels(), 1, w*h*pixsize, fin);
      switch(header.BitsPerPixel)
      {
        case 24: internal_RGBToRGBA(img->pixels(), img->width(), img->height(), 0xFF); // break;
        case 32: internal_swapBytes32_BGRA_RGBA(img->pixels(), img->requiredMemory()); break;
        case 16: internal_A1R5G5B5ToRGBA(img->pixels(), w*h, 0xFF); break;
      }
		}
		else
		{
      Log::error( Say("TGA ERROR: TGA_RGB_UNCOMPRESSED %nbpp not supported.\n") << header.BitsPerPixel );
			fclose(fin);
			return NULL;
		}

	}
	else 
  if (header.ImageType == TGA_8BIT_UNCOMPRESSED || header.ImageType == TGA_8BIT_COMPRESSED)
	{
		if (header.BitsPerPixel == 8)
		{

			unsigned int colmap_count = header.ColMapCount_lo + header.ColMapCount_hi*256;
			assert(colmap_count<=256);
			if (header.ColMapEntrySize == 24)
			{
				TPalette3x256 palette;
				fseek(fin, colmap_offset, SEEK_SET);
				fread(palette, 1, colmap_count*3, fin);

				fseek(fin, pixels_offset, SEEK_SET);
        img->allocate2D(w, h, 4, IF_RGBA, IT_UNSIGNED_BYTE);
        if (header.ImageType == TGA_8BIT_UNCOMPRESSED)
        {
				  fread(img->pixels(), 1, w*h*1, fin);
        }
        else // TGA_8BIT_UNCOMPRESSED
        {
          int pixsize = 1;
          int pixcount = w*h;
          int pix = 0;
          while(pix < pixcount)
          {
            unsigned char header = 0;
            fread(&header, 1, 1, fin);
            if (header >= 128)
            {
              int count = header - 128 + 1;
              unsigned char bgra[4];
              fread(bgra, 1, pixsize, fin);
              while(count--)
              {
                memcpy((unsigned char*)img->pixels() + pix*pixsize, bgra, pixsize);
                pix++;
              }
            }
            else
            {
              int count = header + 1;
              fread((unsigned char*)img->pixels() + pix*pixsize, 1, pixsize*count, fin);
              pix += count;
            }
          }
        }

        internal_8ToRGBA(palette, img->pixels(), img->width(), img->height(), 0xFF);
        internal_swapBytes32_BGRA_RGBA(img->pixels(), img->requiredMemory());
			}
			else if (header.ColMapEntrySize == 32)
			{
				TPalette4x256 palette;
				fseek(fin, colmap_offset, SEEK_SET);
				fread(palette, 1, colmap_count*4, fin);

				fseek(fin, pixels_offset, SEEK_SET);
        img->allocate2D(w, h, 4, IF_RGBA, IT_UNSIGNED_BYTE);
        if (header.ImageType == TGA_8BIT_UNCOMPRESSED)
        {
				  fread(img->pixels(), 1, w*h*1, fin);
        }
        else // TGA_8BIT_UNCOMPRESSED
        {
          int pixsize = 1;
          int pixcount = w*h;
          int pix = 0;
          while(pix < pixcount)
          {
            unsigned char header = 0;
            fread(&header, 1, 1, fin);
            if (header >= 128)
            {
              int count = header - 128 + 1;
              unsigned char bgra[4];
              fread(bgra, 1, pixsize, fin);
              while(count--)
              {
                memcpy((unsigned char*)img->pixels() + pix*pixsize, bgra, pixsize);
                pix++;
              }
            }
            else
            {
              int count = header + 1;
              fread((unsigned char*)img->pixels() + pix*pixsize, 1, pixsize*count, fin);
              pix += count;
            }
          }
        }

        internal_8ToRGBA(palette, img->pixels(), img->width(), img->height());
        internal_swapBytes32_BGRA_RGBA(img->pixels(), img->requiredMemory());
			}
			else
			{
				Log::error( Say("TGA ERROR: TGA_8BIT_UNCOMPRESSED entry size = %n not supported.\n") << header.ColMapEntrySize );
				fclose(fin);
				return NULL;
			}
		}
		else
		{
			Log::error( Say("TGA ERROR: TGA_8BIT_UNCOMPRESSED %nbpp bit not supported.\n") << header.BitsPerPixel );
			fclose(fin);
			return NULL;
		}
	}
  else
	if (header.ImageType == TGA_GRAYSCALE_UNCOMPRESSED || header.ImageType == TGA_GRAYSCALE_COMPRESSED)
	{
		if (header.BitsPerPixel == 8)
		{
			fseek(fin, pixels_offset, SEEK_SET);
      img->allocate2D(w, h, 1, IF_LUMINANCE, IT_UNSIGNED_BYTE);
	    if (header.ImageType == TGA_GRAYSCALE_UNCOMPRESSED)
      {
        fread(img->pixels(), 1, w*h, fin);
      }
      else  // TGA_GRAYSCALE_COMPRESSED
      {
        int pixsize = 1;
        int pixcount = w*h;
        int pix = 0;
        while(pix < pixcount)
        {
          unsigned char header = 0;
          fread(&header, 1, 1, fin);
          if (header >= 128)
          {
            int count = header - 128 + 1;
            unsigned char bgra[4];
            fread(bgra, 1, pixsize, fin);
            while(count--)
            {
              memcpy((unsigned char*)img->pixels() + pix*pixsize, bgra, pixsize);
              pix++;
            }
          }
          else
          {
            int count = header + 1;
            fread((unsigned char*)img->pixels() + pix*pixsize, 1, pixsize*count, fin);
            pix += count;
          }
        }
      }
		}
		else
		{
			Log::error( Say("TGA ERROR: TGA_GRAYSCALE_UNCOMPRESSED %nbpp not supported.\n") << header.BitsPerPixel );
			fclose(fin);
			return NULL;
		}

	}
	else
	{
		Log::error( Say("TGA ERROR: this type %n not supported.\n") << header.ImageType);
		fclose(fin);
		return NULL;
	}

  if (header.ImageDescriptor & (1<<5))
    img->flipVertically();

	fclose(fin);
	return img;
}

vl::ref<Image> vl::loadTIFF(const std::string& file)
{
  ref<Image> img = new Image;

	FILE * fin = fopen( file.c_str(), "rb");
	if (!fin) 
  {
    Log::error( Say("TIFF: could not open '%s'.\n") << file );
		return NULL;
	}

  img->setName(file);

  unsigned char byteorder[2];

	fread(byteorder, 1, 2, fin);

  bool endianess = byteorder[0] == 'M';

  readWord(fin, endianess);

  unsigned long ifd_offset = readDWord(fin, endianess);

  fseek(fin, ifd_offset, SEEK_SET);

  unsigned short dir_count = readWord(fin, endianess);

  int w=0, h=0, compression=0;
  int rows_per_strip=0;
  int strip_count=0;
  int bits_per_sample=0;
  int photometric_interpretation=0;
  int palette_count=0;
  unsigned long palette_offset=0;
  int samples_per_pixel=0;
  int alpha_mode=-1;
  std::vector<unsigned long> strips_offs;
  std::vector<unsigned long> strip_byte_counts;

  for(int idir=0; idir<dir_count; idir++)
  {

    unsigned short tag = readWord(fin, endianess);

    unsigned short type = readWord(fin, endianess);
    char* typestr = "";
    switch(type)
    {
      case 1: typestr = "BYTE"; break;
      case 2: typestr = "ASCII"; break;
      case 3: typestr = "SHORT"; break;
      case 4: typestr = "LONG"; break;
      case 5: typestr = "RATIONAL"; break;
      case 6: typestr = "SBYTE"; break;
      case 7: typestr = "UNDEFINED"; break;
      case 8: typestr = "SSHORT"; break;
      case 9: typestr = "SLONG"; break;
      case 10: typestr = "SRATIONAL"; break;
      case 11: typestr = "FLOAT"; break;
      case 12: typestr = "DOUBLE"; break;
      default:
        typestr = "*** error ***";
    }

    unsigned long count = readDWord(fin, endianess);

    unsigned long val[] = {0,0,0,0};
    unsigned long value_offset = 0;

    if(type == 1 && count<=4) // BYTE
    {
      val[0] = readByte(fin);
      val[1] = readByte(fin);
      val[2] = readByte(fin);
      val[3] = readByte(fin);
    }
    else
    if(type == 3 && count<=2) // SHORT
    {
      val[0] = readWord(fin, endianess);
      val[1] = readWord(fin, endianess);

    }
    else
    if(type == 4 && count<=1) // LONG
    {
      val[0] = readDWord(fin, endianess);

    }
    else
    {
      value_offset = readDWord(fin, endianess);

    }

    switch(tag)
    {

      case 259: 
        compression = val[0];
      break;

      case 257:
        h = val[0];
      break;

      case 256:
        w = val[0];
      break;

      case 278:
        rows_per_strip = val[0];
      break;

      case 273:
        strip_count  = count;

        if (count == 1)
          strips_offs.push_back(val[0]);
        else

        if (count == 2 && type == 3)
        {
          strips_offs.push_back(val[0]);
          strips_offs.push_back(val[1]);
        }

        else
        {
          unsigned long pos = ftell(fin);

          fseek(fin, value_offset, SEEK_SET);
          for(int i=0; i<(int)count; i++)
          {
            if (type == 3)
              strips_offs.push_back( readWord(fin,endianess) );
            else
            if (type == 4)
              strips_offs.push_back( readDWord(fin,endianess) );
          }

          fseek(fin, pos, SEEK_SET);
        }
      break;

      case 279:

        if (count == 1)
          strip_byte_counts.push_back(val[0]);
        else

        if (count == 2 && type == 3)
        {
          strip_byte_counts.push_back(val[0]);
          strip_byte_counts.push_back(val[1]);
        }

        else
        {
          unsigned long pos = ftell(fin);

          fseek(fin, value_offset, SEEK_SET);
          for(int i=0; i<(int)count; i++)
          {
            if (type == 3)
              strip_byte_counts.push_back( readWord(fin,endianess) );
            else
            if (type == 4)
              strip_byte_counts.push_back( readDWord(fin,endianess) );
          }

          fseek(fin, pos, SEEK_SET);
        }
      break;

      case 258:
        bits_per_sample = val[0]; // 0, 4 or 8
      break;

      case 262:

        photometric_interpretation = val[0];
      break;

      case 320:
        palette_count = count;
        palette_offset = value_offset;
      break;

      case 277:

        samples_per_pixel = val[0];
      break;

      case 338:

        alpha_mode = val[0];
      break;
    }
  }

  if (photometric_interpretation == 2)
    bits_per_sample = 8;

  readDWord(fin, endianess);

  if (palette_count>3*256)
  {
    Log::error( Say("TIFF file '%s' seems corrupted.\n") << file);
    fclose(fin);
    return NULL;
  }

  if (compression != 1)
  {
    Log::error( Say("Compressed (type %n) TIFF files (%s) are not supported.\n") << compression << file);
    fclose(fin);
    return NULL;
  }

  if (strips_offs.size() != strip_byte_counts.size())
  {
    Log::error( Say("TIFF file '%s' seems corrupted.\n") << file);
    fclose(fin);
    return NULL;
  }

  if (photometric_interpretation == 2 && samples_per_pixel > 4)
  {
    Log::error( Say("TIFF file '%s': too many samples per pixel (more than 4).\n") << file);
    fclose(fin);
    return NULL; 
  }

  if (alpha_mode != 0 && alpha_mode != -1 && alpha_mode != 2)
  {
    Log::error( Say("TIFF file '%s': not supported premultiplied alpha channels.\n") << file);
    fclose(fin);
    return NULL;
  }  

  if (bits_per_sample < 8)
  {
    Log::error( Say("TIFF file '%s': not supported bits per sample < 8.\n") << file);
    fclose(fin);
    return NULL;
  }  

  if (rows_per_strip == 0)
    rows_per_strip = h;

  if (photometric_interpretation == 3)
  {

    TPalette16x3x256 palette16;
    memset(palette16, 0, sizeof(palette16));
    TPalette3x256 palette8;
    memset(palette8, 0, sizeof(palette8));

    CHECK(palette_count*2 == sizeof(palette16))

    fseek(fin, palette_offset, SEEK_SET);
    for(int i=0; i<palette_count; i++)
      palette16[i] = readWord(fin, endianess);

    int colors = palette_count / 3;
    for(int i=0; i<colors; i++)
    {
      palette8[i*3 + 0] = (unsigned char)(palette16[i + colors*0] >> 8);
      palette8[i*3 + 1] = (unsigned char)(palette16[i + colors*1] >> 8);
      palette8[i*3 + 2] = (unsigned char)(palette16[i + colors*2] >> 8);
    }
    
    img->allocate2D(w, h, 4, IF_RGBA, IT_UNSIGNED_BYTE);
    memset(img->pixels(), 0, img->requiredMemory());

    unsigned char* ptr = (unsigned char*)img->pixels();
    unsigned char* end = (unsigned char*)img->pixels() + img->requiredMemory();
    for(int i=0; i<(int)strips_offs.size(); i++)
    {

      if (ptr + strip_byte_counts[i] > end)
        break;
      fseek(fin, strips_offs[i], SEEK_SET);
      fread(ptr, 1, strip_byte_counts[i], fin);
      ptr += strip_byte_counts[i];
    }

    internal_8ToRGBA(palette8, img->pixels(), img->width(), img->height(), 0xFF);
  }
  else // support RGB
  if (photometric_interpretation == 2)
  {

    img->allocate2D(w, h, 4, IF_RGBA, IT_UNSIGNED_BYTE);
    memset(img->pixels(), 0, img->requiredMemory());

    unsigned char* ptr = (unsigned char*)img->pixels();
    unsigned char* end = (unsigned char*)img->pixels() + img->requiredMemory();
    for(int i=0; i<(int)strips_offs.size(); i++)
    {

      if (ptr + strip_byte_counts[i] > end)
        break;
      fseek(fin, strips_offs[i], SEEK_SET);
      fread(ptr, 1, strip_byte_counts[i], fin);
      ptr += strip_byte_counts[i];
    }

    if (samples_per_pixel == 3)
      internal_RGBToRGBA(img->pixels(), img->width(), img->height(), 0xFF);

  }
  else // 8bit grayscale images
  if ( (photometric_interpretation == 1 || photometric_interpretation == 0) && bits_per_sample == 8)
  {

    if (samples_per_pixel == 1)
      img->allocate2D(w, h, 1, IF_LUMINANCE, IT_UNSIGNED_BYTE);
    else // == 2
    {
      CHECK(samples_per_pixel == 2)
      img->allocate2D(w, h, 2, IF_LUMINANCE_ALPHA, IT_UNSIGNED_BYTE);
    }
    memset(img->pixels(), 0, img->requiredMemory());

    unsigned char* ptr = (unsigned char*)img->pixels();
    unsigned char* end = (unsigned char*)img->pixels() + img->requiredMemory();
    for(int i=0; i<(int)strips_offs.size(); i++)
    {

      if (ptr + strip_byte_counts[i] > end)
        break;
      fseek(fin, strips_offs[i], SEEK_SET);
      fread(ptr, 1, strip_byte_counts[i], fin);

      if (photometric_interpretation == 0)
      {
        for(int n = 0; n<(int)strip_byte_counts[i]; n+= samples_per_pixel)
          ptr[n] = 255 - ptr[n];
      }

      ptr += strip_byte_counts[i];
    }
  } 
  else
  {
    Log::error( Say("TIFF file '%s': format not supported.\n") << file );
    fclose(fin);
    return NULL;
  }

  fclose(fin);

  img->flipVertically();

  return img;
}

namespace 
{

  const unsigned long DDS_CAPS            = 0x00000001;
  const unsigned long DDS_HEIGHT          = 0x00000002;
  const unsigned long DDS_WIDTH           = 0x00000004;
  const unsigned long DDS_PITCH           = 0x00000008;
  const unsigned long DDS_BACKBUFFERCOUNT = 0x00000020;
  const unsigned long DDS_ZBUFFERBITDEPTH = 0x00000040;
  const unsigned long DDS_ALPHABITDEPTH   = 0x00000080;
  const unsigned long DDS_LPSURFACE       = 0x00000800;
  const unsigned long DDS_PIXELFORMAT     = 0x00001000;
  const unsigned long DDS_CKDESTOVERLAY   = 0x00002000;
  const unsigned long DDS_CKDESTBLT       = 0x00004000;
  const unsigned long DDS_CKSRCOVERLAY    = 0x00008000;
  const unsigned long DDS_CKSRCBLT        = 0x00010000;
  const unsigned long DDS_MIPMAPCOUNT     = 0x00020000;
  const unsigned long DDS_REFRESHRATE     = 0x00040000;
  const unsigned long DDS_LINEARSIZE      = 0x00080000;
  const unsigned long DDS_TEXTURESTAGE    = 0x00100000;
  const unsigned long DDS_FVF             = 0x00200000;
  const unsigned long DDS_SRCVBHANDLE     = 0x00400000;
  const unsigned long DDS_DEPTH           = 0x00800000;
  const unsigned long DDS_ALL             = 0x007FF9EE;
  const unsigned long DDS_REQUIRED_FLAGS  = DDS_CAPS|DDS_PIXELFORMAT|DDS_WIDTH|DDS_HEIGHT;

  const unsigned long DDPF_ALPHAPIXELS     = 0x00000001;
  const unsigned long DDPF_ALPHA           = 0x00000002;
  const unsigned long DDPF_FOURCC          = 0x00000004;
  const unsigned long DDPF_INDEXED4        = 0x00000008;
  const unsigned long DDPF_INDEXEDTO8      = 0x00000010;
  const unsigned long DDPF_INDEXED8        = 0x00000020;
  const unsigned long DDPF_RGB             = 0x00000040;
  const unsigned long DDPF_COMPRESSED      = 0x00000080;
  const unsigned long DDPF_RGBTOYUV        = 0x00000100;
  const unsigned long DDPF_YUV             = 0x00000200;
  const unsigned long DDPF_ZBUFFER         = 0x00000400;
  const unsigned long DDPF_PALETTEINDEXED1 = 0x00000800;
  const unsigned long DDPF_PALETTEINDEXED2 = 0x00001000;
  const unsigned long DDPF_ZPIXELS         = 0x00002000;
  const unsigned long DDPF_STENCILBUFFER   = 0x00004000;
  const unsigned long DDPF_ALPHAPREMULT    = 0x00008000;
  const unsigned long DDPF_LUMINANCE       = 0x00020000;
  const unsigned long DDPF_BUMPLUMINANCE   = 0x00040000;
  const unsigned long DDPF_BUMPDUDV        = 0x00080000;

  const unsigned long DDSCAPS_COMPLEX	= 0x00000008;
  const unsigned long DDSCAPS_TEXTURE	= 0x00001000;
  const unsigned long DDSCAPS_MIPMAP	= 0x00400000;

  const unsigned long DDSCAPS2_CUBEMAP	= 0x00000200;
  const unsigned long DDSCAPS2_CUBEMAP_POSITIVEX = 0x00000400;
  const unsigned long DDSCAPS2_CUBEMAP_NEGATIVEX = 0x00000800;
  const unsigned long DDSCAPS2_CUBEMAP_POSITIVEY = 0x00001000;
  const unsigned long DDSCAPS2_CUBEMAP_NEGATIVEY = 0x00002000;
  const unsigned long DDSCAPS2_CUBEMAP_POSITIVEZ = 0x00004000;
  const unsigned long DDSCAPS2_CUBEMAP_NEGATIVEZ = 0x00008000;
  const unsigned long DDSCAPS2_CUBEMAP_FACES = 
    DDSCAPS2_CUBEMAP_POSITIVEX + DDSCAPS2_CUBEMAP_POSITIVEY + DDSCAPS2_CUBEMAP_POSITIVEZ + 
    DDSCAPS2_CUBEMAP_NEGATIVEX + DDSCAPS2_CUBEMAP_NEGATIVEY + DDSCAPS2_CUBEMAP_NEGATIVEZ ;

  const unsigned long DDSCAPS2_VOLUME	= 0x00200000;

  const char* FOURCC_DXT1 = "DXT1";
  const char* FOURCC_DXT2 = "DXT2";
  const char* FOURCC_DXT3 = "DXT3";
  const char* FOURCC_DXT4 = "DXT4";
  const char* FOURCC_DXT5 = "DXT5";
  const char* FOURCC_PALETTE8 = "P8  ";
  const char* FOURCC_GRAY8ALPHA8 = "AG8 ";

  bool isFOURCC(char* code, unsigned long fcc)
  {
    return memcmp(code, &fcc, 4) == 0;
  }
  
  #define IS_BGRA8(pf) \
    ((pf.dwFlags & DDPF_RGB) && \
     (pf.dwFlags & DDPF_ALPHAPIXELS) && \
     (pf.dwRGBBitCount == 32))

  #define IS_BGRX8(pf) \
    ((pf.dwFlags & DDPF_RGB) && \
     !(pf.dwFlags & DDPF_ALPHAPIXELS) && \
     (pf.dwRGBBitCount == 32))

  #define IS_BGR8(pf) \
    ((pf.dwFlags & DDPF_RGB) && \
    !(pf.dwFlags & DDPF_ALPHAPIXELS) && \
     (pf.dwRGBBitCount == 24))

  #define IS_GRAY8(pf) \
    ((pf.dwFlags & DDPF_LUMINANCE) && \
    (pf.dwRGBBitCount == 8) && \
    !(pf.dwFlags & DDPF_ALPHAPIXELS) || \
     isFOURCC("G8 ", pf.dwFourCC ) )

  #define IS_GRAY8_ALPHA8(pf) \
    ((pf.dwFlags & DDPF_LUMINANCE) && \
    (pf.dwRGBBitCount == 16) && \
    (pf.dwFlags & DDPF_ALPHAPIXELS) || \
     isFOURCC("AG8 ", pf.dwFourCC ) )

  #define IS_PALETTE8(pf) isFOURCC("P8  ", pf.dwFourCC )

  #define IS_DXT1(pf) isFOURCC("DXT1", pf.dwFourCC)

  #define IS_DXT3(pf) isFOURCC("DXT3", pf.dwFourCC)

  #define IS_DXT5(pf) isFOURCC("DXT5", pf.dwFourCC)

  typedef struct 
  {
    unsigned long dwSize;
    unsigned long dwFlags;
    unsigned long dwHeight;
    unsigned long dwWidth;
    unsigned long dwPitchOrLinearSize;
    unsigned long dwDepth;
    unsigned long dwMipMapCount;
    unsigned long dwReserved1[ 11 ];

    struct 
    {
      unsigned long dwSize;
      unsigned long dwFlags;
      unsigned long dwFourCC;
      unsigned long dwRGBBitCount;
      unsigned long dwRBitMask;
      unsigned long dwGBitMask;
      unsigned long dwBBitMask;
      unsigned long dwAlphaBitMask;
    } ddpfPixelFormat;

    struct 
    {
      unsigned long dwCaps1;
      unsigned long dwCaps2;
      unsigned long dwReserved[2];
    } ddsCaps;

    unsigned long dwReserved2;

  } DDSURFACEDESC2;

  enum 
  {
    DDS_IMAGE_NULL = 0,
    DDS_IMAGE_2D = 2,
    DDS_IMAGE_3D = 3,
    DDS_IMAGE_CUBEMAP = 4
  };
}

vl::ref<Image> vl::loadDDS(const std::string& file)
{
  ref<Image> img = new Image;

  FILE* fin = fopen(file.c_str(), "rb");
  if (!fin)
  {
    Log::error( Say("DDS: couldn't open file '%s'.\n") << file );
    return NULL;
  }

  char signature[4];
  fread(signature, 1, 4, fin);
  if (strncmp(signature, "DDS ", 4) != 0)
  {
    fclose(fin);
    Log::error( Say("DDS: '%s' is not a DDS file.\n") << file );
    return NULL;
  }

  DDSURFACEDESC2 header;
  memset(&header, 0, sizeof(header));
  CHECK(sizeof(header) == 124);

  header.dwSize = readDWord(fin, false);
  header.dwFlags = readDWord(fin, false);
  header.dwHeight = readDWord(fin, false);
  header.dwWidth = readDWord(fin, false);
  header.dwPitchOrLinearSize = readDWord(fin, false);
  header.dwDepth = readDWord(fin, false);
  header.dwMipMapCount = readDWord(fin, false);
  fread(header.dwReserved1, 1, 11*sizeof(unsigned long), fin);
  header.ddpfPixelFormat.dwSize = readDWord(fin,false);
  header.ddpfPixelFormat.dwFlags = readDWord(fin,false);
  header.ddpfPixelFormat.dwFourCC = readDWord(fin,false);
  header.ddpfPixelFormat.dwRGBBitCount = readDWord(fin,false);
  header.ddpfPixelFormat.dwRBitMask = readDWord(fin,false);
  header.ddpfPixelFormat.dwGBitMask = readDWord(fin,false);
  header.ddpfPixelFormat.dwBBitMask = readDWord(fin,false);
  header.ddpfPixelFormat.dwAlphaBitMask = readDWord(fin,false);
  header.ddsCaps.dwCaps1 = readDWord(fin,false);
  header.ddsCaps.dwCaps2 = readDWord(fin,false);
  header.ddsCaps.dwReserved[0] = readDWord(fin,false);
  header.ddsCaps.dwReserved[1] = readDWord(fin,false);
  header.dwReserved2 = readDWord(fin,false);

  if( header.dwSize != 124 || header.ddpfPixelFormat.dwSize != 32 || 
      (header.dwFlags & DDS_REQUIRED_FLAGS) != DDS_REQUIRED_FLAGS || 
      (header.ddsCaps.dwCaps1 & DDSCAPS_TEXTURE) != DDSCAPS_TEXTURE )
  {
    Log::error( Say("DDS: '%s' corrupted.\n") << file );
    fclose(fin);
    return NULL;
  }

  int image_type = header.dwDepth ? DDS_IMAGE_3D : DDS_IMAGE_2D;

  if (header.ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP)
  {

    bool allfaces = (header.ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP_FACES) != 0;

    if (!allfaces)
    {

      Log::error( Say("DDS: '%s' contains an invalid cubemap.\n") << file );
      fclose(fin);
      return NULL;
    }
    else
      image_type = DDS_IMAGE_CUBEMAP;
  }

  bool reverse_rgba_bgra = false;
  if (header.ddpfPixelFormat.dwRBitMask != 0xFF)
    reverse_rgba_bgra = true;

  int depth = (header.dwFlags & DDS_DEPTH) ? header.dwDepth : 0;
  int mipmaps = (header.dwFlags & DDS_MIPMAPCOUNT) ? header.dwMipMapCount : 1; // implies DDSCAPS_MIPMAP | DDSCAPS_COMPLEX
  int hasalpha = header.ddpfPixelFormat.dwFlags & DDPF_ALPHAPIXELS;

  int max_face = 1;
  if (image_type == DDS_IMAGE_CUBEMAP)
    max_face = 6;
  else
  if(image_type == DDS_IMAGE_3D)
    max_face = depth;

  std::vector< ref<Image> > image;
  for(int i=0; i<mipmaps; i++)
  {
    image.push_back( new Image );
    image.back()->setName(file);
  }

  if (IS_BGRA8(header.ddpfPixelFormat) || IS_BGRX8(header.ddpfPixelFormat))
  {
    for(int i=0, w = header.dwWidth, h = header.dwHeight; i<mipmaps; i++, w/=2, h/=2)
    {
      if (image_type == DDS_IMAGE_2D)
        image[i]->allocate2D(w, h, 1, IF_RGBA, IT_UNSIGNED_BYTE);
      else
      if (image_type == DDS_IMAGE_CUBEMAP)
        image[i]->allocateCubemap(w, h, 1, IF_RGBA, IT_UNSIGNED_BYTE);
    }

    for(int face=0; face<max_face; face++)
    {
      for(int i=0, w = header.dwWidth, h = header.dwHeight; i<mipmaps; i++, w/=2, h/=2)
      {
        int req_mem = Image( w, h, 0, 1, IF_RGBA, IT_UNSIGNED_BYTE ).requiredMemory();
        int offset = req_mem*face;
        fread(image[i]->pixels() + offset, 1, req_mem, fin);

        if(reverse_rgba_bgra)
          internal_swapBytes32_BGRA_RGBA(image[i]->pixels() + offset, req_mem);

        if (IS_BGRX8(header.ddpfPixelFormat))
          internal_fillRGBA32_Alpha(image[i]->pixels() + offset, req_mem, 0xFF);
      }
    }
  }
  else
  if (IS_BGR8(header.ddpfPixelFormat))
  {
    for(int i=0, w = header.dwWidth, h = header.dwHeight; i<mipmaps; i++, w/=2, h/=2)
    {
      if (image_type == DDS_IMAGE_2D)
        image[i]->allocate2D(w, h, 1, IF_RGB, IT_UNSIGNED_BYTE);
      else
      if (image_type == DDS_IMAGE_CUBEMAP)
        image[i]->allocateCubemap(w, h, 1, IF_RGB, IT_UNSIGNED_BYTE);
    }

    for(int face=0; face<max_face; face++)
    {
      for(int i=0, w = header.dwWidth, h = header.dwHeight; i<mipmaps; i++, w/=2, h/=2)
      {
        int req_mem = Image( w, h, 0, 1, IF_RGB, IT_UNSIGNED_BYTE ).requiredMemory();
        int offset = req_mem*face;
        fread(image[i]->pixels() + offset, 1, req_mem, fin);

        if(reverse_rgba_bgra)
          internal_swapBytes24_BGR_RGB(image[i]->pixels() + offset, req_mem);
      }
    }
  }
  else
  if (IS_GRAY8(header.ddpfPixelFormat))
  {
    for(int i=0, w = header.dwWidth, h = header.dwHeight; i<mipmaps; i++, w/=2, h/=2)
    {
      if (image_type == DDS_IMAGE_2D)
        image[i]->allocate2D(w, h, 1, IF_LUMINANCE, IT_UNSIGNED_BYTE);
      else
      if (image_type == DDS_IMAGE_CUBEMAP)
        image[i]->allocateCubemap(w, h, 1, IF_LUMINANCE, IT_UNSIGNED_BYTE);
    }

    for(int face=0; face<max_face; face++)
    {
      for(int i=0, w = header.dwWidth, h = header.dwHeight; i<mipmaps; i++, w/=2, h/=2)
      {
        int req_mem = Image( w, h, 0, 1, IF_LUMINANCE, IT_UNSIGNED_BYTE ).requiredMemory();
        int offset = req_mem*face;
        fread(image[i]->pixels() + offset, 1, req_mem, fin);
      }
    }
  }
  else
  if (IS_GRAY8_ALPHA8(header.ddpfPixelFormat))
  {
    for(int i=0, w = header.dwWidth, h = header.dwHeight; i<mipmaps; i++, w/=2, h/=2)
    {
      if (image_type == DDS_IMAGE_2D)
        image[i]->allocate2D(w, h, 1, IF_LUMINANCE_ALPHA, IT_UNSIGNED_BYTE);
      else
      if (image_type == DDS_IMAGE_CUBEMAP)
        image[i]->allocateCubemap(w, h, 1, IF_LUMINANCE_ALPHA, IT_UNSIGNED_BYTE);
    }

    for(int face=0; face<max_face; face++)
    {
      for(int i=0, w = header.dwWidth, h = header.dwHeight; i<mipmaps; i++, w/=2, h/=2)
      {
        int req_mem = Image( w, h, 0, 1, IF_LUMINANCE_ALPHA, IT_UNSIGNED_BYTE ).requiredMemory();
        int offset = req_mem*face;
        fread(image[i]->pixels() + offset, 1, req_mem, fin);
        if (hasalpha == false)
          internal_fillGray8Alpha8_Alpha(image[i]->pixels() + offset, req_mem, 0xFF);
      }
    }

  }
  else
  if (IS_PALETTE8(header.ddpfPixelFormat))
  {
    for(int i=0, w = header.dwWidth, h = header.dwHeight; i<mipmaps; i++, w/=2, h/=2)
    {
      if (image_type == DDS_IMAGE_2D)
        image[i]->allocate2D(w, h, 1, IF_RGBA, IT_UNSIGNED_BYTE);
      else
      if (image_type == DDS_IMAGE_CUBEMAP)
        image[i]->allocateCubemap(w, h, 1, IF_RGBA, IT_UNSIGNED_BYTE);
    }

    for(int face=0; face<max_face; face++)
    {
      TPalette4x256 palette;
      fread( palette, 1, sizeof(TPalette4x256), fin );
      for(int i=0, w = header.dwWidth, h = header.dwHeight; i<mipmaps; i++, w/=2, h/=2)
      {
        int req_mem1 = w*h*1;
        int req_mem4 = w*h*4;
        int offset = req_mem4*face;

        fread(image[i]->pixels() + offset, 1, req_mem1, fin);
        internal_8ToRGBA( palette, image[i]->pixels() + offset, w, h );
        internal_swapBytes32_BGRA_RGBA(image[i]->pixels() + offset, req_mem4);
        if (hasalpha == false)
          internal_fillRGBA32_Alpha(image[i]->pixels() + offset, req_mem4, 0xFF);
      }
    }
  }
  else
  if ( IS_DXT1(header.ddpfPixelFormat) )
  {
    EImageFormat DXT1 = hasalpha ? IF_COMPRESSED_RGBA_S3TC_DXT1_EXT : IF_COMPRESSED_RGB_S3TC_DXT1_EXT;

    for(int i=0, w = header.dwWidth, h = header.dwHeight; i<mipmaps; i++, w/=2, h/=2)
    {
      if (image_type == DDS_IMAGE_2D)
        image[i]->allocate2D(w, h, 1, DXT1, IT_COMPRESSED_TYPE);
      else
      if (image_type == DDS_IMAGE_CUBEMAP)
        image[i]->allocateCubemap(w, h, 1, DXT1, IT_COMPRESSED_TYPE);
    }

    for(int face=0; face<max_face; face++)
    {
      for(int i=0, w = header.dwWidth, h = header.dwHeight; i<mipmaps; i++, w/=2, h/=2)
      {
        int req_mem = Image( w, h, 0, 1, DXT1, IT_COMPRESSED_TYPE ).requiredMemory();
        int offset = req_mem*face;
        fread(image[i]->pixels() + offset, 1, req_mem, fin);
      }
    }
  }
  else
  if ( IS_DXT3(header.ddpfPixelFormat) )
  {

    for(int i=0, w = header.dwWidth, h = header.dwHeight; i<mipmaps; i++, w/=2, h/=2)
    {
      if (image_type == DDS_IMAGE_2D)
        image[i]->allocate2D(w, h, 1, IF_COMPRESSED_RGBA_S3TC_DXT3_EXT, IT_COMPRESSED_TYPE);
      else
      if (image_type == DDS_IMAGE_CUBEMAP)
        image[i]->allocateCubemap(w, h, 1, IF_COMPRESSED_RGBA_S3TC_DXT3_EXT, IT_COMPRESSED_TYPE);
    }

    for(int face=0; face<max_face; face++)
    {
      for(int i=0, w = header.dwWidth, h = header.dwHeight; i<mipmaps; i++, w/=2, h/=2)
      {
        int req_mem = Image( w, h, 0, 1, IF_COMPRESSED_RGBA_S3TC_DXT3_EXT, IT_COMPRESSED_TYPE ).requiredMemory();
        int offset = req_mem*face;
        fread(image[i]->pixels() + offset, 1, req_mem, fin);
      }
    }
  }
  else
  if ( IS_DXT5(header.ddpfPixelFormat) )
  {

    for(int i=0, w = header.dwWidth, h = header.dwHeight; i<mipmaps; i++, w/=2, h/=2)
    {
      if (image_type == DDS_IMAGE_2D)
        image[i]->allocate2D(w, h, 1, IF_COMPRESSED_RGBA_S3TC_DXT5_EXT, IT_COMPRESSED_TYPE);
      else
      if (image_type == DDS_IMAGE_CUBEMAP)
        image[i]->allocateCubemap(w, h, 1, IF_COMPRESSED_RGBA_S3TC_DXT5_EXT, IT_COMPRESSED_TYPE);
    }

    for(int face=0; face<max_face; face++)
    {
      for(int i=0, w = header.dwWidth, h = header.dwHeight; i<mipmaps; i++, w/=2, h/=2)
      {
        int req_mem = Image( w, h, 0, 1, IF_COMPRESSED_RGBA_S3TC_DXT5_EXT, IT_COMPRESSED_TYPE ).requiredMemory();
        int offset = req_mem*face;
        fread(image[i]->pixels() + offset, 1, req_mem, fin);
      }
    }
  }
  else
  {
    Log::error( Say("DDS: not supported format for '%s'.\n") << file );
    fclose(fin);
    return NULL;
  }

  fclose(fin);

  CHECK(image.size());
  *img = *image[0];
  image.erase(image.begin());
  img->setMipmaps(image);

  return img;
}

bool Image::saveTIFF(const std::string& file)
{
  if (depth())
  {
    Log::error( Say("TIFF: cannot save a 3D image in TIFF format.\n") << file );
    return false;
  }

  const int SHORT = 3;
  const int LONG  = 4;
  const int RATIONAL  = 5;

  if (type() != IT_UNSIGNED_BYTE)
    return false;

  switch(format())
  {
  case IF_RGBA:
    break;
  default:
    return false;
  }

  FILE* fout = fopen(file.c_str(), "wb");
  if (!fout)
  {
    Log::error( Say("TIFF: couldn't open '%s' for writing.\n") << file );
    return false;
  }

  ref<Image> img = new Image;
  *img = *this;

  unsigned char endianess[] = { 'M', 'M' };
  fwrite(endianess, 1, 2, fout);

  unsigned short version = 42;
  writeWord(version, fout);

  unsigned long ifd_offset = 8;
  writeDWord(ifd_offset, fout);

  unsigned short dir_count = 14;
  writeWord(dir_count, fout);

  unsigned short tag, type;
  unsigned long count;

  tag = 256; writeWord(tag, fout);
  type = SHORT; writeWord(type, fout);
  count = 1; writeDWord(count, fout);
  writeWord((unsigned short)img->width(), fout); writeWord(0, fout);
  
  tag = 257; writeWord(tag, fout);
  type = SHORT; writeWord(type, fout);
  count = 1; writeDWord(count, fout);
  writeWord((unsigned short)(img->height()?img->height():1), fout); writeWord(0, fout);
  
  tag = 258; writeWord(tag, fout);
  type = SHORT; writeWord(type, fout);
  count = 4; writeDWord(count, fout);
  writeDWord(10 + dir_count*12 + 4 + 16, fout);
  
  tag = 259; writeWord(tag, fout);
  type = SHORT; writeWord(type, fout);
  count = 1; writeDWord(count, fout);
  writeWord(1, fout); writeWord(0, fout);
  
  tag = 262; writeWord(tag, fout);
  type = SHORT; writeWord(type, fout);
  count = 1; writeDWord(count, fout);
  writeWord(2, fout); writeWord(0, fout);

  tag = 273; writeWord(tag, fout);
  type = LONG; writeWord(type, fout);
  count = 1; writeDWord(count, fout);
  writeDWord(10 + dir_count*12 + 4 + 16 + 8, fout); 

  tag = 277; writeWord(tag, fout);
  type = SHORT; writeWord(type, fout);
  count = 1; writeDWord(count, fout);
  writeWord(4, fout); writeWord(0, fout);

  tag = 278; writeWord(tag, fout);
  type = SHORT; writeWord(type, fout);
  count = 1; writeDWord(count, fout);
  writeWord((unsigned short)(img->height()?img->height():1), fout); writeWord(0, fout);

  tag = 279; writeWord(tag, fout);
  type = LONG; writeWord(type, fout);
  count = 1; writeDWord(count, fout);
  writeDWord(img->requiredMemory(), fout);

  tag = 282; writeWord(tag, fout);
  type = RATIONAL; writeWord(type, fout);
  count = 1; writeDWord(count, fout);
  writeDWord(10 + dir_count*12 + 4 + 0, fout);

  tag = 283; writeWord(tag, fout);
  type = RATIONAL; writeWord(type, fout);
  count = 1; writeDWord(count, fout);
  writeDWord(10 + dir_count*12 + 4 + 8, fout);

  tag = 284; writeWord(tag, fout);
  type = SHORT; writeWord(type, fout);
  count = 1; writeDWord(count, fout);
  writeWord(1, fout); writeWord(0, fout);

  tag = 296; writeWord(tag, fout);
  type = SHORT; writeWord(type, fout);
  count = 1; writeDWord(count, fout);
  writeWord(2, fout); writeWord(0, fout);

  tag = 338; writeWord(tag, fout);
  type = SHORT; writeWord(type, fout);
  count = 1; writeDWord(count, fout);
  writeWord(0, fout); writeWord(0, fout);

  writeDWord(0, fout);

  writeDWord(72, fout);
  writeDWord(1, fout);

  writeDWord(72, fout);
  writeDWord(1, fout);

  writeWord(8, fout);
  writeWord(8, fout);
  writeWord(8, fout);
  writeWord(8, fout);

  img->flipVertically();
  fwrite(img->pixels(), 1, img->requiredMemory(), fout);

  fclose(fout);
  return true;
}

bool Image::saveTGA( const std::string& file )
{
	if (width() * height() == 0)
		return false;

  FILE *fout = fopen(file.c_str(), "wb");

	if(fout == NULL)
	{
    Log::error( Say("TGA: couldn't write to '%s'.\n") << file );
		return false;
	}

	STGAHeader header;
	memset(&header, 0, sizeof(STGAHeader));
	header.ImageType = TGA_RGB_UNCOMPRESSED;
	header.Width_lo  = (unsigned char)(width() & 0x00FF);
	header.Width_hi  = (unsigned char)(width() >> 8);
	header.Height_lo = (unsigned char)(height() & 0x00FF);
	header.Height_hi = (unsigned char)(height() >> 8);
	header.BitsPerPixel = 32;
	fwrite(&header, 1, sizeof(header), fout);
  internal_swapBytes32_BGRA_RGBA(pixels(), requiredMemory());
  fwrite(pixels(), 1, width()*height()*4, fout);
  internal_swapBytes32_BGRA_RGBA(pixels(), requiredMemory());

  writeDWord(0, fout);

  writeDWord(0, fout);

  fwrite("TRUEVISION-XFILE.", 1, 18, fout);

	fclose(fout);
	return true;
}

bool vl::isFormatBMP(const std::string& file)
{
  FILE *fin = fopen(file.c_str(), "rb");

	if(fin == NULL)
		return false;

  BitmapFileHeader bfh;
  memset(&bfh, 0, sizeof(bfh));
	BitmapInfoHeader bih;
  memset(&bih, 0, sizeof(bih));

	fread(&bfh, 1, sizeof(bfh), fin);
	fread(&bih, 1, sizeof(bih), fin);
  fclose(fin);

	if (bfh.Type() != 0x4D42)
  {
    return false;
  }

  switch( bih.BitCount() )
  {
  case 1:
  case 4:
  case 8:
  case 16:
  case 24:
  case 32:
    break;
  default:
    return false;
  }

  switch( bih.Compression() )
  {
  case 0:
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
    break;
  default:
    return false;
  }

  return true;
}

bool vl::isFormatTGA( const std::string& path )
{
	FILE* fin = fopen(path.c_str(), "rb");

	if (fin == NULL)
    return false;

	STGAHeader header;
  memset(&header, 0, sizeof(header));
	fread(&header, 1, sizeof(STGAHeader), fin);

  char signature[17];
  memset(signature, 0, 17);
  fseek(fin, -18, SEEK_END);
  fread(signature, 1, 16, fin);
  fclose(fin);

  if (strcmp("TRUEVISION-XFILE", signature) == 0)
    return true;

  switch( header.ImageType )
  {
    case 0:
    case 1:
    case 2:
    case 3:
    case 9:
    case 10:
    case 11:
      break;
    default:
      return false;
  }

  unsigned int width  = header.Width_lo  + header.Width_hi*256;
  unsigned int height = header.Height_lo + header.Height_hi*256;
  unsigned int bpp = header.BitsPerPixel;

  if (width * height == 0)
    return false;

  switch( bpp )
  {
    case 1:
    case 4:
    case 8:
    case 16:
    case 24:
    case 32:
      break;
    default:
      return false;
  }

  std::string lowcase = path;

  for(int i=0; i<(int)path.size(); i++)
    if (path[i]>='A' && path[i] <= 'Z')
      lowcase[i] = path[i] - 'A' + 'a';

  if ( strstr(lowcase.c_str(), ".tga") != lowcase.c_str() + (strlen(lowcase.c_str()) - 4) )
    return false;

  Log::alert( Say("vl::isFormatTGA: the file '%s' looks like a TGA without the 'TRUEVISION-XFILE' signature.\n") << path );
  return true;
}

bool vl::isFormatTIFF(const std::string& file)
{
  FILE *fin = fopen(file.c_str(), "rb");

	if(fin == NULL)
		return false;

  unsigned char byteorder[2];
	fread(byteorder, 1, 2, fin);
  bool litendian = byteorder[0] == 'M';
  unsigned short version = readWord(fin, litendian);
  fclose(fin);

  if (byteorder[0] != byteorder[1])
    return false;

  if (byteorder[0] != 'M' && byteorder[0] != 'I')
    return false;

  if (version != 42)
    return false;

  return true;
}

bool vl::isFormatDDS(const std::string& file)
{
  FILE* fin = fopen(file.c_str(), "rb");
  if (!fin)
    return false;

  char signature[4];
  fread(signature, 1, 4, fin);
  if (strncmp(signature, "DDS ", 4) != 0)
  {
    fclose(fin);
    return false;
  }

  DDSURFACEDESC2 header;
  memset(&header, 0, sizeof(header));
  CHECK(sizeof(header) == 124);

  header.dwSize = readDWord(fin, false);
  header.dwFlags = readDWord(fin, false);
  header.dwHeight = readDWord(fin, false);
  header.dwWidth = readDWord(fin, false);
  header.dwPitchOrLinearSize = readDWord(fin, false);
  header.dwDepth = readDWord(fin, false);
  header.dwMipMapCount = readDWord(fin, false);
  fread(header.dwReserved1, 1, 11*sizeof(unsigned long), fin);
  header.ddpfPixelFormat.dwSize = readDWord(fin,false);
  header.ddpfPixelFormat.dwFlags = readDWord(fin,false);
  header.ddpfPixelFormat.dwFourCC = readDWord(fin,false);
  header.ddpfPixelFormat.dwRGBBitCount = readDWord(fin,false);
  header.ddpfPixelFormat.dwRBitMask = readDWord(fin,false);
  header.ddpfPixelFormat.dwGBitMask = readDWord(fin,false);
  header.ddpfPixelFormat.dwBBitMask = readDWord(fin,false);
  header.ddpfPixelFormat.dwAlphaBitMask = readDWord(fin,false);
  header.ddsCaps.dwCaps1 = readDWord(fin,false);
  header.ddsCaps.dwCaps2 = readDWord(fin,false);
  header.ddsCaps.dwReserved[0] = readDWord(fin,false);
  header.ddsCaps.dwReserved[1] = readDWord(fin,false);
  header.dwReserved2 = readDWord(fin,false);
  fclose(fin);

  if( header.dwSize != 124 || header.ddpfPixelFormat.dwSize != 32 || 
      (header.dwFlags & DDS_REQUIRED_FLAGS) != DDS_REQUIRED_FLAGS || 
      (header.ddsCaps.dwCaps1 & DDSCAPS_TEXTURE) != DDSCAPS_TEXTURE )
    return false;

  return true;
}

vl::ref<Image> vl::loadImage( const std::string& path)
{
  ref<Image> img = new Image;

  img->setName(path);

  FILE *fin = fopen(path.c_str(), "rb");
	if(fin == NULL)
  {
    Log::error( Say("vl::loadImage(): '%s' file not found.\n") << path);
		return NULL;
  }
  else
    fclose(fin);

  if (isFormatTIFF(path)) 
    return loadTIFF(path);
  else
  if (isFormatBMP(path)) 
    return loadBMP(path);
  else
  if (isFormatDDS(path)) 
    return loadDDS(path);
  else
  if (isFormatTGA(path)) 
    return loadTGA(path);
  else
    Log::error( Say("vl::loadImage('%s'): not supported format.\n") << path);

  return NULL;
}

vl::ref<Image> vl::loadAsCubemap(std::string xp_file, std::string xn_file, std::string yp_file, std::string yn_file, std::string zp_file, std::string zn_file)
{
  ref<Image> xp = vl::loadImage(xp_file);
  ref<Image> xn = vl::loadImage(xn_file);
  ref<Image> yp = vl::loadImage(yp_file);
  ref<Image> yn = vl::loadImage(yn_file);
  ref<Image> zp = vl::loadImage(zp_file);
  ref<Image> zn = vl::loadImage(zn_file);

  ref<Image> img = new Image;

  img->createCubemap(xp.get(), xn.get(), yp.get(), yn.get(), zp.get(), zn.get());

  return img;
}
