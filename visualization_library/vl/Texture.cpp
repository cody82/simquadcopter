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

#include "vl/Texture.hpp"
#include "vl/CHECK.hpp"
#include "vl/Image.hpp"
#include "vl/math3D.hpp"
#include "vl/Say.hpp"
#include "vl/Log.hpp"

using namespace vl;

Texture::~Texture()
{
  if (mHandle)
  glDeleteTextures(1, &mHandle);
  mHandle = 0;
}

Texture::Texture(int width, ETextureFormat intformat, EHasBorder border)
{
  GLCHECK4()
  setDimension( TD_TEXTURE_1D );
  setInternalFormat(intformat);
  setBorder(border);
  setWidth(width);
  setHeight(0);
  setDepth(0);
  mHasMipMaps = false;
	glGenTextures( 1, &mHandle );
	glBindTexture( dimension(), mHandle ); GLCHECK4()
  glTexImage1D( dimension(), 0, intformat, width, border == BORDER_ON?1:0, GL_RGBA/*not used*/, GL_UNSIGNED_BYTE/*not used*/, NULL); GLCHECK4()
}

Texture::Texture(int width, int height, ETextureFormat intformat, EHasBorder border)
{

  GLCHECK4()
  setDimension( TD_TEXTURE_2D );
  setInternalFormat(intformat);
  setBorder(border);
  setWidth(width);
  setHeight(height);
  setDepth(0);
  mHasMipMaps = false;
	glGenTextures( 1, &mHandle );
	glBindTexture( dimension(), mHandle ); GLCHECK4()
  glTexImage2D( dimension(), 0, intformat, width, height, border == BORDER_ON?1:0, GL_RGBA/*not used*/, GL_UNSIGNED_BYTE/*not used*/, NULL); GLCHECK4()
}

Texture::Texture(int width, int height, int depth, ETextureFormat intformat, EHasBorder border)
{
  GLCHECK4()
  setDimension( TD_TEXTURE_3D );
  setInternalFormat(intformat);
  setBorder(border);
  setWidth(width);
  setHeight(height);
  setDepth(depth);
  mHasMipMaps = false;
	glGenTextures( 1, &mHandle );
	glBindTexture( dimension(), mHandle ); GLCHECK4()
  glTexImage3D( dimension(), 0, intformat, width, height, depth, border == BORDER_ON?1:0, GL_RGBA/*not used*/, GL_UNSIGNED_BYTE/*not used*/, NULL); GLCHECK4()
}

Texture::Texture(Image* image, ETextureFormat intformat, EHasMipmaps mipmaps_on , EHasBorder border):
  mHandle(0), mDimension(TD_TEXTURE_2D), mWidth(0), mHeight(0), mDepth(0), mBorder(border), mInternalFormat(intformat), mHasMipMaps(false)
{

  if (image && image->isValid())
  {
    if (image->isCubemap())
      createCubemap(image, intformat, mipmaps_on, border);
    else
      createTexture(image, intformat, mipmaps_on, border);
  }
  else
  {
    Log::bug("Texture construnctor called with an invalid Image!\n");
  }
}

Texture::Texture():
  mHandle(0), mDimension(TD_TEXTURE_2D), mWidth(0), mHeight(0), mDepth(0), mBorder(BORDER_OFF), mInternalFormat(TF_RGBA), mHasMipMaps(false)
{
}

bool Texture::isValid()
{
  bool x = mWidth != 0 && mHeight == 0 && mDepth == 0;
  bool y = mWidth != 0 && mHeight != 0 && mDepth == 0;
  bool z = mWidth != 0 && mHeight != 0 && mDepth != 0;
  return handle() != 0 && (x|y|z);
}

