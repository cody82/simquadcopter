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

#ifndef mat4d_INCLUDE_DEFINE
#define mat4d_INCLUDE_DEFINE

#include "vl/Object.hpp"
#include "vl/OpenGL.hpp"
#include "vl/vec4d.hpp"
#include "vl/mat3d.hpp"
#include "vl/mat4.hpp"

namespace vl
{

  class mat4d;
  mat4d operator*(const mat4d& m2, const mat4d& m1);

  class mat4d
  {
  public:

    explicit mat4d(const mat4& m)
    {
	    e(0,0) = m.e(0,0); e(0,1) = m.e(0,1); e(0,2) = m.e(0,2); e(0,3) = m.e(0,3);
	    e(1,0) = m.e(1,0); e(1,1) = m.e(1,1); e(1,2) = m.e(1,2); e(1,3) = m.e(1,3);
	    e(2,0) = m.e(2,0); e(2,1) = m.e(2,1); e(2,2) = m.e(2,2); e(2,3) = m.e(2,3);
	    e(3,0) = m.e(3,0); e(3,1) = m.e(3,1); e(3,2) = m.e(3,2); e(3,3) = m.e(3,3);
    }

    mat4d()
    {
	    *this = I();
    }

    explicit mat4d( GLdouble e00, GLdouble e01, GLdouble e02, GLdouble e03,
                    GLdouble e10, GLdouble e11, GLdouble e12, GLdouble e13,
                    GLdouble e20, GLdouble e21, GLdouble e22, GLdouble e23,
                    GLdouble e30, GLdouble e31, GLdouble e32, GLdouble e33 )
    {
	    e(0,0) = e00; e(0,1) = e01; e(0,2) = e02; e(0,3) = e03;
	    e(1,0) = e10; e(1,1) = e11; e(1,2) = e12; e(1,3) = e13;
	    e(2,0) = e20; e(2,1) = e21; e(2,2) = e22; e(2,3) = e23;
	    e(3,0) = e30; e(3,1) = e31; e(3,2) = e32; e(3,3) = e33;
    }

    const vec4d& operator[](unsigned int i) const { CHECK(i<4); return mVec[i]; }

    vec4d& operator[](unsigned int i) { CHECK(i<4); return mVec[i]; }

    vec3d getX() const;

    vec3d getY() const;

    vec3d getZ() const;

    vec3d getT() const;

    const mat4d& setX(const vec3d& v);

    const mat4d& setY(const vec3d& v);

    const mat4d& setZ(const vec3d& v);

    const mat4d& setT(const vec3d& v);

    bool operator==(const mat4d& m) const
    {
	    return memcmp(m.mVec, mVec, sizeof(GLdouble)*4*4) == 0;
    }

    bool operator!=(const mat4d& m) const
    {
	    return !(*this == m);
    }

    const mat4d& operator=(const mat4d& m)
    {
	    memcpy(mVec, m.mVec, sizeof(GLdouble)*16);
	    return *this;
    }

    mat4d operator+(const mat4d& m) const
    {
      mat4d t;
      for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
          t[i][j] = e(i,j) + m[i][j];

      return t;
    }

    const mat4d& operator+=(const mat4d& m)
    {
      return *this = *this + m;
    }

    mat4d operator-(const mat4d& m) const
    {
      mat4d t;
      for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
          t[i][j] = e(i,j) - m[i][j];

      return t;
    }

    const mat4d& operator-=(const mat4d& m)
    {
      return *this = *this - m;
    }

    const mat4d& operator*=(const mat4d& m)
    {
      return *this = *this * m;
    }

    mat4d operator-() const
    {
      mat4d t;
      for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
          t[i][j] = -e(i,j);

      return t;
    }

    mat4d operator+(GLdouble f) const
    {
      mat4d t;
      for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
          t[i][j] = e(i,j) + f;

      return t;
    }

    const mat4d& operator+=(GLdouble f)
    {
      return *this = *this + f;
    }

    mat4d operator-(GLdouble f) const
    {
      mat4d t;
      for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
          t[i][j] = e(i,j) - f;

      return t;
    }

    const mat4d& operator-=(GLdouble f)
    {
      return *this = *this - f;
    }

    mat4d operator*(GLdouble f) const
    {
      mat4d t;
      for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
          t[i][j] = e(i,j) * f;

      return t;
    }

    const mat4d& operator*=(GLdouble f)
    {
      mat4d t;
      for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
          e(i,j) *= f;

      return *this;
    }

    mat4d operator/(GLdouble f) const
    {
      f = 1.0f / f;
      mat4d t;
      for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
          t[i][j] = e(i,j) * f;

      return t;
    }

    const mat4d& operator/=(GLdouble f)
    {
      f = 1.0f / f;
      mat4d t;
      for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
          e(i,j) *= f;

      return *this;
    }

    bool isIdentity() const
    {
      mat4d i = I();
      return memcmp( ptr(), i.ptr(), sizeof(GLdouble)*16 ) == 0;
    }

