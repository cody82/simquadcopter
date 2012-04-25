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

#include "vl/quat.hpp"
#include "vl/math3D.hpp"
#include <cmath>

using namespace vl;

GLdouble quat::length() const
{
  return sqrt( this->dot(*this) );
}

quat quat::normalize()
{
  GLdouble one_over_mag = 1.0/length();
  x() *= one_over_mag;
  y() *= one_over_mag;
  z() *= one_over_mag;
  w() *= one_over_mag;
  return *this;
}

quat quat::fromEulerXYZ(GLdouble degX, GLdouble degY, GLdouble degZ )
{
  *this = quat(degX, vec3d(1,0,0)), quat(degY, vec3d(0,1,0)) * quat(degZ, vec3d(0,0,1));
  return *this;
}

quat quat::fromEulerZYX(GLdouble degZ, GLdouble degY, GLdouble degX )
{
  *this = quat(degZ, vec3d(0,0,1)) * quat(degY, vec3d(0,1,0)) * quat(degX, vec3d(1,0,0));
  return *this;
}

quat quat::fromMatrix(const mat4d& m)
{
  GLdouble tr, s, q[4];
  int i, j, k;

  int nxt[3] = {1, 2, 0};

  tr = m[0][0] + m[1][1] + m[2][2];

  if (tr > 0.0) 
  {
    s = sqrt (tr + 1.0);
    w() = s / 2.0;
    s = 0.5 / s;
    x() = (m[1][2] - m[2][1]) * s;
    y() = (m[2][0] - m[0][2]) * s;
    z() = (m[0][1] - m[1][0]) * s;
  } 
  else 
  {		

    i = 0;
    if (m[1][1] > m[0][0]) i = 1;
    if (m[2][2] > m[i][i]) i = 2;
    j = nxt[i];
    k = nxt[j];

    s = sqrt ((m[i][i] - (m[j][j] + m[k][k])) + 1.0);

    q[i] = s * 0.5;

    if (s != 0.0) 
      s = 0.5 / s;

    q[3] = (m[j][k] - m[k][j]) * s;
    q[j] = (m[i][j] + m[j][i]) * s;
    q[k] = (m[i][k] + m[k][i]) * s;

    x() = q[0];
    y() = q[1];
    z() = q[2];
    w() = q[3];

  }

  return *this;
}

quat quat::fromAxisAngle(GLdouble degrees, const vec3d& axis)
{
  degrees *= vl::DEG_TO_RAD;
  GLdouble sa2 = sin(degrees * 0.5);
  vec3d na = axis;
  na.normalize();

  mVec[0] = na.x() * sa2;
  mVec[1] = na.y() * sa2;
  mVec[2] = na.z() * sa2;
  mVec[3] = cos(degrees * 0.5);
  return *this;
}

void quat::toAxisAngle( vec4d& axis, GLdouble& angle ) const
{
  GLdouble scale = sqrt( x() * x() + y() * y() + z() * z() );
  axis.x() = (x() / scale);
  axis.y() = (y() / scale);
  axis.z() = (z() / scale);
  angle = acos( w() ) * 2.0f;
}

mat4d quat::toMatrix() const
{
  GLdouble x2 = x() * x();
  GLdouble y2 = y() * y();
  GLdouble z2 = z() * z();
  GLdouble xy = x() * y();
  GLdouble xz = x() * z();
  GLdouble yz = y() * z();
  GLdouble wx = w() * x();
  GLdouble wy = w() * y();
  GLdouble wz = w() * z();
 
  return mat4d( 
    (1.0f - 2.0f * (y2 + z2)), (2.0f * (xy + wz)),        (2.0f * (xz - wy)),        0.0f,
    (2.0f * (xy - wz)),        (1.0f - 2.0f * (x2 + z2)), (2.0f * (yz + wx)),        0.0f,
    (2.0f * (xz + wy)),        (2.0f * (yz - wx)),        (1.0f - 2.0f * (x2 + y2)), 0.0f,
    0.0f,                      0.0f,                      0.0f,                      1.0f );

}

quat quat::slerp( GLdouble t, const quat& a, const quat& b ) const
{
  const GLdouble eps = 0.00001;
  GLdouble scale_a, scale_b, omega, sinom;
  GLdouble cosom = a.dot(b);
  
  quat b2(b);
  
  if ( cosom < 0.0 )
  { 
      cosom = -cosom; 
      b2 = -b;
  }

  if( cosom < 1.0 - eps )
  {
    omega = acos(cosom);  
    sinom = sin(omega);
    scale_a = sin( (1.0-t) * omega) / sinom;
    scale_b = sin(  t      * omega) / sinom;
  }
  else
  {
    scale_a = 1.0 - t;
    scale_b = t;
  }

  return (a*scale_a) + (b2*scale_b);
}

const quat& quat::fromVectors(const vec3d& from, const vec3d& to)
{
  vec3d a,b;
  a = from;
  b = to;
  a.normalize();
  b.normalize();
  GLdouble cosa = ::dot(a,b);
  if ( fabs(cosa) < 0.0001f)
  {
    *this = quat().setNoRotation();
  }
  else
  {
    vec3d axis,n2;
    axis = cross(a,b);
    axis.normalize();
    GLdouble alpha = acos( cosa );
    *this = quat().fromAxisAngle(alpha*RAD_TO_DEG, axis);
  }
  return *this;
}

