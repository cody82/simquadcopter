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

#include "vl/Camera.hpp"
#include "vl/GlobalState.hpp"
#include "vl/OpenGL.hpp"
#include "vl/mat4d.hpp"
#include "vl/AABB.hpp"
#include "vl/Log.hpp"
#include "vl/Say.hpp"

using namespace vl;

void RenderTarget::activateDrawBuffers()
{
  GLCHECK4()

  CHECK(!mDrawBuffers.empty())

#ifndef NDEBUG
  GLint fbo = 0;
  if (GLEW_EXT_framebuffer_object)
    glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &fbo);
  if (fbo)
  {
    std::set<GLenum> legal;
    legal.insert(GL_NONE);
    legal.insert(GL_COLOR_ATTACHMENT0_EXT);
    legal.insert(GL_COLOR_ATTACHMENT1_EXT);
    legal.insert(GL_COLOR_ATTACHMENT2_EXT);
    legal.insert(GL_COLOR_ATTACHMENT3_EXT);
    legal.insert(GL_COLOR_ATTACHMENT4_EXT);
    legal.insert(GL_COLOR_ATTACHMENT5_EXT);
    legal.insert(GL_COLOR_ATTACHMENT6_EXT);
    legal.insert(GL_COLOR_ATTACHMENT7_EXT);
    legal.insert(GL_COLOR_ATTACHMENT8_EXT);
    legal.insert(GL_COLOR_ATTACHMENT9_EXT);
    legal.insert(GL_COLOR_ATTACHMENT10_EXT);
    legal.insert(GL_COLOR_ATTACHMENT11_EXT);
    legal.insert(GL_COLOR_ATTACHMENT12_EXT);
    legal.insert(GL_COLOR_ATTACHMENT13_EXT);
    legal.insert(GL_COLOR_ATTACHMENT14_EXT);
    legal.insert(GL_COLOR_ATTACHMENT15_EXT);
    for(unsigned i=0; i<mDrawBuffers.size(); i++)
    {
      if(legal.find(mDrawBuffers[i]) == legal.end())
        Log::error(Say("FBO bound but RenderTarget::setDrawBuffers() called with non FBO buffer id 0x%h04n or not called at all.\n") << mDrawBuffers[i]);
    }
  }
  else
  {
    std::set<GLenum> legal;
    legal.insert(GL_NONE);
    legal.insert(GL_BACK_LEFT);
    legal.insert(GL_BACK_RIGHT);
    legal.insert(GL_FRONT_LEFT);
    legal.insert(GL_FRONT_RIGHT);
    legal.insert(GL_AUX0);
    legal.insert(GL_AUX1);
    legal.insert(GL_AUX2);
    legal.insert(GL_AUX3);
    for(unsigned i=0; i<mDrawBuffers.size(); i++)
    {
      if(legal.find(mDrawBuffers[i]) == legal.end())
        Log::error(Say("FBO not bound or not supported but RenderTarget::setDrawBuffers() probably called with FBO buffer id 0x%h04n\n") << mDrawBuffers[i]);
    }
  }
#endif

  if (mDrawBuffers.size() > 1 && GLEW_ARB_draw_buffers)
  {
    glDrawBuffers( (GLsizei)mDrawBuffers.size(), (GLenum*)&mDrawBuffers[0] );
    GLCHECK4()
  }
  else
  {
    glDrawBuffer( mDrawBuffers[0] );
    GLCHECK4()
  }
}

Viewport::Viewport()
{
  mX = 0;
  mY = 0;
  mWidth = 0;
  mHeight = 0;
	mBorder = 0;
	mClearColor = vec4(0.8f,0,0.1f,1);
  mBorderColor = vec4(0.1f,0.1f,0.2f,1);
  mClearDepth = 1.0f;
  mClearStencil = 0;
	mClearFlags = CF_CLEAR_COLOR_DEPTH;
}

Viewport::Viewport(int x, int y, int w, int h)
{
  mX = x;
  mY = y;
  mWidth  = w;
  mHeight = h;
	mBorder = 0;
	mClearColor = vec4(0.8f,0,0.1f,1);
  mBorderColor = vec4(0.1f,0.1f,0.2f,1);
  mClearDepth = 1.0f;
  mClearStencil = 0;
	mClearFlags = CF_CLEAR_COLOR_DEPTH;
}

