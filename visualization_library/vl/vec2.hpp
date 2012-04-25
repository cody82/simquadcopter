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

#ifndef vec2_INCLUDE_DEFINE
#define vec2_INCLUDE_DEFINE

#include "vl/OpenGL.hpp"
#include <cmath> // sqrt

namespace vl
{

  class vec2
  {
  public:

    explicit vec2(GLfloat x, GLfloat y)
    {
	    mVec[0] = x;
	    mVec[1] = y;
    }

    vec2()
    {
	    mVec[0] = 0.0f;
	    mVec[1] = 0.0f;
    }

	  GLfloat& x() { return mVec[0]; }

	  GLfloat& y() { return mVec[1]; }

	  GLfloat& s() { return mVec[0]; }

	  GLfloat& t() { return mVec[1]; }

	  const GLfloat& x() const { return mVec[0]; }

	  const GLfloat& y() const { return mVec[1]; }

	  const GLfloat& s() const { return mVec[0]; }

	  const GLfloat& t() const { return mVec[1]; }

    const vec2& xy() const { return *this; }
	  const vec2& st() const { return *this; }

    GLfloat& operator[](unsigned int i) { CHECK(i<2); return ptr()[i]; }

    const GLfloat& operator[](unsigned int i) const { CHECK(i<2); return mVec[i]; }

	  GLfloat* ptr() { return mVec; }

	  const GLfloat* ptr() const { return mVec; }

    const vec2&operator=(const vec2& v) 
    {
	    x() = v.x();
	    y() = v.y();
	    return *this;
    }

    const vec2& operator=(GLfloat f) 
    {
	    x() = f;
	    y() = f;
	    return *this;
    }

    vec2 operator+(const vec2& v) const 
    {
	    return vec2(x()+v.x(), y()+v.y());
    }

    const vec2& operator+=(const vec2& v) 
    {
	    *this = *this + v;
      return *this;
    }

    vec2 operator+(GLfloat f) const 
    {
	    return vec2(x()+f, y()+f);
    }

    vec2 operator-() const 
    {
	    return vec2(-x(), -y());
    }

    vec2 operator-(const vec2& v) const 
    {
	    return vec2(x()-v.x(), y()-v.y());
    }

    const vec2& operator-=(const vec2& v) 
    {
	    *this = *this - v;
      return *this;
    }

    vec2 operator-(GLfloat f) const 
    {
	    return vec2(x()-f, y()-f);
    }

    vec2 operator*(const vec2& v) const
    {
      return vec2( x()*v.x(), y()*v.y() );
    }

    const vec2& operator*=(const vec2& v)
    {
      *this = *this * v;
      return *this;
    }

    vec2 operator*(GLfloat f) const
    {
	    return vec2(x()*f, y()*f);
    }

    const vec2& operator*=(GLfloat f)
    {
      *this = *this * f;
      return *this;
    }

    vec2 operator/(const vec2& v) const
    {
      return vec2( x()/v.x(), y()/v.y() );
    }

    const vec2& operator/=(const vec2& v)
    {
      *this = *this / v;
      return *this;
    }

    vec2 operator/(GLfloat f) const
    {
      f = 1.0f / f;
	    return vec2( (GLfloat)(x()*f), (GLfloat)(y()*f) );
    }

    const vec2& operator/=(GLfloat f)
    {
      *this = *this / f;
      return *this;
    }

    GLfloat length() const { return sqrt( x()*x()+y()*y() ); }

    GLfloat lengthSquared() const { return x()*x()+y()*y(); }

    const vec2& normalize() 
    {
	    GLfloat l = length();

	    if (l>0.00001)
	    {
		    l = 1.0f/l;
		    x() *= l;
		    y() *= l;
	    }
      else
        *this = 0;
	    return *this;
    }

    bool operator==(const vec2& v) const 
    {
	    return x() == v.x() && y() == v.y();
    }

    bool operator !=(const vec2& v) const 
    {
	    return !(*this == v);
    }

    bool operator<(const vec2& v) const
    {
      if (x() != v.x())
        return x() < v.x();
      else
        return y() < v.y();
    }

    bool isNull() const
    {
	    if ( x() || y() )
		    return false;
	    else
		    return true;
    }

  protected:
	  GLfloat mVec[2];

  };

  inline vec2 operator*(GLfloat f, const vec2& v)
  {
    return v * f;
  }

  inline GLfloat dot(const vec2& v1, const vec2& v2)
  {
    return v1.x()*v2.x() + v1.y()*v2.y();
  }

}

#endif
