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

#include "vl/TrackballManipulator.hpp"
#include "vl/Camera.hpp"

using namespace vl;

void TrackballManipulator::mouseDownEvent(EMouseButton btn, int x, int y)
{
  if ( camera() == NULL )
    return;

  if ( (btn & mMouseButton) == 0)
    return;

  CHECK(camera()->renderTarget())

  x -= camera()->viewport()->x();
  y -= camera()->renderTarget()->height() - 1 - (camera()->viewport()->y() + camera()->viewport()->height() -1);

  int w = camera()->viewport()->width();
  int h = camera()->viewport()->height();

  if (x<0 || y<0 || x>=w || y>=h)
    return;

  mStartX = x;
  mStartY = y;

  if (mTransform)
  {
    mStartMatrix = mTransform->localMatrix();
    mPivot = mStartMatrix.getT();
  }
  else
    mStartMatrix = camera()->viewMatrix();
}

void TrackballManipulator::mouseUpEvent(EMouseButton, int x, int y)
{
  if ( camera() == NULL )
    return;

  CHECK(camera()->renderTarget())

  x -= camera()->viewport()->x();
  y -= camera()->renderTarget()->height() - 1 - (camera()->viewport()->y() + camera()->viewport()->height() -1);

  if (mStartX != INVALID_VAL && mStartY != INVALID_VAL)
  {
    if (mTransform)
      mTransform->setLocalMatrix( mat4d::translation(mPivot) * trackballRotation(x,y) * mat4d::translation(-mPivot) * mStartMatrix );
    else
      camera()->setViewMatrix( mat4d::translation(mPivot) * trackballRotation(x,y) * mat4d::translation(-mPivot) * mStartMatrix );
  }
  mStartX = INVALID_VAL;
  mStartY = INVALID_VAL;
}

void TrackballManipulator::mouseMoveEvent(int x, int y)
{
  if ( camera() == NULL )
    return;

  CHECK(camera()->renderTarget())

  x -= camera()->viewport()->x();
  y -= camera()->renderTarget()->height() - 1 - (camera()->viewport()->y() + camera()->viewport()->height() -1);

  if (mStartX != INVALID_VAL && mStartY != INVALID_VAL)
  {
    if (mTransform)
    {
      mTransform->setLocalMatrix( mat4d::translation(mPivot) * trackballRotation(x,y) * mat4d::translation(-mPivot) * mStartMatrix );
      mStartMatrix = mTransform->localMatrix();
    }
    else
    {
      camera()->setViewMatrix( mat4d::translation(mPivot) * trackballRotation(x,y) * mat4d::translation(-mPivot) * mStartMatrix );
      mStartMatrix = camera()->viewMatrix();
    }

    mStartX = x;
    mStartY = y;
  }
}

mat4d TrackballManipulator::trackballRotation(int x, int y)
{

  if( x==mStartX  && y==mStartY )
    return mat4d();

  CHECK(mStartX != INVALID_VAL);
  CHECK(mStartY != INVALID_VAL);
  CHECK(camera())
  vec3d a = computeVector(mStartX, mStartY);
  vec3d b = computeVector(x, y);
  vec3d n = cross(a, b);
  n.normalize();
  a.normalize();
  b.normalize();
  double alpha = acos(dot(a,b)) * (mTransform ? 1 : -1);
  vec3d nc =  camera()->viewMatrix().get3x3() * n;
  if (mTransform && mTransform->parent())
  {
    nc = mTransform->parent()->getComputedLocalToWorld().inverse() * nc;
  }
  return mat4d::rotation(alpha*RAD_TO_DEG, nc);
}

vec3d TrackballManipulator::computeVector(int x, int y)
{
  CHECK(camera())
  vec3d v((double)x,(double)y,0);
  vec3d c(camera()->viewport()->width() / 2.0f, camera()->viewport()->height() / 2.0f, 0);
  v -= c;
  v.x() /= camera()->viewport()->width()  * 0.5f;
  v.y() /= camera()->viewport()->height() * 0.5f;
  v.x() *= 0.7f;
  v.y() *= 0.7f;
  v.y() = -v.y();
  if (v.length() > 1.0f)
  {
    v.normalize();

  }

  double z2 = 1 - v.x()*v.x() - v.y()*v.y();
  if (z2 < 0) z2 = 0;
  v.z() = sqrt( z2 );
  return v;
}

void TrackballManipulator::mouseWheelEvent(int n)
{
  if (!camera())
    return;
  mat4d m = camera()->viewMatrix();
  vec3d v = m.getT() + m.getZ()*n*mZoomSpeed;
  m.setT(v);
  camera()->setViewMatrix(m);
}

