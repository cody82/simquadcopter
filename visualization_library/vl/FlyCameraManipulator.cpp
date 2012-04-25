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

#include "vl/FlyCameraManipulator.hpp"
#include "vl/Time.hpp"
#include "vl/Camera.hpp"

using namespace vl;

FlyCameraManipulator::FlyCameraManipulator()
{
  mRotationSpeed = 0.5;
  mMovementSpeed = 50.0f;
  mXDegrees = 0;
  mYDegrees = 0;
  mLastX = -1;
  mLastY = -1;
  mLastTime = 0;
  mPosition = vec3d(0,20,20);

  setKeysForward(Key_W);
  setKeysBackward(Key_S);
  setKeysLeft(Key_A);
  setKeysRight(Key_D);
  setKeysUp(Key_W, Key_Shift);
  setKeysDown(Key_S, Key_Shift);
}

void FlyCameraManipulator::mouseMoveEvent(int x, int y)
{
  if ( camera() == NULL )
    return;

  CHECK(openglWidget());

  if (mLastX == -1)
  {
    mLastX = (int)camera()->viewport()->center().x();

    mLastY = camera()->renderTarget()->height() - camera()->viewport()->height()/2 - camera()->viewport()->y();
    openglWidget()->ignoreNextMouseMoveEvent();
    openglWidget()->setMousePosition( mLastX, mLastY );

    return;
  }

  mXDegrees -= (y - mLastY) * mRotationSpeed;
  mYDegrees -= (x - mLastX) * mRotationSpeed;
  openglWidget()->ignoreNextMouseMoveEvent();
  openglWidget()->setMousePosition(mLastX, mLastY);
}

void FlyCameraManipulator::renderEvent()
{
  if (camera() == NULL)
    return;

  if (mLastTime == 0)
  {
    mLastTime = Time::timerSeconds();
    return;
  }
  double dt = Time::timerSeconds() - mLastTime;
  mLastTime = Time::timerSeconds();

  mat4d m = mat4d::translation(mPosition);
  m *= mat4d::rotation( mYDegrees, vec3d(0,1,0), mXDegrees, vec3d(1,0,0) );
  camera()->setViewMatrix(m);

  vec3d direction;
  bool okmodifier;
  bool modifier = openglWidget()->isKeyPressed( Key_Alt ) || openglWidget()->isKeyPressed( Key_Ctrl ) || openglWidget()->isKeyPressed( Key_Shift );

  okmodifier = (mKeysLeft[1] == Key_None) ? modifier == false : openglWidget()->isKeyPressed( mKeysLeft[1] );
  if ( openglWidget()->isKeyPressed(mKeysLeft[0]) && okmodifier )
    direction.x() = -1;

  okmodifier = (mKeysRight[1] == Key_None) ? modifier == false : openglWidget()->isKeyPressed(mKeysRight[1]);
  if ( openglWidget()->isKeyPressed(mKeysRight[0]) && okmodifier )
    direction.x() = +1;

  okmodifier = (mKeysBackward[1] == Key_None) ? modifier == false : openglWidget()->isKeyPressed(mKeysBackward[1]);
  if ( openglWidget()->isKeyPressed(mKeysBackward[0]) && okmodifier )
    direction.z() = -1;

  okmodifier = (mKeysForward[1] == Key_None) ? modifier == false : openglWidget()->isKeyPressed(mKeysForward[1]);
  if ( openglWidget()->isKeyPressed(mKeysForward[0]) && okmodifier )
    direction.z() = +1;

  okmodifier = (mKeysUp[1] == Key_None) ? modifier == false : openglWidget()->isKeyPressed(mKeysUp[1]);
  if ( openglWidget()->isKeyPressed(mKeysUp[0]) && okmodifier )
    direction.y() = +1;

  okmodifier = (mKeysDown[1] == Key_None) ? modifier == false : openglWidget()->isKeyPressed(mKeysDown[1]);
  if ( openglWidget()->isKeyPressed(mKeysDown[0]) && okmodifier )
    direction.y() = -1;

  vec3d dir;
  dir += camera()->viewMatrix().getX() * direction.x();
  dir += camera()->viewMatrix().getY() * direction.y();
  dir -= camera()->viewMatrix().getZ() * direction.z();
  dir.normalize();
  mPosition += dir * (double)(dt * mMovementSpeed);
}

void FlyCameraManipulator::setCamera(Camera* camera) { mCamera = camera; }

Camera* FlyCameraManipulator::camera() { return mCamera.get(); }
const Camera* FlyCameraManipulator::camera() const { return mCamera.get(); }

void FlyCameraManipulator::prepareToReconnect()
{
  if ( camera() == NULL )
    return;

  mLastX = -1;
  mLastY = -1;
  setPosition( camera()->viewMatrix().getT() );
  double x, y;
  camera()->viewMatrix().getYXRotationAngles( y, x );
  setXDegrees(x);
  setYDegrees(y);
}

