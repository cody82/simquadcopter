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

#include "vl/FrameBufferObject.hpp"
#include "vl/Say.hpp"
#include "vl/Log.hpp"

using namespace vl;

void FBORenderTarget::activate() 
{
  if (!GLEW_EXT_framebuffer_object)
    return;

#ifndef NDEBUG
  if ( width() <= 0 || height() <= 0 )
  {
    Log::error(Say("FBORenderTarget::activate() called with illegal dimensions: width = %n, height = %n\n") << width() << height() );
    CHECK(0)
  }
#endif
  if ( width() <= 0 || height() <= 0 )
    return;

  if (!mHandle)
    glGenFramebuffersEXT(1, &mHandle); GLCHECK4()
  
  CHECK(mHandle)
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mHandle); GLCHECK4()

  CHECK(width())
  CHECK(height())

  std::map< EAttachmentPoint, ref<FBOAttachment> >::iterator it = mFBOAttachments.begin();
  for(; it != mFBOAttachments.end(); ++it)
    if (it->second)
      it->second->init( width(), height(), it->first );

#ifndef NDEBUG
  GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT); GLCHECK4()
  if ( status != GL_FRAMEBUFFER_COMPLETE_EXT )
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); GLCHECK4()

  switch(status)
  {
  case GL_FRAMEBUFFER_COMPLETE_EXT:
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
    Log::error("FBORenderTarget::activate(): GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT\n"); CHECK(0);
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
    Log::error("FBORenderTarget::activate(): GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT\n"); CHECK(0);
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
    Log::error("FBORenderTarget::activate(): GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT\n"); CHECK(0);
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
    Log::error("FBORenderTarget::activate(): GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT\n"); CHECK(0);
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
    Log::error("FBORenderTarget::activate(): GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT\n"); CHECK(0);
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
    Log::error("FBORenderTarget::activate(): GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT\n"); CHECK(0);
    break;
  case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
    Log::error("FBORenderTarget::activate(): GL_FRAMEBUFFER_UNSUPPORTED_EXT\n");
    break;
  }
#endif
}

GLenum FBORenderTarget::checkFramebufferStatus()
{
  if (!GLEW_EXT_framebuffer_object)
    return 0;

  if (!mHandle)
    glGenFramebuffersEXT(1, &mHandle); GLCHECK4()
  
  CHECK(mHandle)

  GLint fbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &fbo);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, handle()); GLCHECK4()

  CHECK(width())
  CHECK(height())

  std::map< EAttachmentPoint, ref<FBOAttachment> >::iterator it = mFBOAttachments.begin();
  for(; it != mFBOAttachments.end(); ++it)
    if (it->second)
      it->second->init( width(), height(), it->first );

  GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT); GLCHECK4()

  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo); GLCHECK4()

#ifndef NDEBUG
  switch(status)
  {
  case GL_FRAMEBUFFER_COMPLETE_EXT:
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
    Log::error("FBORenderTarget::activate(): GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT\n"); CHECK(0);
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
    Log::error("FBORenderTarget::activate(): GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT\n"); CHECK(0);
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
    Log::error("FBORenderTarget::activate(): GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT\n"); CHECK(0);
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
    Log::error("FBORenderTarget::activate(): GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT\n"); CHECK(0);
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
    Log::error("FBORenderTarget::activate(): GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT\n"); CHECK(0);
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
    Log::error("FBORenderTarget::activate(): GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT\n"); CHECK(0);
    break;
  case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
    Log::error("FBORenderTarget::activate(): GL_FRAMEBUFFER_UNSUPPORTED_EXT\n");
    break;
  }
#endif

  return status;
}

void FBORenderTarget::addColorAttachment(EAttachmentPoint color_attachment, FBOAttachment* attachment) 
{ 
  removeAttachment(color_attachment);
  mFBOAttachments[color_attachment] = attachment; 
  attachment->mFBORenderTargets.insert(this);
}

void FBORenderTarget::addTextureAttachment(EAttachmentPoint color_attachment, FBOAttachment* attachment) 
{ 
  removeAttachment(color_attachment);
  mFBOAttachments[color_attachment] = attachment; 
  attachment->mFBORenderTargets.insert(this);
}

void FBORenderTarget::addDepthAttachment(FBOAttachment* attachment) 
{ 
  removeAttachment(AP_DEPTH_ATTACHMENT_EXT);
  mFBOAttachments[AP_DEPTH_ATTACHMENT_EXT] = attachment; 
  attachment->mFBORenderTargets.insert(this);
}

void FBORenderTarget::removeAttachment(FBOAttachment* attachment)
{

  std::vector<EAttachmentPoint> attachment_points;
  std::map< EAttachmentPoint, ref<FBOAttachment> >::iterator it = mFBOAttachments.begin();
  for(; it != mFBOAttachments.end(); ++it)
    if (it->second == attachment)
      attachment_points.push_back(it->first);

  for(unsigned i=0; i<attachment_points.size(); i++)
    removeAttachment( attachment_points[i] );
}

void FBORenderTarget::removeAttachment(EAttachmentPoint attachment)
{
  if (handle() && GLEW_EXT_framebuffer_object)
  {

    GLint fbo;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &fbo);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mHandle); GLCHECK4()

    glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, attachment, GL_RENDERBUFFER_EXT, 0 ); GLCHECK4()

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo); GLCHECK4()
  }

  FBOAttachment* fbo_attachment = mFBOAttachments.find(attachment) != mFBOAttachments.end() ? mFBOAttachments[attachment].get() : NULL;
  if (fbo_attachment)
  {
    fbo_attachment->mFBORenderTargets.erase( this );
  }
  mFBOAttachments.erase(attachment);
}

