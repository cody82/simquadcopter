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
#include "vl/CHECK.hpp"
#include "vl/Say.hpp"
#include "vl/Log.hpp"
#include <map>
#include <cmath>

using namespace vl;

Image::Image()
{
  clear();
}

Image::Image(const std::string& path)
{
  clear();
  setName(path);
  ref<Image> img = loadImage(path);
  CHECK(img);
  if (!img)
    return;

  mPixels.swap(img->mPixels);
  mMipmaps.swap(img->mMipmaps);
  mWidth = img->mWidth;
  mHeight = img->mHeight;
  mDepth = img->mDepth;
  mPitch = img->mPitch;
  mByteAlign = img->mByteAlign;
  mFormat    = img->mFormat;
  mType      = img->mType;
  mIsCubemap = img->mIsCubemap;
}

Image::Image(int x, int y, int z, int bytealign, EImageFormat format, EImageType type): 
  mPixels(0), mWidth(x), mHeight(y), mDepth(z), mPitch(0), mByteAlign(1), mFormat(format), mType(type), mIsCubemap(false)
{
  setByteAlignment(bytealign);
}

bool Image::isCubemap() const
{
  return mIsCubemap;
}

bool Image::isValid() const
{

  bool x = mWidth > 0 && mHeight == 0 && mDepth == 0;
  bool y = mWidth > 0 && mHeight > 0 && mDepth == 0;
  bool z = mWidth > 0 && mHeight > 0 && mDepth > 0;

  bool okformat = false;

  switch(type())
  {
    default:
    break;

    case IT_UNSIGNED_BYTE:
    case IT_BYTE:
    case IT_UNSIGNED_SHORT:
    case IT_SHORT:
    case IT_UNSIGNED_INT:
    case IT_INT:
    case IT_FLOAT:
    {
      switch(format())
      {
        case IF_COMPRESSED_RGB_S3TC_DXT1_EXT:
        case IF_COMPRESSED_RGBA_S3TC_DXT1_EXT:
        case IF_COMPRESSED_RGBA_S3TC_DXT3_EXT:
        case IF_COMPRESSED_RGBA_S3TC_DXT5_EXT:
        {
          break;
        }

        default:
        {
          okformat = true;
        }
      }
    }
  }

  switch(type())
  {
    default:
    break;

    case IT_COMPRESSED_TYPE:
    {
      switch(format())
      {
        case IF_COMPRESSED_RGB_S3TC_DXT1_EXT:
        case IF_COMPRESSED_RGBA_S3TC_DXT1_EXT:
        case IF_COMPRESSED_RGBA_S3TC_DXT3_EXT:
        case IF_COMPRESSED_RGBA_S3TC_DXT5_EXT:
        {
          okformat = true;
          break;
        }

        default:
        {
          break;
        }
      }
    }
  }

  switch(format())
  {
    default:
    break;

    case IF_RGB:
    case IF_BGR:
    {
      switch(type())
      {
        case IT_UNSIGNED_BYTE_3_3_2:
        case IT_UNSIGNED_BYTE_2_3_3_REV:
        case IT_UNSIGNED_SHORT_5_6_5:
        case IT_UNSIGNED_SHORT_5_6_5_REV:
        {
          okformat = true;
          break;
        }

        default:
        {
          break;
        }
      }
      break;
    }

    case IF_RGBA:
    case IF_BGRA:
    {
      switch(type())
      {
        case IT_UNSIGNED_SHORT_4_4_4_4:
        case IT_UNSIGNED_SHORT_4_4_4_4_REV:
        case IT_UNSIGNED_SHORT_5_5_5_1:
        case IT_UNSIGNED_SHORT_1_5_5_5_REV:
        case IT_UNSIGNED_INT_8_8_8_8:
        case IT_UNSIGNED_INT_8_8_8_8_REV:
        case IT_UNSIGNED_INT_10_10_10_2:
        case IT_UNSIGNED_INT_2_10_10_10_REV:
        {
          okformat = true;
          break;
        }

        default:
        {
          break;
        }
      }
    }
  }

  bool isvalid = okformat && (x|y|z) && (pitch() % mByteAlign == 0);

  PrintDebug( isvalid ? "" : ( okformat ? "Image::isValid(): invalid dimensions or pitch/bytealign combination:\n" : "Image::isValid() reported an invalid format/type combination:\n") + print() );

  return okformat && (x|y|z) && (pitch() % mByteAlign == 0);
}

