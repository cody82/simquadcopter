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

#include "vl/Text.hpp"
#include "vl/GlobalState.hpp"
#include "vl/Image.hpp"
#include "vl/Say.hpp"
#include "vl/Log.hpp"
#include "vl/Actor.hpp"

#include "ft2build.h"
#include FT_FREETYPE_H

using namespace vl;

#undef __FTERRORS_H__                                           
#define FT_ERRORDEF( e, v, s )  { e, s },                       
#define FT_ERROR_START_LIST     {                               
#define FT_ERROR_END_LIST       { 0, 0 } };                     
                                                               
const struct                                                    
{                                                               
 int          err_code;                                        
 const char*  err_msg;                                         
} ft_errors[] =
                                                               
#include FT_ERRORS_H                                            

const char* get_ft_error_message(int error)
{
  int i=0;
  while( ft_errors[i].err_msg && ft_errors[i].err_code != error )
    i++;
  return ft_errors[i].err_msg;
}
  
namespace vl
{
  extern FT_Library FreeTypeLibrary;
}

Glyph::~Glyph()
{
  if (mTextureHandle)
  {
    glDeleteTextures(1, &mTextureHandle);
    mTextureHandle = 0;
  }
}

Font::Font()
{
  mHeight = 0;
  mFT_Face = NULL;
  mSmooth = false;
  setSize(14);
}

Font::Font(const std::string& font_file, int size ) 
{
  mHeight = 0;
  mFT_Face = NULL;
  mSmooth = false;
  setFontFile(font_file);
  setSize(size);
}

Font::~Font()
{
  if (mFT_Face)
  {
    FT_Done_Face(mFT_Face); mFT_Face = NULL;
  }
}

void Font::operator=(const Font& other)
{
  if (mFT_Face)
  {
    FT_Done_Face(mFT_Face); mFT_Face = NULL;
  }
  mGlyphMap.clear();
  mFontFile = other.mFontFile;
  mSize = other.mSize;
  mHeight = other.mHeight;
}

void Font::setSize(int size) 
{ 
  if(mSize != size)
  {
    mSize = size ; 

    mGlyphMap.clear();

  }
}
void Font::setFontFile(const std::string& font_file) 
{ 
  if(font_file == mFontFile)
    return;

  mFontFile = font_file; 

  mGlyphMap.clear();

  if (mFT_Face)
  {
    FT_Done_Face(mFT_Face); 
    mFT_Face = NULL;
  }

  FT_Error error = 0;

  if (mFT_Face == NULL)
  {
    error = FT_New_Face( FreeTypeLibrary, 
                         mFontFile.c_str(), 
                         0, 
                         &mFT_Face );
  }

  if (error)
  { 
    Log::error(Say("FT_New_Face error (%s): %s\n") << font_file << get_ft_error_message(error) );
    CHECK(0);
    return;
  }
}

