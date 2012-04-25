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

#ifndef vec3_INCLUDE_DEFINE
#define vec3_INCLUDE_DEFINE

#include "vl/vec2.hpp"

namespace vl
{

  class vec3 
  {
  public:

    explicit vec3(GLuint rgb)
    {
      GLubyte r = GLubyte((rgb >> 16) & 0xFF);
      GLubyte g = GLubyte((rgb >> 8) & 0xFF);
      GLubyte b = GLubyte(rgb & 0xFF);
	    mVec[0] = r / 255.0f;
	    mVec[1] = g / 255.0f;
	    mVec[2] = b / 255.0f;
    }
    explicit vec3(const vec2& v, GLfloat z) 
    {
	    mVec[0] = v.x();
	    mVec[1] = v.y();
	    mVec[2] = z;
    }
    explicit vec3(GLfloat x, GLfloat y, GLfloat z) 
    {
	    mVec[0] = x;
	    mVec[1] = y;
	    mVec[2] = z;
    }
    vec3()
    {
	    mVec[0] = 0.0f;
	    mVec[1] = 0.0f;
	    mVec[2] = 0.0f;
    }

	  GLfloat& x() { return mVec[0]; }

	  GLfloat& y() { return mVec[1]; }

	  GLfloat& z() { return mVec[2]; }

	  GLfloat& r() { return mVec[0]; }

	  GLfloat& g() { return mVec[1]; }

	  GLfloat& b() { return mVec[2]; }

	  GLfloat& s() { return mVec[0]; }

	  GLfloat& t() { return mVec[1]; }

	  GLfloat& p() { return mVec[2]; }

	  const GLfloat& x() const { return mVec[0]; }

	  const GLfloat& y() const { return mVec[1]; }

	  const GLfloat& z() const { return mVec[2]; }

	  const GLfloat& r() const { return mVec[0]; }

	  const GLfloat& g() const { return mVec[1]; }

	  const GLfloat& b() const { return mVec[2]; }

	  const GLfloat& s() const { return mVec[0]; }

	  const GLfloat& t() const { return mVec[1]; }

	  const GLfloat& p() const { return mVec[2]; }

	  const vec3& xyz() const { return *this; }
	  const vec3& stp() const { return *this; }
	  const vec3& rgb() const { return *this; }

	  vec2 xy() const { vec2 v; v.x() = x(); v.y() = y(); return v; }
	  vec2 st() const { return xy(); }

    GLfloat& operator[](unsigned int i) { CHECK(i<3); return ptr()[i]; }

    const GLfloat& operator[](unsigned int i) const { CHECK(i<3); return mVec[i]; }

	  GLfloat* ptr() { return mVec; }

	  const GLfloat* ptr() const { return mVec; }

    vec3 operator=(const vec3& v) 
    {
	    x() = v.x();
	    y() = v.y();
	    z() = v.z();
	    return v;
    }

    const vec3& operator=(GLfloat f) 
    {
	    x() = f;
	    y() = f;
	    z() = f;
	    return *this;
    }

    vec3 operator+(vec3 v) const 
    {
	    return vec3(x()+v.x(), y()+v.y(), z()+v.z());
    }

    const vec3& operator+=(vec3 v) 
    {
      *this = vec3(x()+v.x(), y()+v.y(), z()+v.z());
      return *this;
    }

    vec3 operator+(GLfloat f) const 
    {
	    return vec3(x()+f, y()+f, z()+f);
    }

    vec3 operator-() const 
    {
	    return vec3(-x(), -y(), -z());
    }

    vec3 operator-(vec3 v) const 
    {
	    return vec3(x()-v.x(), y()-v.y(), z()-v.z());
    }

    const vec3& operator-=(vec3 v) 
    {
      *this = vec3(x()-v.x(), y()-v.y(), z()-v.z());
      return *this;
    }

    vec3 operator-(GLfloat f) const 
    {
	    return vec3(x()-f, y()-f, z()-f);
    }

    vec3 operator*(const vec3& v) const
    {
      return vec3( x()*v.x(), y()*v.y(), z()*v.z() );
    }

    const vec3& operator*=(const vec3& v)
    {
      *this = *this * v;
      return *this;
    }

    vec3 operator*(GLfloat f) const
    {
	    return vec3(x()*f, y()*f, z()*f);
    }

    const vec3& operator*=(GLfloat f)
    {
	    *this = *this * f;
      return *this;
    }

    vec3 operator/(const vec3& v) const
    {
      return vec3( x()/v.x(), y()/v.y(), z()/v.z() );
    }

    const vec3& operator/=(const vec3& v)
    {
      *this = *this / v;
      return *this;
    }

    vec3 operator/(GLfloat f) const
    {
      f = 1.0f / f;
	    return vec3(x()*f, y()*f, z()*f);
    }

    const vec3& operator/=(GLfloat f)
    {
	    *this = *this / f;
      return *this;
    }

    GLfloat length() const { return sqrt( x()*x()+y()*y()+z()*z() ); }

    GLfloat lengthSquared() const { return x()*x()+y()*y()+z()*z(); } 

    const vec3& normalize()
    {
	    GLfloat l = length();

	    if (l>0.00001) {
		    l = 1.0f/l;
		    x() *= l;
		    y() *= l;
		    z() *= l;
	    }
      else
        *this = 0;
	    return *this;
    }

    bool operator==(const vec3& v) const
    {
	    return x() == v.x() && y() == v.y() && z() == v.z();
    }

    bool operator!=(const vec3& v) const
    {
	    return x() != v.x() || y() != v.y() || z() != v.z();
    }

    bool operator<(const vec3& v) const
    {
      if (x() != v.x())
        return x() < v.x();
      if (y() != v.y())
        return y() < v.y();
      else
        return z() < v.z();
    }

    bool isNull() const
    {
	    if ( x() || y() || z() )
		    return false;
	    else
		    return true;
    }

  protected:
	  GLfloat mVec[3];
  };

  inline vec3 operator*(GLfloat f, const vec3& v)
  {
    return v * f;
  }

  inline GLfloat dot(const vec3& v1, const vec3& v2)
  {
    return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z();
  }

  inline vec3 cross(const vec3& v1, const vec3& v2)
  {
    vec3 t;
    t.x() = v1.y()*v2.z() - v1.z()*v2.y() ;
    t.y() = v1.z()*v2.x() - v1.x()*v2.z() ;
    t.z() = v1.x()*v2.y() - v1.y()*v2.x() ;
    return t;
  }

}

#endif