std::string Image::printType() const
{
  std::map<int, char*> ty;

  ty[IT_COMPRESSED_TYPE] = "IT_COMPRESSED_TYPE";
  ty[IT_UNSIGNED_BYTE] = "IT_UNSIGNED_BYTE";
  ty[IT_BYTE] = "IT_BYTE";
  ty[IT_UNSIGNED_SHORT] = "IT_UNSIGNED_SHORT";
  ty[IT_SHORT] = "IT_SHORT";
  ty[IT_UNSIGNED_INT] = "IT_UNSIGNED_INT";
  ty[IT_INT] = "IT_INT";
  ty[IT_FLOAT] = "IT_FLOAT";
  ty[IT_UNSIGNED_BYTE_3_3_2] = "IT_UNSIGNED_BYTE_3_3_2";
  ty[IT_UNSIGNED_BYTE_2_3_3_REV] = "IT_UNSIGNED_BYTE_2_3_3_REV";
  ty[IT_UNSIGNED_SHORT_5_6_5] = "IT_UNSIGNED_SHORT_5_6_5";
  ty[IT_UNSIGNED_SHORT_5_6_5_REV] = "IT_UNSIGNED_SHORT_5_6_5_REV";
  ty[IT_UNSIGNED_SHORT_4_4_4_4] = "IT_UNSIGNED_SHORT_4_4_4_4";
  ty[IT_UNSIGNED_SHORT_4_4_4_4_REV] = "IT_UNSIGNED_SHORT_4_4_4_4_REV";
  ty[IT_UNSIGNED_SHORT_5_5_5_1] = "IT_UNSIGNED_SHORT_5_5_5_1";
  ty[IT_UNSIGNED_SHORT_1_5_5_5_REV] = "IT_UNSIGNED_SHORT_1_5_5_5_REV";
  ty[IT_UNSIGNED_INT_8_8_8_8] = "IT_UNSIGNED_INT_8_8_8_8";
  ty[IT_UNSIGNED_INT_8_8_8_8_REV] = "IT_UNSIGNED_INT_8_8_8_8_REV";
  ty[IT_UNSIGNED_INT_10_10_10_2] = "IT_UNSIGNED_INT_10_10_10_2";
  ty[IT_UNSIGNED_INT_2_10_10_10_REV] = "IT_UNSIGNED_INT_2_10_10_10_REV";

  CHECK(ty[type()] != NULL)

  return ty[type()];
}

std::string Image::printFormat() const
{
  std::map<int, char*> fo;

  fo[IF_RGB] = "IF_RGB";
  fo[IF_RGBA] = "IF_RGBA";
  fo[IF_BGR] = "IF_BGR";
  fo[IF_BGRA] = "IF_BGRA";
  fo[IF_RED] = "IF_RED";
  fo[IF_GREEN] = "IF_GREEN";
  fo[IF_BLUE] = "IF_BLUE";
  fo[IF_ALPHA] = "IF_ALPHA";
  fo[IF_LUMINANCE] = "IF_LUMINANCE";
  fo[IF_LUMINANCE_ALPHA] = "IF_LUMINANCE_ALPHA";
  fo[IF_COMPRESSED_RGB_S3TC_DXT1_EXT] = "IF_COMPRESSED_RGB_S3TC_DXT1_EXT";
  fo[IF_COMPRESSED_RGBA_S3TC_DXT1_EXT] = "IF_COMPRESSED_RGBA_S3TC_DXT1_EXT";
  fo[IF_COMPRESSED_RGBA_S3TC_DXT3_EXT] = "IF_COMPRESSED_RGBA_S3TC_DXT3_EXT";
  fo[IF_COMPRESSED_RGBA_S3TC_DXT5_EXT] = "IF_COMPRESSED_RGBA_S3TC_DXT5_EXT";

  CHECK( fo[format()] != NULL );

  return fo[format()];
}