Glyph* Font::glyph(int character)
{
  ref<Glyph>& glyph = mGlyphMap[character];

  if (glyph == NULL)
  {
    glyph = new Glyph;
    glyph->setFont(this);

    FT_Error error = 0;

    error = FT_Set_Char_Size( 
              mFT_Face, /* handle to face object           */
              0,        /* char_width in 1/64th of points  */  
              mSize*64, /* char_height in 1/64th of points */  
              96,       /* horizontal device resolution    */  
              96 );     /* vertical device resolution      */

    if(error)
    {

      if ( (mFT_Face->face_flags & FT_FACE_FLAG_SCALABLE) == 0 && mFT_Face->num_fixed_sizes)
      {

        int best_match_index = -1;
        int best_match_size  = 0;
        for( int i=0; i < mFT_Face->num_fixed_sizes; i++ )
        {
          int size = mFT_Face->available_sizes[i].y_ppem/64;

          if (size <= mSize)
          {
            if (best_match_index == -1 || (mSize - size) < (mSize - best_match_size) )
            {
              best_match_index = i;
              best_match_size  = size;
            }
          }
        }

        if (best_match_index == -1)
          best_match_index = 0;

        error = FT_Select_Size(mFT_Face, best_match_index);
        if (error)
          Log::error(Say("FT_Select_Size error (%s): %s\n") << fontFile() << get_ft_error_message(error) );
        CHECK(!error)
      }

      {
        Log::error(Say("FT_Set_Char_Size error (%s): %s\n") << fontFile() << get_ft_error_message(error) );
        CHECK(0);
        return glyph.get();
      }
    }

    mHeight = mFT_Face->size->metrics.height / 64.0f;

    error = FT_Load_Char( mFT_Face, character, FT_LOAD_DEFAULT );

    if(error)
    {
      Log::error(Say("FT_Load_Char error (%s): %s\n") << fontFile() << get_ft_error_message(error) );
      CHECK(0);
      glyph = NULL;
      return glyph.get();
    }

    glyph->setGlyphIndex( FT_Get_Char_Index( mFT_Face, character ) );

    error = FT_Render_Glyph( 
              mFT_Face->glyph,  /* glyph slot */  
              FT_RENDER_MODE_NORMAL ); /* render mode: FT_RENDER_MODE_MONO or FT_RENDER_MODE_NORMAL */ 

    if(error)
    {

      error = FT_Load_Glyph( 
                mFT_Face,/* handle to face object */  
                0,       /* glyph index           */  
                FT_LOAD_DEFAULT ); /* load flags, see below */
      glyph->setGlyphIndex(0);

      error = FT_Render_Glyph(
                mFT_Face->glyph,  /* glyph slot */  
                FT_RENDER_MODE_NORMAL ); /* render mode: FT_RENDER_MODE_MONO or FT_RENDER_MODE_NORMAL */ 
    }

    if(error)
    {
      Log::error(Say("FT_Render_Glyph error (%s): %s\n") << fontFile() << get_ft_error_message(error) );
      CHECK(0);
      glyph = NULL;
      return glyph.get();
    }

    bool ok_format = mFT_Face->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY || mFT_Face->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_MONO;
    ok_format &= mFT_Face->glyph->bitmap.palette_mode == 0;
    ok_format &= mFT_Face->glyph->bitmap.pitch > 0 || mFT_Face->glyph->bitmap.buffer == NULL;

    if (!ok_format)
    {
      Log::error( Say("Font::glyph() error (%s): glyph format not supported. Visualization Library currently supports only FT_PIXEL_MODE_GRAY and FT_PIXEL_MODE_MONO.\n") << fontFile() );
      CHECK(0);
      return glyph.get();
    }

    if ( mFT_Face->glyph->bitmap.buffer )
    {
      if (mHeight == 0)
        mHeight = (float)mFT_Face->glyph->bitmap.rows;

      glyph->setWidth ( mFT_Face->glyph->bitmap.width);
      glyph->setHeight( mFT_Face->glyph->bitmap.rows);
      glyph->setLeft  ( mFT_Face->glyph->bitmap_left);
      glyph->setTop   ( mFT_Face->glyph->bitmap_top);

      CHECK( mFT_Face->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY || mFT_Face->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_MONO )
      CHECK( mFT_Face->glyph->bitmap.palette_mode == 0 )
      CHECK( mFT_Face->glyph->bitmap.pitch > 0 )

      GLuint texhdl;
      glGenTextures( 1, &texhdl );
      glyph->setTextureHandle(texhdl);
      glBindTexture( GL_TEXTURE_2D, glyph->textureHandle() );

      int texsize[] = { 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 0 };
      int max_tex_size = 0;
      glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);

      int w=0, h=0, margin = 1;

      for(int i=0; texsize[i]; i++)
      {
        if ( (texsize[i] >= glyph->width() + margin*2 && texsize[i] >= glyph->height() + margin*2) || texsize[i+1] > max_tex_size )
        {
          w = texsize[i];
          h = texsize[i];
          break;
        }
      }
      CHECK(w)
      CHECK(h)

#if(1)

      glyph->setS0( 0 );
      glyph->setT0( 1 );
      glyph->setS1((margin*2 + glyph->width() ) /(float)(w-1) );
      glyph->setT1( 1 -(margin*2 + glyph->height() ) /(float)(h-1) );

#else
      glyph->setS0((float)margin /(float)w);
      glyph->setT0( 1 -(float)margin /(float)h);
      glyph->setS1(((float)margin + glyph->width() ) /(float)w);
      glyph->setT1( 1 -((float)margin + glyph->height() ) /(float)h);
#endif

      ref<Image> img = new Image;
      img->allocate2D(w, h, 1, IF_RGBA, IT_UNSIGNED_BYTE);

      for(int byte=0; byte<img->requiredMemory(); byte+=4)
      {
#if (1)
        img->pixels()[byte + 0] = 0xFF;
        img->pixels()[byte + 1] = 0xFF;
        img->pixels()[byte + 2] = 0xFF;
        img->pixels()[byte + 3] = 0x0;
#else
        img->pixels()[byte + 0] = 0x0;
        img->pixels()[byte + 1] = 0x0;
        img->pixels()[byte + 2] = 0x0;
        img->pixels()[byte + 3] = 0xFF;
#endif
      }

      for(int y=0; y<glyph->height(); y++)
      {
        for(int x=0; x<glyph->width(); x++) 
        {
          int offset_1 = (x+margin) * 4 + (w-1-y-margin) * img->pitch();
          int offset_2 = 0;
          if (mFT_Face->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
            offset_2 = x / 8 + y * abs(mFT_Face->glyph->bitmap.pitch);
          else
            offset_2 = x + y * mFT_Face->glyph->bitmap.pitch;

#if (1)
          if (mFT_Face->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
            img->pixels()[ offset_1+3 ] = (mFT_Face->glyph->bitmap.buffer[ offset_2 ] >> (7-x%8)) & 0x1 ? 0xFF : 0x0;
          else
            img->pixels()[ offset_1+3 ] = mFT_Face->glyph->bitmap.buffer[ offset_2 ];
#else

          img->pixels()[ offset_1+0 ] = face->glyph->bitmap.buffer[ offset_2 ]; // 0xFF;
          img->pixels()[ offset_1+1 ] = face->glyph->bitmap.buffer[ offset_2 ]; // 0xFF;
          img->pixels()[ offset_1+2 ] = face->glyph->bitmap.buffer[ offset_2 ]; // 0xFF;
          img->pixels()[ offset_1+3 ] = 0xFF; // face->glyph->bitmap.buffer[ offset_2 ];
#endif
        }
      }

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width(), img->height(), 0, img->format(), img->type(), img->pixels() );

      if ( smooth() )
      {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
      }
      else
      {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
      }

      if (GLEW_EXT_texture_filter_anisotropic)
      {
        GLfloat max_anisotropy;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropy);
      }

      GLCHECK4();
    }

    glyph->setAdvance( vec2( (float)mFT_Face->glyph->advance.x / 64.0f, (float)mFT_Face->glyph->advance.y / 64.0f ) );
  }

  return glyph.get();
}

