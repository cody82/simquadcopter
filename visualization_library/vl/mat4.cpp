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

#include <cstdio>
#include <cmath>
#include "vl/mat4.hpp"
#include "vl/vec3.hpp"
#include "vl/math3D.hpp"

using namespace vl;

vec3 mat4::getX() const
{
	vec3 v;
	v.x() = e(0,0);
	v.y() = e(0,1);
	v.z() = e(0,2);
	return v;
}

vec3 mat4::getY() const
{
	vec3 v;
	v.x() = e(1,0);
	v.y() = e(1,1);
	v.z() = e(1,2);
	return v;
}

vec3 mat4::getZ() const
{
	vec3 v;
	v.x() = e(2,0);
	v.y() = e(2,1);
	v.z() = e(2,2);
	return v;
}

vec3 mat4::getT() const
{
	vec3 v;
	v.x() = e(3,0);
	v.y() = e(3,1);
	v.z() = e(3,2);
	return v;
}

const mat4& mat4::setX(const vec3& v)
{
	e(0,0) = v.x();
	e(0,1) = v.y();
	e(0,2) = v.z();
	return *this;
}

const mat4& mat4::setY(const vec3& v)
{
	e(1,0) = v.x();
	e(1,1) = v.y();
	e(1,2) = v.z();
	return *this;
}

const mat4& mat4::setZ(const vec3& v)
{
	e(2,0) = v.x();
	e(2,1) = v.y();
	e(2,2) = v.z();
	return *this;
}

const mat4& mat4::setT(const vec3& v)
{
	e(3,0) = v.x();
	e(3,1) = v.y();
	e(3,2) = v.z();
	return *this;
}

void mat4::setAsLookAt( const vec3& eye, const vec3& center, const vec3& up)
{
  setIdentity();
  vec3 y = vec3(up).normalize();
  vec3 z = (eye - center).normalize(); // == -(center-eye)
  vec3 x = cross(-z,y).normalize();
  y = cross(z, x).normalize();
  setT(eye);
  setX(x);
  setY(y);
  setZ(z);
}

void mat4::getAsLookAt( vec3& eye, vec3& look, vec3& up, vec3& right) const
{
  eye = getT();
  look = -getZ();
  up = getY();
  right = getX();
}

void mat4::setAsPerspectiveProjection(GLfloat fovy, GLfloat aspect_ratio, GLfloat znear, GLfloat zfar)
{
  GLdouble rads = (fovy / 2.0) * (3.141592653589793238 / 180.0);
  GLdouble dz = zfar - znear;
  GLdouble sa = sin(rads);
  if ((dz == 0) || (sa == 0) || (aspect_ratio == 0))
    return;
  GLdouble ctan = cos(rads) / sa;

  e(0,0) = (GLfloat)(ctan / aspect_ratio);
  e(1,1) = (GLfloat)(ctan);
  e(2,2) = (GLfloat)(-(zfar + znear) / dz);
  e(2,3) = -1.0f;
  e(3,2) = (GLfloat)(-2.0f * znear * zfar / dz);
  e(3,3) = 0.0f;
}

#undef near
#undef far
#undef left
#undef right
#undef top
#undef bottom
void mat4::setAsFrustum( GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far)
{
  if (near <= 0.0 || far <= 0.0 || near == far || left == right || top == bottom)
    return;

  GLfloat x = (2.0f*near) / (right-left);
  GLfloat y = (2.0f*near) / (top-bottom);
  GLfloat a1 = (right+left) / (right-left);
  GLfloat a2 = (top+bottom) / (top-bottom);
  GLfloat b1 = -(far+near) / ( far-near);
  GLfloat b2 = -(2.0f*far*near) / (far-near);

  e(0,0) = x;     e(1,0) = 0.0f;  e(2,0) = a1;      e(3,0) = 0.0f;
  e(0,1) = 0.0f;  e(1,1) = y;     e(2,1) = a2;      e(3,1) = 0.0f;
  e(0,2) = 0.0f;  e(1,2) = 0.0f;  e(2,2) = b1;      e(3,2) = b2;
  e(0,3) = 0.0f;  e(1,3) = 0.0f;  e(2,3) = -1.0f;   e(3,3) = 0.0f;
}