std::string Image::print() const
{
  return Say(
  "name   = %s\n"
  "width  = %n\n"
  "height = %n\n"
  "depth  = %n\n"
  "format = %s\n"
  "type   = %s\n"
  "pitch  = %n\n"
  "bytealign = %n\n"
  )
  << name()
  << width()
  << height()
  << depth()
  << printFormat()
  << printType()
  << pitch()
  << byteAlignment();
}

EImageDimension Image::dimension() const
{
  if(mWidth > 0 && mHeight == 0 && mDepth == 0 && !mIsCubemap) return Image1D;
  if(mWidth > 0 && mHeight > 0  && mDepth == 0 && !mIsCubemap) return Image2D;
  if(mWidth > 0 && mHeight > 0  && mDepth >  0 && !mIsCubemap) return Image3D;
  if(mWidth > 0 && mHeight > 0  && mDepth == 0 &&  mIsCubemap) return ImageCubeMap;
  return ImageDimensionError;
}  

int Image::bitsPerPixel() const
{
  int comp_size = 0;

  switch(type())
  {
    default:
    break;

    case IT_UNSIGNED_BYTE:  comp_size = sizeof(GLubyte)  * 8; break;
    case IT_BYTE:           comp_size = sizeof(GLbyte)   * 8; break;
    case IT_UNSIGNED_SHORT: comp_size = sizeof(GLushort) * 8; break;
    case IT_SHORT:          comp_size = sizeof(GLshort)  * 8; break;
    case IT_UNSIGNED_INT:   comp_size = sizeof(GLuint)   * 8; break;
    case IT_INT:            comp_size = sizeof(GLint)    * 8; break;
    case IT_FLOAT:          comp_size = sizeof(GLfloat)  * 8; break;

    case IT_UNSIGNED_BYTE_3_3_2:         return 8;
    case IT_UNSIGNED_BYTE_2_3_3_REV:     return 8;
    case IT_UNSIGNED_SHORT_5_6_5:        return 16;
    case IT_UNSIGNED_SHORT_5_6_5_REV:    return 16;
    case IT_UNSIGNED_SHORT_4_4_4_4:      return 16;
    case IT_UNSIGNED_SHORT_4_4_4_4_REV:  return 16;
    case IT_UNSIGNED_SHORT_5_5_5_1:      return 16;
    case IT_UNSIGNED_SHORT_1_5_5_5_REV:  return 16;
    case IT_UNSIGNED_INT_8_8_8_8:        return 32;
    case IT_UNSIGNED_INT_8_8_8_8_REV:    return 32;
    case IT_UNSIGNED_INT_10_10_10_2:     return 32;
    case IT_UNSIGNED_INT_2_10_10_10_REV: return 32;
  }

  switch(format())
  {
  case IF_RED:             return comp_size * 1; 
  case IF_GREEN:           return comp_size * 1; 
  case IF_BLUE:            return comp_size * 1; 
  case IF_ALPHA:           return comp_size * 1; 
  case IF_LUMINANCE:       return comp_size * 1; 
  case IF_LUMINANCE_ALPHA: return comp_size * 2; 
  case IF_RGB:             return comp_size * 3; 
  case IF_BGR:             return comp_size * 3; 
  case IF_RGBA:            return comp_size * 4; 
  case IF_BGRA:            return comp_size * 4; 

  case IF_COMPRESSED_RGB_S3TC_DXT1_EXT:  return 4; // 8 bytes (64 bits) per block per 16 pixels
  case IF_COMPRESSED_RGBA_S3TC_DXT1_EXT: return 4; // 8 bytes (64 bits) per block per 16 pixels
  case IF_COMPRESSED_RGBA_S3TC_DXT3_EXT: return 8; // 16 bytes (128 bits) per block per 16 pixels
  case IF_COMPRESSED_RGBA_S3TC_DXT5_EXT: return 8; // 16 bytes (128 bits) per block per 16 pixels
  }
  
  CHECK(0)

  return 0;
}