bool Texture::supports(Image* image, ETextureFormat intformat, EHasBorder border)
{
  GLCHECK4();

  GLint width = 0;
  const int xsize = image->width();
  const int ysize = image->height();
  const int zsize = image->depth();

  if (image->dimension() == 4) // cube maps
  {
    if (!GLEW_ARB_texture_cube_map)
      return false;

    if ( isCompressedFormat(intformat) && intformat == (int)image->format() )
    {
      glTexImage2D(GL_PROXY_TEXTURE_CUBE_MAP, 0, intformat, xsize + (border?2:0), ysize + (border?2:0), border?1:0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    }
    else
    {
      glTexImage2D(GL_PROXY_TEXTURE_CUBE_MAP, 0, intformat, xsize + (border?2:0), ysize + (border?2:0), border?1:0, image->format(), image->type(), NULL);
    }
    glGetTexLevelParameteriv(GL_PROXY_TEXTURE_CUBE_MAP, 0, GL_TEXTURE_WIDTH, &width);
  }
  else
  if (image->dimension() == 3) // 3d textures
  {
    if (!GL_EXT_texture3D)
      return false;

    if ( isCompressedFormat(intformat) && intformat == (int)image->format() )
    {
      glTexImage3D(GL_PROXY_TEXTURE_3D, 0, intformat, xsize + (border?2:0), ysize + (border?2:0), zsize + (border?2:0), border?1:0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    }
    else
    {
      glTexImage3D(GL_PROXY_TEXTURE_3D, 0, intformat, xsize + (border?2:0), ysize + (border?2:0), zsize + (border?2:0), border?1:0, image->format(), image->type(), NULL);
    }
    glGetTexLevelParameteriv(GL_PROXY_TEXTURE_3D, 0, GL_TEXTURE_WIDTH, &width);
  }
  else
  if (image->dimension() == 2) // 2d textures
  {
    if ( isCompressedFormat(intformat) && intformat == (int)image->format() )
    {
      glTexImage2D(GL_PROXY_TEXTURE_2D, 0, intformat, xsize + (border?2:0), ysize + (border?2:0), border?1:0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    }
    else
    {
      glTexImage2D(GL_PROXY_TEXTURE_2D, 0, intformat, xsize + (border?2:0), ysize + (border?2:0), border?1:0, image->format(), image->type(), NULL);
    }
    glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
  }
  else
  if (image->dimension() == 1) // 1d textures
  {
    if ( isCompressedFormat(intformat) && intformat == (int)image->format() )
    {
      glTexImage1D(GL_PROXY_TEXTURE_1D, 0, intformat, xsize + (border?2:0), border?1:0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    }
    else
    {
      glTexImage1D(GL_PROXY_TEXTURE_1D, 0, intformat, xsize + (border?2:0), border?1:0, image->format(), image->type(), NULL);
    }
    glGetTexLevelParameteriv(GL_PROXY_TEXTURE_1D, 0, GL_TEXTURE_WIDTH, &width);
  }

  return width != 0;
}

void Texture::createCubemap(Image* image, ETextureFormat intformat, EHasMipmaps mipmaps_on, EHasBorder border)
{
  CHECK(mHandle == 0);

  setName( image->name() );

  if ( GLEW_ARB_texture_cube_map == false )
  {
    Log::error("Texture::createCubemap(): GL_ARB_texture_cube_map not supported.\n");
    CHECK(0);
    return;
  }

  std::vector< ref<Image> > mipmaps = image->mipmaps();
  mipmaps.insert(mipmaps.begin(), image);

  if ( (intformat !=  (int)image->format()) && Image::isCompressedFormat( image->format() ) )
  {
    Log::bug("Texture::createCubemap(): compressed images can only be used to generate textures with the same format.\n"
             "You are probably trying to use a compressed Image to create a texture with a different format.\n");
    CHECK(0);
    intformat = (ETextureFormat)image->format();
  }

  if ( image->width() != image->height() )
  {
    Log::bug("Texture::createCubemap(): cubemaps must have square faces.\n");
    CHECK(0);
    return;
  }

  if ( !supports(image, intformat, border) )
  {
    Log::error("Texture::createCubemap(): you requested a not supported format.\n");
    CHECK(0);
    return;
  }

  mHasMipMaps = mipmaps_on == MIPMAPS_ON;
  setInternalFormat(intformat);
  setBorder(border);
  setDimension(TD_TEXTURE_CUBE_MAP);
  setWidth( image->width() );
  setHeight( image->height() );
  setDepth(0);

  glGenTextures( 1, &mHandle ); GLCHECK4()
  glBindTexture( GL_TEXTURE_CUBE_MAP, mHandle ); GLCHECK4()

  for(int mip=0; mip<(int)mipmaps.size(); mip++)
  {

    ref<Image> cubemap = mipmaps[mip];

    GLvoid* face_pxl[] = {
      cubemap->pixelsXP(),
      cubemap->pixelsXN(),
      cubemap->pixelsYP(),
      cubemap->pixelsYN(),
      cubemap->pixelsZP(),
      cubemap->pixelsZN()
    };

    int usecompressed = (intformat ==  (int)image->format()) && isCompressedFormat(intformat);

    for(int iface=0; iface<6; iface++)
    {
	    unsigned char *tex_pixels = (unsigned char*)face_pxl[iface];

      int brd = border ? 1:0;
      glPixelStorei( GL_UNPACK_ALIGNMENT, cubemap->byteAlignment() ); GLCHECK4()

      if (!mipmaps_on) // don't use mipmaps
      {
        if (usecompressed)
        {
          glCompressedTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + iface, 0, intformat, cubemap->width()+2*brd, cubemap->height()+2*brd, brd, cubemap->requiredMemory() / 6, tex_pixels ); GLCHECK4()
        }
        else
        {
          glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + iface, 0, intformat, cubemap->width()+2*brd, cubemap->height()+2*brd, brd, cubemap->format(), cubemap->type(), tex_pixels ); GLCHECK4()
        }
      }
      else // create mipmaps
      if (mipmaps_on && mipmaps.size() == 1)
      {
        if (usecompressed)
        {
          glCompressedTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + iface, mip, intformat, cubemap->width()+2*brd, cubemap->height()+2*brd, brd, cubemap->requiredMemory() / 6, NULL ); GLCHECK4()
        }
        else
        {
          glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + iface, mip, intformat, cubemap->width()+2*brd, cubemap->height()+2*brd, brd, cubemap->format(), cubemap->type(), NULL ); GLCHECK4()
        }
        gluBuild2DMipmaps( GL_TEXTURE_CUBE_MAP_POSITIVE_X + iface, intformat, cubemap->width(), cubemap->height(), cubemap->format(), cubemap->type(), tex_pixels	); GLCHECK4()
      }
      else // use existing mipmaps
      {
        if (usecompressed)
        {
          glCompressedTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + iface, mip, intformat, cubemap->width()+2*brd, cubemap->height()+2*brd, brd, cubemap->requiredMemory() / 6, tex_pixels ); GLCHECK4()
        }
        else
        {
          glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + iface, mip, intformat, cubemap->width()+2*brd, cubemap->height()+2*brd, brd, cubemap->format(), cubemap->type(), tex_pixels ); GLCHECK4()
        }
      }
    }

    if (!mipmaps_on)
      break;
  }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 4); GLCHECK4()
}