void Font::setSmooth(bool smooth)
{
  mSmooth = smooth;
  std::map<int, ref<Glyph> >::iterator it = mGlyphMap.begin();
  for( ; it != mGlyphMap.end(); ++it )
  {
    const ref<Glyph>& glyph = it->second;
    if (glyph->textureHandle() == 0)
      continue;

    glBindTexture( GL_TEXTURE_2D, glyph->textureHandle() );

    if (smooth)
    {
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    }
    else
    {
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    }
  }
}

void Text::draw(Actor* actor, int render_stream, unsigned int)
{
  if ( text().empty() )
    return;

  ref<Shader> saved_state = new Shader;
  ref<Shader> my_state    = new Shader;
  *my_state = *saved_state = *GlobalState::renderStream( render_stream )->currentShader();  

  for(int i=0; i<MAX_TEXTURE_UNITS; i++)
    my_state->setTextureUnit(i, NULL);

  my_state->applyShader( render_stream );

  GLboolean depth_mask=0;
  glGetBooleanv(GL_DEPTH_WRITEMASK, &depth_mask);
  GLboolean color_mask[4];
  glGetBooleanv(GL_COLOR_WRITEMASK, color_mask);
  GLint stencil_front_mask=0;
  glGetIntegerv(GL_STENCIL_WRITEMASK, &stencil_front_mask);
  GLint stencil_back_mask=0;
  if (GLEW_VERSION_2_0)
    glGetIntegerv(GL_STENCIL_BACK_WRITEMASK, &stencil_back_mask);

  if ( backgroundEnabled() || borderEnabled() )
  {
    glDepthMask(GL_FALSE);

    glStencilMask(0); 

  }

  if (backgroundEnabled())
    drawBackground( render_stream, actor );

  if (borderEnabled())
    drawBorder( render_stream, actor );

  GLboolean depth_mask2=0;
  glGetBooleanv(GL_DEPTH_WRITEMASK, &depth_mask2);
  glDepthMask(GL_FALSE);

  if (shadowEnabled())
  {
    drawText( render_stream, actor, shadowColor(), shadowVector() );
  }

  if (outlineEnabled())
  {
    drawText( render_stream, actor, outlineColor(), vec2(-1,0) );
    drawText( render_stream, actor, outlineColor(), vec2(+1,0) );
    drawText( render_stream, actor, outlineColor(), vec2(0,-1) );
    drawText( render_stream, actor, outlineColor(), vec2(0,+1) );
  }
  glDepthMask(depth_mask2);

  drawText( render_stream, actor, color(), vec2(0,0) );

  if (backgroundEnabled() || borderEnabled())
  {

    glDepthMask(depth_mask);
    glStencilMask(stencil_front_mask);
    if (GLEW_VERSION_2_0)
      glStencilMaskSeparate(GL_BACK, stencil_back_mask);

    glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);

    if (backgroundEnabled())
      drawBackground( render_stream, actor );

    if (borderEnabled())
      drawBorder( render_stream, actor );

    glColorMask(color_mask[0],color_mask[1],color_mask[2],color_mask[3]);
  }

  GlobalState::renderStream( render_stream )->currentShader()->reset();

  saved_state->applyShader( render_stream );
}

void Text::drawText(int render_stream, Actor* actor, const vec4& color, const vec2& offset)
{
  if(!mFont)
  {
    Log::error("Text::drawText() error: no Font assigned to the Text object.\n");
    CHECK(0);
    return;
  }

  if (!font()->mFT_Face)
  {
    Log::error("Text::drawText() error: invalid FT_Face: probably you tried to load an unsupported font format.\n");
    CHECK(0);
    return;
  }

  GLint viewport[] = {0,0,0,0};
  glGetIntegerv(GL_VIEWPORT, viewport);
  GLCHECK4()

  if (viewport[2] < 1) viewport[2] = 1;
  if (viewport[3] < 1) viewport[3] = 1;

  if (mode() == Text2D)
  {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    GLCHECK4();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); 
    glLoadIdentity();
    gluOrtho2D( -0.5f, viewport[2]-0.5f, -0.5f, viewport[3]-0.5f );
    GLCHECK4();
  }
 
