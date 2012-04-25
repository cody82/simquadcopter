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

#ifndef Image_INCUDE_DEFINE
#define Image_INCUDE_DEFINE

#include "vl/Object.hpp"
#include "vl/OpenGL.hpp"
#include "vl/enums.hpp"
#include <string>
#include <vector>

namespace vl
{

  class Image: public Object
  {
    Image(const Image& img): Object(img) {CHECK(0)}
  public:
    Image();
    Image(const std::string& path);
    Image(int x, int y, int z, int bytealign, EImageFormat format, EImageType type);
    virtual ~Image() { clear(); }
    void operator=(const Image& other);
    bool isCubemap() const;
    bool isValid() const;
    EImageDimension dimension() const;
    void allocate1D(int x, EImageFormat format, EImageType type);
    void allocate2D(int x, int y, int bytealign, EImageFormat format, EImageType type);
    void allocate3D(int x, int y, int z, int bytealign, EImageFormat format, EImageType type);
    void allocateCubemap(int x, int y, int bytealign, EImageFormat format, EImageType type);
    void clear();
    int byteAlignment() const;
    void setByteAlignment(int bytealign);
    int bitsPerPixel() const;
    int requiredMemory() const;
    std::string print() const;
    std::string printType() const;
    std::string printFormat() const;

    void setWidth(int x) { mWidth = x; updatePitch(); }
    void setHeight(int y) { mHeight = y; }
    void setDepth(int z) { mDepth = z; }
    void setFormat(EImageFormat format) { mFormat = format; updatePitch(); }
    void setType(EImageType type) { mType=type; updatePitch(); }

    const GLubyte* pixels() const { if (mPixels.size()) return &mPixels[0]; else return NULL; }
    GLubyte* pixels() { if (mPixels.size()) return &mPixels[0]; else return NULL; }
    bool empty() { return pixels() == NULL; }
    GLubyte* pixelsZSlice(int slice);
    GLubyte* pixelsXP();
    GLubyte* pixelsXN();
    GLubyte* pixelsYP();
    GLubyte* pixelsYN();
    GLubyte* pixelsZP();
    GLubyte* pixelsZN();
    void setMipmaps(const std::vector< ref<Image> >& mipmaps) { mMipmaps = mipmaps; };
    const std::vector< ref<Image> >& mipmaps() const { return mMipmaps; };
    void clearMipmaps() { mMipmaps.clear(); }
    void setName(const std::string& name) { mName = name; }
    const std::string& name() const { return mName; }
    int width() const { return mWidth; }
    int height() const { return mHeight; }
    int depth() const { return mDepth; }
    int pitch() const { return mPitch; }
    EImageFormat format() const { return mFormat; }
    EImageType type() const { return mType; }
    int alphaBits() const;

    static int requiredMemory(int xsize, int ysize, int zsize, int bytealign, EImageFormat format, EImageType type);
    static int isCompressedFormat(EImageFormat fmt);

    void readPixels(int x, int y, int width, int height, EReadDrawBuffer read_buffer);

    void flipVertically();

    Image* createCubemap(Image* xp, Image* xn, Image* yp, Image* yn, Image* zp, Image* zn);

    bool saveTIFF(const std::string& file);
    bool saveTGA(const std::string& file);

  protected:
    void updatePitch();

  protected:
    std::vector<GLubyte> mPixels;
    std::vector< ref<Image> > mMipmaps;
    int mWidth;
    int mHeight;
    int mDepth;
    int mPitch;
    int mByteAlign;
    EImageFormat mFormat;
    EImageType mType;
    bool mIsCubemap;
  };

  bool isFormatBMP(const std::string& file);
  bool isFormatTGA(const std::string& file);
  bool isFormatTIFF(const std::string& file);
  bool isFormatDDS(const std::string& file);

  ref<Image> loadImage(const std::string& file);
  ref<Image> loadBMP(const std::string& file);
  ref<Image> loadTGA(const std::string& file);
  ref<Image> loadTIFF(const std::string& file);
  ref<Image> loadDDS(const std::string& file);
  ref<Image> loadAsCubemap(std::string xp_file, std::string xn_file, std::string yp_file, std::string yn_file, std::string zp_file, std::string zn_file);
}

#endif
