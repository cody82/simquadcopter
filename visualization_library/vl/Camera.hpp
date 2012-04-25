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

#ifndef Camera_INCLUDE_DEFINE
#define Camera_INCLUDE_DEFINE

#include "vl/CHECK.hpp"
#include "vl/math3D.hpp"
#include "vl/vec4d.hpp"
#include "vl/ShaderNode.hpp"
#include <string>
#include <map>
#include <set>

namespace vl
{

  class RenderTarget: public Object
  {
  public:
    RenderTarget(): mWidth(0), mHeight(0)
    {
      setDrawBuffers(RDB_BACK_LEFT);
    }
    RenderTarget(int width, int height): mWidth(width), mHeight(height)
    {
      setDrawBuffers(RDB_BACK_LEFT);
    }
    int width() const { return mWidth; }
    int height() const { return mHeight; }
    void setWidth(int width) { mWidth = width; }
    void setHeight(int height) { mHeight = height; }

    virtual void activate()
    {

      if (GLEW_EXT_framebuffer_object)
      {
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); GLCHECK4()
      }
      GLCHECK4()
    }

    virtual void activateDrawBuffers();

    void setDrawBuffers(EReadDrawBuffer draw_buffer)
    {
      std::vector< EReadDrawBuffer > draw_buffers;
      draw_buffers.push_back(draw_buffer);
      setDrawBuffers(draw_buffers);
    }
    void setDrawBuffers(EReadDrawBuffer draw_buffer1, EReadDrawBuffer draw_buffer2)
    {
      std::vector< EReadDrawBuffer > draw_buffers;
      draw_buffers.push_back(draw_buffer1);
      draw_buffers.push_back(draw_buffer2);
      setDrawBuffers(draw_buffers);
    }
    void setDrawBuffers(EReadDrawBuffer draw_buffer1, EReadDrawBuffer draw_buffer2, EReadDrawBuffer draw_buffer3)
    {
      std::vector< EReadDrawBuffer > draw_buffers;
      draw_buffers.push_back(draw_buffer1);
      draw_buffers.push_back(draw_buffer2);
      draw_buffers.push_back(draw_buffer3);
      setDrawBuffers(draw_buffers);
    }
    void setDrawBuffers(EReadDrawBuffer draw_buffer1, EReadDrawBuffer draw_buffer2, EReadDrawBuffer draw_buffer3, EReadDrawBuffer draw_buffer4)
    {
      std::vector< EReadDrawBuffer > draw_buffers;
      draw_buffers.push_back(draw_buffer1);
      draw_buffers.push_back(draw_buffer2);
      draw_buffers.push_back(draw_buffer3);
      draw_buffers.push_back(draw_buffer4);
      setDrawBuffers(draw_buffers);
    }
    void setDrawBuffers(const std::vector< EReadDrawBuffer >& draw_buffers) { mDrawBuffers = draw_buffers; }
    const std::vector< EReadDrawBuffer >& drawBuffers() { return mDrawBuffers; }