#if (1)
  AABB rbbox = rawBoundingBox( text() ); // for text alignment
  AABB bbox = rbbox; 
  bbox.setMaxCorner( bbox.maxCorner() + vec3d(2*margin(),2*margin(),0) );

	glEnable(GL_TEXTURE_2D);

  vec2 pen(0,0);

  float texc[] = { 0,0,0,0,0,0,0,0 };
  glEnableClientState( GL_TEXTURE_COORD_ARRAY );
  if (GLEW_ARB_multitexture)
    glClientActiveTexture( GL_TEXTURE0 );
  glTexCoordPointer(2, GL_FLOAT, 0, texc);

  vec4 cols[] = { color, color, color, color};
  glEnableClientState( GL_COLOR_ARRAY );
  glColorPointer(4, GL_FLOAT, 0, cols);

  vec3 norms[] = { vec3(0,0,1), vec3(0,0,1), vec3(0,0,1), vec3(0,0,1) };
  glEnableClientState( GL_NORMAL_ARRAY );
  glNormalPointer(GL_FLOAT, 0, norms);

  vec2 vect[4];
  glEnableClientState( GL_VERTEX_ARRAY );
  glVertexPointer(2, GL_FLOAT, 0, vect[0].ptr());

  FT_Long use_kerning = FT_HAS_KERNING( font()->mFT_Face );
  FT_UInt previous = 0;

  CHECK(text().length())

  std::vector< std::wstring > lines;
  lines.push_back( std::wstring() );
  for(unsigned i=0; i<text().length(); i++)
  {
    if (text()[i] == '\n')
    {

      lines.push_back( std::wstring() );
    }
    else
      lines.back().push_back( text()[i] );
  }

  for(unsigned iline=0; iline<lines.size(); iline++)
  {

    if (textAlignment() == TextAlignJustify)
    {

      for(unsigned ch=0, skip=0; ch < lines[iline].length(); ++ch)
      {
        if ( lines[iline][ch] != ' ' && lines[iline][ch] != '\t' )
        {
          lines[iline] = &lines[iline][0] + skip;
          break;
        }
        else
          skip++;
      }

      for(size_t ch=lines[iline].length(), skip=0; ch--; )
      {
        if ( lines[iline][ch] != ' ' && lines[iline][ch] != '\t' )
        {
          lines[iline].resize( lines[iline].length() - skip );
          break;
        }
        else
          skip++;
      }
    }

    AABB linebox = rawBoundingBox( lines[iline] );
    int displace = 0;
    int just_space = 0;
    int just_remained_space = 0;
    int space_count = 0;
    for(int c=0; c<(int)lines[iline].length(); c++)
      if ( lines[iline][c] == ' ' )
        space_count++;

    if (space_count && textAlignment() == TextAlignJustify)
    {
      just_space          = int(rbbox.width() - linebox.width()) / space_count;
      just_remained_space = int(rbbox.width() - linebox.width()) % space_count;
    }

    if (layout() == RightToLeftText)
    {
      if (textAlignment() == TextAlignRight)
        displace = 0;
      else
      if (textAlignment() == TextAlignLeft)
        displace = - int(rbbox.width() - linebox.width());
      else
      if (textAlignment() == TextAlignCenter)
        displace = - int((rbbox.width() - linebox.width()) / 2.0f);
    }
    if (layout() == LeftToRightText)
    {
      if (textAlignment() == TextAlignRight)
        displace = int(rbbox.width() - linebox.width());
      else
      if (textAlignment() == TextAlignLeft)
        displace = 0;
      else
      if (textAlignment() == TextAlignCenter)
        displace = + int((rbbox.width() - linebox.width()) / 2.0f);
    }

    for(int c=0; c<(int)lines[iline].length(); c++)
    {
      if (c == 0 && iline != 0)
      {
        pen.y() -= mFont->mHeight;
        pen.x()  = 0;

      }

      const ref<Glyph>& glyph = mFont->glyph( lines[iline][c] );

      if (!glyph)
        continue;

      if ( kerningEnabled() && use_kerning && previous && glyph->glyphIndex() ) 
      { 
        FT_Vector delta; delta.y = 0;
        if (layout() == LeftToRightText)
        {
          FT_Get_Kerning( font()->mFT_Face, previous, glyph->glyphIndex(), FT_KERNING_DEFAULT, &delta );
          pen.x() += delta.x / 64.0f; 
        }
        else
        if (layout() == RightToLeftText)
        {
          FT_Get_Kerning( font()->mFT_Face, glyph->glyphIndex(), previous, FT_KERNING_DEFAULT, &delta );
          pen.x() -= delta.x / 64.0f; 
        }
        pen.y() += delta.y / 64.0f; 
      }
      previous = glyph->glyphIndex();

      if (glyph->textureHandle())
      {
        glBindTexture( GL_TEXTURE_2D, glyph->textureHandle() );

        texc[0] = glyph->s0();
        texc[1] = glyph->t1();
        texc[2] = glyph->s1();
        texc[3] = glyph->t1();
        texc[4] = glyph->s1();
        texc[5] = glyph->t0();
        texc[6] = glyph->s0();
        texc[7] = glyph->t0();

        int left = layout() == RightToLeftText ? -glyph->left() : +glyph->left();

        vect[0].x() = pen.x() + glyph->width()*0 + left -1;
        vect[0].y() = pen.y() + glyph->height()*0 + glyph->top() - glyph->height() -1;

        vect[1].x() = pen.x() + glyph->width()*1 + left +1; 
        vect[1].y() = pen.y() + glyph->height()*0 + glyph->top() - glyph->height() -1;

        vect[2].x() = pen.x() + glyph->width()*1 + left +1; 
        vect[2].y() = pen.y() + glyph->height()*1 + glyph->top() - glyph->height() +1;

        vect[3].x() = pen.x() + glyph->width()*0 + left -1; 
        vect[3].y() = pen.y() + glyph->height()*1 + glyph->top() - glyph->height() +1;

        if (layout() == RightToLeftText)
        {
  #if (1)
          vect[0].x() -= glyph->width()-1 +2;
          vect[1].x() -= glyph->width()-1 +2;
          vect[2].x() -= glyph->width()-1 +2;
          vect[3].x() -= glyph->width()-1 +2;
  #endif
        }

        vect[0].y() -= mFont->mHeight;
        vect[1].y() -= mFont->mHeight;
        vect[2].y() -= mFont->mHeight;
        vect[3].y() -= mFont->mHeight;

  #if (1)

        vect[0] -= bbox.minCorner().to_vec3().xy();
        vect[1] -= bbox.minCorner().to_vec3().xy();
        vect[2] -= bbox.minCorner().to_vec3().xy();
        vect[3] -= bbox.minCorner().to_vec3().xy();
  #endif

  #if (1)

        vect[0].x() += margin() + displace;
        vect[1].x() += margin() + displace;
        vect[2].x() += margin() + displace;
        vect[3].x() += margin() + displace;

        vect[0].y() += margin();
        vect[1].y() += margin();
        vect[2].y() += margin();
        vect[3].y() += margin();
  #endif

        vect[0] += offset;
        vect[1] += offset;
        vect[2] += offset;
        vect[3] += offset;

        for(int i=0; i<4; i++)
        {
          if (alignment() & AlignHCenter)
          {
            CHECK( !(alignment() & AlignRight) )
            CHECK( !(alignment() & AlignLeft) )
            vect[i].x() -= (int)(bbox.width() / 2.0f);
          }

          if (alignment() & AlignRight)
          {
            CHECK( !(alignment() & AlignHCenter) )
            CHECK( !(alignment() & AlignLeft) )
            vect[i].x() -= (int)bbox.width();
          }

          if (alignment() & AlignTop)
          {
            CHECK( !(alignment() & AlignBottom) )
            CHECK( !(alignment() & AlignVCenter) )
            vect[i].y() -= (int)bbox.height();
          }

          if (alignment() & AlignVCenter)
          {
            CHECK( !(alignment() & AlignTop) )
            CHECK( !(alignment() & AlignBottom) )
            vect[i].y() -= int(bbox.height() / 2.0);
          }
        }

        mat4d m = mMatrix;
        
#if(1)
        if ( !actor->transform() )
        {
          if (viewportAlignment() & AlignHCenter)
          {
            CHECK( !(viewportAlignment() & AlignRight) )
            CHECK( !(viewportAlignment() & AlignLeft) )

            m.translate( int((viewport[2]-1.0f) / 2.0f), 0, 0);
          }

          if (viewportAlignment() & AlignRight)
          {
            CHECK( !(viewportAlignment() & AlignHCenter) )
            CHECK( !(viewportAlignment() & AlignLeft) )

            m.translate( int(viewport[2]-1.0f), 0, 0);
          }

          if (viewportAlignment() & AlignTop)
          {
            CHECK( !(viewportAlignment() & AlignBottom) )
            CHECK( !(viewportAlignment() & AlignVCenter) )

            m.translate( 0, int(viewport[3]-1.0f), 0);
          }

          if (viewportAlignment() & AlignVCenter)
          {
            CHECK( !(viewportAlignment() & AlignTop) )
            CHECK( !(viewportAlignment() & AlignBottom) )

            m.translate( 0, int((viewport[3]-1.0f) / 2.0f), 0);
          }
        }
#endif

        vect[0] = (m * (vec2d)vect[0]).to_vec2();
        vect[1] = (m * (vec2d)vect[1]).to_vec2();
        vect[2] = (m * (vec2d)vect[2]).to_vec2();
        vect[3] = (m * (vec2d)vect[3]).to_vec2();

        if ( actor->transform() && mode() == Text2D )
        {
          vec4d v(0,0,0,1);
          v = actor->transform()->localToWorldMatrix( render_stream ) * v;

          GlobalState::renderStream( render_stream )->currentCamera()->project(v,v);

          v.x() -= viewport[0];
          v.y() -= viewport[1];

          v.x() = int(v.x());
          v.y() = int(v.y());

          vect[0] += v.xy().to_vec2();
          vect[1] += v.xy().to_vec2();
          vect[2] += v.xy().to_vec2();
          vect[3] += v.xy().to_vec2();
        }

        glDrawArrays(GL_QUADS, 0, 4);

  #if (0)
        vec4 red[] = { vec4(1,0,0,1), vec4(1,0,0,1), vec4(1,0,0,1), vec4(1,0,0,1) };
        glDisable(GL_TEXTURE_2D);
        glColorPointer(4, GL_FLOAT, 0, red);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glColorPointer(4, GL_FLOAT, 0, cols);
        glEnable(GL_TEXTURE_2D);
  #endif
      }

      if (just_space && lines[iline][c] == ' ' && iline != lines.size()-1)
      {
        if (layout() == LeftToRightText)
        {
          pen.x() += just_space + (just_remained_space?1:0);

        }
        else
        if (layout() == RightToLeftText)
        {
          pen.x() -= just_space + (just_remained_space?1:0);

        }
        if(just_remained_space) 
          just_remained_space--;
      }

      if (layout() == LeftToRightText)
      {
        pen.x() += glyph->advance().x();

      }
      else
      if (layout() == RightToLeftText)
      {
        pen.x() -= glyph->advance().x();

      }

    }
  }

  glDisableClientState( GL_VERTEX_ARRAY );
  glDisableClientState( GL_NORMAL_ARRAY );
  glDisableClientState( GL_COLOR_ARRAY );
  glDisableClientState( GL_TEXTURE_COORD_ARRAY );