void FBORenderTarget::removeAllAttachments()
{

  std::vector<EAttachmentPoint> attachment_points;
  std::map< EAttachmentPoint, ref<FBOAttachment> >::iterator it = mFBOAttachments.begin();
  for(; it != mFBOAttachments.end(); ++it)
    attachment_points.push_back(it->first);

  for(unsigned i=0; i<attachment_points.size(); i++)
    removeAttachment( attachment_points[i] );
}

void FBOTexture1D::init(int w, int h, EAttachmentPoint attachment)
{
  CHECK(texture())
  CHECK(texture()->handle())
  CHECK(texture()->dimension() == GL_TEXTURE_1D)
  CHECK( w == texture()->width()  );
  CHECK( h == 1 );

  glFramebufferTexture1DEXT(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_1D, texture()->handle(), level() ); GLCHECK4()

  glBindTexture(GL_TEXTURE_1D, texture()->handle());
  glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glBindTexture(GL_TEXTURE_1D, 0);
}

void FBOTexture1D::destroy()
{
  FBOAttachment::destroy();
}

void FBOTexture2D::init(int w, int h, EAttachmentPoint attachment)
{
  CHECK(texture())
  CHECK(texture()->handle())
  CHECK(texture()->dimension() == GL_TEXTURE_2D)
  CHECK( w == texture()->width()  );
  CHECK( h == texture()->height() );

  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_2D, texture()->handle(), level() ); GLCHECK4()

  glBindTexture(GL_TEXTURE_2D, texture()->handle());
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void FBOTexture2D::destroy()
{
  FBOAttachment::destroy();
}

void FBOTexture3D::init(int w, int h, EAttachmentPoint attachment)
{
  CHECK(texture())
  CHECK(texture()->handle())
  CHECK(texture()->dimension() == GL_TEXTURE_3D)
  CHECK( w == texture()->width()  );
  CHECK( h == texture()->height() );
  CHECK( zOffset() <= texture()->depth() );  

  glFramebufferTexture3DEXT(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_3D, texture()->handle(), level(), zOffset() ); GLCHECK4()

  glBindTexture(GL_TEXTURE_3D, texture()->handle());
  glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glBindTexture(GL_TEXTURE_3D, 0);
}

void FBOTexture3D::destroy()
{
  FBOAttachment::destroy();
}

void FBOColorBuffer::init(int w, int h, EAttachmentPoint attachment)
{
  CHECK(w)
  CHECK(h)

  if (!GLEW_EXT_framebuffer_object)
    return;

  mWidth  = w;
  mHeight = h;

  if ( !mHandle || w != width() || h != height() )
  {
    glGenRenderbuffersEXT(1, &mHandle); GLCHECK4()
    CHECK(mHandle)
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, mHandle); GLCHECK4()
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, type(), width(), height()); GLCHECK4()
  }

  glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, attachment, GL_RENDERBUFFER_EXT, handle() ); GLCHECK4()
}

void FBOColorBuffer::destroy()
{
  GLCHECK4()
  FBOAttachment::destroy();
  mWidth  = 0;
  mHeight = 0;
  if (mHandle)
  {
    glDeleteRenderbuffersEXT(1, &mHandle); GLCHECK4()
    mHandle = 0;
  }
}

void FBODepthBuffer::init(int w, int h, EAttachmentPoint)
{
  CHECK(w)
  CHECK(h)

  if (!GLEW_EXT_framebuffer_object)
    return;

  mWidth  = w;
  mHeight = h;

  if ( !mHandle || w != width() || h != height() )
  {
    glGenRenderbuffersEXT(1, &mHandle); GLCHECK4()
    CHECK(mHandle)
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, mHandle); GLCHECK4()
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, type(), width(), height()); GLCHECK4()
  }

  glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, handle() ); GLCHECK4()
}

void FBODepthBuffer::destroy()
{
  GLCHECK4()
  FBOAttachment::destroy();
  mWidth  = 0;
  mHeight = 0;
  if (mHandle)
  {
    glDeleteRenderbuffersEXT(1, &mHandle); GLCHECK4()
    mHandle = 0;
  }
}

void FBOStencilBuffer::init(int w, int h, EAttachmentPoint)
{
  CHECK(w)
  CHECK(h)

  if (!GLEW_EXT_framebuffer_object)
    return;

  mWidth  = w;
  mHeight = h;

  if ( !mHandle || w != width() || h != height() )
  {
    glGenRenderbuffersEXT(1, &mHandle); GLCHECK4()
    CHECK(mHandle)
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, mHandle); GLCHECK4()
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, type(), width(), height()); GLCHECK4()
  }

  glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, handle() ); GLCHECK4()
}

void FBOStencilBuffer::destroy()
{
  GLCHECK4()
  FBOAttachment::destroy();
  mWidth  = 0;
  mHeight = 0;
  if (mHandle)
  {
    glDeleteRenderbuffersEXT(1, &mHandle); GLCHECK4()
    mHandle = 0;
  }
}

void FBOAttachment::destroy()
{
  std::set< ref<FBORenderTarget> > fbos = fboRenderTargets();
  std::set< ref<FBORenderTarget> >::iterator it = fbos.begin();
  for(; it != fbos.end(); ++it)
  {
    it->get()->removeAttachment(this);
  }
}

