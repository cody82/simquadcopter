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

#ifndef FrameBufferObject_INCLUDE_DEFINE
#define FrameBufferObject_INCLUDE_DEFINE

#include "vl/Camera.hpp"

namespace vl
{

  class FBORenderTarget;
  class FBOAttachment: public Object
  {
    friend class FBORenderTarget;
  public:
    virtual ~FBOAttachment() { destroy(); }

    virtual void destroy();
    virtual void init(int w, int h, EAttachmentPoint attachment)  = 0;

    const std::set< ref<FBORenderTarget> >& fboRenderTargets() const { return mFBORenderTargets; }

  protected:
    std::set< ref<FBORenderTarget> > mFBORenderTargets;
  };

  class FBOColorBuffer: public FBOAttachment
  {
  private:
    FBOColorBuffer(const FBOColorBuffer& other): FBOAttachment(other) {}
    void operator=(const FBOColorBuffer&) {}

  public:
    FBOColorBuffer(EColorBufferFormat type)
    {
      mHandle = 0;
      mWidth  = 0;
      mHeight = 0;
      mType = type;
    }

    ~FBOColorBuffer()
    {
      destroy();
    }

    void init(int w, int h, EAttachmentPoint attachment);

    void destroy();

    GLuint handle() const { return mHandle; }
    void setType(EColorBufferFormat type) { if (type != mType) { /* schedules recreation */ mWidth = mHeight = 0; mType = type; } }
    EColorBufferFormat type() const { return mType; }
    int width() const { return mWidth; }
    int height() const { return mHeight; }

  protected:
    GLuint mHandle;
    int mWidth;
    int mHeight;
    EColorBufferFormat mType;
  };

  class FBODepthBuffer: public FBOAttachment
  {
  private:
    FBODepthBuffer(const FBODepthBuffer&other): FBOAttachment(other) {}
    void operator=(const FBODepthBuffer&) {}

  public:
    FBODepthBuffer(EDepthType type)
    {
      mHandle = 0;
      mWidth  = 0;
      mHeight = 0;
      mType = type;
    }

    ~FBODepthBuffer()
    {
      destroy();
    }

    void init(int w, int h, EAttachmentPoint attachment);

    void destroy();

    GLuint handle() const { return mHandle; }
    void setType(EDepthType type) { if (type != mType) { /* schedules recreation */mWidth=mHeight=0; mType = type; } }
    EDepthType type() const { return mType; }
    int width() const { return mWidth; }
    int height() const { return mHeight; }

  protected:
    GLuint mHandle;
    int mWidth;
    int mHeight;
    EDepthType mType;
  };

  class FBOStencilBuffer: public FBOAttachment
  {
  private:
    FBOStencilBuffer(const FBOStencilBuffer& other): FBOAttachment(other) {}
    void operator=(const FBOStencilBuffer&) {}

  public:
    FBOStencilBuffer(EStencilType type)
    {
      mHandle = 0;
      mWidth  = 0;
      mHeight = 0;
      mType = type;
    }

    ~FBOStencilBuffer()
    {
      destroy();
    }

    void init(int w, int h, EAttachmentPoint attachment);

    void destroy();

    GLuint handle() const { return mHandle; }
    void setType(EStencilType type) { if (type != mType) { /* schedules recreation */mWidth=mHeight=0; mType = type; } }
    EStencilType type() const { return mType; }
    int width() const { return mWidth; }
    int height() const { return mHeight; }

  protected:
    GLuint mHandle;
    int mWidth;
    int mHeight;
    EStencilType mType;
  };

  class FBOTexture1D: public FBOAttachment
  {
  public:
    FBOTexture1D(Texture* texture, int level)
    {
      mLevel      = level;
      mTexture    = texture;
    }

    Texture* texture() { return mTexture.get(); }
    const Texture* texture() const { return mTexture.get(); }
    int level() const { return mLevel; }

    virtual void init(int w, int h, EAttachmentPoint attachment);
    virtual void destroy();

  protected:
    ref<Texture> mTexture;
    int mLevel;
  };

  class FBOTexture2D: public FBOAttachment
  {
  public:
    FBOTexture2D(Texture* texture, int level)
    {
      mLevel      = level;
      mTexture    = texture;
    }

    Texture* texture() { return mTexture.get(); }
    const Texture* texture() const { return mTexture.get(); }
    int level() const { return mLevel; }

    virtual void init(int w, int h, EAttachmentPoint attachment);
    virtual void destroy();

  protected:
    ref<Texture> mTexture;
    int mLevel;
  };

  class FBOTexture3D: public FBOAttachment
  {
  public:
    FBOTexture3D(Texture* texture, int level, int zoffset)
    {
      mLevel   = level;
      mTexture = texture;
      mZOffset = zoffset;
    }

    Texture* texture() { return mTexture.get(); }
    const Texture* texture() const { return mTexture.get(); }
    int level() const { return mLevel; }
    void setLevel(int level) { mLevel = level; }
    int zOffset() const { return mZOffset; }
    void setZOffset(int zoffset) { mZOffset = zoffset; }

    virtual void init(int w, int h, EAttachmentPoint attachment);
    virtual void destroy();

  protected:
    ref<Texture> mTexture;
    int mLevel;
    int mZOffset;
  };

  class FBORenderTarget: public RenderTarget
  {
  private:
    FBORenderTarget(const FBORenderTarget& other): RenderTarget(other), mHandle(0) { }
    const FBORenderTarget& operator=(const FBORenderTarget&)
    {
      return *this;
    }
  public:
    FBORenderTarget(): mHandle(0)
    {
    }
    FBORenderTarget(int w, int h): mHandle(0)
    {
      setWidth(w);
      setHeight(h);
    }
    ~FBORenderTarget()
    {
      destroy();
    }

    virtual void activate();
    GLenum checkFramebufferStatus();

    void destroy()
    {
      GLCHECK4()
      removeAllAttachments();
      if (handle())
      {
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); GLCHECK4()
        glDeleteFramebuffersEXT(1,&mHandle); GLCHECK4()
        mHandle = 0;
      }
      setWidth(0);
      setHeight(0);
    }

    void addColorAttachment(EAttachmentPoint color_attachment, FBOAttachment* attachment);
    void addTextureAttachment(EAttachmentPoint color_attachment, FBOAttachment* attachment);
    void addDepthAttachment(FBOAttachment* attachment);
    const std::map< EAttachmentPoint, ref<FBOAttachment> >& fboAttachments() const { return mFBOAttachments; }
    void removeAttachment(FBOAttachment* attachment);
    void removeAttachment(EAttachmentPoint attachment);
    void removeAllAttachments();

    GLuint handle() const { return mHandle; }

  public:
    std::map< EAttachmentPoint, ref<FBOAttachment> > mFBOAttachments;
    GLuint mHandle;
  };
}

#endif
