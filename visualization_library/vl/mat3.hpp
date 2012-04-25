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

#ifndef Mat3_INCLUDE_DEFINE
#define Mat3_INCLUDE_DEFINE

#include "vl/Object.hpp"
#include "vl/OpenGL.hpp"
#include "vl/vec3.hpp"
#include "vl/mat2.hpp"

namespace vl
{

  class mat3;
  mat3 operator*(const mat3& m1, const mat3& m2);

  class mat3
  {
  public:

    mat3()
    {
	    *this = I();
    }

    explicit mat3( GLfloat e00, GLfloat e01, GLfloat e02,
                   GLfloat e10, GLfloat e11, GLfloat e12,
                   GLfloat e20, GLfloat e21, GLfloat e22 )
    {
	    e(0,0) = e00; e(0,1) = e01; e(0,2) = e02; 
	    e(1,0) = e10; e(1,1) = e11; e(1,2) = e12; 
	    e(2,0) = e20; e(2,1) = e21; e(2,2) = e22; 
    }

    const vec3& operator[](unsigned int i) const { CHECK(i<3); return mVec[i]; }

    vec3& operator[](unsigned int i) { CHECK(i<3); return mVec[i]; }

    vec2 getX() const;

    vec2 getY() const;

    vec2 getT() const;

    const mat3& setX(const vec2& v);

    const mat3& setY(const vec2& v);

    const mat3& setT(const vec2& v);

    bool operator==(const mat3& m) const 
    {
	    return memcmp(m.mVec, mVec, sizeof(GLfloat)*9) == 0;
    }

    bool operator!=(const mat3& m) const 
    {
	    return !(*this == m);
    }

    const mat3& operator=(const mat3& m) 
    {
	    memcpy(mVec, m.mVec, sizeof(GLfloat)*9);
	    return *this;
    }

    mat3 operator+(const mat3& m) const
    {
      mat3 t;
      for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
          t[i][j] = e(i,j) + m[i][j];

      return t;
    }

    const mat3& operator+=(const mat3& m)
    {
      return *this = *this + m;
    }

    mat3 operator-(const mat3& m) const
    {
      mat3 t;
      for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
          t[i][j] = e(i,j) - m[i][j];

      return t;
    }

    const mat3& operator-=(const mat3& m)
    {
      return *this = *this - m;
    }

    const mat3& operator*=(const mat3& m)
    {
      return *this = *this * m;
    }

    mat3 operator-() const
    {
      mat3 t;
      for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
          t[i][j] = -e(i,j);

      return t;
    }

    mat3 operator+(GLfloat f) const
    {
      mat3 t;
      for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
          t[i][j] = e(i,j) + f;

      return t;
    }

    const mat3& operator+=(GLfloat f)
    {
      return *this = *this + f;
    }

    mat3 operator-(GLfloat f) const
    {
      mat3 t;
      for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
          t[i][j] = e(i,j) - f;

      return t;
    }

    const mat3& operator-=(GLfloat f)
    {
      return *this = *this - f;
    }

    mat3 operator*(GLfloat f) const
    {
      mat3 t;
      for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
          t[i][j] = e(i,j) * f;

      return t;
    }

    const mat3& operator*=(GLfloat f)
    {
      mat3 t;
      for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
          e(i,j) *= f;

      return *this;
    }

    mat3 operator/(GLfloat f) const
    {
      f = 1.0f / f;
      mat3 t;
      for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
          t[i][j] = e(i,j) * f;

      return t;
    }

    const mat3& operator/=(GLfloat f)
    {
      f = 1.0f / f;
      mat3 t;
      for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
          e(i,j) *= f;

      return *this;
    }

    bool isIdentity() const
    {
      mat3 i = I();
      return memcmp( ptr(), i.ptr(), sizeof(GLfloat)*9 ) == 0;
    }

    mat2 get2x2() const
    {
      mat2 t;
      t.e(0,0) = e(0,0); t.e(0,1) = e(0,1);
      t.e(1,0) = e(1,0); t.e(1,1) = e(1,1);
      return t;
    }

    void set2x2(const mat2& m)
    {
      e(0,0) = m.e(0,0); e(0,1) = m.e(0,1);
      e(1,0) = m.e(1,0); e(1,1) = m.e(1,1);
    }

    GLfloat* ptr()
    {
	    return &e(0,0);
    }

    const GLfloat* ptr() const
    {
	    return &e(0,0);
    }

    const mat3& transpose()
    {
	    mat3 m;
	    for(int i=0; i<3; i++)
		    for(int j=0; j<3; j++) {
			    m.mVec[i][j] = mVec[j][i];
		    }
	    memcpy(mVec, m.mVec, sizeof(GLfloat)*9);
	    return *this;
    }

    bool isNull() const
    {
	    for(int i=0; i<3; i++)
		    for(int j=0; j<3; j++)
			    if( mVec[j][i] != 0.0 )
				    return false;
	    return true;
    }

