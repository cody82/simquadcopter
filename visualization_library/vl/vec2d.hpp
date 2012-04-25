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

#ifndef vec2d_INCLUDE_DEFINE
#define vec2d_INCLUDE_DEFINE

#include "vl/OpenGL.hpp"
#include <cmath> // sqrt

#include "vl/vec2.hpp"

namespace vl
{

  class vec2d
  {
  public:

    vec2 to_vec2() const
    {
      vec2 v;
      v.x() = (GLfloat)x();
      v.y() = (GLfloat)y();
      return v;
    }
    explicit vec2d(const vec2& v)
    {
      x() = v.x();
      y() = v.y();
    }
    explicit vec2d(GLdouble x, GLdouble y)
    {
	    mVec[0] = x;
	    mVec[1] = y;
    }

    vec2d()
    {
	    mVec[0] = 0.0f;
	    mVec[1] = 0.0f;
    }

	  GLdouble& x() { return mVec[0]; }

	  GLdouble& y() { return mVec[1]; }

	  GLdouble& s() { return mVec[0]; }

	  GLdouble& t() { return mVec[1]; }

	  const GLdouble& x() const { return mVec[0]; }

	  const GLdouble& y() const { return mVec[1]; }

	  const GLdouble& s() const { return mVec[0]; }

	  const GLdouble& t() const { return mVec[1]; }

    const vec2d& xy() const { return *this; }
	  const vec2d& st() const { return *this; }

    GLdouble& operator[](unsigned int i) { CHECK(i<2); return ptr()[i]; }

    const GLdouble& operator[](unsigned int i) const { CHECK(i<2); return mVec[i]; }

	  GLdouble* ptr() { return mVec; }

	  const GLdouble* ptr() const { return mVec; }

    const vec2d&operator=(const vec2d& v) 
    {
	    x() = v.x();
	    y() = v.y();
	    return *this;
    }

    const vec2d& operator=(GLdouble f) 
    {
	    x() = f;
	    y() = f;
	    return *this;
    }

    vec2d operator+(const vec2d& v) const 
    {
	    return vec2d(x()+v.x(), y()+v.y());
    }

    const vec2d& operator+=(const vec2d& v) 
    {
	    *this = *this + v;
      return *this;
    }

    vec2d operator+(GLdouble f) const 
    {
	    return vec2d(x()+f, y()+f);
    }

    vec2d operator-() const 
    {
	    return vec2d(-x(), -y());
    }

    vec2d operator-(const vec2d& v) const 
    {
	    return vec2d(x()-v.x(), y()-v.y());
    }

    const vec2d& operator-=(const vec2d& v) 
    {
	    *this = *this - v;
      return *this;
    }

    vec2d operator-(GLdouble f) const 
    {
	    return vec2d(x()-f, y()-f);
    }

    vec2d operator*(const vec2d& v) const
    {
      return vec2d( x()*v.x(), y()*v.y() );
    }

    const vec2d& operator*=(const vec2d& v)
    {
      *this = *this * v;
      return *this;
    }

    vec2d operator*(GLdouble f) const
    {
	    return vec2d(x()*f, y()*f);
    }

    const vec2d& operator*=(GLdouble f)
    {
      *this = *this * f;
      return *this;
    }

    vec2d operator/(const vec2d& v) const
    {
      return vec2d( x()/v.x(), y()/v.y() );
    }

    const vec2d& operator/=(const vec2d& v)
    {
      *this = *this / v;
      return *this;
    }

    vec2d operator/(GLdouble f) const
    {
      f = 1.0f / f;
	    return vec2d( (GLdouble)(x()*f), (GLdouble)(y()*f) );
    }

    const vec2d& operator/=(GLdouble f)
    {
      *this = *this / f;
      return *this;
    }

    GLdouble length() const { return sqrt( x()*x()+y()*y() ); }

    GLdouble lengthSquared() const { return x()*x()+y()*y(); }

    const vec2d& normalize() 
    {
	    GLdouble l = length();

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

    bool operator==(const vec2d& v) const 
    {
	    return x() == v.x() && y() == v.y();
    }

    bool operator !=(const vec2d& v) const 
    {
	    return !(*this == v);
    }

    bool operator<(const vec2d& v) const
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
	  GLdouble mVec[2];

  };

  inline vec2d operator*(GLdouble f, const vec2d& v)
  {
    return v * f;
  }

  inline GLdouble dot(const vec2d& v1, const vec2d& v2)
  {
    return v1.x()*v2.x() + v1.y()*v2.y();
  }

}

#endif
