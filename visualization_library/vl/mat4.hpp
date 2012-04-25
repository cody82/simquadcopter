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

#ifndef mat4_INCLUDE_DEFINE
#define mat4_INCLUDE_DEFINE

#include "vl/Object.hpp"
#include "vl/OpenGL.hpp"
#include "vl/vec4.hpp"
#include "vl/mat3.hpp"

namespace vl
{

  class mat4;
  mat4 operator*(const mat4& m2, const mat4& m1);

  class mat4
  {
  public:

    mat4()
    {
	    *this = I();
    }

    explicit mat4( GLfloat e00, GLfloat e01, GLfloat e02, GLfloat e03,
                   GLfloat e10, GLfloat e11, GLfloat e12, GLfloat e13,
                   GLfloat e20, GLfloat e21, GLfloat e22, GLfloat e23,
                   GLfloat e30, GLfloat e31, GLfloat e32, GLfloat e33 )
    {
	    e(0,0) = e00; e(0,1) = e01; e(0,2) = e02; e(0,3) = e03;
	    e(1,0) = e10; e(1,1) = e11; e(1,2) = e12; e(1,3) = e13;
	    e(2,0) = e20; e(2,1) = e21; e(2,2) = e22; e(2,3) = e23;
	    e(3,0) = e30; e(3,1) = e31; e(3,2) = e32; e(3,3) = e33;
    }

    const vec4& operator[](unsigned int i) const { CHECK(i<4); return mVec[i]; }

    vec4& operator[](unsigned int i) { CHECK(i<4); return mVec[i]; }

    vec3 getX() const;

    vec3 getY() const;

    vec3 getZ() const;

    vec3 getT() const;

    const mat4& setX(const vec3& v);

    const mat4& setY(const vec3& v);

    const mat4& setZ(const vec3& v);

    const mat4& setT(const vec3& v);

    bool operator==(const mat4& m) const 
    {
	    return memcmp(m.mVec, mVec, sizeof(GLfloat)*4*4) == 0;
    }

    bool operator!=(const mat4& m) const 
    {
	    return !(*this == m);
    }

    const mat4& operator=(const mat4& m) 
    {
	    memcpy(mVec, m.mVec, sizeof(GLfloat)*16);
	    return *this;
    }

    mat4 operator+(const mat4& m) const
    {
      mat4 t;
      for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
          t[i][j] = e(i,j) + m[i][j];

      return t;
    }

    const mat4& operator+=(const mat4& m)
    {
      return *this = *this + m;
    }

    mat4 operator-(const mat4& m) const
    {
      mat4 t;
      for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
          t[i][j] = e(i,j) - m[i][j];

      return t;
    }

    const mat4& operator-=(const mat4& m)
    {
      return *this = *this - m;
    }

    const mat4& operator*=(const mat4& m)
    {
      return *this = *this * m;
    }

    mat4 operator-() const
    {
      mat4 t;
      for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
          t[i][j] = -e(i,j);

      return t;
    }

    mat4 operator+(GLfloat f) const
    {
      mat4 t;
      for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
          t[i][j] = e(i,j) + f;

      return t;
    }

    const mat4& operator+=(GLfloat f)
    {
      return *this = *this + f;
    }

    mat4 operator-(GLfloat f) const
    {
      mat4 t;
      for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
          t[i][j] = e(i,j) - f;

      return t;
    }

    const mat4& operator-=(GLfloat f)
    {
      return *this = *this - f;
    }

    mat4 operator*(GLfloat f) const
    {
      mat4 t;
      for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
          t[i][j] = e(i,j) * f;

      return t;
    }

    const mat4& operator*=(GLfloat f)
    {
      mat4 t;
      for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
          e(i,j) *= f;

      return *this;
    }

    mat4 operator/(GLfloat f) const
    {
      f = 1.0f / f;
      mat4 t;
      for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
          t[i][j] = e(i,j) * f;

      return t;
    }

    const mat4& operator/=(GLfloat f)
    {
      f = 1.0f / f;
      mat4 t;
      for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
          e(i,j) *= f;

      return *this;
    }

    bool isIdentity() const
    {
      mat4 i = I();
      return memcmp( ptr(), i.ptr(), sizeof(GLfloat)*16 ) == 0;
    }

    mat3 get3x3() const
    {
      mat3 t;
      t.e(0,0) = e(0,0); t.e(0,1) = e(0,1); t.e(0,2) = e(0,2);
      t.e(1,0) = e(1,0); t.e(1,1) = e(1,1); t.e(1,2) = e(1,2);
      t.e(2,0) = e(2,0); t.e(2,1) = e(2,1); t.e(2,2) = e(2,2);
      return t;
    }

