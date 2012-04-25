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

#ifndef GLCopyTexSubImage_INCLUDE_DEFINE
#define GLCopyTexSubImage_INCLUDE_DEFINE

#include "vl/Camera.hpp"
#include "vl/Texture.hpp"

namespace vl
{

  class CopyTexSubImage: public RenderFinishedCallback
  {
  public:
    CopyTexSubImage(): mReadBuffer(RDB_BACK_LEFT) {}

    EReadDrawBuffer drawBuffer() const { return mReadBuffer; }
    void setReadBuffer(EReadDrawBuffer draw_buffer) { mReadBuffer = draw_buffer; }

  protected:
    EReadDrawBuffer mReadBuffer;
  };

  class GLCopyTexSubImage1D: public CopyTexSubImage
  {
  public:
    GLCopyTexSubImage1D(int level, int xoffset, int x, int y, int width, Texture* texture=NULL, EReadDrawBuffer read_buffer=RDB_BACK_LEFT)
    {
      mLevel = level;
      mXOffset = xoffset;
      mX = x;
      mY = y;
      mWidth  = width;
      mTexture = texture;
      setReadBuffer(read_buffer);
    }

    void setTexture(Texture* tex) { mTexture = tex; }
    void setLevel(int level) { mLevel = level; }
    void setXOffset(int xoffset) { mXOffset = xoffset; }
    void setX(int x) { mX = x; }
    void setY(int y) { mY = y; }
    void setWidth(int width) { mWidth = width; }

    Texture* texture() { return mTexture.get(); }
    const Texture* texture() const { return mTexture.get(); }
    int level() const { return mLevel; }
    int xoffset() const { return mXOffset; }
    int x() const { return mX; }
    int y() const { return mY; }
    int width() const { return mWidth; }

    virtual void renderFinished(const Camera*)
    {
      GLCHECK4()
      CHECK(texture()->dimension() == TD_TEXTURE_1D)

      glDrawBuffer(drawBuffer()); GLCHECK4()
      glBindTexture(TD_TEXTURE_1D, texture()->handle() ); GLCHECK4()
      glCopyTexSubImage1D(TD_TEXTURE_1D, level(), xoffset(), x(), y(), width()); GLCHECK4()
      glBindTexture(TD_TEXTURE_1D, 0 ); GLCHECK4()
    }

  protected:
    ref<Texture> mTexture;
    int mLevel;
    int mXOffset;
    int mX;
    int mY;
    int mWidth;
  };

  class GLCopyTexSubImage2D: public CopyTexSubImage
  {
  public:

    GLCopyTexSubImage2D(int level, int xoffset, int yoffset, int x, int y, int width, int height, Texture* texture=NULL, ECopyTex2D target=CT2D_TEXTURE_2D, EReadDrawBuffer read_buffer=RDB_BACK_LEFT)
    {
      mLevel = level;
      mXOffset = xoffset;
      mYOffset = yoffset;
      mX = x;
      mY = y;
      mWidth  = width;
      mHeight = height;
      mTexture = texture;
      mTarget = target;
      setReadBuffer(read_buffer);
    }

    void setTexture(Texture* tex) { mTexture = tex; }
    void setLevel(int level) { mLevel = level; }
    void setXOffset(int xoffset) { mXOffset = xoffset; }
    void setYOffset(int yoffset) { mYOffset = yoffset; }
    void setX(int x) { mX = x; }
    void setY(int y) { mY = y; }
    void setWidth(int width) { mWidth = width; }
    void setHeight(int height) { mHeight = height; }
    void setTarget(ECopyTex2D target) { mTarget = target; }

    Texture* texture() { return mTexture.get(); }
    const Texture* texture() const { return mTexture.get(); }
    int level() const { return mLevel; }
    int xoffset() const { return mXOffset; }
    int yoffset() const { return mYOffset; }
    int x() const { return mX; }
    int y() const { return mY; }
    int width() const { return mWidth; }
    int height() const { return mHeight; }
    ECopyTex2D target() const { return mTarget; }

    virtual void renderFinished(const Camera*)
    {
      GLCHECK4()
      CHECK( (texture()->dimension() == TD_TEXTURE_CUBE_MAP && target() != CT2D_TEXTURE_2D) ||
             (texture()->dimension() == TD_TEXTURE_2D       && target() == CT2D_TEXTURE_2D)  )

      glDrawBuffer(drawBuffer()); GLCHECK4()
      glBindTexture(target() == CT2D_TEXTURE_2D?GL_TEXTURE_2D:GL_TEXTURE_CUBE_MAP, texture()->handle() ); GLCHECK4()
      glCopyTexSubImage2D(target(), level(), xoffset(), yoffset(), x(), y(), width(), height()); GLCHECK4()
      glBindTexture(target() == CT2D_TEXTURE_2D?GL_TEXTURE_2D:GL_TEXTURE_CUBE_MAP, 0 ); GLCHECK4()
    }

  protected:
    ref<Texture> mTexture;
    int mLevel;
    int mXOffset;
    int mYOffset;
    int mX;
    int mY;
    int mWidth;
    int mHeight;
    ECopyTex2D mTarget;
  };

  class GLCopyTexSubImage3D: public CopyTexSubImage
  {
  public:
    GLCopyTexSubImage3D(int level, int xoffset, int yoffset, int zoffset, int x, int y, int width, int height, Texture* texture, EReadDrawBuffer read_buffer=RDB_BACK_LEFT)
    {
      mLevel = level;
      mXOffset = xoffset;
      mYOffset = yoffset;
      mZOffset = zoffset;
      mX = x;
      mY = y;
      mWidth  = width;
      mHeight = height;
      mTexture = texture;
      setReadBuffer(read_buffer);
    }

    void setTexture(Texture* tex) { mTexture = tex; }
    void setLevel(int level) { mLevel = level; }
    void setXOffset(int xoffset) { mXOffset = xoffset; }
    void setYOffset(int yoffset) { mYOffset = yoffset; }
    void setZOffset(int zoffset) { mZOffset = zoffset; }
    void setX(int x) { mX = x; }
    void setY(int y) { mY = y; }
    void setWidth(int width) { mWidth = width; }
    void setHeight(int height) { mHeight = height; }

    Texture* texture() { return mTexture.get(); }
    const Texture* texture() const { return mTexture.get(); }
    int level() const { return mLevel; }
    int xoffset() const { return mXOffset; }
    int yoffset() const { return mYOffset; }
    int zoffset() const { return mZOffset; }
    int x() const { return mX; }
    int y() const { return mY; }
    int width() const { return mWidth; }
    int height() const { return mHeight; }

    virtual void renderFinished(const Camera*)
    {
      if (GLEW_VERSION_1_2)
      {
        GLCHECK4()
        CHECK( texture()->dimension() == TD_TEXTURE_3D )
        glDrawBuffer(drawBuffer()); GLCHECK4()
        glBindTexture(texture()->dimension(), texture()->handle() ); GLCHECK4()
        glCopyTexSubImage3D(texture()->dimension(), level(), xoffset(), yoffset(), zoffset(), x(), y(), width(), height()); GLCHECK4()
        glBindTexture(texture()->dimension(), 0 ); GLCHECK4()
      }
    }

  protected:
    ref<Texture> mTexture;
    int mLevel;
    int mXOffset;
    int mYOffset;
    int mZOffset;
    int mX;
    int mY;
    int mWidth;
    int mHeight;
  };

}

#endif