#endif

  GLCHECK4();

  if (mode() == Text2D)
  {
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix(); GLCHECK4()

    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); GLCHECK4()
  }
}

AABB Text::rawBoundingBox(const std::wstring& text) const
{
  AABB aabb;

  if(!mFont)
  {
    Log::error("Text::rawBoundingBox() error: no Font assigned to the Text object.\n");
    CHECK(0);
    return aabb;
  }

  if (!font()->mFT_Face)
  {
    Log::error("Text::rawBoundingBox() error: invalid FT_Face: probably you tried to load an unsupported font format.\n");
    CHECK(0);
    return aabb;
  }

  vec2 pen(0,0);
  vec3 vect[4];

  FT_Long use_kerning = FT_HAS_KERNING( font()->mFT_Face );
  FT_UInt previous = 0;

  for(int c=0; c<(int)text.length(); c++)
  {
    if (text[c] == '\n')
    {
      pen.y() -= mFont->mHeight ? mFont->mHeight : mFont->mSize;
      pen.x()  = 0;
      continue;
    }

    const ref<Glyph>& glyph = mFont->glyph(text[c]);

    if (glyph == NULL)
      continue;

    if ( kerningEnabled() && use_kerning && previous && glyph->glyphIndex()) 
    { 
      FT_Vector delta; delta.y = 0;
      if (layout() == LeftToRightText)
      {
        FT_Get_Kerning( font()->mFT_Face, previous, glyph->glyphIndex(), FT_KERNING_DEFAULT, &delta );
        pen.x() += delta.x / 64.0f; 
      }
      else
      if (layout() == RightToLeftText)
      {
        FT_Get_Kerning( font()->mFT_Face, glyph->glyphIndex(), previous, FT_KERNING_DEFAULT, &delta );
        pen.x() -= delta.x / 64.0f; 
      }
      pen.y() += delta.y / 64.0f; 
    }
    previous = glyph->glyphIndex();

    if ( glyph->textureHandle() )
    {
      int left = layout() == RightToLeftText ? -glyph->left() : +glyph->left();

      vect[0].x() = pen.x() + glyph->width()*0 + left -1;
      vect[0].y() = pen.y() + glyph->height()*0 + glyph->top() - glyph->height() -1;

      vect[1].x() = pen.x() + glyph->width()*1 + left +1; 
      vect[1].y() = pen.y() + glyph->height()*0 + glyph->top() - glyph->height() -1;

      vect[2].x() = pen.x() + glyph->width()*1 + left +1; 
      vect[2].y() = pen.y() + glyph->height()*1 + glyph->top() - glyph->height() +1;

      vect[3].x() = pen.x() + glyph->width()*0 + left -1; 
      vect[3].y() = pen.y() + glyph->height()*1 + glyph->top() - glyph->height() +1;

      if (layout() == RightToLeftText)
      {
#if (1)
        vect[0].x() -= glyph->width()-1 +2;
        vect[1].x() -= glyph->width()-1 +2;
        vect[2].x() -= glyph->width()-1 +2;
        vect[3].x() -= glyph->width()-1 +2;
#endif
      }

      vect[0].y() -= mFont->mHeight;
      vect[1].y() -= mFont->mHeight;
      vect[2].y() -= mFont->mHeight;
      vect[3].y() -= mFont->mHeight;

#if(0)

        vect[0].x() += margin();
        vect[1].x() += margin();
        vect[2].x() += margin();
        vect[3].x() += margin();

      vect[0].y() += margin();
      vect[1].y() += margin();
      vect[2].y() += margin();
      vect[3].y() += margin();
#endif

    }

    aabb.addPoint( (vec3d)vect[0] );
    aabb.addPoint( (vec3d)vect[1] );
    aabb.addPoint( (vec3d)vect[2] );
    aabb.addPoint( (vec3d)vect[3] );

    if (layout() == LeftToRightText)
      pen += glyph->advance();
    else
    if (layout() == RightToLeftText)
      pen -= glyph->advance();
  }

  return aabb;
}