    void set3x3(const mat3& m)
    {
      e(0,0) = m.e(0,0); e(0,1) = m.e(0,1); e(0,2) = m.e(0,2);
      e(1,0) = m.e(1,0); e(1,1) = m.e(1,1); e(1,2) = m.e(1,2);
      e(2,0) = m.e(2,0); e(2,1) = m.e(2,1); e(2,2) = m.e(2,2);
    }

    GLfloat* ptr()
    {
	    return &e(0,0);
    }

    const GLfloat* ptr() const
    {
	    return &e(0,0);
    }

    const mat4& transpose()
    {
	    mat4 m;
	    for(int i=0; i<4; i++)
		    for(int j=0; j<4; j++) {
			    m.e(i,j) = e(j,i);
		    }
	    memcpy(mVec, m.mVec, sizeof(GLfloat)*16);
	    return *this;
    }

    bool isNull() const
    {
	    for(int i=0; i<4; i++)
		    for(int j=0; j<4; j++)
			    if( e(j,i) != 0.0 )
				    return false;
	    return true;
    }

    void setIdentity()
    {
      *this = I();
    }

    mat4 getCleanInverse(bool *invertible = NULL) const
    {
	    mat4 t = inverse(invertible);
	    t.e(0,3) = 0.0;
	    t.e(1,3) = 0.0;
	    t.e(2,3) = 0.0;
	    t.e(3,3) = 1.0;
	    return t;
    }

    mat4 inverse(bool *invertible=NULL) const;

    const mat4& invert(bool *invertible=NULL)
    {
      *this = inverse(invertible);
      return *this;
    }

    const GLfloat& e(int i, int j) const { return mVec[i][j]; }

    GLfloat& e(int i, int j) { return mVec[i][j]; }

    void setAsPerspectiveProjection(GLfloat fovy, GLfloat aspect_ratio, GLfloat znear, GLfloat zfar);

    void setAsFrustum( GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far);

    void setAsOrthographicProjection(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far);

    void setAsOrthographicProjection2D(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top);

    void setAsLookAt( const vec3& eye, const vec3& center, const vec3& up);

    void getAsLookAt( vec3& eye, vec3& look, vec3& up, vec3& right) const;

    static mat4 rotation( GLdouble degrees, GLdouble x, GLdouble y, GLdouble z );

    static mat4 rotation( GLdouble degrees, const vec3& v )
    {
      return rotation( degrees, v.x(), v.y(), v.z() );
    }

    static mat4 rotation( GLdouble degrees1, const vec3& v1, GLdouble degrees2, const vec3& v2 )
    {
      return rotation( degrees1, v1.x(), v1.y(), v1.z() ) * rotation( degrees2, v2.x(), v2.y(), v2.z() );
    }

    static mat4 rotation( GLdouble degrees1, const vec3& v1, GLdouble degrees2, const vec3& v2, GLdouble degrees3, const vec3& v3 )
    {
      return rotation( degrees1, v1.x(), v1.y(), v1.z() ) * rotation( degrees2, v2.x(), v2.y(), v2.z() ) * rotation( degrees3, v3.x(), v3.y(), v3.z() );
    }

    const mat4& rotate( GLdouble degrees, const vec3& v )
    {
      return rotate( degrees, v.x(), v.y(), v.z() );
    }

    const mat4& rotate( GLdouble degrees, GLdouble x, GLdouble y, GLdouble z )
    {
      *this = rotation(degrees, x, y, z) * *this;
      return *this;
    }

    const mat4& rotate( GLdouble degrees1, const vec3& v1, GLdouble degrees2, const vec3& v2 )
    {
      *this = rotation(degrees1, v1, degrees2, v2) * *this;
      return *this;
    }

    void getYXRotationAngles( GLdouble& degrees_y, GLdouble& degrees_x ) const;

    void getYXRotationAngles( GLfloat& degrees_y, GLfloat& degrees_x ) const
    {
      GLdouble y, x;
      getYXRotationAngles(y,x);
      degrees_x = (GLfloat)x;
      degrees_y = (GLfloat)y;
    }

    const mat4& rotate( GLdouble degrees1, const vec3& v1, GLdouble degrees2, const vec3& v2, GLdouble degrees3, const vec3& v3 )
    {
      *this = rotation(degrees1, v1, degrees2, v2, degrees3, v3) * *this;
      return *this;
    }

    static mat4 rotationXYZ( GLdouble degX, GLdouble degY, GLdouble degZ)
    {
      return rotation(degX, 1,0,0) * rotation(degY, 0,1,0) * rotation(degZ, 0,0,1);
    }

    const mat4& rotateXYZ( GLdouble degX, GLdouble degY, GLdouble degZ )
    {
      *this = rotationXYZ(degX, degY, degZ) * *this;
      return *this;
    }

