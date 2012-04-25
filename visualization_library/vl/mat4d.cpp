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

#include "vl/mat4d.hpp"
#include "vl/vec3d.hpp"
#include "vl/math3D.hpp"
#include <cstdio>
#include <cmath>

using namespace vl;

vec3d mat4d::getX() const
{
	vec3d v;
	v.x() = e(0,0);
	v.y() = e(0,1);
	v.z() = e(0,2);
	return v;
}

vec3d mat4d::getY() const
{
	vec3d v;
	v.x() = e(1,0);
	v.y() = e(1,1);
	v.z() = e(1,2);
	return v;
}

vec3d mat4d::getZ() const
{
	vec3d v;
	v.x() = e(2,0);
	v.y() = e(2,1);
	v.z() = e(2,2);
	return v;
}

vec3d mat4d::getT() const
{
	vec3d v;
	v.x() = e(3,0);
	v.y() = e(3,1);
	v.z() = e(3,2);
	return v;
}

const mat4d& mat4d::setX(const vec3d& v) 
{
	e(0,0) = v.x();
	e(0,1) = v.y();
	e(0,2) = v.z();
	return *this;
}

const mat4d& mat4d::setY(const vec3d& v) 
{
	e(1,0) = v.x();
	e(1,1) = v.y();
	e(1,2) = v.z();
	return *this;
}

const mat4d& mat4d::setZ(const vec3d& v) 
{
	e(2,0) = v.x();
	e(2,1) = v.y();
	e(2,2) = v.z();
	return *this;
}

const mat4d& mat4d::setT(const vec3d& v) 
{
	e(3,0) = v.x();
	e(3,1) = v.y();
	e(3,2) = v.z();
	return *this;
}

void mat4d::setAsLookAt( const vec3d& eye, const vec3d& center, const vec3d& up)
{
  setIdentity();
  vec3d y = vec3d(up).normalize();
  vec3d z = (eye - center).normalize(); // == -(center-eye)
  vec3d x = cross(-z,y).normalize();
  y = cross(z, x).normalize();
  setT(eye);
  setX(x);
  setY(y);
  setZ(z);
}

void mat4d::getAsLookAt( vec3d& eye, vec3d& look, vec3d& up, vec3d& right) const
{
  eye = getT();
  look = -getZ();
  up = getY();
  right = getX();
}

void mat4d::setAsPerspectiveProjection(GLdouble fovy, GLdouble aspect_ratio, GLdouble znear, GLdouble zfar)
{
  GLdouble rads = (fovy / 2.0) * (3.141592653589793238 / 180.0);
  GLdouble dz = zfar - znear;
  GLdouble sa = sin(rads);
  if ((dz == 0) || (sa == 0) || (aspect_ratio == 0)) 
    return;
  GLdouble ctan = cos(rads) / sa;

  e(0,0) = (GLdouble)(ctan / aspect_ratio);
  e(1,1) = (GLdouble)(ctan);
  e(2,2) = (GLdouble)(-(zfar + znear) / dz);
  e(2,3) = -1.0f;
  e(3,2) = (GLdouble)(-2.0f * znear * zfar / dz);
  e(3,3) = 0.0f;
}

#undef near
#undef far
#undef left
#undef right
#undef top
#undef bottom
void mat4d::setAsFrustum( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far)
{
  if (near <= 0.0 || far <= 0.0 || near == far || left == right || top == bottom)
    return;

  GLdouble x = (2.0f*near) / (right-left);
  GLdouble y = (2.0f*near) / (top-bottom);
  GLdouble a1 = (right+left) / (right-left);
  GLdouble a2 = (top+bottom) / (top-bottom);
  GLdouble b1 = -(far+near) / ( far-near);
  GLdouble b2 = -(2.0f*far*near) / (far-near);

  e(0,0) = x;     e(1,0) = 0.0f;  e(2,0) = a1;      e(3,0) = 0.0f;
  e(0,1) = 0.0f;  e(1,1) = y;     e(2,1) = a2;      e(3,1) = 0.0f;
  e(0,2) = 0.0f;  e(1,2) = 0.0f;  e(2,2) = b1;      e(3,2) = b2;
  e(0,3) = 0.0f;  e(1,3) = 0.0f;  e(2,3) = -1.0f;   e(3,3) = 0.0f;
}