void Text::drawBackground(int render_stream, Actor* actor)
{
  GLint viewport[] = {0,0,0,0};
  glGetIntegerv(GL_VIEWPORT, viewport);
  GLCHECK4()

  if (viewport[2] < 1) viewport[2] = 1;
  if (viewport[3] < 1) viewport[3] = 1;

  if (mode() == Text2D)
  {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    GLCHECK4();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); 
    glLoadIdentity();
    gluOrtho2D( -0.5f, viewport[2]-0.5f, -0.5f, viewport[3]-0.5f );
    GLCHECK4();
  }

  vec4 cols[] = { mBackgroundColor, mBackgroundColor, mBackgroundColor, mBackgroundColor };
  glEnableClientState( GL_COLOR_ARRAY );
  glColorPointer(4, GL_FLOAT, 0, cols);

  vec3 norms[] = { vec3(0,0,1), vec3(0,0,1), vec3(0,0,1), vec3(0,0,1) };
  glEnableClientState( GL_NORMAL_ARRAY );
  glNormalPointer(GL_FLOAT, 0, norms);

  vec3d a,b,c,d;
  boundingBoxTransformed( a, b, c, d, GlobalState::renderStream( render_stream )->currentCamera(), mode() == Text2D ? actor : NULL );
  vec2 vect[] = { a.xy().to_vec2(), b.xy().to_vec2(), c.xy().to_vec2(), d.xy().to_vec2() };
  glEnableClientState( GL_VERTEX_ARRAY );
  glVertexPointer(2, GL_FLOAT, 0, vect);

  glDrawArrays(GL_QUADS,0,4);

  glDisableClientState( GL_VERTEX_ARRAY );
  glDisableClientState( GL_NORMAL_ARRAY );
  glDisableClientState( GL_COLOR_ARRAY );

  if (mode() == Text2D)
  {
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix(); GLCHECK4()

    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); GLCHECK4()
  }
}

