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

#include "vl/mat3.hpp"
#include <cstdio>
#include <cmath>

using namespace vl;

vec2 mat3::getX() const
{
	vec2 v;
	v.x() = e(0,0);
	v.y() = e(0,1);
	return v;
}

vec2 mat3::getY() const
{
	vec2 v;
	v.x() = e(1,0);
	v.y() = e(1,1);
	return v;
}

vec2 mat3::getT() const
{
	vec2 v;
	v.x() = e(2,0);
	v.y() = e(2,1);
	return v;
}

const mat3& mat3::setX(const vec2& v) 
{
	e(0,0) = v.x();
	e(0,1) = v.y();
	return *this;
}

const mat3& mat3::setY(const vec2& v) 
{
	e(1,0) = v.x();
	e(1,1) = v.y();
	return *this;
}

const mat3& mat3::setT(const vec2& v) 
{
	e(2,0) = v.x();
	e(2,1) = v.y();
	return *this;
}

mat3 mat3::rotation( GLdouble degrees )
{
  mat3 rot;
  degrees = degrees * (3.14159265f / 180.0f);
  GLdouble s = (GLdouble) sin( degrees );
  GLdouble c = (GLdouble) cos( degrees );
  rot.e(0,0) = (GLfloat)c;
  rot.e(1,1) = (GLfloat)c;
  rot.e(0,1) = (GLfloat)s;
  rot.e(1,0) = -(GLfloat)s;
  return rot;
}

mat3 mat3::inverse(bool *invertible) const
{
  const float epsilon = 0.0001f;
  const float& a11 = e(0,0);
  const float& a12 = e(0,1);
  const float& a13 = e(0,2);
  const float& a21 = e(1,0);
  const float& a22 = e(1,1);
  const float& a23 = e(1,2);
  const float& a31 = e(2,0);
  const float& a32 = e(2,1);
  const float& a33 = e(2,2);

  float DET = a11*(a33*a22-a32*a23)-a21*(a33*a12-a32*a13)+a31*(a23*a12-a22*a13);

  if (invertible)
    if (DET < epsilon)
      *invertible = false;
    else
      *invertible = true;

  mat3 m;
  m *= 0;

  if (DET < epsilon)
    return m;

  m = mat3(
    a33*a22-a32*a23,    -(a33*a12-a32*a13), a23*a12-a22*a13, 
    -(a33*a21-a31*a23), a33*a11-a31*a13,    -(a23*a11-a21*a13),
    a32*a21-a31*a22,    -(a32*a11-a31*a12), a22*a11-a21*a12
  );
  m /= DET;
  return m;
}