void mat4d::setAsOrthographicProjection(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far)
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

void mat4d::setAsOrthographicProjection2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top)
{
  setAsOrthographicProjection(left, right, bottom, top, -1, +1);
}

mat4d mat4d::rotation( GLdouble degrees, GLdouble x, GLdouble y, GLdouble z )
{
  mat4d rot;

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

        rot.e(0,0) = (GLdouble)c;
        rot.e(1,1) = (GLdouble)c;
        if (z < 0.0f) 
        {
          rot.e(0,1) = -(GLdouble)s;
          rot.e(1,0) = (GLdouble)s;
        }
        else 
        {
          rot.e(0,1) = (GLdouble)s;
          rot.e(1,0) = -(GLdouble)s;
        }
        return rot;
      }
    }
    else if (z == 0.0f) 
    {

      rot.e(0,0) = (GLdouble)c;
      rot.e(2,2) = (GLdouble)c;
      if (y < 0.0f) 
      {
        rot.e(0,2) = (GLdouble)s;
        rot.e(2,0) = -(GLdouble)s;
      }
      else 
      {
        rot.e(0,2) = -(GLdouble)s;
        rot.e(2,0) = (GLdouble)s;
      }
      return rot;
    }
  }
  else if (y == 0.0f) 
  {
    if (z == 0.0f) 
    {

      rot.e(1,1) = (GLdouble)c;
      rot.e(2,2) = (GLdouble)c;
      if (x < 0.0f) 
      {
        rot.e(1,2) = -(GLdouble)s;
        rot.e(2,1) = (GLdouble)s;
      }
      else 
      {
        rot.e(1,2) = (GLdouble)s;
        rot.e(2,1) = -(GLdouble)s;
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

  rot.e(0,0) = (GLdouble)((one_c * xx) + c ); rot.e(0,1) = (GLdouble)((one_c * xy) + zs); rot.e(0,2) = (GLdouble)((one_c * zx) - ys);
  rot.e(1,0) = (GLdouble)((one_c * xy) - zs); rot.e(1,1) = (GLdouble)((one_c * yy) + c ); rot.e(1,2) = (GLdouble)((one_c * yz) + xs);
  rot.e(2,0) = (GLdouble)((one_c * zx) + ys); rot.e(2,1) = (GLdouble)((one_c * yz) - xs); rot.e(2,2) = (GLdouble)((one_c * zz) + c );
  return rot ;
}

mat4d mat4d::inverse(bool *invertible) const
{
  int i,j,k;
  int pvt_i[4], pvt_j[4];
  GLdouble pvt_val;
  GLdouble	tmp;
  GLdouble det;
  mat4d m;

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

mat4d mat4d::rotation(const vec3d& from, const vec3d& to)
{
  vec3d a,b;
  a = from;
  b = to;
  a.normalize();
  b.normalize();
  GLdouble cosa = dot(a,b);
  if ( fabs(cosa) < 0.0f)
  {
    return I();
  }
  else
  {
    vec3d axis,n2;
    axis = cross(a,b);
    axis.normalize();
    GLdouble alpha = acos( cosa );
    return rotation(alpha*RAD_TO_DEG, axis.x(), axis.y(), axis.z());
  }
}

void mat4d::getYXRotationAngles( GLdouble& degrees_y, GLdouble& degrees_x ) const
{
  vec3d vx = getX();
  vec3d vy = getY();
  vec3d vz = getZ();

  degrees_x = acos( dot(vy,vec3d(0,1,0)) ) * RAD_TO_DEG;
  if( dot(vz, vec3d(0,1,0)) > 0 )
    degrees_x = -degrees_x;

  degrees_y = acos( dot(vx, vec3d(1,0,0)) ) * RAD_TO_DEG;
  if( dot(vz, vec3d(1,0,0)) < 0 )
    degrees_y = -degrees_y;
  if (fabs(degrees_x) > 90.0)
    degrees_y = -degrees_y;
}

