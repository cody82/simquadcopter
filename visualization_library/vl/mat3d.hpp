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

#ifndef mat3d_INCLUDE_DEFINE
#define mat3d_INCLUDE_DEFINE

#include "vl/Object.hpp"
#include "vl/OpenGL.hpp"
#include "vl/vec3d.hpp"
#include "vl/mat2d.hpp"
#include "vl/mat3.hpp"

namespace vl
{

  class mat3d;
  mat3d operator*(const mat3d& m1, const mat3d& m2);

  class mat3d
  {
  public:

    explicit mat3d(const mat3& m)
    {
	    e(0,0) = m.e(0,0); e(0,1) = m.e(0,1); e(0,2) = m.e(0,2);
	    e(1,0) = m.e(1,0); e(1,1) = m.e(1,1); e(1,2) = m.e(1,2);
	    e(2,0) = m.e(2,0); e(2,1) = m.e(2,1); e(2,2) = m.e(2,2);
    }

    mat3d()
    {
	    *this = I();
    }

    explicit mat3d( GLdouble e00, GLdouble e01, GLdouble e02,
                   GLdouble e10, GLdouble e11, GLdouble e12,
                   GLdouble e20, GLdouble e21, GLdouble e22 )
    {
	    e(0,0) = e00; e(0,1) = e01; e(0,2) = e02; 
	    e(1,0) = e10; e(1,1) = e11; e(1,2) = e12; 
	    e(2,0) = e20; e(2,1) = e21; e(2,2) = e22; 
    }

    const vec3d& operator[](unsigned int i) const { CHECK(i<3); return mVec[i]; }

    vec3d& operator[](unsigned int i) { CHECK(i<3); return mVec[i]; }

    vec2d getX() const;

    vec2d getY() const;

    vec2d getT() const;

    const mat3d& setX(const vec2d& v);

    const mat3d& setY(const vec2d& v);

    const mat3d& setT(const vec2d& v);

    bool operator==(const mat3d& m) const 
    {
	    return memcmp(m.mVec, mVec, sizeof(GLdouble)*9) == 0;
    }

    bool operator!=(const mat3d& m) const 
    {
	    return !(*this == m);
    }

    const mat3d& operator=(const mat3d& m) 
    {
	    memcpy(mVec, m.mVec, sizeof(GLdouble)*9);
	    return *this;
    }

    mat3d operator+(const mat3d& m) const
    {
      mat3d t;
      for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
          t[i][j] = e(i,j) + m[i][j];

      return t;
    }

    const mat3d& operator+=(const mat3d& m)
    {
      return *this = *this + m;
    }

    mat3d operator-(const mat3d& m) const
    {
      mat3d t;
      for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
          t[i][j] = e(i,j) - m[i][j];

      return t;
    }

    const mat3d& operator-=(const mat3d& m)
    {
      return *this = *this - m;
    }

    const mat3d& operator*=(const mat3d& m)
    {
      return *this = *this * m;
    }

    mat3d operator-() const
    {
      mat3d t;
      for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
          t[i][j] = -e(i,j);

      return t;
    }

    mat3d operator+(GLdouble f) const
    {
      mat3d t;
      for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
          t[i][j] = e(i,j) + f;

      return t;
    }

    const mat3d& operator+=(GLdouble f)
    {
      return *this = *this + f;
    }

    mat3d operator-(GLdouble f) const
    {
      mat3d t;
      for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
          t[i][j] = e(i,j) - f;

      return t;
    }

    const mat3d& operator-=(GLdouble f)
    {
      return *this = *this - f;
    }

    mat3d operator*(GLdouble f) const
    {
      mat3d t;
      for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
          t[i][j] = e(i,j) * f;

      return t;
    }

    const mat3d& operator*=(GLdouble f)
    {
      mat3d t;
      for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
          e(i,j) *= f;

      return *this;
    }

    mat3d operator/(GLdouble f) const
    {
      f = 1.0f / f;
      mat3d t;
      for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
          t[i][j] = e(i,j) * f;

      return t;
    }

    const mat3d& operator/=(GLdouble f)
    {
      f = 1.0f / f;
      mat3d t;
      for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
          e(i,j) *= f;

      return *this;
    }

    bool isIdentity() const
    {
      mat3d i = I();
      return memcmp( ptr(), i.ptr(), sizeof(GLdouble)*9 ) == 0;
    }