    static mat4 rotationZYX( GLdouble degZ, GLdouble degY, GLdouble degX)
    {
      return rotation(degZ, 0,0,1) * rotation(degY, 0,1,0) * rotation(degX, 1,0,0);
    }

    const mat4& rotateZYX( GLdouble degZ, GLdouble degY, GLdouble degX )
    {
      *this = rotationZYX(degZ, degY, degX) * *this;
      return *this;
    }

    static mat4 rotation(const vec4& from, const vec4& to)
    {
      return rotation( from.xyz(), to.xyz() );
    }

    static mat4 rotation(const vec3& from, const vec3& to);

    const mat4& rotate(const vec4& from, const vec4& to)
    {
      *this = rotation(from, to) * *this;
      return *this;
    }

    const mat4& rotate(const vec3& from, const vec3& to)
    {
      *this = rotation(from, to) * *this;
      return *this;
    }

    static mat4 translation( const vec3& v )
    {
      return translation(v.x(), v.y(), v.z());
    }

    static mat4 translation( GLfloat x, GLfloat y, GLfloat z )
    {
	    mat4 tr = I();
	    tr.e(3,0) = x;
	    tr.e(3,1) = y;
	    tr.e(3,2) = z;
	    return tr;
    }

    const mat4& translate( GLfloat x, GLfloat y, GLfloat z )
    {
      *this = translation(x,y,z) * *this;
	    return *this;
    }

    const mat4& translate( const vec3& v )
    {
      *this = translation(v) * *this;
	    return *this;
    }

    static mat4 scaling( const vec3& v )
    {
      return scaling( v.x(), v.y(), v.z() );
    }

    static mat4 scaling( GLfloat x, GLfloat y, GLfloat z )
    {
	    mat4 sc = I();
	    sc.e(0,0) = x;
	    sc.e(1,1) = y;
	    sc.e(2,2) = z;
	    return sc;
    }

    const mat4& scale( GLfloat x, GLfloat y, GLfloat z )
    {
      *this = scaling(x,y,z) * *this;
	    return *this;
    }

    static mat4 I()
    {
      return mat4( 1,0,0,0,  0,1,0,0,  0,0,1,0, 0,0,0,1 );
    }

  protected:
	  vec4 mVec[4];
  };

  inline mat4 operator*(const mat4& m2, const mat4& m1)
  {
    mat4 t;
    t.e(0,0) = m1.e(0,0)*m2.e(0,0) + m1.e(0,1)*m2.e(1,0) + m1.e(0,2)*m2.e(2,0) + m1.e(0,3)*m2.e(3,0);
    t.e(1,0) = m1.e(1,0)*m2.e(0,0) + m1.e(1,1)*m2.e(1,0) + m1.e(1,2)*m2.e(2,0) + m1.e(1,3)*m2.e(3,0);
    t.e(2,0) = m1.e(2,0)*m2.e(0,0) + m1.e(2,1)*m2.e(1,0) + m1.e(2,2)*m2.e(2,0) + m1.e(2,3)*m2.e(3,0);
    t.e(3,0) = m1.e(3,0)*m2.e(0,0) + m1.e(3,1)*m2.e(1,0) + m1.e(3,2)*m2.e(2,0) + m1.e(3,3)*m2.e(3,0);

    t.e(0,1) = m1.e(0,0)*m2.e(0,1) + m1.e(0,1)*m2.e(1,1) + m1.e(0,2)*m2.e(2,1) + m1.e(0,3)*m2.e(3,1);
    t.e(1,1) = m1.e(1,0)*m2.e(0,1) + m1.e(1,1)*m2.e(1,1) + m1.e(1,2)*m2.e(2,1) + m1.e(1,3)*m2.e(3,1);
    t.e(2,1) = m1.e(2,0)*m2.e(0,1) + m1.e(2,1)*m2.e(1,1) + m1.e(2,2)*m2.e(2,1) + m1.e(2,3)*m2.e(3,1);
    t.e(3,1) = m1.e(3,0)*m2.e(0,1) + m1.e(3,1)*m2.e(1,1) + m1.e(3,2)*m2.e(2,1) + m1.e(3,3)*m2.e(3,1);

    t.e(0,2) = m1.e(0,0)*m2.e(0,2) + m1.e(0,1)*m2.e(1,2) + m1.e(0,2)*m2.e(2,2) + m1.e(0,3)*m2.e(3,2);
    t.e(1,2) = m1.e(1,0)*m2.e(0,2) + m1.e(1,1)*m2.e(1,2) + m1.e(1,2)*m2.e(2,2) + m1.e(1,3)*m2.e(3,2);
    t.e(2,2) = m1.e(2,0)*m2.e(0,2) + m1.e(2,1)*m2.e(1,2) + m1.e(2,2)*m2.e(2,2) + m1.e(2,3)*m2.e(3,2);
    t.e(3,2) = m1.e(3,0)*m2.e(0,2) + m1.e(3,1)*m2.e(1,2) + m1.e(3,2)*m2.e(2,2) + m1.e(3,3)*m2.e(3,2);

    t.e(0,3) = m1.e(0,0)*m2.e(0,3) + m1.e(0,1)*m2.e(1,3) + m1.e(0,2)*m2.e(2,3) + m1.e(0,3)*m2.e(3,3);
    t.e(1,3) = m1.e(1,0)*m2.e(0,3) + m1.e(1,1)*m2.e(1,3) + m1.e(1,2)*m2.e(2,3) + m1.e(1,3)*m2.e(3,3);
    t.e(2,3) = m1.e(2,0)*m2.e(0,3) + m1.e(2,1)*m2.e(1,3) + m1.e(2,2)*m2.e(2,3) + m1.e(2,3)*m2.e(3,3);
    t.e(3,3) = m1.e(3,0)*m2.e(0,3) + m1.e(3,1)*m2.e(1,3) + m1.e(3,2)*m2.e(2,3) + m1.e(3,3)*m2.e(3,3);
    return t;
  }

