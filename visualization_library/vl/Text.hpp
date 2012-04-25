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

#ifndef Text_INCLUDE_DEFINE
#define Text_INCLUDE_DEFINE

#include "vl/Drawable.hpp"
#include "vl/vec4d.hpp"
#include "vl/enums.hpp"
#include <string>
#include <map>

struct FT_FaceRec_;
typedef struct FT_FaceRec_*  FT_Face;

namespace vl
{
  class Font;

  class Glyph: public Object
  {
  private:
    Glyph(const Glyph& other): Object(other)  {}
    void operator=(const Glyph&){}

  public:
    Glyph(): mTextureHandle(0), mWidth(0), mHeight(0), mLeft(0), mTop(0), mS0(0), mT0(0), mS1(0), mT1(0),mGlyphIndex(0), mFont(NULL) {}

    ~Glyph();

    GLuint textureHandle() const { return mTextureHandle; }
    void setTextureHandle(GLuint handle) { mTextureHandle = handle; }

    int width() const { return mWidth; }
    void setWidth(int width) { mWidth = width; }

    int height() const { return mHeight; }
    void setHeight(int height) { mHeight = height; }

    int left() const { return mLeft; }
    void setLeft(int left) { mLeft = left; }

    int top() const { return mTop; }
    void setTop(int top) { mTop = top; }

    float s0() const { return mS0; }
    void setS0(float s0) { mS0 = s0; }

    float t0() const { return mT0; }
    void setT0(float t0) { mT0 = t0; }

    float s1() const { return mS1; }
    void setS1(float s1) { mS1 = s1; }

    float t1() const { return mT1; }
    void setT1(float t1) { mT1 = t1; }

    const vec2& advance() const { return mAdvance; }
    void setAdvance(const vec2& advance) { mAdvance = advance; }

    unsigned int glyphIndex() const { return mGlyphIndex; }
    void setGlyphIndex(unsigned int glyph_index) { mGlyphIndex = glyph_index; }

    const Font* font() const { return mFont; }
    void setFont(Font* font) { mFont = font; }

  protected:
    GLuint mTextureHandle;
    int mWidth;
    int mHeight;
    int mLeft;
    int mTop;
    float mS0;
    float mT0;
    float mS1;
    float mT1;
    vec2 mAdvance;
    unsigned int mGlyphIndex; // glyph index
    Font* mFont;
  };

  class Font: public Object
  {
    friend class Text;
  public:
    void operator=(const Font& other);
    Font(const Font& other): Object(other) { *this = other; }
    Font();
    Font(const std::string& font_file, int size );
    ~Font();

    const std::string& fontFile() const { return mFontFile; }
    void setFontFile(const std::string& font_file);
    int size() const { return mSize; }
    void setSize(int size);
    Glyph* glyph(int character);
    void setSmooth(bool smooth);
    bool smooth() const { return mSmooth; }

  protected:
    std::string mFontFile;
    std::map< int, ref<Glyph> > mGlyphMap;
    FT_Face mFT_Face;
    int mSize;
    float mHeight;
    bool mSmooth;
  };

  class Text: public Drawable
  {
  public:
    Text(): mColor(1,1,1,1), mBorderColor(0,0,0,1), mBackgroundColor(1,1,1,1), mOutlineColor(0,0,0,1), mShadowColor(0,0,0,0.5f), mShadowVector(2,-2), 
      mInterlineSpacing(5), mAlignment(AlignTop|AlignLeft), mViewportAlignment(AlignTop|AlignLeft), mMargin(5), mMode(Text2D), mLayout(LeftToRightText), mTextAlignment(TextAlignLeft), 
      mBorderEnabled(false), mBackgroundEnabled(false), mOutlineEnabled(false), mShadowEnabled(false), mKerningEnabled(true) {}

    const std::wstring& text() const { return mText; }
    void setText(const std::wstring& text) { mText = text; }

    const vec4& color() const { return mColor; }
    void setColor(const vec4& color) { mColor = color; }

