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

#ifndef FlyCameraManipulator_INCLUDE_DEFINE
#define FlyCameraManipulator_INCLUDE_DEFINE

#include "vl/OpenGLWidget.hpp"
#include "vl/vec3d.hpp"

namespace vl
{
  class Camera;

  class FlyCameraManipulator: public EventListener
  {
  public:
    FlyCameraManipulator();

    virtual void mouseMoveEvent(int x, int y);

    virtual void renderEvent();

    void setCamera(Camera* camera);

    Camera* camera();
    const Camera* camera() const;

    void setKeysForward(EKey key, EKey modifier = Key_None)  { mKeysForward[0] = key; mKeysForward[1] = modifier; } ;
    void setKeysBackward(EKey key, EKey modifier = Key_None) { mKeysBackward[0] = key; mKeysBackward[1] = modifier; } ;
    void setKeysLeft(EKey key, EKey modifier = Key_None)     { mKeysLeft[0] = key; mKeysLeft[1] = modifier; } ;
    void setKeysRight(EKey key, EKey modifier = Key_None)    { mKeysRight[0] = key; mKeysRight[1] = modifier; } ;
    void setKeysUp(EKey key, EKey modifier = Key_None)       { mKeysUp[0] = key; mKeysUp[1] = modifier; } ;
    void setKeysDown(EKey key, EKey modifier = Key_None)     { mKeysDown[0] = key; mKeysDown[1] = modifier; } ;

    void prepareToReconnect();

    void setPosition(vec3d position) { mPosition = position; }

    const vec3d& position() const { return mPosition; }

    void setXDegrees(double degree) { mXDegrees = degree; }

    double xDegrees() { return mXDegrees; }

    void setYDegrees(double degree) { mYDegrees = degree; }

    double yDegrees() { return mYDegrees; }

  protected:
    ref<Camera> mCamera;
    vec3d mPosition;
    double mLastTime;
    double mRotationSpeed;
    double mMovementSpeed;
    double mXDegrees;
    double mYDegrees;
    int mLastX;
    int mLastY;
    EKey mKeysForward[2];
    EKey mKeysBackward[2];
    EKey mKeysUp[2];
    EKey mKeysDown[2];
    EKey mKeysLeft[2];
    EKey mKeysRight[2];
  };
}

#endif
