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

#ifndef mat2_INCLUDE_DEFINE
#define mat2_INCLUDE_DEFINE

#include "vl/Object.hpp"
#include "vl/OpenGL.hpp"
#include "vl/vec2.hpp"

namespace vl
{

  class mat2;
  mat2 operator*(const mat2& m1, const mat2& m2);

  class mat2
  {
  public:

    mat2()
    {
	    *this = I();
    }

    explicit mat2( GLfloat e00, GLfloat e01,
                   GLfloat e10, GLfloat e11  )
    {
	    e(0,0) = e00; e(0,1) = e01;
	    e(1,0) = e10; e(1,1) = e11;
    }

    const vec2& operator[](unsigned int i) const { CHECK(i<2); return mVec[i]; }

    vec2& operator[](unsigned int i) { CHECK(i<2); return mVec[i]; }

    bool operator==(const mat2& m) const 
    {
	    return memcmp(m.mVec, mVec, sizeof(GLfloat)*4) == 0;
    }

    bool operator!=(const mat2& m) const 
    {
	    return !(*this == m);
    }

    const mat2& operator=(const mat2& m) 
    {
	    memcpy(mVec, m.mVec, sizeof(GLfloat)*4);
	    return *this;
    }

    mat2 operator+(const mat2& m) const
    {
      mat2 t;
      for(int i=0; i<2; i++)
        for(int j=0; j<2; j++)
          t[i][j] = e(i,j) + m[i][j];

      return t;
    }

    const mat2& operator+=(const mat2& m)
    {
      return *this = *this + m;
    }

    mat2 operator-(const mat2& m) const
    {
      mat2 t;
      for(int i=0; i<2; i++)
        for(int j=0; j<2; j++)
          t[i][j] = e(i,j) - m[i][j];

      return t;
    }

    const mat2& operator-=(const mat2& m)
    {
      return *this = *this - m;
    }

    const mat2& operator*=(const mat2& m)
    {
      return *this = *this * m;
    }

    mat2 operator-() const
    {
      mat2 t;
      for(int i=0; i<2; i++)
        for(int j=0; j<2; j++)
          t[i][j] = -e(i,j);

      return t;
    }

    mat2 operator+(GLfloat f) const
    {
      mat2 t;
      for(int i=0; i<2; i++)
        for(int j=0; j<2; j++)
          t[i][j] = e(i,j) + f;

      return t;
    }

    const mat2& operator+=(GLfloat f)
    {
      return *this = *this + f;
    }

    mat2 operator-(GLfloat f) const
    {
      mat2 t;
      for(int i=0; i<2; i++)
        for(int j=0; j<2; j++)
          t[i][j] = e(i,j) - f;

      return t;
    }

    const mat2& operator-=(GLfloat f)
    {
      return *this = *this - f;
    }

    mat2 operator*(GLfloat f) const
    {
      mat2 t;
      for(int i=0; i<2; i++)
        for(int j=0; j<2; j++)
          t[i][j] = e(i,j) * f;

      return t;
    }

    const mat2& operator*=(GLfloat f)
    {
      mat2 t;
      for(int i=0; i<2; i++)
        for(int j=0; j<2; j++)
          e(i,j) *= f;

      return *this;
    }

    mat2 operator/(GLfloat f) const
    {
      f = 1.0f / f;
      mat2 t;
      for(int i=0; i<2; i++)
        for(int j=0; j<2; j++)
          t[i][j] = e(i,j) * f;

      return t;
    }

    const mat2& operator/=(GLfloat f)
    {
      f = 1.0f / f;
      mat2 t;
      for(int i=0; i<2; i++)
        for(int j=0; j<2; j++)
          e(i,j) *= f;

      return *this;
    }

    bool isIdentity() const
    {
      mat2 i = I();
      return memcmp( ptr(), i.ptr(), sizeof(GLfloat)*4 ) == 0;
    }

    GLfloat* ptr()
    {
	    return &e(0,0);
    }

    const GLfloat* ptr() const
    {
	    return &e(0,0);
    }

    const mat2& transpose()
    {
	    mat2 m;
	    for(int i=0; i<2; i++)
		    for(int j=0; j<2; j++) {
			    m.mVec[i][j] = mVec[j][i];
		    }
	    memcpy(mVec, m.mVec, sizeof(GLfloat)*4);
	    return *this;
    }

    bool isNull() const
    {
	    for(int i=0; i<2; i++)
		    for(int j=0; j<2; j++)
			    if( mVec[j][i] != 0.0 )
				    return false;
	    return true;
    }

    void setIdentity()
    {
      *this = I();
    }

    mat2 inverse(bool *invertible=NULL) const
    {
      const float epsilon = 0.0001f;
      const float& a11 = e(0,0); 
      const float& a12 = e(0,1); 
      const float& a21 = e(1,0); 
      const float& a22 = e(1,1); 

      float DET = a11*a22-a12*a21;

      if (invertible)
        if (DET < epsilon)
          *invertible = false;
        else
          *invertible = true;
      
      mat2 m;
      m *= 0;

      if (DET < epsilon)
        return m;

      m = mat2(+a22, -a12, -a21, +a11) / DET;

      return m;
    }

    const mat2& invert(bool *invertible=NULL)
    {
      *this = inverse(invertible);
      return *this;
    }

    const GLfloat& e(int i, int j) const { return mVec[i][j]; }

    GLfloat& e(int i, int j) { return mVec[i][j]; }

    static mat2 I()
    {
      return mat2( 1,0, 0,1 );
    }

  protected:
	  vec2 mVec[2];
  };

  inline mat2 operator*(const mat2& m2, const mat2& m1)
  {
    mat2 t;
    t.e(0,0) = m1.e(0,0)*m2.e(0,0) + m1.e(0,1)*m2.e(1,0);
    t.e(1,0) = m1.e(1,0)*m2.e(0,0) + m1.e(1,1)*m2.e(1,0);

    t.e(0,1) = m1.e(0,0)*m2.e(0,1) + m1.e(0,1)*m2.e(1,1);
    t.e(1,1) = m1.e(1,0)*m2.e(0,1) + m1.e(1,1)*m2.e(1,1);

    return t;
  }

  inline mat2 operator+(GLfloat f, const mat2& m)
  {
    return m + f;
  }

  inline mat2 operator*(GLfloat f, const mat2& m)
  {
    return m * f;
  }

  inline vec2 operator*(const mat2& m, const vec2& v)
  {
    vec2 t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(1,0);
    t.y() = v.x()*m.e(0,1) + v.y()*m.e(1,1);
    return t;
  }

  inline vec2 operator*(const vec2& v, const mat2& m)
  {
    vec2 t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(0,1);
    t.y() = v.x()*m.e(1,0) + v.y()*m.e(1,1);
    return t;
  }

}

#endif