void Viewport::activate()
{

  GLint x = mX + mBorder;
	GLint y = mY + mBorder;
	GLint w = mWidth  - mBorder * 2;
  GLint h = mHeight - mBorder * 2;

  if (w < 1) w = 1;
  if (h < 1) h = 1;

  glViewport(x, y, w, h);  GLCHECK4()

  if (mClearFlags) 
  {
    glDepthMask(GL_TRUE); GLCHECK4()
    glStencilMask(GL_TRUE); GLCHECK4()
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE); GLCHECK4()
    glEnable(GL_SCISSOR_TEST); GLCHECK4()

    if (mBorder > 0 && (mClearFlags & GL_COLOR_BUFFER_BIT))
    {
      glScissor(mX, mY, mWidth, mHeight); GLCHECK4()
      glClearColor( mBorderColor.r(), mBorderColor.g(), mBorderColor.b(), mBorderColor.a() ); GLCHECK4()
	    glClear(GL_COLOR_BUFFER_BIT); GLCHECK4()
    }

    glScissor(x, y, w, h); GLCHECK4()
    glClearColor( mClearColor.r(), mClearColor.g(), mClearColor.b(), mClearColor.a() ); GLCHECK4()
    glClearDepth( mClearDepth ); GLCHECK4()
    GLint stencil_bits;
    glGetIntegerv(GL_STENCIL_BITS, &stencil_bits);
    glClearStencil( mClearStencil ); GLCHECK4()
    glClear(mClearFlags); GLCHECK4()
    glDisable(GL_SCISSOR_TEST); GLCHECK4()
  }  	
}

Camera::Camera(std::string name) {
	mActive = true;
	mName = name;
	mFOV = 60;
	mNearPlane = 1.0f;
	mFarPlane  = 4000.0f;
  mCameraSlot = 0;
  mViewport = new Viewport;

  mProjectionMatrix.setAsPerspectiveProjection(mFOV, 640.0f/480.0f, mNearPlane, mFarPlane);
}

void Camera::applyModelViewMatrix(const mat4d& matrix)
{

  mat4d m = viewMatrix().getCleanInverse() * matrix;
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixd( m.ptr() );
}

void Camera::applyViewMatrix()
{

  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixd( viewMatrix().getCleanInverse().ptr() );
}

void Camera::activate() 
{

  GlobalState::renderStream( renderStream() )->setCurrentCamera(this);

  GLint x = mViewport->x()      + mViewport->border();
	GLint y = mViewport->y()      + mViewport->border();
	GLint w = mViewport->width()  - mViewport->border() * 2;
	GLint h = mViewport->height() - mViewport->border() * 2;
	
  glMatrixMode( GL_PROJECTION );
	glLoadMatrixd( projectionMatrix().ptr() );

  applyViewMatrix();

	GLdouble mvmatrix[16];
	GLdouble projmatrix[16];
	GLint viewport[4] = { x, y, w, h };
	glGetDoublev(GL_MODELVIEW_MATRIX,  mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);

	GLfloat left   = x -1.0f;
	GLfloat bottom = y -1.0f;
	GLfloat right  = (GLfloat)x + w;
	GLfloat top    = (GLfloat)y + h;
	GLdouble vx, vy, vz;

	vec3d o = viewMatrix().getT();
	gluUnProject(x  /* whatever */, y/* whatever */, 0.0/* near clipping plane */, mvmatrix, projmatrix, viewport, &vx, &vy, &vz);
	vec3d n((GLfloat)vx,(GLfloat)vy,(GLfloat)vz);
	gluUnProject(x  /* whatever */, y/* whatever */, 1.0/* far  clipping plane */, mvmatrix, projmatrix, viewport, &vx, &vy, &vz);
	vec3d f((GLfloat)vx,(GLfloat)vy,(GLfloat)vz);
	gluUnProject(left, bottom, 0.0/* near clipping plane */, mvmatrix, projmatrix, viewport, &vx, &vy, &vz);
	vec3d a = vec3d((GLfloat)vx,(GLfloat)vy,(GLfloat)vz) - o;
	gluUnProject(left, top,    0.0/* near clipping plane */, mvmatrix, projmatrix, viewport, &vx, &vy, &vz);
	vec3d b = vec3d((GLfloat)vx,(GLfloat)vy,(GLfloat)vz) - o;
	gluUnProject(right, top,   0.0/* near clipping plane */, mvmatrix, projmatrix, viewport, &vx, &vy, &vz);
	vec3d c = vec3d((GLfloat)vx,(GLfloat)vy,(GLfloat)vz) - o;
	gluUnProject(right, bottom,0.0/* near clipping plane */, mvmatrix, projmatrix, viewport, &vx, &vy, &vz);
	vec3d d = vec3d((GLfloat)vx,(GLfloat)vy,(GLfloat)vz) - o;
	vec3d cam_look = -viewMatrix().getZ();

  mFrustum.setPlane(0, Plane(n, -cam_look));
	mFrustum.setPlane(1, Plane(f,  cam_look));
	mFrustum.setPlane(2, Plane(o, cross(b,a).normalize()));
	mFrustum.setPlane(3, Plane(o, cross(d,c).normalize()));
	mFrustum.setPlane(4, Plane(o, cross(c,b).normalize()));
	mFrustum.setPlane(5, Plane(o, cross(a,d).normalize()));
}