  inline mat4 operator+(GLfloat f, const mat4& m)
  {
    return m + f;
  }

  inline mat4 operator*(GLfloat f, const mat4& m)
  {
    return m * f;
  }

  inline vec4 operator*(const mat4& m, const vec4& v)
  {
    vec4 t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(1,0) + v.z()*m.e(2,0) + v.w()*m.e(3,0);
    t.y() = v.x()*m.e(0,1) + v.y()*m.e(1,1) + v.z()*m.e(2,1) + v.w()*m.e(3,1);
    t.z() = v.x()*m.e(0,2) + v.y()*m.e(1,2) + v.z()*m.e(2,2) + v.w()*m.e(3,2);
    t.w() = v.x()*m.e(0,3) + v.y()*m.e(1,3) + v.z()*m.e(2,3) + v.w()*m.e(3,3);
    return t;
  }

  inline vec3 operator*(const mat4& m, const vec3& v)
  {
    vec3 t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(1,0) + v.z()*m.e(2,0) + 1.0f*m.e(3,0);
    t.y() = v.x()*m.e(0,1) + v.y()*m.e(1,1) + v.z()*m.e(2,1) + 1.0f*m.e(3,1);
    t.z() = v.x()*m.e(0,2) + v.y()*m.e(1,2) + v.z()*m.e(2,2) + 1.0f*m.e(3,2);
    return t;
  }

  inline vec2 operator*(const mat4& m, const vec2& v)
  {
    vec2 t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(1,0) + /*0.0f*m.e(2,0) +*/ 1.0f*m.e(3,0);
    t.y() = v.x()*m.e(0,1) + v.y()*m.e(1,1) + /*0.0f*m.e(2,1) +*/ 1.0f*m.e(3,1);
    return t;
  }

  inline vec4 operator*(const vec4& v, const mat4& m)
  {
    vec4 t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(0,1) + v.z()*m.e(0,2) + v.w()*m.e(0,3);
    t.y() = v.x()*m.e(1,0) + v.y()*m.e(1,1) + v.z()*m.e(1,2) + v.w()*m.e(1,3);
    t.z() = v.x()*m.e(2,0) + v.y()*m.e(2,1) + v.z()*m.e(2,2) + v.w()*m.e(2,3);
    t.w() = v.x()*m.e(3,0) + v.y()*m.e(3,1) + v.z()*m.e(3,2) + v.w()*m.e(3,3);
    return t;
  }

  inline vec3 operator*(const vec3& v, const mat4& m)
  {
    vec3 t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(0,0) + v.z()*m.e(0,0) + 1.0f*m.e(0,0);
    t.y() = v.x()*m.e(1,0) + v.y()*m.e(1,0) + v.z()*m.e(1,0) + 1.0f*m.e(1,0);
    t.z() = v.x()*m.e(2,0) + v.y()*m.e(2,0) + v.z()*m.e(2,0) + 1.0f*m.e(2,0);
    return t;
  }

  inline vec2 operator*(const vec2& v, const mat4& m)
  {
    vec2 t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(0,1) + /*0.0f*m.e(2,0) +*/ 1.0f*m.e(0,3);
    t.y() = v.x()*m.e(1,0) + v.y()*m.e(1,1) + /*0.0f*m.e(2,1) +*/ 1.0f*m.e(1,3);
    return t;
  }

}

#endif
