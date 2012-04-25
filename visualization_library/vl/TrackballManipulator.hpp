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

#ifndef TrackballManipulator_INCLUDE_DEFINE
#define TrackballManipulator_INCLUDE_DEFINE

#include "vl/OpenGLWidget.hpp"
#include "vl/Camera.hpp"
#include "vl/vec3d.hpp"
#include "vl/mat4d.hpp"

namespace vl
{
  class Camera;
  class Transform;

  class TrackballManipulator: public EventListener
  {
    static const int INVALID_VAL = -256*256*256;
  public:
    TrackballManipulator(): mStartX(INVALID_VAL), mStartY(INVALID_VAL), mMouseButton(LeftButton), mZoomSpeed(3.0f) {}

    virtual void mouseDownEvent(EMouseButton, int x, int y);

    virtual void mouseUpEvent(EMouseButton, int x, int y);

    virtual void mouseMoveEvent(int x, int y);

    virtual void mouseWheelEvent(int /*n*/);

    mat4d trackballRotation(int x, int y);

    vec3d computeVector(int x, int y);

    void setCamera(Camera* camera) { mCamera = camera; }

    Camera* camera() { return mCamera.get(); }

    void prepareToReconnect() { mStartX = INVALID_VAL; mStartY = INVALID_VAL; }

    void setPivot(vec3d pivot) { mPivot = pivot; }

    vec3d pivot() const { return mPivot; }

    void setTransform(Transform* tr) { mTransform = tr; }

    Transform* transform() { return mTransform.get(); }

    int mouseButton() const { return mMouseButton; }
    void setMouseButton(int mouse_button_combination) { mMouseButton = mouse_button_combination; }

    float zoomSpeed() const { return mZoomSpeed; }
    void setZoomSpeed(float speed) { mZoomSpeed = speed; }

  protected:
    ref<Camera> mCamera;
    int mStartX;
    int mStartY;
    mat4d mStartMatrix;
    vec3d mPivot;
    ref<Transform> mTransform;
    int mMouseButton;
    float mZoomSpeed;
  };
}

#endif