void Texture::createTexture(Image* image, ETextureFormat intformat, EHasMipmaps mipmaps_on, EHasBorder border)
{
  CHECK(mHandle == 0);
  CHECK(border == false) // temporary check

  setName( image->name() );

  GLCHECK4()

  if (image == NULL || image->isValid() == false || image->pixels() == NULL)
  {
    Log::bug("Texture::createTexture() called with an invalid Image.\n");
    return;
  }

  int xsize = image->width();
  int ysize = image->height();

  if ( (intformat != (int)image->format()) && Image::isCompressedFormat( image->format() ) )
  {
    Log::bug("Texture::createTexture(): compressed images can only be used to generate textures with the same format. "
             "You are probably trying to use a compressed Image to create a texture with a different format.\n");
     CHECK(0);
    intformat = (ETextureFormat)image->format();
  }

  if ( !supports(image, intformat, border ) )
  {
    Log::error("Texture::createTexture(): the format requested is not supported.\n");
    CHECK(0);
    return;
  }

	unsigned char *tex_pixels = NULL;
  tex_pixels = (unsigned char*)image->pixels();

  ETextureDimension texdim[] = { TD_TEXTURE_1D, TD_TEXTURE_2D, TD_TEXTURE_3D, TD_TEXTURE_CUBE_MAP };
  setDimension( texdim[(int)image->dimension()-1] );
  setInternalFormat(intformat);
  setBorder(border);
  setWidth(xsize);
  setHeight(ysize);
  setDepth(0);
  mHasMipMaps = mipmaps_on == MIPMAPS_ON;

  GLCHECK4()
	glGenTextures( 1, &mHandle );
  GLCHECK4()
	glBindTexture( dimension(), mHandle ); GLCHECK4()

  int usecompressed = (intformat == (int)image->format()) && isCompressedFormat(intformat);
  int brd = border ? 1:0;

  glPixelStorei( GL_UNPACK_ALIGNMENT, image->byteAlignment() ); GLCHECK4()

  if ( mipmaps_on && image->mipmaps().empty() ) // generate mipmaps
  {
    switch( dimension() )
    {
    case TD_TEXTURE_1D:
      if (usecompressed)
      {
        Log::bug("Texture::createTexture(): cannot generate mipmaps from a compressed format.\n"
                 "If you are loading the compressed image from a DDS file be sure it includes all the mipmaps.\n"
                 "Note that you can still generate mipmapped compressed textures using an uncompressed Image as source.\n");
        CHECK(0);
        glCompressedTexImage1D( GL_TEXTURE_1D, 0, intformat, xsize+2*brd, brd, image->requiredMemory(), NULL ); GLCHECK4()
      }
      else
      {
        glTexImage1D( GL_TEXTURE_1D, 0, intformat, xsize+2*brd, brd, image->format(), image->type(), NULL ); GLCHECK4()
      }
	    gluBuild1DMipmaps( GL_TEXTURE_1D, intformat, xsize, image->format(), image->type(), tex_pixels	); GLCHECK4()
      break;
    case TD_TEXTURE_2D:
    {
      if (usecompressed)
      {
        Log::bug("Texture::createTexture(): cannot generate mipmaps from a compressed format.\n"
                 "If you are loading the compressed image from a DDS file be sure it includes all the mipmaps.\n"
                 "Note that you can still generate mipmapped compressed textures using an uncompressed Image as source.\n");
        CHECK(0);
        glCompressedTexImage2D( GL_TEXTURE_2D, 0, intformat, xsize+2*brd, ysize+2*brd, brd, image->requiredMemory(), NULL ); GLCHECK4()
      }
      else
      {
        glTexImage2D( GL_TEXTURE_2D, 0, intformat, xsize+2*brd, ysize+2*brd, brd, image->format(), image->type(), NULL ); GLCHECK4()
      }
	    gluBuild2DMipmaps( GL_TEXTURE_2D, intformat, xsize, ysize, image->format(), image->type(), tex_pixels	); GLCHECK4()
      break;
    }
    case TD_TEXTURE_3D:
      Log::bug("Texture::createTexture(): mipmapping generation for 3d textures not supported yet.");
      CHECK(0);

      break;
    default:
      CHECK(0);
    }
  }
  else // use existing mipmaps
  if ( mipmaps_on && image->mipmaps().size() )
  {
    switch( dimension() )
    {
    case TD_TEXTURE_1D:
    {

      std::vector< ref<Image> > mipmaps = image->mipmaps();
      mipmaps.insert(mipmaps.begin(), image);
      for(int i=0; i<(int)mipmaps.size(); i++)
      {
        int xsize = mipmaps[i]->width();
        if (usecompressed)
        {
          glCompressedTexImage1D( GL_TEXTURE_1D, i, intformat, xsize+2*brd, brd, mipmaps[i]->requiredMemory(), mipmaps[i]->pixels() ); GLCHECK4()
        }
        else
        {
          glTexImage1D( GL_TEXTURE_1D, i, intformat, xsize+2*brd, brd, mipmaps[i]->format(), mipmaps[i]->type(), mipmaps[i]->pixels() ); GLCHECK4()
        }
      }
      break;
    }

    case TD_TEXTURE_2D:
    {

      std::vector< ref<Image> > mipmaps = image->mipmaps();
      mipmaps.insert(mipmaps.begin(), image);
      for(int i=0; i<(int)mipmaps.size(); i++)
      {
        int xsize = mipmaps[i]->width();
        int ysize = mipmaps[i]->height();
        if (usecompressed)
        {
          glCompressedTexImage2D( GL_TEXTURE_2D, i, intformat, xsize+2*brd, ysize+2*brd, brd, mipmaps[i]->requiredMemory(), mipmaps[i]->pixels() ); GLCHECK4()
        }
        else
        {
          glTexImage2D( GL_TEXTURE_2D, i, intformat, xsize+2*brd, ysize+2*brd, brd, mipmaps[i]->format(), mipmaps[i]->type(), mipmaps[i]->pixels() ); GLCHECK4()
        }
      }
      break;
    }

    case TD_TEXTURE_3D:
      CHECK(0);
      break;

    default:
      CHECK(0);
    }
  }
  else // no mipmaps
  {
    switch( dimension() )
    {
      case TD_TEXTURE_1D:
      {
        if (usecompressed)
        {
          glCompressedTexImage1D( GL_TEXTURE_1D, 0, intformat, xsize+2*brd, brd, image->requiredMemory(), image->pixels() ); GLCHECK4()
        }
        else
        {
          glTexImage1D( GL_TEXTURE_1D, 0, intformat, xsize+2*brd, brd, image->format(), image->type(), image->pixels() ); GLCHECK4()
        }
      }
      break;
      case TD_TEXTURE_2D:
      {
        if (usecompressed)
        {
          glCompressedTexImage2D( GL_TEXTURE_2D, 0, intformat, xsize+2*brd, ysize+2*brd, brd, image->requiredMemory(), image->pixels() ); GLCHECK4()
        }
        else
        {
          glTexImage2D( GL_TEXTURE_2D, 0, intformat, xsize+2*brd, ysize+2*brd, brd, image->format(), image->type(), image->pixels() ); GLCHECK4()
        }
      }
      break;
      case TD_TEXTURE_3D:
      {
        CHECK(0)
      }
      break;

      default:
        CHECK(0);
    }
  }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 4); GLCHECK4()
}

