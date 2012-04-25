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

#ifndef Texture_INCUDE_DEFINE
#define Texture_INCUDE_DEFINE

#include "vl/Object.hpp"
#include "vl/OpenGL.hpp"
#include "vl/enums.hpp"
namespace vl
{
  class Image;

  class Texture: public Object
  {
  public:
    Texture(Image* image, ETextureFormat intformat = TF_RGBA, EHasMipmaps mipmaps_on = MIPMAPS_ON, EHasBorder border=BORDER_OFF);
    Texture(int width, ETextureFormat intformat = TF_RGBA, EHasBorder border=BORDER_OFF);
    Texture(int width, int height, ETextureFormat intformat = TF_RGBA, EHasBorder border=BORDER_OFF);
    Texture(int width, int height, int depth, ETextureFormat intformat = TF_RGBA, EHasBorder border=BORDER_OFF);
    Texture();
    virtual ~Texture();

    void createTexture(Image* image, ETextureFormat intformat, EHasMipmaps mipmaps_on, EHasBorder border=BORDER_OFF);
    void createCubemap(Image* cubemap, ETextureFormat intformat, EHasMipmaps mipmaps_on, EHasBorder border=BORDER_OFF);

    bool isValid();
    bool hasMipMaps() const { return mHasMipMaps; }

    void setId(GLuint id) { mHandle = id; }
    void setDimension(ETextureDimension dimension) { mDimension = dimension; }
    void setWidth(int x) { mWidth = x; }
    void setHeight(int y) { mHeight = y; }
    void setDepth(int z) { mDepth = z; }
    void setBorder(EHasBorder border) { mBorder = border; }
    void setInternalFormat(ETextureFormat format) { mInternalFormat = format; }
    void setName( const std::string& name ) { mName = name; }
    const std::string& name() const { return mName; }

    GLuint handle() const { return mHandle; }
    ETextureDimension dimension() const { return mDimension; }
    int width() const { return mWidth; }
    int height() const { return mHeight; }
    int depth() const { return mDepth; }
    EHasBorder border() const { return mBorder; }
    ETextureFormat internalFormat() const { return mInternalFormat; }

    static bool supports(Image* image, ETextureFormat intformat, EHasBorder border);
    static bool isCompressedFormat(ETextureFormat format);

  private:
    Texture(const Texture& other): Object(other) {}
    void operator=(const Texture&) {}

  protected:
    GLuint mHandle;
    ETextureDimension mDimension;
    int mWidth;
    int mHeight;
    int mDepth;
    EHasBorder mBorder;
    ETextureFormat mInternalFormat;
    bool mHasMipMaps;
  };
}

#endif