int Image::alphaBits() const
{
  int comp_size = 0;

  switch(type())
  {
    default:
    break;

    case IT_UNSIGNED_BYTE:  comp_size = sizeof(GLubyte)  * 8; break;
    case IT_BYTE:           comp_size = sizeof(GLbyte)   * 8; break;
    case IT_UNSIGNED_SHORT: comp_size = sizeof(GLushort) * 8; break;
    case IT_SHORT:          comp_size = sizeof(GLshort)  * 8; break;
    case IT_UNSIGNED_INT:   comp_size = sizeof(GLuint)   * 8; break;
    case IT_INT:            comp_size = sizeof(GLint)    * 8; break;
    case IT_FLOAT:          comp_size = sizeof(GLfloat)  * 8; break;

    case IT_UNSIGNED_BYTE_3_3_2:         return 0;
    case IT_UNSIGNED_BYTE_2_3_3_REV:     return 0;
    case IT_UNSIGNED_SHORT_5_6_5:        return 0;
    case IT_UNSIGNED_SHORT_5_6_5_REV:    return 0;
    case IT_UNSIGNED_SHORT_4_4_4_4:      return 4;
    case IT_UNSIGNED_SHORT_4_4_4_4_REV:  return 4;
    case IT_UNSIGNED_SHORT_5_5_5_1:      return 1;
    case IT_UNSIGNED_SHORT_1_5_5_5_REV:  return 1;
    case IT_UNSIGNED_INT_8_8_8_8:        return 8;
    case IT_UNSIGNED_INT_8_8_8_8_REV:    return 8;
    case IT_UNSIGNED_INT_10_10_10_2:     return 2;
    case IT_UNSIGNED_INT_2_10_10_10_REV: return 2;
  }

  switch(format())
  {
  case IF_RED:             return comp_size * 0; 
  case IF_GREEN:           return comp_size * 0; 
  case IF_BLUE:            return comp_size * 0; 
  case IF_ALPHA:           return comp_size * 1; 
  case IF_LUMINANCE:       return comp_size * 0; 
  case IF_LUMINANCE_ALPHA: return comp_size * 1; 
  case IF_RGB:             return comp_size * 0; 
  case IF_BGR:             return comp_size * 0; 
  case IF_RGBA:            return comp_size * 1; 
  case IF_BGRA:            return comp_size * 1; 

  case IF_COMPRESSED_RGB_S3TC_DXT1_EXT:  return 0; // 8 bytes (64 bits) per block per 16 pixels
  case IF_COMPRESSED_RGBA_S3TC_DXT1_EXT: return 1; // 8 bytes (64 bits) per block per 16 pixels
  case IF_COMPRESSED_RGBA_S3TC_DXT3_EXT: return 4; // 16 bytes (64 bits for uncompressed alpha + 64 bits for RGB) per block per 16 pixels
  case IF_COMPRESSED_RGBA_S3TC_DXT5_EXT: return 4; // 16 bytes (64 bits for   compressed alpha + 64 bits for RGB) per block per 16 pixels
  }

  CHECK(0)

  return 0;
}

int Image::isCompressedFormat(EImageFormat fmt)
{
  switch(fmt)
  {
  case IF_COMPRESSED_RGB_S3TC_DXT1_EXT:
  case IF_COMPRESSED_RGBA_S3TC_DXT1_EXT:
  case IF_COMPRESSED_RGBA_S3TC_DXT3_EXT:
  case IF_COMPRESSED_RGBA_S3TC_DXT5_EXT:
    return true;

  default:
    return false;
  }
}

