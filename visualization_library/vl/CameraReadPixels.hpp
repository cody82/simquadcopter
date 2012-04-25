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

#ifndef CameraReadPixels_INCLUDE_DEFINE
#define CameraReadPixels_INCLUDE_DEFINE

#include "vl/Camera.hpp"

namespace vl
{

  class CameraReadPixels: public RenderFinishedCallback
  {
  public:
    CameraReadPixels():
      mX ( 0 ),  
      mY ( 0 ),  
      mWidth ( 0 ),  
      mHeight ( 0 ),  
      mReadBuffer ( RDB_BACK_LEFT ) {}

    CameraReadPixels(int x, int y, int width, int height, EReadDrawBuffer read_buffer, Image* image):
      mX ( x ),  
      mY ( y ),  
      mWidth  ( width ),  
      mHeight ( height ),  
      mReadBuffer ( read_buffer ),  
      mImage ( image ) {}

    virtual void renderFinished(const Camera*)
    {
      if (mImage == NULL)
        mImage = new Image;
      mImage->readPixels(mX, mY, mWidth, mHeight, mReadBuffer);
      if ( savePath().length() )
      {
        if ( strstr(savePath().c_str(), ".TGA") || strstr(savePath().c_str(), ".tga") )
          mImage->saveTGA(savePath());
        else
        if ( strstr(savePath().c_str(), ".TIF") || strstr(savePath().c_str(), ".tif") )
          mImage->saveTIFF(savePath());

        else
        {
          Log::error( Say("CameraReadPixels: unknown format for file: '%s'\n") << savePath() );
        }
      }
    }

    void setup(int x, int y, int width, int height, EReadDrawBuffer read_buffer)
    {
      mX = x;
      mY = y;
      mWidth  = width;
      mHeight = height;
      mReadBuffer = read_buffer;
    }

    int x() const { return mX; }
    int y() const { return mY; }
    int width() const { return mWidth; }
    int height() const { return mHeight; }
    EReadDrawBuffer readBuffer() const { return mReadBuffer; }
    Image* image() { return mImage.get(); }
    const Image* image() const { return mImage.get(); }
    const std::string& savePath() const { return mSavePath; }

    void setX(int x) { mX = x; }
    void setY(int y) { mY = y; }
    void setWidth(int width) { mWidth = width; }
    void setHeight(int height) { mHeight = height; }
    void setReadBuffer(EReadDrawBuffer buffer) { mReadBuffer = buffer; }
    void setImage(Image* image) { mImage = image; }
    void setSavePath(const std::string& path) { mSavePath = path; }

  protected:
    int mX;
    int mY;
    int mWidth;
    int mHeight;
    EReadDrawBuffer mReadBuffer;
    ref<Image> mImage;
    std::string mSavePath;
  };
}

#endif
