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

#include "vl/DrawPixels.hpp"
#include "vl/Actor.hpp"
#include "vl/GlobalState.hpp"
#include <map>

using namespace vl;

DrawPixels::Draw::Draw()
{
  mAlign = AlignBottom | AlignLeft;
  mPBOHandle = 0;
  mWidth = 0;
  mByteAlignment = 0;
  mType   = IT_UNSIGNED_BYTE;
  mFormat = IF_RGBA;
}

DrawPixels::Draw::Draw(Image* img, int scrx, int scry, int startx, int starty, int width, int height, int alignment)
{
  mAlign = alignment;

  if (width < 0)
    width = img->width()  - startx;

  if (height < 0)
    height = img->height() - starty;

  mImage     = img;
  mPos       = ivec2(scrx, scry);
  mStart     = ivec2(startx, starty);
  mSize      = ivec2(width, height);

  mPBOHandle = 0;

  mWidth = 0;
  mByteAlignment = 0;
  mType   = IT_UNSIGNED_BYTE;
  mFormat = IF_RGBA;
}

DrawPixels::Draw::Draw(const Draw& other): Object(other)
{
  *this = other;
}

void DrawPixels::Draw::operator=(const Draw& other) 
{
  mImage = other.mImage;
  mPos   = other.mPos;
  mStart = other.mStart;
  mSize  = other.mSize;
  mAlign = other.mAlign;
  if (mPBOHandle)
    glDeleteBuffers(1, &mPBOHandle); mPBOHandle = 0; GLCHECK4()

  mPBOHandle = 0;
  mWidth = 0;
  mByteAlignment = 0;
  mType   = IT_UNSIGNED_BYTE;
  mFormat = IF_RGBA;
}

DrawPixels::Draw::~Draw()
{
  if (mPBOHandle)
  {
    glDeleteBuffers(1, &mPBOHandle);  GLCHECK4()
  }
  mPBOHandle = 0; 
}

DrawPixels::DrawPixels() 
{
  mUsePixelBufferObject = false;
}

void DrawPixels::draw(Actor* actor, int render_stream, unsigned int)
{
  GLCHECK4()

  GLint viewport[] = {0,0,0,0};
  glGetIntegerv(GL_VIEWPORT, viewport);
  GLCHECK4()

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  GLCHECK4();

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  gluOrtho2D( 0, viewport[2], 0, viewport[3] ); GLCHECK4();
  glTranslatef(0.4f,0.4f,0); GLCHECK4();

  glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT); GLCHECK4();

  for(int i=0; i<(int)mDraws.size(); i++)
  {
    const ref<Draw>& cmd = mDraws[i];

    if (cmd->mPBOHandle == 0 && cmd->mImage == 0)
      continue;

    CHECK( cmd->mPBOHandle || (cmd->mImage && cmd->mImage->pixels() ) )
    CHECK( cmd->mPBOHandle || (cmd->mImage && cmd->mImage->isValid()) )
    CHECK( cmd->mImage || cmd->mPBOHandle )
    CHECK( cmd->mStart.x() >= 0 )
    CHECK( cmd->mStart.y() >= 0 )

    int pos_x = cmd->mPos.x();
    int pos_y = cmd->mPos.y();

    if (cmd->align() & AlignRight)
      pos_x -= cmd->mSize.x() -1;
    if (cmd->align() & AlignHCenter)
      pos_x -= cmd->mSize.x() / 2;
    if (cmd->align() & AlignTop)
      pos_y -= cmd->mSize.y() -1;
    if (cmd->align() & AlignVCenter)
      pos_y -= cmd->mSize.y() / 2;

    if ( actor->transform() )
    {
      vec4d v(0,0,0,1);
      v = actor->transform()->localToWorldMatrix( render_stream ) * v;

      GlobalState::renderStream( render_stream )->currentCamera()->project(v,v);

      v.x() -= viewport[0];
      v.y() -= viewport[1];

      pos_x += int(v.x() + 0.5);
      pos_y += int(v.y() + 0.5);
    }

    if ( pos_x + cmd->mSize.x() -1 < 0 )
      continue;

    if ( pos_y + cmd->mSize.y() -1 < 0 )
      continue;

    if (pos_x > viewport[2] - 1)
      continue;

    if (pos_y > viewport[3] - 1)
      continue;

    if (cmd->mImage && !mUsePixelBufferObject)
    {
      cmd->mWidth         = cmd->mImage->width();
      cmd->mByteAlignment = cmd->mImage->byteAlignment();
      cmd->mType          = cmd->mImage->type();
      cmd->mFormat        = cmd->mImage->format();
    }

    int clip_left   = pos_x < 0 ? -pos_x : 0;
    int clip_bottom = pos_y < 0 ? -pos_y : 0;
    int clip_right  = (pos_x+cmd->mSize.x()-1)-( viewport[2]-1 );
    int clip_top    = (pos_y+cmd->mSize.y()-1)-( viewport[3]-1 );

    if (clip_right < 0)
      clip_right = 0;

    if (clip_top < 0)
      clip_top = 0;

    glRasterPos2i( pos_x + clip_left, pos_y + clip_bottom);

    GLCHECK4()

    glPixelStorei( GL_UNPACK_ALIGNMENT, cmd->mByteAlignment );
    glPixelStorei( GL_UNPACK_ROW_LENGTH, cmd->mWidth );
    glPixelStorei( GL_UNPACK_SKIP_PIXELS, cmd->mStart.x() + clip_left );
    glPixelStorei( GL_UNPACK_SKIP_ROWS, cmd->mStart.y() + clip_bottom );
    GLCHECK4()

    if ( cmd->mPBOHandle )
    {
      glBindBuffer( GL_PIXEL_UNPACK_BUFFER, cmd->mPBOHandle ); GLCHECK4()
      glDrawPixels( cmd->mSize.x() -clip_left -clip_right, cmd->mSize.y() -clip_bottom -clip_top, cmd->mFormat, cmd->mType, 0 ); GLCHECK4();
    }
    else
    {
      if ( (GLEW_ARB_pixel_buffer_object || GLEW_EXT_pixel_buffer_object) )
        glBindBuffer( GL_PIXEL_UNPACK_BUFFER, 0 );
      glDrawPixels( cmd->mSize.x() -clip_left -clip_right, cmd->mSize.y() -clip_bottom -clip_top, cmd->mFormat, cmd->mType, cmd->mImage->pixels() ); GLCHECK4();
    }
  }

  GLCHECK4();

  if ( (GLEW_ARB_pixel_buffer_object || GLEW_EXT_pixel_buffer_object) )
    glBindBuffer( GL_PIXEL_UNPACK_BUFFER, 0 );

  GLCHECK4()

  glPopClientAttrib();

  GLCHECK4();

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix(); GLCHECK4()

  glMatrixMode(GL_PROJECTION);
  glPopMatrix(); GLCHECK4()
}