bool Texture::isCompressedFormat(ETextureFormat format)
{
  GLint comp[] =
  {
    TF_COMPRESSED_ALPHA_ARB,
    TF_COMPRESSED_INTENSITY_ARB,
    TF_COMPRESSED_LUMINANCE_ARB,
    TF_COMPRESSED_LUMINANCE_ALPHA_ARB,
    TF_COMPRESSED_RGB_ARB,
    TF_COMPRESSED_RGBA_ARB,

    TF_COMPRESSED_RGB_FXT1_3DFX,
    TF_COMPRESSED_RGBA_FXT1_3DFX,

    TF_COMPRESSED_RGB_S3TC_DXT1_EXT,
    TF_COMPRESSED_RGBA_S3TC_DXT1_EXT,
    TF_COMPRESSED_RGBA_S3TC_DXT3_EXT,
    TF_COMPRESSED_RGBA_S3TC_DXT5_EXT,

    TF_COMPRESSED_LUMINANCE_LATC1_EXT,
    TF_COMPRESSED_SIGNED_LUMINANCE_LATC1_EXT,
    TF_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT,
    TF_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_EXT,

    TF_COMPRESSED_RED_RGTC1_EXT,
    TF_COMPRESSED_SIGNED_RED_RGTC1_EXT,
    TF_COMPRESSED_RED_GREEN_RGTC2_EXT,
    TF_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT,
    0
  };

  for(int i=0; comp[i]; i++)
  {
    if(comp[i] == format)
      return true;
  }

  return false;
}
