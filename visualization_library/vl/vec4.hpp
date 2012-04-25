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

#ifndef vec4_INCLUDE_DEFINE
#define vec4_INCLUDE_DEFINE

#include "vl/vec3.hpp"

namespace vl
{

  class vec4
  {
  public:

    explicit vec4(GLuint rgb, GLubyte a=0xFF)
    {
      GLubyte r = GLubyte((rgb >> 16) & 0xFF);
      GLubyte g = GLubyte((rgb >> 8) & 0xFF);
      GLubyte b = GLubyte(rgb & 0xFF);
	    mVec[0] = r / 255.0f;
	    mVec[1] = g / 255.0f;
	    mVec[2] = b / 255.0f;
	    mVec[3] = a / 255.0f;
    }
    explicit vec4(GLfloat x, GLfloat y, GLfloat z, GLfloat w=1) 
    {
	    mVec[0] = x;
	    mVec[1] = y;
	    mVec[2] = z;
	    mVec[3] = w;
    }
    explicit vec4(const vec3& v, GLfloat w) 
    {
	    mVec[0] = v.x();
	    mVec[1] = v.y();
	    mVec[2] = v.z();
	    mVec[3] = w;
    }
    explicit vec4(const vec2& v, GLfloat z, GLfloat w)
    {
	    mVec[0] = v.x();
	    mVec[1] = v.y();
	    mVec[2] = z;
	    mVec[3] = w;
    }
    explicit vec4(const vec2& v1, const vec2& v2)
    {
	    mVec[0] = v1.x();
	    mVec[1] = v1.y();
	    mVec[2] = v2.x();
	    mVec[3] = v2.y();
    }
    vec4()
    {
	    mVec[0] = 0.0f;
	    mVec[1] = 0.0f;
	    mVec[2] = 0.0f;
	    mVec[3] = 1.0f;
    }

	  GLfloat& x() { return mVec[0]; }

	  GLfloat& y() { return mVec[1]; }

	  GLfloat& z() { return mVec[2]; }

	  GLfloat& w() { return mVec[3]; }

	  GLfloat& r() { return mVec[0]; }

	  GLfloat& g() { return mVec[1]; }

	  GLfloat& b() { return mVec[2]; }

	  GLfloat& a() { return mVec[3]; }

	  GLfloat& s() { return mVec[0]; }

	  GLfloat& t() { return mVec[1]; }

	  GLfloat& p() { return mVec[2]; }

	  GLfloat& q() { return mVec[3]; }

	  const GLfloat& x() const { return mVec[0]; }

	  const GLfloat& y() const { return mVec[1]; }

	  const GLfloat& z() const { return mVec[2]; }

	  const GLfloat& w() const { return mVec[3]; }

	  const GLfloat& r() const { return mVec[0]; }

	  const GLfloat& g() const { return mVec[1]; }

	  const GLfloat& b() const { return mVec[2]; }

	  const GLfloat& a() const { return mVec[3]; }

	  const GLfloat& s() const { return mVec[0]; }

	  const GLfloat& t() const { return mVec[1]; }

	  const GLfloat& p() const { return mVec[2]; }

	  const GLfloat& q() const { return mVec[3]; }

	  const vec4& xyzw() const { return *this; }
	  const vec4& stpq() const { return *this; }
	  const vec4& rgba() const { return *this; }

	  vec3 xyz() const { vec3 v; v.x() = x(); v.y() = y(); v.z() = z(); return v; }
	  vec3 stp() const { return xyz(); }
	  vec3 rgb() const { return xyz(); }

	  vec2 xy() const { vec2 v; v.x() = x(); v.y() = y(); return v; }
	  vec2 st() const { return xy(); }

    GLfloat& operator[](unsigned int i) { CHECK(i<4); return ptr()[i]; }

    const GLfloat& operator[](unsigned int i) const { CHECK(i<4); return mVec[i]; }

	  GLfloat* ptr() { return mVec; }

	  const GLfloat* ptr() const { return mVec; }

    vec4 operator=(const vec4& v) 
    {
	    x() = v.x();
	    y() = v.y();
	    z() = v.z();
	    w() = v.w();
	    return v;
    }

    vec4 operator=(GLfloat f) 
    {
	    x() = f;
	    y() = f;
	    z() = f;
	    w() = f;
	    return *this;
    }

    vec4 operator+(const vec4& v) const 
    {
	    return vec4(x()+v.x(), y()+v.y(), z()+v.z(), w()+v.w());
    }

    const vec4& operator+=(const vec4& v) 
    {
      *this = *this + v;
	    return *this;
    }

    vec4 operator+(GLfloat f) const 
    {
	    return vec4(x()+f, y()+f, z()+f, w()+f);
    }

    vec4 operator-() const 
    {
	    return vec4(-x(), -y(), -z(), -w());
    }

    vec4 operator-(const vec4& v) const 
    {
	    return vec4(x()-v.x(), y()-v.y(), z()-v.z(), w()-v.w());
    }

    const vec4& operator-=(const vec4& v) 
    {
	    *this = *this - v;
	    return *this;
    }

    vec4 operator-(GLfloat f) const 
    {
	    return vec4(x()-f, y()-f, z()-f, w()-f);
    }

    vec4 operator*(const vec4& v) const
    {
      return vec4(x()*v.x(), y()*v.y(), z()*v.z(), w()*v.w());
    }

    const vec4& operator*=(const vec4& v)
    {
      *this = *this * v;
      return *this;
    }

    vec4 operator*(GLfloat f) const
    {
	    return vec4(x()*f, y()*f, z()*f, w()*f);
    }

    const vec4& operator*=(GLfloat f)
    {
	    *this = *this * f;
	    return *this;
    }

    vec4 operator/(const vec4& v) const
    {
      return vec4(x()/v.x(), y()/v.y(), z()/v.z(), w()/v.w());
    }

    const vec4& operator/=(const vec4& v)
    {
      *this = *this / v;
      return *this;
    }

    vec4 operator/(GLfloat f) const
    {
      f = 1.0f / f;
	    return vec4(x()*f, y()*f, z()*f, w()*f);
    }

    const vec4& operator/=(GLfloat f)
    {
	    *this = *this / f;
	    return *this;
    }

    GLfloat length() const { return sqrt( x()*x()+y()*y()+z()*z()+w()*w() ); }

    GLfloat lengthSquared() const { return x()*x()+y()*y()+z()*z()+w()*w(); }

    const vec4& normalize() 
    {
	    GLfloat l = length();

	    if (l>0.00001)
	    {
		    l = 1.0f/l;
		    x() *= l;
		    y() *= l;
		    z() *= l;
		    w() *= l;
	    }
      else
        *this = 0;
	    return *this;
    }

    bool operator==(const vec4& v) const 
    {
	    return x() == v.x() && y() == v.y() && z() == v.z() && w() == v.w();
    }

    bool operator !=(const vec4& v) const 
    {
	    return !(*this == v);
    }

    bool operator<(const vec4& v) const
    {
      if (x() != v.x())
        return x() < v.x();
      if (y() != v.y())
        return y() < v.y();
      if (z() != v.z())
        return z() < v.z();
      else
        return w() < v.w();
    }

    bool isNull() const
    {
	    if ( x() || y() || z() || w() )
		    return false;
	    else
		    return true;
    }

  protected:
	  GLfloat mVec[4];
  };

  inline vec4 operator*(GLfloat f, const vec4& v)
  {
    return v * f;
  }

  inline GLfloat dot(const vec4& v1, const vec4& v2)
  {
    return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z() + v1.w()*v2.w();
  }

}

#endif
