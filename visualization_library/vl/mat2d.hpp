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

#ifndef mat2d_INCLUDE_DEFINE
#define mat2d_INCLUDE_DEFINE

#include "vl/Object.hpp"
#include "vl/OpenGL.hpp"
#include "vl/vec2d.hpp"
#include "vl/mat2.hpp"

namespace vl
{

  class mat2d;
  mat2d operator*(const mat2d& m1, const mat2d& m2);

  class mat2d
  {
  public:

    explicit mat2d(const mat2& m)
    {
	    e(0,0) = m.e(0,0); e(0,1) = m.e(0,1);
	    e(1,0) = m.e(1,0); e(1,1) = m.e(1,1);
    }

    mat2d()
    {
	    *this = I();
    }

    explicit mat2d( GLdouble e00, GLdouble e01,
                    GLdouble e10, GLdouble e11  )
    {
	    e(0,0) = e00; e(0,1) = e01;
	    e(1,0) = e10; e(1,1) = e11;
    }

    const vec2d& operator[](unsigned int i) const { CHECK(i<2); return mVec[i]; }

    vec2d& operator[](unsigned int i) { CHECK(i<2); return mVec[i]; }

    bool operator==(const mat2d& m) const 
    {
	    return memcmp(m.mVec, mVec, sizeof(GLdouble)*4) == 0;
    }

    bool operator!=(const mat2d& m) const 
    {
	    return !(*this == m);
    }

    const mat2d& operator=(const mat2d& m) 
    {
	    memcpy(mVec, m.mVec, sizeof(GLdouble)*4);
	    return *this;
    }

    mat2d operator+(const mat2d& m) const
    {
      mat2d t;
      for(int i=0; i<2; i++)
        for(int j=0; j<2; j++)
          t[i][j] = e(i,j) + m[i][j];

      return t;
    }

    const mat2d& operator+=(const mat2d& m)
    {
      return *this = *this + m;
    }

    mat2d operator-(const mat2d& m) const
    {
      mat2d t;
      for(int i=0; i<2; i++)
        for(int j=0; j<2; j++)
          t[i][j] = e(i,j) - m[i][j];

      return t;
    }

    const mat2d& operator-=(const mat2d& m)
    {
      return *this = *this - m;
    }

    const mat2d& operator*=(const mat2d& m)
    {
      return *this = *this * m;
    }

    mat2d operator-() const
    {
      mat2d t;
      for(int i=0; i<2; i++)
        for(int j=0; j<2; j++)
          t[i][j] = -e(i,j);

      return t;
    }

    mat2d operator+(GLdouble f) const
    {
      mat2d t;
      for(int i=0; i<2; i++)
        for(int j=0; j<2; j++)
          t[i][j] = e(i,j) + f;

      return t;
    }

    const mat2d& operator+=(GLdouble f)
    {
      return *this = *this + f;
    }

    mat2d operator-(GLdouble f) const
    {
      mat2d t;
      for(int i=0; i<2; i++)
        for(int j=0; j<2; j++)
          t[i][j] = e(i,j) - f;

      return t;
    }

    const mat2d& operator-=(GLdouble f)
    {
      return *this = *this - f;
    }

    mat2d operator*(GLdouble f) const
    {
      mat2d t;
      for(int i=0; i<2; i++)
        for(int j=0; j<2; j++)
          t[i][j] = e(i,j) * f;

      return t;
    }

    const mat2d& operator*=(GLdouble f)
    {
      mat2d t;
      for(int i=0; i<2; i++)
        for(int j=0; j<2; j++)
          e(i,j) *= f;

      return *this;
    }

    mat2d operator/(GLdouble f) const
    {
      f = 1.0f / f;
      mat2d t;
      for(int i=0; i<2; i++)
        for(int j=0; j<2; j++)
          t[i][j] = e(i,j) * f;

      return t;
    }

    const mat2d& operator/=(GLdouble f)
    {
      f = 1.0f / f;
      mat2d t;
      for(int i=0; i<2; i++)
        for(int j=0; j<2; j++)
          e(i,j) *= f;

      return *this;
    }

    bool isIdentity() const
    {
      mat2d i = I();
      return memcmp( ptr(), i.ptr(), sizeof(GLdouble)*4 ) == 0;
    }

    GLdouble* ptr()
    {
	    return &e(0,0);
    }

    const GLdouble* ptr() const
    {
	    return &e(0,0);
    }

    const mat2d& transpose()
    {
	    mat2d m;
	    for(int i=0; i<2; i++)
		    for(int j=0; j<2; j++) {
			    m.mVec[i][j] = mVec[j][i];
		    }
	    memcpy(mVec, m.mVec, sizeof(GLdouble)*4);
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

    mat2d inverse(bool *invertible=NULL) const
    {
      const double epsilon = 0.0001f;
      const double& a11 = e(0,0); 
      const double& a12 = e(0,1); 
      const double& a21 = e(1,0); 
      const double& a22 = e(1,1); 

      double DET = a11*a22-a12*a21;

      if (invertible)
        if (DET < epsilon)
          *invertible = false;
        else
          *invertible = true;
      
      mat2d m;
      m *= 0;

      if (DET < epsilon)
        return m;

      m = mat2d(+a22, -a12, -a21, +a11) / DET;

      return m;
    }

    const mat2d& invert(bool *invertible=NULL)
    {
      *this = inverse(invertible);
      return *this;
    }

    const GLdouble& e(int i, int j) const { return mVec[i][j]; }

    GLdouble& e(int i, int j) { return mVec[i][j]; }

    static mat2d I()
    {
      return mat2d( 1,0, 0,1 );
    }

  protected:
	  vec2d mVec[2];
  };

  inline mat2d operator*(const mat2d& m2, const mat2d& m1)
  {
    mat2d t;
    t.e(0,0) = m1.e(0,0)*m2.e(0,0) + m1.e(0,1)*m2.e(1,0);
    t.e(1,0) = m1.e(1,0)*m2.e(0,0) + m1.e(1,1)*m2.e(1,0);

    t.e(0,1) = m1.e(0,0)*m2.e(0,1) + m1.e(0,1)*m2.e(1,1);
    t.e(1,1) = m1.e(1,0)*m2.e(0,1) + m1.e(1,1)*m2.e(1,1);

    return t;
  }

  inline mat2d operator+(GLdouble f, const mat2d& m)
  {
    return m + f;
  }

  inline mat2d operator*(GLdouble f, const mat2d& m)
  {
    return m * f;
  }

  inline vec2d operator*(const mat2d& m, const vec2d& v)
  {
    vec2d t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(1,0);
    t.y() = v.x()*m.e(0,1) + v.y()*m.e(1,1);
    return t;
  }

  inline vec2d operator*(const vec2d& v, const mat2d& m)
  {
    vec2d t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(0,1);
    t.y() = v.x()*m.e(1,0) + v.y()*m.e(1,1);
    return t;
  }

}

#endif