int Image::requiredMemory() const
{
  if ( !isValid() )
    return 0;
  else
  {
    int y = height() ? height() : 1;
    int z = depth() ? depth() : 1;
    int req_mem = pitch() * y * z;

    if (req_mem < 8 && format() == IF_COMPRESSED_RGB_S3TC_DXT1_EXT)
      req_mem = 8;

    if (req_mem < 8 && format() == IF_COMPRESSED_RGBA_S3TC_DXT1_EXT)
      req_mem = 8;

    if (req_mem < 16 && format() == IF_COMPRESSED_RGBA_S3TC_DXT3_EXT)
      req_mem = 16;

    if (req_mem < 16 && format() == IF_COMPRESSED_RGBA_S3TC_DXT5_EXT)
      req_mem = 16;

    if (mIsCubemap)
      req_mem *= 6;

    return req_mem;
  }
}

int Image::requiredMemory(int xsize, int ysize, int zsize, int bytealign, EImageFormat format, EImageType type)
{
  return Image(xsize, ysize, zsize, bytealign, format, type).requiredMemory();
}

int Image::byteAlignment() const
{
  return mByteAlign;
}

void Image::setByteAlignment(int bytealign)
{

  CHECK(mPixels.empty());

  if (!mPixels.empty())
    return;

  switch(bytealign)
  {
  default:
    CHECK(0)
  case 0:
    bytealign = sizeof(GLubyte*);
  case 1:
  case 2:
  case 4:
  case 8:
    break;
  }

  mByteAlign = bytealign;

  updatePitch();
}

void Image::updatePitch()
{
  int xbits = mWidth * bitsPerPixel();
  int xbytes = (xbits / 8) + ( xbits % 8 ? 1 : 0);
  mPitch = (xbytes / mByteAlign * mByteAlign) + ((xbytes % mByteAlign)? mByteAlign : 0);
}

void Image::allocateCubemap(int x, int y, int bytealign, EImageFormat format, EImageType type)
{
  setWidth(x);
  setHeight(y);
  setDepth(0);
  setFormat(format);
  setType(type);
  setByteAlignment(bytealign);
  mIsCubemap = true;

  mPixels.clear();
  mPixels.resize(requiredMemory());
}

void Image::allocate1D(int x, EImageFormat format, EImageType type)
{
  CHECK(x);
  setWidth(x);
  setHeight(0);
  setDepth(0);
  setFormat(format);
  setType(type);
  setByteAlignment(1);
  mIsCubemap = false;

  mPixels.clear();
  mPixels.resize(requiredMemory());
}

void Image::allocate2D(int x, int y, int bytealign, EImageFormat format, EImageType type)
{
  CHECK(x);
  CHECK(y);
  setWidth(x);
  setHeight(y);
  setDepth(0);
  setFormat(format);
  setType(type);
  setByteAlignment(bytealign);
  mIsCubemap = false;

  int req_mem = requiredMemory();
  if (req_mem == 0)
    Log::bug("Image::allocate2D couldn't allocate memory, maybe your image is invalid.\n");
  mPixels.clear();
  mPixels.resize(req_mem);
}

void Image::allocate3D(int x, int y, int z, int bytealign, EImageFormat format, EImageType type)
{
  CHECK(x);
  CHECK(y);
  CHECK(z);
  setWidth(x);
  setHeight(y);
  setDepth(z);
  setFormat(format);
  setType(type);
  setByteAlignment(bytealign);
  mIsCubemap = false;

  mPixels.clear();
  mPixels.resize(requiredMemory());
}

void Image::clear()
{
  mPixels.clear();
  mMipmaps.clear();
  mName.clear();
  mWidth = 0;
  mHeight = 0;
  mDepth = 0;
  mPitch = 0;
  mFormat = IF_RGBA;
  mByteAlign = 1;
  mType = IT_UNSIGNED_BYTE;
  mIsCubemap = false;
}

void Image::operator=(const Image& other) 
{

  mPixels = other.mPixels;

  mWidth  = other.mWidth;
  mHeight  = other.mHeight;
  mDepth  = other.mDepth;
  mPitch  = other.mPitch;
  mByteAlign = other.mByteAlign;
  mFormat    = other.mFormat;
  mType      = other.mType;
  mIsCubemap = other.mIsCubemap;

  mMipmaps.resize(other.mMipmaps.size());
  for(int i=0; i<(int)mMipmaps.size(); i++)
  {
    mMipmaps[i] = new Image;
    *mMipmaps[i] = *other.mMipmaps[i];
  }
}