    mat2d get2x2() const
    {
      mat2d t;
      t.e(0,0) = e(0,0); t.e(0,1) = e(0,1);
      t.e(1,0) = e(1,0); t.e(1,1) = e(1,1);
      return t;
    }

    void set2x2(const mat2d& m)
    {
      e(0,0) = m.e(0,0); e(0,1) = m.e(0,1);
      e(1,0) = m.e(1,0); e(1,1) = m.e(1,1);
    }

    GLdouble* ptr()
    {
	    return &e(0,0);
    }

    const GLdouble* ptr() const
    {
	    return &e(0,0);
    }

    const mat3d& transpose()
    {
	    mat3d m;
	    for(int i=0; i<3; i++)
		    for(int j=0; j<3; j++) {
			    m.mVec[i][j] = mVec[j][i];
		    }
	    memcpy(mVec, m.mVec, sizeof(GLdouble)*9);
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

    mat3d inverse(bool *invertible=NULL) const;

    const mat3d& invert(bool *invertible=NULL)
    {
      *this = inverse(invertible);
      return *this;
    }

    const GLdouble& e(int i, int j) const { return mVec[i][j]; }

    GLdouble& e(int i, int j) { return mVec[i][j]; }

    static mat3d rotation( GLdouble degrees );

    const mat3d& rotate( GLdouble degrees )
    {
      *this = rotation(degrees) * *this;
      return *this;
    }

    static mat3d translation( const vec2d& v )
    {
      return translation( v.x(), v.y() );
    }

    static mat3d translation( GLdouble x, GLdouble y )
    {
	    mat3d tr = I();
	    tr.e(2,0) = x;
	    tr.e(2,1) = y;
	    return tr;
    }

    const mat3d& translate( GLdouble x, GLdouble y )
    {
      *this = *this * translation(x,y);
	    return *this;
    }

    const mat3d& translate( const vec2d& v )
    {
      *this = translation(v) * *this;
	    return *this;
    }

    static mat3d scaling( GLdouble x, GLdouble y )
    {
	    mat3d sc = I();
	    sc.e(0,0) = x;
	    sc.e(1,1) = y;
	    return sc;
    }

    const mat3d& scale( GLdouble x, GLdouble y )
    {
      *this = scaling(x,y) * *this;
	    return *this;
    }

    static mat3d I()
    {
      return mat3d( 1,0,0,  0,1,0,  0,0,1 );
    }

  protected:
	  vec3d mVec[3];
  };

  inline mat3d operator*(const mat3d& m2, const mat3d& m1)
  {
    mat3d t;
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

  inline mat3d operator+(GLdouble f, const mat3d& m)
  {
    return m + f;
  }

  inline mat3d operator*(GLdouble f, const mat3d& m)
  {
    return m * f;
  }

  inline vec3d operator*(const mat3d& m, const vec3d& v)
  {
    vec3d t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(1,0) + v.z()*m.e(2,0);
    t.y() = v.x()*m.e(0,1) + v.y()*m.e(1,1) + v.z()*m.e(2,1);
    t.z() = v.x()*m.e(0,2) + v.y()*m.e(1,2) + v.z()*m.e(2,2);
    return t;
  }

  inline vec2d operator*(const mat3d& m, const vec2d& v)
  {
    vec2d t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(1,0) /*+ 0.0f*m.e(2,0)*/;
    t.y() = v.x()*m.e(0,1) + v.y()*m.e(1,1) /*+ 0.0f*m.e(2,1)*/;
    return t;
  }

  inline vec3d operator*(const vec3d& v, const mat3d& m)
  {
    vec3d t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(0,1) + v.z()*m.e(0,2);
    t.y() = v.x()*m.e(1,0) + v.y()*m.e(1,1) + v.z()*m.e(1,2);
    t.z() = v.x()*m.e(2,0) + v.y()*m.e(2,1) + v.z()*m.e(2,2);
    return t;
  }

  inline vec2d operator*(const vec2d& v, const mat3d& m)
  {
    vec2d t;
    t.x() = v.x()*m.e(0,0) + v.y()*m.e(0,1) /*+ 0.0f*m.e(2,0)*/;
    t.y() = v.x()*m.e(1,0) + v.y()*m.e(1,1) /*+ 0.0f*m.e(2,1)*/;
    return t;
  }

}

#endif