void mat4::setAsOrthographicProjection(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far)
{
   e(0,0) = 2.0f / (right-left);
   e(1,0) = 0.0f;
   e(2,0) = 0.0f;
   e(3,0) = -(right+left) / (right-left);

   e(0,1) = 0.0f;
   e(1,1) = 2.0f / (top-bottom);
   e(2,1) = 0.0f;
   e(3,1) = -(top+bottom) / (top-bottom);

   e(0,2) = 0.0f;
   e(1,2) = 0.0f;
   e(2,2) = -2.0f / (far-near);
   e(3,2) = -(far+near) / (far-near);

   e(0,3) = 0.0f;
   e(1,3) = 0.0f;
   e(2,3) = 0.0f;
   e(3,3) = 1.0f;
}

void mat4::setAsOrthographicProjection2D(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top)
{
  setAsOrthographicProjection(left, right, bottom, top, -1, +1);
}

mat4 mat4::rotation( GLdouble degrees, GLdouble x, GLdouble y, GLdouble z )
{
  mat4 rot;

  if ( degrees == 0.0f || (x == 0 && y ==0 && z == 0) )
	  return rot;

  degrees = degrees * (3.14159265f / 180.0f);

  GLdouble xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c, s, c;

  s = (GLdouble) sin( degrees );
  c = (GLdouble) cos( degrees );

  if (x == 0.0f)
  {
    if (y == 0.0f)
    {
      if (z != 0.0f)
      {

        rot.e(0,0) = (GLfloat)c;
        rot.e(1,1) = (GLfloat)c;
        if (z < 0.0f)
        {
          rot.e(0,1) = -(GLfloat)s;
          rot.e(1,0) = (GLfloat)s;
        }
        else
        {
          rot.e(0,1) = (GLfloat)s;
          rot.e(1,0) = -(GLfloat)s;
        }
        return rot;
      }
    }
    else if (z == 0.0f)
    {

      rot.e(0,0) = (GLfloat)c;
      rot.e(2,2) = (GLfloat)c;
      if (y < 0.0f)
      {
        rot.e(0,2) = (GLfloat)s;
        rot.e(2,0) = -(GLfloat)s;
      }
      else
      {
        rot.e(0,2) = -(GLfloat)s;
        rot.e(2,0) = (GLfloat)s;
      }
      return rot;
    }
  }
  else if (y == 0.0f)
  {
    if (z == 0.0f)
    {

      rot.e(1,1) = (GLfloat)c;
      rot.e(2,2) = (GLfloat)c;
      if (x < 0.0f)
      {
        rot.e(1,2) = -(GLfloat)s;
        rot.e(2,1) = (GLfloat)s;
      }
      else
      {
        rot.e(1,2) = (GLfloat)s;
        rot.e(2,1) = -(GLfloat)s;
      }
      return rot;
    }
  }

  GLdouble dot = x*x + y*y + z*z;

  if (dot > 1.00001 || dot < 0.99999)
  {
	  GLdouble mag = (GLdouble) sqrt(dot);
	  x /= mag;
	  y /= mag;
	  z /= mag;
  }

  xx = x *x;
  yy = y * y;
  zz = z * z;
  xy = x * y;
  yz = y * z;
  zx = z * x;
  xs = x * s;
  ys = y * s;
  zs = z * s;
  one_c = 1.0f - c;

  rot.e(0,0) = (GLfloat)((one_c * xx) + c ); rot.e(0,1) = (GLfloat)((one_c * xy) + zs); rot.e(0,2) = (GLfloat)((one_c * zx) - ys);
  rot.e(1,0) = (GLfloat)((one_c * xy) - zs); rot.e(1,1) = (GLfloat)((one_c * yy) + c ); rot.e(1,2) = (GLfloat)((one_c * yz) + xs);
  rot.e(2,0) = (GLfloat)((one_c * zx) + ys); rot.e(2,1) = (GLfloat)((one_c * yz) - xs); rot.e(2,2) = (GLfloat)((one_c * zz) + c );
  return rot ;
}