    void setIdentity()
    {
      *this = I();
    }

    mat3 inverse(bool *invertible=NULL) const;

    const mat3& invert(bool *invertible=NULL)
    {
      *this = inverse(invertible);
      return *this;
    }

    const GLfloat& e(int i, int j) const { return mVec[i][j]; }

    GLfloat& e(int i, int j) { return mVec[i][j]; }

    static mat3 rotation( GLdouble degrees );

    const mat3& rotate( GLdouble degrees )
    {
      *this = rotation(degrees) * *this;
      return *this;
    }

    static mat3 translation( const vec2& v )
    {
      return translation( v.x(), v.y() );
    }

    static mat3 translation( GLfloat x, GLfloat y )
    {
	    mat3 tr = I();
	    tr.e(2,0) = x;
	    tr.e(2,1) = y;
	    return tr;
    }

    const mat3& translate( GLfloat x, GLfloat y )
    {
      *this = *this * translation(x,y);
	    return *this;
    }

    const mat3& translate( const vec2& v )
    {
      *this = translation(v) * *this;
	    return *this;
    }

    static mat3 scaling( GLfloat x, GLfloat y )
    {
	    mat3 sc = I();
	    sc.e(0,0) = x;
	    sc.e(1,1) = y;
	    return sc;
    }

    const mat3& scale( GLfloat x, GLfloat y )
    {
      *this = scaling(x,y) * *this;
	    return *this;
    }

    static mat3 I()
    {
      return mat3( 1,0,0,  0,1,0,  0,0,1 );
    }

  protected:
	  vec3 mVec[3];
  };

  inline mat3 operator*(const mat3& m2, const mat3& m1)
  {
    mat3 t;
    t.e(0,0) = m1.e(0,0)*m2.e(0,0) + m1.e(0,1)*m2.e(1,0) + m1.e(0,2)*m2.e(2,0);
    t.e(1,0) = m1.e(1,0)*m2.e(0,0) + m1.e(1,1)*m2.e(1,0) + m1.e(1,2)*m2.e(2,0);
    t.e(2,0) = m1.e(2,0)*m2.e(0,0) + m1.e(2,1)*m2.e(1,0) + m1.e(2,2)*m2.e(2,0);

    t.e(0,1) = m1.e(0,0)*m2.e(0,1) + m1.e(0,1)*m2.e(1,1) + m1.e(0,2)*m2.e(2,1);
    t.e(1,1) = m1.e(1,0)*m2.e(0,1) + m1.e(1,1)*m2.e(1,1) + m1.e(1,2)*m2.e(2,1);
    t.e(2,1) = m1.e(2,0)*m2.e(0,1) + m1.e(2,1)*m2.e(1,1) + m1.e(2,2)*m2.e(2,1);

    t.e(0,2) = m1.e(0,0)*m2.e(0,2) + m1.e(0,1)*m2.e(1,2) + m1.e(0,2)*m2.e(2,2);
    t.e(1,2) = m1.e(1,0)*m2.e(0,2) + m1.e(1,1)*m2.e(1,2) + m1.e(1,2)*m2.e(2,2);
    t.e(2,2) = m1.e(2,0)*m2.e(0,2) + m1.e(2,1)*m2.e(1,2) + m1.e(2,2)*m2.e(2,2);

    return t;
  }

  inline mat3 operator+(GLfloat f, const mat3& m)
  {
    return m + f;
  }

  inline mat3 operator*(GLfloat f, const mat3& m)
  {
    return m * f;
  }

  inline vec3 operator*(const mat3& m, const vec3& v)
  {
    vec3 t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(1,0) + v.z()*m.e(2,0);
    t.y() = v.x()*m.e(0,1) + v.y()*m.e(1,1) + v.z()*m.e(2,1);
    t.z() = v.x()*m.e(0,2) + v.y()*m.e(1,2) + v.z()*m.e(2,2);
    return t;
  }

  inline vec2 operator*(const mat3& m, const vec2& v)
  {
    vec2 t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(1,0) /*+ 0.0f*m.e(2,0)*/;
    t.y() = v.x()*m.e(0,1) + v.y()*m.e(1,1) /*+ 0.0f*m.e(2,1)*/;
    return t;
  }

  inline vec3 operator*(const vec3& v, const mat3& m)
  {
    vec3 t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(0,1) + v.z()*m.e(0,2);
    t.y() = v.x()*m.e(1,0) + v.y()*m.e(1,1) + v.z()*m.e(1,2);
    t.z() = v.x()*m.e(2,0) + v.y()*m.e(2,1) + v.z()*m.e(2,2);
    return t;
  }

  inline vec2 operator*(const vec2& v, const mat3& m)
  {
    vec2 t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(0,1) /*+ 0.0f*m.e(2,0)*/;
    t.y() = v.x()*m.e(1,0) + v.y()*m.e(1,1) /*+ 0.0f*m.e(2,1)*/;
    return t;
  }

}

#endif