    mat3d get3x3() const
    {
      mat3d t;
      t.e(0,0) = e(0,0); t.e(0,1) = e(0,1); t.e(0,2) = e(0,2);
      t.e(1,0) = e(1,0); t.e(1,1) = e(1,1); t.e(1,2) = e(1,2);
      t.e(2,0) = e(2,0); t.e(2,1) = e(2,1); t.e(2,2) = e(2,2);
      return t;
    }

    void set3x3(const mat3d& m)
    {
      e(0,0) = m.e(0,0); e(0,1) = m.e(0,1); e(0,2) = m.e(0,2);
      e(1,0) = m.e(1,0); e(1,1) = m.e(1,1); e(1,2) = m.e(1,2);
      e(2,0) = m.e(2,0); e(2,1) = m.e(2,1); e(2,2) = m.e(2,2);
    }

    GLdouble* ptr()
    {
	    return &e(0,0);
    }

    const GLdouble* ptr() const
    {
	    return &e(0,0);
    }

    const mat4d& transpose()
    {
	    mat4d m;
	    for(int i=0; i<4; i++)
		    for(int j=0; j<4; j++) {
			    m.e(i,j) = e(j,i);
		    }
	    memcpy(mVec, m.mVec, sizeof(GLdouble)*16);
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

    mat4d getCleanInverse(bool *invertible = NULL) const
    {
	    mat4d t = inverse(invertible);
	    t.e(0,3) = 0.0;
	    t.e(1,3) = 0.0;
	    t.e(2,3) = 0.0;
	    t.e(3,3) = 1.0;
	    return t;
    }

    mat4d inverse(bool *invertible=NULL) const;

    const mat4d& invert(bool *invertible=NULL)
    {
      *this = inverse(invertible);
      return *this;
    }

    const GLdouble& e(int i, int j) const { return mVec[i][j]; }

    GLdouble& e(int i, int j) { return mVec[i][j]; }

    void setAsPerspectiveProjection(GLdouble fovy, GLdouble aspect_ratio, GLdouble znear, GLdouble zfar);

    void setAsFrustum( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far);

    void setAsOrthographicProjection(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far);

    void setAsOrthographicProjection2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top);

    void setAsLookAt( const vec3d& eye, const vec3d& center, const vec3d& up);

    void getAsLookAt( vec3d& eye, vec3d& look, vec3d& up, vec3d& right) const;

    static mat4d rotation( GLdouble degrees, GLdouble x, GLdouble y, GLdouble z );

    static mat4d rotation( GLdouble degrees, const vec3d& v )
    {
      return rotation( degrees, v.x(), v.y(), v.z() );
    }

    static mat4d rotation( GLdouble degrees1, const vec3d& v1, GLdouble degrees2, const vec3d& v2 )
    {
      return rotation( degrees1, v1.x(), v1.y(), v1.z() ) * rotation( degrees2, v2.x(), v2.y(), v2.z() );
    }

    static mat4d rotation( GLdouble degrees1, const vec3d& v1, GLdouble degrees2, const vec3d& v2, GLdouble degrees3, const vec3d& v3 )
    {
      return rotation( degrees1, v1.x(), v1.y(), v1.z() ) * rotation( degrees2, v2.x(), v2.y(), v2.z() ) * rotation( degrees3, v3.x(), v3.y(), v3.z() );
    }

    const mat4d& rotate( GLdouble degrees, const vec3d& v )
    {
      return rotate( degrees, v.x(), v.y(), v.z() );
    }

    const mat4d& rotate( GLdouble degrees, GLdouble x, GLdouble y, GLdouble z )
    {
      *this = rotation(degrees, x, y, z) * *this;
      return *this;
    }

    const mat4d& rotate( GLdouble degrees1, const vec3d& v1, GLdouble degrees2, const vec3d& v2 )
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

    const mat4d& rotate( GLdouble degrees1, const vec3d& v1, GLdouble degrees2, const vec3d& v2, GLdouble degrees3, const vec3d& v3 )
    {
      *this = rotation(degrees1, v1, degrees2, v2, degrees3, v3) * *this;
      return *this;
    }

    static mat4d rotationXYZ( GLdouble degX, GLdouble degY, GLdouble degZ)
    {
      return rotation(degX, 1,0,0) * rotation(degY, 0,1,0) * rotation(degZ, 0,0,1);
    }

    const mat4d& rotateXYZ( GLdouble degX, GLdouble degY, GLdouble degZ )
    {
      *this = rotationXYZ(degX, degY, degZ) * *this;
      return *this;
    }

    static mat4d rotationZYX( GLdouble degZ, GLdouble degY, GLdouble degX)
    {
      return rotation(degZ, 0,0,1) * rotation(degY, 0,1,0) * rotation(degX, 1,0,0);
    }

    const mat4d& rotateZYX( GLdouble degZ, GLdouble degY, GLdouble degX )
    {
      *this = rotationZYX(degZ, degY, degX) * *this;
      return *this;
    }

    static mat4d rotation(const vec4d& from, const vec4d& to)
    {
      return rotation( from.xyz(), to.xyz() );
    }

    static mat4d rotation(const vec3d& from, const vec3d& to);

    const mat4d& rotate(const vec4d& from, const vec4d& to)
    {
      *this = rotation(from, to) * *this;
      return *this;
    }