mat4 mat4::inverse(bool *invertible) const
{
  int i,j,k;
  int pvt_i[4], pvt_j[4];
  GLfloat pvt_val;
  GLfloat	tmp;
  GLfloat det;
  mat4 m;

  if (invertible)
	  *invertible = true;

  m = *this;

  det = 1.0f;
  for (k=0; k<4; k++)
  {
	  pvt_val=m[k][k];
	  pvt_i[k]=k;
	  pvt_j[k]=k;
	  for (i=k; i<4; i++)
    {
	    for (j=k; j<4; j++)
      {
		    if (fabs(m[i][j]) > fabs(pvt_val))
        {
		      pvt_i[k]=i;
		      pvt_j[k]=j;
		      pvt_val=m[i][j];
		    }
		  }
  	}

	  det*=pvt_val;
	  if (fabs(det)<0.0001)
    {
	    if (invertible)
		    *invertible = false;
      m *= 0;
	    return m;
	  }

	  i=pvt_i[k];
	  if (i!=k)
    {
	    for (j=0; j<4; j++)
      {
		    tmp=-m[k][j];
		    m[k][j]=m[i][j];
		    m[i][j]=tmp;
	    }
	  }

	  j=pvt_j[k];
	  if (j!=k)
    {
	    for (i=0; i<4; i++)
      {
		    tmp=-m[i][k];
		    m[i][k]=m[i][j];
		    m[i][j]=tmp;
	    }
	  }

	  for (i=0; i<4; i++) {
	    if (i!=k) m[i][k]/=( -pvt_val) ;
	  }

	  for (i=0; i<4; i++)
    {
	    tmp = m[i][k];
	    for (j=0; j<4; j++)
      {
		    if (i!=k && j!=k) m[i][j]+=tmp*m[k][j];
	    }
	  }

	  for (j=0; j<4; j++)
    {
	    if (j!=k) m[k][j]/=pvt_val;
	  }

	  m[k][k] = 1.0f/pvt_val;
  }

  for (k=4-2; k>=0; k--)
  {
	  i=pvt_j[k];
	  if (i!=k)
    {
	    for(j=0; j<4; j++)
      {
		    tmp = m[k][j];
		    m[k][j]=-m[i][j];
		    m[i][j]=tmp;
	    }
	  }

	  j=pvt_i[k];
	  if (j!=k)
	  for (i=0; i<4; i++)
    {
	    tmp=m[i][k];
	    m[i][k]=-m[i][j];
	    m[i][j]=tmp;
	  }
  }

  return m;
}

mat4 mat4::rotation(const vec3& from, const vec3& to)
{
  vec3 a,b;
  a = from;
  b = to;
  a.normalize();
  b.normalize();
  GLfloat cosa = dot(a,b);
  if ( fabs(cosa) < 0.0f)
  {
    return I();
  }
  else
  {
    vec3 axis,n2;
    axis = cross(a,b);
    axis.normalize();
    GLfloat alpha = acos( cosa );
    return rotation(alpha*RAD_TO_DEG, axis.x(), axis.y(), axis.z());
  }
}

void mat4::getYXRotationAngles( GLdouble& degrees_y, GLdouble& degrees_x ) const
{
  vec3 vx = getX();
  vec3 vy = getY();
  vec3 vz = getZ();

  degrees_x = acos( dot(vy,vec3(0,1,0)) ) * RAD_TO_DEG;
  if( dot(vz, vec3(0,1,0)) > 0 )
    degrees_x = -degrees_x;

  degrees_y = acos( dot(vx, vec3(1,0,0)) ) * RAD_TO_DEG;
  if( dot(vz, vec3(1,0,0)) < 0 )
    degrees_y = -degrees_y;
  if (fabs(degrees_x) > 90.0)
    degrees_y = -degrees_y;
}