void Text::drawBorder(int render_stream, Actor* actor)
{
  GLint viewport[] = {0,0,0,0};
  glGetIntegerv(GL_VIEWPORT, viewport);
  GLCHECK4()

  if (viewport[2] < 1) viewport[2] = 1;
  if (viewport[3] < 1) viewport[3] = 1;

  if (mode() == Text2D)
  {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    GLCHECK4();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); 
    glLoadIdentity();
    gluOrtho2D( -0.5f, viewport[2]-0.5f, -0.5f, viewport[3]-0.5f );
    GLCHECK4();
  }

  vec4 cols[] = { mBorderColor, mBorderColor, mBorderColor, mBorderColor };
  glEnableClientState( GL_COLOR_ARRAY );
  glColorPointer(4, GL_FLOAT, 0, cols);

  vec3 norms[] = { vec3(0,0,1), vec3(0,0,1), vec3(0,0,1), vec3(0,0,1) };
  glEnableClientState( GL_NORMAL_ARRAY );
  glNormalPointer(GL_FLOAT, 0, norms);

  vec3d a,b,c,d;
  boundingBoxTransformed( a, b, c, d, GlobalState::renderStream( render_stream )->currentCamera(), mode() == Text2D ? actor : NULL );
  vec2 vect[] = { a.xy().to_vec2(), b.xy().to_vec2(), c.xy().to_vec2(), d.xy().to_vec2() };
  glEnableClientState( GL_VERTEX_ARRAY );
  glVertexPointer(2, GL_FLOAT, 0, vect);

  glDrawArrays(GL_LINE_LOOP,0,4);

  glDisableClientState( GL_VERTEX_ARRAY );
  glDisableClientState( GL_NORMAL_ARRAY );
  glDisableClientState( GL_COLOR_ARRAY );

  if (mode() == Text2D)
  {
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix(); GLCHECK4()

    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); GLCHECK4()
  }
}

AABB Text::boundingBox() const
{
  return boundingBox(text());
}