GLubyte* Image::pixelsXP()
{
  if( dimension() != 4 || !pixels())
  {
    CHECK(0);
    return NULL;
  }
  else
  {
    return pixels();
  }
}

GLubyte* Image::pixelsXN()
{
  if( dimension() != 4 || !pixels())
  {
    CHECK(0);
    return NULL;
  }
  else
  {
    return (GLubyte*)pixels() + Image(width(), height(), 0, byteAlignment(), format(), type() ).requiredMemory() * 1;
  }
}

GLubyte* Image::pixelsYP()
{
  if( dimension() != 4 || !pixels())
  {
    CHECK(0);
    return NULL;
  }
  else
  {
    return (GLubyte*)pixels() + Image(width(), height(), 0, byteAlignment(), format(), type() ).requiredMemory() * 2;
  }
}

GLubyte* Image::pixelsYN()
{
  if( dimension() != 4 || !pixels())
  {
    CHECK(0);
    return NULL;
  }
  else
  {
    return (GLubyte*)pixels() + Image(width(), height(), 0, byteAlignment(), format(), type() ).requiredMemory() * 3;
  }
}

GLubyte* Image::pixelsZP()
{
  if( dimension() != 4 || !pixels())
  {
    CHECK(0);
    return NULL;
  }
  else
  {
    return (GLubyte*)pixels() + Image(width(), height(), 0, byteAlignment(), format(), type() ).requiredMemory() * 4;
  }
}

GLubyte* Image::pixelsZN()
{
  if ( dimension() != 4 || !pixels())
  {
    CHECK(0);
    return NULL;
  }
  else
  {
    return (GLubyte*)pixels() + Image(width(), height(), 0, byteAlignment(), format(), type() ).requiredMemory() * 5;
  }
}

GLubyte* Image::pixelsZSlice(int slice)
{
  CHECK(slice < depth());
  CHECK(slice >= 0 );
  if (mIsCubemap || !pixels())
    return NULL;
  else
  {
    return (GLubyte*)pixels() + Image(width(), height(), 0, byteAlignment(), format(), type() ).requiredMemory() * slice;
  }
}

Image* Image::createCubemap(Image* xp, Image* xn, Image* yp, Image* yn, Image* zp, Image* zn)
{
  clear();

  ref<Image> img[] = {xp, xn, yp, yn, zp, zn};

  if (img[0]->width() != img[0]->height())
  {
    Log::error("Cubemap creation failed: all the images must be square.\n");
    return NULL;
  }

  for(int i=0; i<6; i++)
  {
    if (img[i] == NULL || img[i]->isValid() == false || img[i]->pixels() == NULL || img[i]->dimension() != 2)
    {
      Log::error("Cubemap creation failed: one or more image is invalid (could be NULL, not allocated, not 2D, wrong internal_ configuration or other).\n");
      return NULL;
    }

    if (img[0]->width() != img[i]->width())
    {
      Log::error("Cubemap creation failed: the faces of the cube must have the very same dimensions.\n");
      return NULL;
    }

    if (img[0]->height() != img[i]->height())
    {
      Log::error("Cubemap creation failed: the faces of the cube must have the very same dimensions.\n");
      return NULL;
    }

    if (img[0]->format() != img[i]->format())
    {
      Log::error("Cubemap creation failed: the faces of the cube must have the very same format.\n");
      return NULL;
    }

    if (img[0]->type() != img[i]->type())
    {
      Log::error("Cubemap creation failed: the faces of the cube must have the very same type.\n");
      return NULL;
    }

    if (img[0]->byteAlignment() != img[i]->byteAlignment())
    {
      Log::error("Cubemap creation failed: the faces of the cube must have the very same byte alignment.\n");
      return NULL;
    }

    if (img[0]->requiredMemory() != img[i]->requiredMemory())
    {
      Log::error("Cubemap creation failed: the faces of the cube must require the very same amount of memory.\n");
      return NULL;
    }
  }

  ref<Image> cubemap = new Image;
  cubemap->allocateCubemap( img[0]->width(), img[0]->height(), img[0]->byteAlignment(), img[0]->format(), img[0]->type() );

  memcpy( cubemap->pixelsXP(), img[0]->pixels(), img[0]->requiredMemory() );
  memcpy( cubemap->pixelsXN(), img[1]->pixels(), img[0]->requiredMemory() );
  memcpy( cubemap->pixelsYP(), img[2]->pixels(), img[0]->requiredMemory() );
  memcpy( cubemap->pixelsYN(), img[3]->pixels(), img[0]->requiredMemory() );
  memcpy( cubemap->pixelsZP(), img[4]->pixels(), img[0]->requiredMemory() );
  memcpy( cubemap->pixelsZN(), img[5]->pixels(), img[0]->requiredMemory() );

  return cubemap.get();
}