void Camera::setProjectionMatrixPerspective()
{
  GLint w = mViewport->width()  - mViewport->border()*2;
  GLint h = mViewport->height() - mViewport->border()*2;  	
  GLfloat aspect_ratio = (GLfloat)w/h;
  mProjectionMatrix.setAsPerspectiveProjection(mFOV, aspect_ratio, mNearPlane, mFarPlane);
}

void Camera::setProjectionMatrixOrtho()
{

  mProjectionMatrix.setAsOrthographicProjection(
    0, (GLfloat)mViewport->width() -1, 
    0, (GLfloat)mViewport->height()-1,
    mNearPlane, mFarPlane );
}

void Camera::setProjectionMatrixOrtho2D()
{

  mProjectionMatrix.setAsOrthographicProjection(
    0, (GLfloat)mViewport->width() -1,
    0, (GLfloat)mViewport->height()-1,
    -1, +1);
}

void Camera::setViewMatrixAsLookAt( const vec3d& eye, const vec3d& center, const vec3d& up)
{
  mViewMatrix.setAsLookAt(eye,center,up);
  mInverseViewMatrix = mViewMatrix;
  mInverseViewMatrix.invert();
}

void Camera::getViewMatrixAsLookAt( vec3d& eye, vec3d& look, vec3d& up, vec3d& right) const
{
  mViewMatrix.getAsLookAt(eye,look,up,right);
}

bool Camera::project(const vec4d& in, vec4d& out) const
{
    out = mProjectionMatrix * mInverseViewMatrix * in;

    if (out.w() == 0.0f) 
      return false;

    out.x() /= out.w();
    out.y() /= out.w();
    out.z() /= out.w();

    out.x() = out.x() * 0.5f + 0.5f;
    out.y() = out.y() * 0.5f + 0.5f;
    out.z() = out.z() * 0.5f + 0.5f;

    out.x() = out.x() * mViewport->width()  + mViewport->x();
    out.y() = out.y() * mViewport->height() + mViewport->y();
    return true;
}

bool Camera::unproject(const vec3d& win, vec4d& out) const
{
    vec4d v;
    v.x() = win.x();
    v.y() = win.y();
    v.z() = win.z();
    v.w() = 1.0f;

    v.x() = (v.x() - mViewport->x()) / mViewport->width();
    v.y() = (v.y() - mViewport->y()) / mViewport->height();

    v.x() = v.x() * 2.0f - 1.0f;
    v.y() = v.y() * 2.0f - 1.0f;
    v.z() = v.z() * 2.0f - 1.0f;

    bool invertible=true;
    mat4d inverse = (mProjectionMatrix * mInverseViewMatrix).inverse(&invertible);
    if (!invertible)
      return false;

    v = inverse * v;
    if (out.w() == 0.0) 
      return false;

    v.x() /= v.w();
    v.y() /= v.w();
    v.z() /= v.w();
    out = v;
    return true;
}

bool Camera::frustumCull(const AABB& aabb)
{
  if (aabb.isEmpty())
    return false;
  for(int i=0; i<6; i++)
  {
    if ( frustum().plane(i).isOutside(aabb) )
      return true;
  }
  return false;
}

namespace
{
  bool callback_sort(const ref<RenderFinishedCallback>& a, const ref<RenderFinishedCallback>& b)
  {
    return a->rank() < b->rank();
  }
}

void Camera::dispatchRenderFinished() 
{ 

  std::sort( mRenderFinishedCallback.begin(), mRenderFinishedCallback.end() );

  std::vector< ref<RenderFinishedCallback> > temp = mRenderFinishedCallback;
  for(unsigned i=0; i<temp.size(); i++)
  {
    temp[i]->renderFinished(this);
    if (temp[i]->removeAfterCall())
      removeRenderFinishedCallback( temp[i].get() ); // note: use the pointer not the index!
  }
}

void Camera::removeRenderFinishedCallback(RenderFinishedCallback* call_back) 
{ 
  for(int i=0; i<renderFinishedCallbackCount(); i++) 
    if (renderFinishedCallback(i) == call_back)
    {
      removeRenderFinishedCallback(i);
      return;
    }
}

bool Camera::isPointInside(int x, int y)
{

  x -= viewport()->x();
  y -= renderTarget()->height() - 1 - (viewport()->y() + viewport()->height() -1);

  if (x<0 || y<0 || x>=viewport()->width() || y>=viewport()->height())
    return false;
  else
    return true;  
}