    const mat4d& rotate(const vec3d& from, const vec3d& to)
    {
      *this = rotation(from, to) * *this;
      return *this;
    }

    static mat4d translation( const vec3d& v )
    {
      return translation(v.x(), v.y(), v.z());
    }

    static mat4d translation( GLdouble x, GLdouble y, GLdouble z )
    {
	    mat4d tr = I();
	    tr.e(3,0) = x;
	    tr.e(3,1) = y;
	    tr.e(3,2) = z;
	    return tr;
    }

    const mat4d& translate( GLdouble x, GLdouble y, GLdouble z )
    {
      *this = translation(x,y,z) * *this;
	    return *this;
    }

    const mat4d& translate( const vec3d& v )
    {
      *this = translation(v) * *this;
	    return *this;
    }

    static mat4d scaling( const vec3d& v )
    {
      return scaling( v.x(), v.y(), v.z() );
    }

    static mat4d scaling( GLdouble x, GLdouble y, GLdouble z )
    {
	    mat4d sc = I();
	    sc.e(0,0) = x;
	    sc.e(1,1) = y;
	    sc.e(2,2) = z;
	    return sc;
    }

    const mat4d& scale( GLdouble x, GLdouble y, GLdouble z )
    {
      *this = scaling(x,y,z) * *this;
	    return *this;
    }

    static mat4d I()
    {
      return mat4d( 1,0,0,0,  0,1,0,0,  0,0,1,0, 0,0,0,1 );
    }

  protected:
	  vec4d mVec[4];
  };

  inline mat4d operator*(const mat4d& m2, const mat4d& m1)
  {
    mat4d t;
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

  inline mat4d operator+(GLdouble f, const mat4d& m)
  {
    return m + f;
  }

  inline mat4d operator*(GLdouble f, const mat4d& m)
  {
    return m * f;
  }

  inline vec4d operator*(const mat4d& m, const vec4d& v)
  {
    vec4d t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(1,0) + v.z()*m.e(2,0) + v.w()*m.e(3,0);
    t.y() = v.x()*m.e(0,1) + v.y()*m.e(1,1) + v.z()*m.e(2,1) + v.w()*m.e(3,1);
    t.z() = v.x()*m.e(0,2) + v.y()*m.e(1,2) + v.z()*m.e(2,2) + v.w()*m.e(3,2);
    t.w() = v.x()*m.e(0,3) + v.y()*m.e(1,3) + v.z()*m.e(2,3) + v.w()*m.e(3,3);
    return t;
  }

  inline vec3d operator*(const mat4d& m, const vec3d& v)
  {
    vec3d t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(1,0) + v.z()*m.e(2,0) + 1.0f*m.e(3,0);
    t.y() = v.x()*m.e(0,1) + v.y()*m.e(1,1) + v.z()*m.e(2,1) + 1.0f*m.e(3,1);
    t.z() = v.x()*m.e(0,2) + v.y()*m.e(1,2) + v.z()*m.e(2,2) + 1.0f*m.e(3,2);
    return t;
  }

  inline vec2d operator*(const mat4d& m, const vec2d& v)
  {
    vec2d t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(1,0) + /*0.0f*m.e(2,0) +*/ 1.0f*m.e(3,0);
    t.y() = v.x()*m.e(0,1) + v.y()*m.e(1,1) + /*0.0f*m.e(2,1) +*/ 1.0f*m.e(3,1);
    return t;
  }

  inline vec4d operator*(const vec4d& v, const mat4d& m)
  {
    vec4d t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(0,1) + v.z()*m.e(0,2) + v.w()*m.e(0,3);
    t.y() = v.x()*m.e(1,0) + v.y()*m.e(1,1) + v.z()*m.e(1,2) + v.w()*m.e(1,3);
    t.z() = v.x()*m.e(2,0) + v.y()*m.e(2,1) + v.z()*m.e(2,2) + v.w()*m.e(2,3);
    t.w() = v.x()*m.e(3,0) + v.y()*m.e(3,1) + v.z()*m.e(3,2) + v.w()*m.e(3,3);
    return t;
  }

  inline vec3d operator*(const vec3d& v, const mat4d& m)
  {
    vec3d t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(0,0) + v.z()*m.e(0,0) + 1.0f*m.e(0,0);
    t.y() = v.x()*m.e(1,0) + v.y()*m.e(1,0) + v.z()*m.e(1,0) + 1.0f*m.e(1,0);
    t.z() = v.x()*m.e(2,0) + v.y()*m.e(2,0) + v.z()*m.e(2,0) + 1.0f*m.e(2,0);
    return t;
  }

  inline vec2d operator*(const vec2d& v, const mat4d& m)
  {
    vec2d t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(0,1) + /*0.0f*m.e(2,0) +*/ 1.0f*m.e(0,3);
    t.y() = v.x()*m.e(1,0) + v.y()*m.e(1,1) + /*0.0f*m.e(2,1) +*/ 1.0f*m.e(1,3);
    return t;
  }

  class MatrixObject: public Object
  {
  public:
    mat4d mMatrix;
  };

}

#endif