AABB Text::boundingBox(const std::wstring& text) const
{
  AABB bbox = rawBoundingBox( text );

  bbox.setMaxCorner( bbox.maxCorner() + vec3d(2*margin(),2*margin(),0) );

  vec3d min = bbox.minCorner() - bbox.minCorner();
  vec3d max = bbox.maxCorner() - bbox.minCorner();

  if (alignment() & AlignHCenter)
  {
    CHECK( !(alignment() & AlignRight) )
    CHECK( !(alignment() & AlignLeft) )
    min.x() -= int(bbox.width() / 2.0);
    max.x() -= int(bbox.width() / 2.0);
  }

  if (alignment() & AlignRight)
  {
    CHECK( !(alignment() & AlignHCenter) )
    CHECK( !(alignment() & AlignLeft) )
    min.x() -= (int)bbox.width();
    max.x() -= (int)bbox.width();
  }

  if (alignment() & AlignTop)
  {
    CHECK( !(alignment() & AlignBottom) )
    CHECK( !(alignment() & AlignVCenter) )
    min.y() -= (int)bbox.height();
    max.y() -= (int)bbox.height();
  }

  if (alignment() & AlignVCenter)
  {
    CHECK( !(alignment() & AlignTop) )
    CHECK( !(alignment() & AlignBottom) )
    min.y() -= int(bbox.height() / 2.0);
    max.y() -= int(bbox.height() / 2.0);
  }

  AABB aabb;
  aabb.setMinCorner(min);
  aabb.setMaxCorner(max);
  return aabb;
}

AABB Text::boundingBoxTransformed(Camera* camera, Actor* actor) const
{
  vec3d a, b, c, d;
  return boundingBoxTransformed(a, b, c, d, camera, actor);
}

AABB Text::boundingBoxTransformed(vec3d& a, vec3d& b, vec3d& c, vec3d& d, Camera* camera, Actor* actor) const
{
  AABB bbox = boundingBox();

  a = bbox.minCorner();
  b.x() = (float)bbox.maxCorner().x();
  b.y() = (float)bbox.minCorner().y();
  c = bbox.maxCorner();
  d.x() = (float)bbox.minCorner().x();
  d.y() = (float)bbox.maxCorner().y();

  a.z() = b.z() = c.z() = d.z() = 0;

  mat4d m = mMatrix;

#if (1)
  int w = camera->viewport()->width();
  int h = camera->viewport()->height();

  if (w < 1) w = 1;
  if (h < 1) h = 1;
  
  if(!actor || !actor->transform())
  {
    if (viewportAlignment() & AlignHCenter)
    {
      CHECK( !(viewportAlignment() & AlignRight) )
      CHECK( !(viewportAlignment() & AlignLeft) )

      m.translate( int((w-1.0f) / 2.0f), 0, 0);
    }

    if (viewportAlignment() & AlignRight)
    {
      CHECK( !(viewportAlignment() & AlignHCenter) )
      CHECK( !(viewportAlignment() & AlignLeft) )

      m.translate( int(w-1.0f), 0, 0);
    }

    if (viewportAlignment() & AlignTop)
    {
      CHECK( !(viewportAlignment() & AlignBottom) )
      CHECK( !(viewportAlignment() & AlignVCenter) )

      m.translate( 0, int(h-1.0f), 0);
    }

    if (viewportAlignment() & AlignVCenter)
    {
      CHECK( !(viewportAlignment() & AlignTop) )
      CHECK( !(viewportAlignment() & AlignBottom) )

      m.translate( 0, int((h-1.0f) / 2.0f), 0);
    }
  }
#endif

  a = m * a;
  b = m * b;
  c = m * c;
  d = m * d;

  if ( actor && actor->transform() )
  {
    if ( mode() == Text3D )
    {
      a = actor->transform()->localToWorldMatrix( camera->renderStream() ) * a;
      b = actor->transform()->localToWorldMatrix( camera->renderStream() ) * b;
      c = actor->transform()->localToWorldMatrix( camera->renderStream() ) * c;
      d = actor->transform()->localToWorldMatrix( camera->renderStream() ) * d;
    }
    else
    if ( mode() == Text2D )
    {

      vec4d v(0,0,0,1);
      v = actor->transform()->localToWorldMatrix( camera->renderStream() ) * v;

      camera->project(v,v);

      GLint viewport[] = {0,0,0,0};
      glGetIntegerv(GL_VIEWPORT, viewport); GLCHECK4()
      v.x() -= viewport[0];
      v.y() -= viewport[1];

      v.x() = int(v.x());
      v.y() = int(v.y());

      a += v.xyz();
      b += v.xyz();
      c += v.xyz();
      d += v.xyz();
    }
  }

  bbox.setEmpty();
  bbox.addPoint(a);
  bbox.addPoint(b);
  bbox.addPoint(c);
  bbox.addPoint(d);
  return bbox;
}

void Text::translate(double x, double y, double z)
{
  mMatrix.translate(x,y,z);
}

void Text::rotate(double degrees, double x, double y, double z)
{
  mMatrix.rotate(degrees,x,y,z);
}

void Text::resetMatrix()
{
  mMatrix.setIdentity();
}