void DrawPixels::destroyAllPixelBufferObjects()
{
  for(int i=0; i<(int)mDraws.size(); i++)
  {
    glDeleteBuffers(1, &mDraws[i]->mPBOHandle); GLCHECK4()
    mDraws[i]->mPBOHandle = 0;
  }
}

void DrawPixels::releaseImages()
{
  for(int i=0; i<(int)mDraws.size(); i++)
  {
    mDraws[i]->mImage = NULL;
  }
}

bool DrawPixels::generatePixelBufferObjects()
{
  if ( !( GLEW_ARB_pixel_buffer_object || GLEW_EXT_pixel_buffer_object ) )
    return false;

  std::map< ref<Image>, GLuint> pbomap;

  for(int i=0; i<(int)mDraws.size(); i++)
  {
    if ( mDraws[i]->mPBOHandle )
      continue;

    if ( mDraws[i]->mImage == NULL )
      continue;

    GLuint pbo_handle = pbomap[mDraws[i]->mImage];
    if ( pbo_handle == 0 )
    {
      glGenBuffers(1, &mDraws[i]->mPBOHandle); GLCHECK4()
      glBindBuffer( GL_PIXEL_UNPACK_BUFFER, mDraws[i]->mPBOHandle ); GLCHECK4()
      glBufferData( GL_PIXEL_UNPACK_BUFFER, mDraws[i]->mImage->requiredMemory(), mDraws[i]->mImage->pixels(), GL_STATIC_DRAW ); GLCHECK4()
      glBindBuffer( GL_PIXEL_UNPACK_BUFFER, 0 ); GLCHECK4()
      pbo_handle = mDraws[i]->mPBOHandle;
    }
    mDraws[i]->mPBOHandle = pbo_handle;
    pbomap[mDraws[i]->mImage] = pbo_handle;

    mDraws[i]->mWidth         = mDraws[i]->mImage->width();
    mDraws[i]->mByteAlignment = mDraws[i]->mImage->byteAlignment();
    mDraws[i]->mType          = mDraws[i]->mImage->type();
    mDraws[i]->mFormat        = mDraws[i]->mImage->format();
  }
  return true;
}

void DrawPixels::setUsePixelBufferObject(bool use_pbo) 
{ 
  if ( (GLEW_ARB_pixel_buffer_object || GLEW_EXT_pixel_buffer_object) )
    mUsePixelBufferObject = use_pbo; 
  else
    mUsePixelBufferObject = false;
}