void Image::flipVertically()
{
  if (dimension() == Image1D)
    return;

  assert(pixels());
  int row_size = pitch();
  std::vector<GLubyte> row1;
  row1.resize(row_size);

  std::vector<GLubyte*> pxl;

  if (dimension() == Image2D)
  {
    pxl.push_back( (GLubyte*)pixels() );
  }
  else
  if (dimension() == ImageCubeMap)
  {
    pxl.push_back( (GLubyte*)pixelsXP() );
    pxl.push_back( (GLubyte*)pixelsXN() );
    pxl.push_back( (GLubyte*)pixelsYP() );
    pxl.push_back( (GLubyte*)pixelsYN() );
    pxl.push_back( (GLubyte*)pixelsZP() );
    pxl.push_back( (GLubyte*)pixelsZN() );
  }
  else
  if (dimension() == Image3D)
  {
    for(int zslice=0; zslice<depth(); zslice++)
      pxl.push_back( (GLubyte*)pixelsZSlice(zslice) );
  }

  for(int img=0; img<(int)pxl.size(); img++)
  {
    for(int i=0; i<height()/2; i++) 
    {
      int j = height() - 1 - i;
      memcpy(&row1[0], pxl[img]+i*row_size, row_size);
      memcpy(pxl[img]+i*row_size, pxl[img]+j*row_size, row_size);
      memcpy(pxl[img]+j*row_size,  &row1[0], row_size);
    }
  }
}

void Image::readPixels(int x, int y, int w, int h, EReadDrawBuffer read_buffer)
{
  if (w != width() || h != height() || dimension() != Image2D || pixels() == NULL)
    allocate2D( w, h, 1, format(), type() );

  glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);

  glPixelStorei( GL_PACK_ALIGNMENT, byteAlignment() ); GLCHECK4()
  glPixelStorei( GL_PACK_ROW_LENGTH, width() ); GLCHECK4()
  glPixelStorei( GL_PACK_SKIP_PIXELS, 0); GLCHECK4()
  glPixelStorei( GL_PACK_SKIP_ROWS,   0); GLCHECK4()
  glPixelStorei( GL_PACK_SWAP_BYTES,  0); GLCHECK4()
  glPixelStorei( GL_PACK_LSB_FIRST,   0); GLCHECK4()
  if (GLEW_VERSION_1_2)
  {
    glPixelStorei( GL_PACK_IMAGE_HEIGHT, 0 ); GLCHECK4()
    glPixelStorei( GL_PACK_SKIP_IMAGES,  0 ); GLCHECK4()
  }

  GLint fbo = 0;
  if (GLEW_EXT_framebuffer_object)
    glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &fbo);

  GLint prev = 0;
  glGetIntegerv( GL_READ_BUFFER, &prev );
  glReadBuffer( read_buffer ); GLCHECK4()
  glReadPixels( x, y, w, h, format(), type(), pixels() ); GLCHECK4()
  glReadBuffer( prev ); GLCHECK4()

  glPopClientAttrib();
}

