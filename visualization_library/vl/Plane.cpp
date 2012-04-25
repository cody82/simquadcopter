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

#include "vl/Plane.hpp"
#include "vl/AABB.hpp"

using namespace vl;

Plane::Plane(const vec3d& o, const vec3d& n) 
{
	mNormal = n;
	mOrigin = dot(o, n);
}

double Plane::distance(const vec3d &v) const 
{
	return dot(v, mNormal) - mOrigin;
}

bool Plane::isOutside(const AABB& aabb) const
{
  vec3d pt;
  pt.x() = mNormal.x() >= 0 ? aabb.minCorner().x() : aabb.maxCorner().x();
  pt.y() = mNormal.y() >= 0 ? aabb.minCorner().y() : aabb.maxCorner().y();
  pt.z() = mNormal.z() >= 0 ? aabb.minCorner().z() : aabb.maxCorner().z();
  return distance(pt) >= 0;
}

int Plane::classify(const AABB& aabb) const
{
  vec3d corner[] = 
  {
    vec3d(aabb.minCorner().x(), aabb.minCorner().y(), aabb.minCorner().z()),
    vec3d(aabb.minCorner().x(), aabb.maxCorner().y(), aabb.minCorner().z()),
    vec3d(aabb.maxCorner().x(), aabb.maxCorner().y(), aabb.minCorner().z()),
    vec3d(aabb.maxCorner().x(), aabb.minCorner().y(), aabb.minCorner().z()),
    vec3d(aabb.minCorner().x(), aabb.minCorner().y(), aabb.maxCorner().z()),
    vec3d(aabb.minCorner().x(), aabb.maxCorner().y(), aabb.maxCorner().z()),
    vec3d(aabb.maxCorner().x(), aabb.maxCorner().y(), aabb.maxCorner().z()),
    vec3d(aabb.maxCorner().x(), aabb.minCorner().y(), aabb.maxCorner().z())
  };

  int left=0;
  int right=0;
  double const NEPS = -0.0001f;
  double const PEPS = +0.0001f;

  for(int i=0 ;i<8; i++)
  {
    if ( distance(corner[i]) < NEPS ) 
      left++;
    else 
    if ( distance(corner[i]) > PEPS ) 
      right++;

    if(left && right) // its clearly intersecting the plane
      return 0;
  }

  if (left)
    return -1;
  else 
  if (right)
    return +1;
  else // all the points were on the plane
    return 0;
}

void Plane::applyPlane(int plane, const mat4d& mat)
{
  vec3d pt1 = mNormal * mOrigin;
  vec3d pt2 = mNormal * mOrigin + mNormal;

  pt1 = mat * pt1;
  pt2 = mat * pt2;

  vec3d n = pt2 - pt1;
  double orig = dot(n, pt1);

  double equation[] = {
    n.x(),
    n.y(),
    n.z(),
    -orig
  };

  glClipPlane(GL_CLIP_PLANE0 + plane,equation);
}

void Plane::followTransform(Transform* transform) 
{
  mFollowedTransform = transform; 
}

Transform* Plane::followedTransform()
{
  return mFollowedTransform.get(); 
}