    const vec4& borderColor() const { return mBorderColor; }
    void setBorderColor(const vec4& border_color) { mBorderColor = border_color; }

    const vec4& outlineColor() const { return mOutlineColor; }
    void setOutlineColor(const vec4& outline_color) { mOutlineColor = outline_color; }

    const vec4& backgroundColor() const { return mBackgroundColor; }
    void setBackgroundColor(const vec4& background_color) { mBackgroundColor = background_color; }

    const vec4& shadowColor() const { return mShadowColor; }
    void setShadowColor(const vec4& shadow_color) { mShadowColor = shadow_color; }

    const vec2& shadowVector() const { return mShadowVector; }
    void setShadowVector(const vec2& shadow_vector) { mShadowVector = shadow_vector; }

    int margin() const { return mMargin; }
    void setMargin(int margin) { mMargin = margin; }

    Font* font() const { return mFont.get(); }
    void setFont(Font* font) { mFont = font; }

    const mat4d matrix() const { return mMatrix; }
    void setMatrix(const mat4d& matrix) { mMatrix = matrix; }

    int  alignment() const { return mAlignment; }
    void setAlignment(int  align) { mAlignment = align; }

    int  viewportAlignment() const { return mViewportAlignment; }
    void setViewportAlignment(int  align) { mViewportAlignment = align; }

    float interlineSpacing() const { return mInterlineSpacing; }
    void setInterlineSpacing(float spacing) { mInterlineSpacing = spacing; }

    ETextMode mode() const { return mMode; }
    void setMode(ETextMode mode) { mMode = mode; }

    ETextLayout layout() const { return mLayout; }
    void setLayout(ETextLayout layout) { mLayout = layout; }

    ETextAlign textAlignment() const { return mTextAlignment; }
    void setTextAlignment(ETextAlign align) { mTextAlignment = align; }

    bool borderEnabled() const { return mBorderEnabled; }
    void setBorderEnabled(bool border) { mBorderEnabled = border; }

    bool backgroundEnabled() const { return mBackgroundEnabled; }
    void setBackgroundEnabled(bool background) { mBackgroundEnabled = background; }

    bool kerningEnabled() const { return mKerningEnabled; }
    void setKerningEnabled(bool kerning) { mKerningEnabled = kerning; }

    bool outlineEnabled() const { return mOutlineEnabled; }
    void setOutlineEnabled(bool outline) { mOutlineEnabled = outline; }

    bool shadowEnabled() const { return mShadowEnabled; }
    void setShadowEnabled(bool shadow) { mShadowEnabled = shadow; }

    virtual void draw(Actor*, int, unsigned int);
    virtual void computeAABB() {}
    AABB boundingBox() const;
    AABB boundingBox(const std::wstring& text) const;
    AABB boundingBoxTransformed(vec3d& a, vec3d& b, vec3d& c, vec3d& d, Camera* camera, Actor* actor=NULL) const;
    AABB boundingBoxTransformed(Camera* camera, Actor* actor=NULL) const;

    void translate(double x, double y, double z);
    void rotate(double degrees, double x, double y, double z);
    void resetMatrix();

  protected:
    void drawText(int render_stream, Actor*, const vec4& color, const vec2& offset);
    void drawBackground(int render_stream, Actor* actor);
    void drawBorder(int render_stream, Actor* actor);
    AABB rawBoundingBox(const std::wstring& text) const;

  protected:
    ref<Font> mFont;
    std::wstring mText;
    vec4 mColor;
    vec4 mBorderColor;
    vec4 mBackgroundColor;
    vec4 mOutlineColor;
    vec4 mShadowColor;
    vec2 mShadowVector;
    mat4d mMatrix;
    float mInterlineSpacing;
    int mAlignment;
    int mViewportAlignment;
    int mMargin;
    ETextMode mMode;
    ETextLayout mLayout;
    ETextAlign mTextAlignment;
    bool mBorderEnabled;
    bool mBackgroundEnabled;
    bool mOutlineEnabled;
    bool mShadowEnabled;
    bool mKerningEnabled;
  };

}

#endif
