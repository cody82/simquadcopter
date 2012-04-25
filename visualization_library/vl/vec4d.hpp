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

#ifndef vec4d_INCLUDE_DEFINE
#define vec4d_INCLUDE_DEFINE

#include "vl/vec3d.hpp"
#include "vl/vec4.hpp"

namespace vl
{

  class vec4d
  {
  public:

    vec4 to_vec4() const
    {
      vec4 v;
      v.x() = (GLfloat)x();
      v.y() = (GLfloat)y();
      v.z() = (GLfloat)z();
      v.w() = (GLfloat)w();
      return v;
    }

    explicit vec4d(const vec4& v)
    {
      x() = v.x();
      y() = v.y();
      z() = v.z();
      w() = v.w();
    }
    explicit vec4d(GLuint rgb, GLubyte a=0xFF)
    {
      GLubyte r = GLubyte((rgb >> 16) & 0xFF);
      GLubyte g = GLubyte((rgb >> 8) & 0xFF);
      GLubyte b = GLubyte(rgb & 0xFF);
	    mVec[0] = r / 255.0f;
	    mVec[1] = g / 255.0f;
	    mVec[2] = b / 255.0f;
	    mVec[3] = a / 255.0f;
    }
    explicit vec4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w=1) 
    {
	    mVec[0] = x;
	    mVec[1] = y;
	    mVec[2] = z;
	    mVec[3] = w;
    }
    explicit vec4d(const vec3d& v, GLdouble w) 
    {
	    mVec[0] = v.x();
	    mVec[1] = v.y();
	    mVec[2] = v.z();
	    mVec[3] = w;
    }
    explicit vec4d(const vec2d& v, GLdouble z, GLdouble w)
    {
	    mVec[0] = v.x();
	    mVec[1] = v.y();
	    mVec[2] = z;
	    mVec[3] = w;
    }
    explicit vec4d(const vec2d& v1, const vec2d& v2)
    {
	    mVec[0] = v1.x();
	    mVec[1] = v1.y();
	    mVec[2] = v2.x();
	    mVec[3] = v2.y();
    }
    vec4d()
    {
	    mVec[0] = 0.0f;
	    mVec[1] = 0.0f;
	    mVec[2] = 0.0f;
	    mVec[3] = 1.0f;
    }

	  GLdouble& x() { return mVec[0]; }

	  GLdouble& y() { return mVec[1]; }

	  GLdouble& z() { return mVec[2]; }

	  GLdouble& w() { return mVec[3]; }

	  GLdouble& r() { return mVec[0]; }

	  GLdouble& g() { return mVec[1]; }

	  GLdouble& b() { return mVec[2]; }

	  GLdouble& a() { return mVec[3]; }

	  GLdouble& s() { return mVec[0]; }

	  GLdouble& t() { return mVec[1]; }

	  GLdouble& p() { return mVec[2]; }

	  GLdouble& q() { return mVec[3]; }

	  const GLdouble& x() const { return mVec[0]; }

	  const GLdouble& y() const { return mVec[1]; }

	  const GLdouble& z() const { return mVec[2]; }

	  const GLdouble& w() const { return mVec[3]; }

	  const GLdouble& r() const { return mVec[0]; }

	  const GLdouble& g() const { return mVec[1]; }

	  const GLdouble& b() const { return mVec[2]; }

	  const GLdouble& a() const { return mVec[3]; }

	  const GLdouble& s() const { return mVec[0]; }

	  const GLdouble& t() const { return mVec[1]; }

	  const GLdouble& p() const { return mVec[2]; }

	  const GLdouble& q() const { return mVec[3]; }

	  const vec4d& xyzw() const { return *this; }
	  const vec4d& stpq() const { return *this; }
	  const vec4d& rgba() const { return *this; }

	  vec3d xyz() const { vec3d v; v.x() = x(); v.y() = y(); v.z() = z(); return v; }
	  vec3d stp() const { return xyz(); }
	  vec3d rgb() const { return xyz(); }

	  vec2d xy() const { vec2d v; v.x() = x(); v.y() = y(); return v; }
	  vec2d st() const { return xy(); }

    GLdouble& operator[](unsigned int i) { CHECK(i<4); return ptr()[i]; }

    const GLdouble& operator[](unsigned int i) const { CHECK(i<4); return mVec[i]; }

	  GLdouble* ptr() { return mVec; }

	  const GLdouble* ptr() const { return mVec; }

    vec4d operator=(const vec4d& v) 
    {
	    x() = v.x();
	    y() = v.y();
	    z() = v.z();
	    w() = v.w();
	    return v;
    }

    vec4d operator=(GLdouble f) 
    {
	    x() = f;
	    y() = f;
	    z() = f;
	    w() = f;
	    return *this;
    }

    vec4d operator+(const vec4d& v) const 
    {
	    return vec4d(x()+v.x(), y()+v.y(), z()+v.z(), w()+v.w());
    }

    const vec4d& operator+=(const vec4d& v) 
    {
      *this = *this + v;
	    return *this;
    }

    vec4d operator+(GLdouble f) const 
    {
	    return vec4d(x()+f, y()+f, z()+f, w()+f);
    }

    vec4d operator-() const 
    {
	    return vec4d(-x(), -y(), -z(), -w());
    }

    vec4d operator-(const vec4d& v) const 
    {
	    return vec4d(x()-v.x(), y()-v.y(), z()-v.z(), w()-v.w());
    }

    const vec4d& operator-=(const vec4d& v) 
    {
	    *this = *this - v;
	    return *this;
    }

    vec4d operator-(GLdouble f) const 
    {
	    return vec4d(x()-f, y()-f, z()-f, w()-f);
    }

    vec4d operator*(const vec4d& v) const
    {
      return vec4d(x()*v.x(), y()*v.y(), z()*v.z(), w()*v.w());
    }

    const vec4d& operator*=(const vec4d& v)
    {
      *this = *this * v;
      return *this;
    }

    vec4d operator*(GLdouble f) const
    {
	    return vec4d(x()*f, y()*f, z()*f, w()*f);
    }

    const vec4d& operator*=(GLdouble f)
    {
	    *this = *this * f;
	    return *this;
    }

    vec4d operator/(const vec4d& v) const
    {
      return vec4d(x()/v.x(), y()/v.y(), z()/v.z(), w()/v.w());
    }

    const vec4d& operator/=(const vec4d& v)
    {
      *this = *this / v;
      return *this;
    }

    vec4d operator/(GLdouble f) const
    {
      f = 1.0f / f;
	    return vec4d(x()*f, y()*f, z()*f, w()*f);
    }

    const vec4d& operator/=(GLdouble f)
    {
	    *this = *this / f;
	    return *this;
    }

    GLdouble length() const { return sqrt( x()*x()+y()*y()+z()*z()+w()*w() ); }

    GLdouble lengthSquared() const { return x()*x()+y()*y()+z()*z()+w()*w(); }

    const vec4d& normalize() 
    {
	    GLdouble l = length();

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

    bool operator==(const vec4d& v) const 
    {
	    return x() == v.x() && y() == v.y() && z() == v.z() && w() == v.w();
    }

    bool operator !=(const vec4d& v) const 
    {
	    return !(*this == v);
    }

    bool operator<(const vec4d& v) const
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
	  GLdouble mVec[4];
  };

  inline vec4d operator*(GLdouble f, const vec4d& v)
  {
    return v * f;
  }

  inline GLdouble dot(const vec4d& v1, const vec4d& v2)
  {
    return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z() + v1.w()*v2.w();
  }

}

#endif