  protected:
    int mWidth;
    int mHeight;
    std::vector< EReadDrawBuffer > mDrawBuffers;
  };

  class Viewport: public Object
  {
  public:
    Viewport();
    Viewport(int x, int y, int w, int h);

    void activate();

    bool null() { return mWidth == 0 || mHeight == 0; }

    void set(int x, int y, int w, int h) { mX = x; mY = y; mWidth = w; mHeight = h; }
    void setX(int x) { mX = x; }
    int x() const { return mX; }
    void setY(int y) { mY = y; }
    int y() const { return mY; }
    void setWidth(int width) { mWidth = width; }
    int width() const { return mWidth; }
    void setHeight(int height) { mHeight = height; }
    int height() const { return mHeight; }
    vec3d center() const { return vec3d(mX + mWidth / 2.0f, mY + mHeight / 2.0f, 0.0f); }

    void setBorder(int border) { mBorder = border; }
    int border() const { return mBorder; }

    const vec4& borderColor() const { return mBorderColor; }
    void setBorderColor(const vec4& color) { mBorderColor = color; }

    void setClearColor(const vec4& color) { mClearColor = color; }
    const vec4& clearColor() const { return mClearColor; }

    void setClearStencil(GLint stencil) { mClearStencil = stencil; }
    GLint clearStencil() const { return mClearStencil; }

    void setClearDetph(GLdouble detph) { mClearDepth = detph; }
    GLdouble clearDetph() const { return mClearDepth; }

    void setClearFlags(EClearFlags clear_flags) { mClearFlags = clear_flags; }
    EClearFlags clearFlags() { return mClearFlags; }

  protected:
	  vec4 mBorderColor;
	  vec4 mClearColor;
    GLdouble mClearDepth;
    GLint mClearStencil;
    EClearFlags mClearFlags;
	  int mBorder;
	  int mX;
	  int mY;
	  int mWidth;
	  int mHeight;
  };

  class Frustum
  {
  public:
    void setPlane(int i, const Plane& plane) { CHECK(i>=0 && i<6); mPlanes[i] = plane; }
    const Plane& plane(int i) const { CHECK(i>=0 && i<6); return mPlanes[i]; }

  protected:
    Plane mPlanes[6];
  };

  class RenderFinishedCallback: public Object
  {
  public:
    RenderFinishedCallback(): mRank(0.0f), mRemoveAfterCall(false) {}
    virtual ~RenderFinishedCallback() {}
    virtual void renderFinished(const Camera* camera) = 0;

    void setRemoveAfterCall(bool remove) { mRemoveAfterCall = remove; }
    bool removeAfterCall() const { return mRemoveAfterCall; }

    void setRank(float rank) { mRank = rank; }
    float rank() const { return mRank; }

  protected:
    float mRank;
    bool mRemoveAfterCall;
  };

  class Camera: public Object
  {
  public:
	  Camera(std::string name="camera");
	  void activate();
    void applyModelViewMatrix(const mat4d&);
    void applyViewMatrix();
    void setName(const std::string& name) { mName = name; }
    const std::string& name() const { return mName; }
    void setActive(bool active) { mActive = active; }
    bool active() const { return mActive; }
    void setCameraSlot(int slot) { mCameraSlot = slot; }
    int renderStream() const { return mCameraSlot; }
    void setFOV(GLdouble fov) { mFOV = fov; }
    GLdouble fov() const { return mFOV; }
    void setNearPlane(GLdouble nearplane) { mNearPlane = nearplane; }
    GLdouble nearPlane() const { return mNearPlane; }
    void setFarPlane(GLdouble farplane) { mFarPlane = farplane; }
    GLdouble farPlane() const { return mFarPlane; }
    void setFrustum(const Frustum& frustum) { mFrustum = frustum; }
    const Frustum& frustum() const { return mFrustum; }
    void setViewport(Viewport* viewport) { mViewport = viewport; }
    Viewport* viewport() { return mViewport.get(); }
    void followTransform(Transform* transform) { mFollowTransform = transform; }
    Transform* followedTransform() { return mFollowTransform.get(); }

    void setViewMatrix(const mat4d& mat) { mViewMatrix = mat; mInverseViewMatrix = mat; mInverseViewMatrix.invert(); }
    const mat4d& viewMatrix() const { return mViewMatrix; }
    const mat4d& inverseViewMatrix() const { return mInverseViewMatrix; }
    void setProjectionMatrix(const mat4d& mat) { mProjectionMatrix = mat; }
    const mat4d& projectionMatrix() const { return mProjectionMatrix; }

    void setProjectionMatrixPerspective();

    void setProjectionMatrixOrtho();

    void setProjectionMatrixOrtho2D();

    void setViewMatrixAsLookAt( const vec3d& eye, const vec3d& center, const vec3d& up);

    void getViewMatrixAsLookAt( vec3d& eye, vec3d& look, vec3d& up, vec3d& right) const;

    bool project(const vec4d& in, vec4d& out) const;

    bool unproject(const vec3d& win, vec4d& out) const;

    bool isPointInside(int x, int y);

    bool frustumCull(const AABB& aabb);

    RenderTarget* renderTarget() { return mRenderTarget.get(); }
    void setRenderTarget(RenderTarget* render_target) { mRenderTarget = render_target; }

    void dispatchRenderFinished();
    void addRenderFinishedCallback(RenderFinishedCallback* callback)
    {
      mRenderFinishedCallback.push_back(callback);
    }
    int renderFinishedCallbackCount() const { return (int)mRenderFinishedCallback.size(); }
    const RenderFinishedCallback* renderFinishedCallback(int index) const { return mRenderFinishedCallback[index].get(); }
    RenderFinishedCallback* renderFinishedCallback(int index) { return mRenderFinishedCallback[index].get(); }
    void removeRenderFinishedCallback(int index) { mRenderFinishedCallback.erase( mRenderFinishedCallback.begin() + index ); }
    void removeRenderFinishedCallback(RenderFinishedCallback* call_back);
    void removeAllRenderFinishedCallbacks() { mRenderFinishedCallback.clear(); }

  protected:
    std::vector< ref<RenderFinishedCallback> > mRenderFinishedCallback;
	  mat4d mInverseViewMatrix;
	  mat4d mViewMatrix;
	  mat4d mProjectionMatrix;
    ref<RenderTarget> mRenderTarget;
	  ref<Viewport> mViewport;
	  Frustum mFrustum;
    ref<Transform> mFollowTransform;
	  GLdouble mFOV;
	  GLdouble mNearPlane;
	  GLdouble mFarPlane;
    int mCameraSlot;
	  bool mActive;
  };
}

#endif
