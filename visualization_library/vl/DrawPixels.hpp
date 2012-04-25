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

#ifndef DrawPixels_INCLUDE_DEFINE
#define DrawPixels_INCLUDE_DEFINE

#include "vl/Drawable.hpp"
#include "vl/Image.hpp"
#include "vl/ivec2.hpp"

namespace vl
{

  class DrawPixels: public Drawable
  {
  public:

    class Draw: public Object
    {
      friend class DrawPixels;

    public:
      Draw();
      Draw(Image* img, int scrx, int scry, int startx, int starty, int width=-1, int height=-1, int alignment = AlignBottom | AlignLeft);
      Draw(const Draw& other);
      void operator=(const Draw& other) ;
      ~Draw();

      GLuint pboHandle() const { return mPBOHandle; }

      int align() const { return mAlign; }
      void setAlign(int align) { mAlign = align; }

    protected:
      ref<Image> mImage;
      ivec2 mPos;
      ivec2 mStart;
      ivec2 mSize;
      GLuint mPBOHandle;
      int mAlign;

      int mWidth;
      int mByteAlignment;
      EImageType   mType;
      EImageFormat mFormat;
    };

    DrawPixels() ;

    void computeAABB() {}

    void draw(Actor* actor, int render_stream, unsigned int);

    void addDraw(Draw* cmd) { mDraws.push_back(cmd); }

    void removeAllDraws() { mDraws.clear(); }

    Draw* draw(int i) { return mDraws[i].get(); }
    const Draw* draw(int i) const { return mDraws[i].get(); }

    int drawCount() const { return (int)mDraws.size(); }

    void destroyAllPixelBufferObjects();

    void releaseImages();

    bool generatePixelBufferObjects();

    void setUsePixelBufferObject(bool use_pbo) ;

    bool usePixelBufferObject() const { return mUsePixelBufferObject; }

  protected:
    std::vector< ref<Draw> > mDraws;
    bool mUsePixelBufferObject;
  };

}

#endif
