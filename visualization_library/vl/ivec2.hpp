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

#ifndef ivec2_INCLUDE_DEFINE
#define ivec2_INCLUDE_DEFINE

#include "vl/OpenGL.hpp"

namespace vl
{

  class ivec2
  {
  public:

    explicit ivec2(GLint x, GLint y)
    {
	    mVec[0] = x;
	    mVec[1] = y;
    }

    ivec2()
    {
	    mVec[0] = 0;
	    mVec[1] = 0;
    }

	  GLint& x() { return mVec[0]; }

	  GLint& y() { return mVec[1]; }

	  GLint& s() { return mVec[0]; }

	  GLint& t() { return mVec[1]; }

	  const GLint& x() const { return mVec[0]; }

	  const GLint& y() const { return mVec[1]; }

	  const GLint& s() const { return mVec[0]; }

	  const GLint& t() const { return mVec[1]; }

    const ivec2& xy() const { return *this; }
	  const ivec2& st() const { return *this; }

    GLint& operator[](unsigned int i) { CHECK(i<2); return ptr()[i]; }

    const GLint& operator[](unsigned int i) const { CHECK(i<2); return mVec[i]; }

	  GLint* ptr() { return mVec; }

	  const GLint* ptr() const { return mVec; }

    const ivec2&operator=(const ivec2& v) 
    {
	    x() = v.x();
	    y() = v.y();
	    return *this;
    }

    const ivec2& operator=(GLint f) 
    {
	    x() = f;
	    y() = f;
	    return *this;
    }

    ivec2 operator+(const ivec2& v) const 
    {
	    return ivec2(x()+v.x(), y()+v.y());
    }

    const ivec2& operator+=(const ivec2& v) 
    {
	    *this = *this + v;
      return *this;
    }

    ivec2 operator+(GLint f) const 
    {
	    return ivec2(x()+f, y()+f);
    }

    ivec2 operator-() const 
    {
	    return ivec2(-x(), -y());
    }

    ivec2 operator-(const ivec2& v) const 
    {
	    return ivec2(x()-v.x(), y()-v.y());
    }

    const ivec2& operator-=(const ivec2& v) 
    {
	    *this = *this - v;
      return *this;
    }

    ivec2 operator-(GLint f) const 
    {
	    return ivec2(x()-f, y()-f);
    }

    ivec2 operator*(const ivec2& v) const
    {
      return ivec2( x()*v.x(), y()*v.y() );
    }

    const ivec2& operator*=(const ivec2& v)
    {
      *this = *this * v;
      return *this;
    }

    ivec2 operator*(GLint f) const
    {
	    return ivec2(x()*f, y()*f);
    }

    const ivec2& operator*=(GLint f)
    {
      *this = *this * f;
      return *this;
    }

    ivec2 operator/(const ivec2& v) const
    {
      return ivec2( x()/v.x(), y()/v.y() );
    }

    const ivec2& operator/=(const ivec2& v)
    {
      *this = *this / v;
      return *this;
    }

    ivec2 operator/(GLint f) const
    {
	    return ivec2( (GLint)(x()/f), (GLint)(y()/f) );
    }

    const ivec2& operator/=(GLint f)
    {
      *this = *this / f;
      return *this;
    }

    bool operator==(const ivec2& v) const 
    {
	    return x() == v.x() && y() == v.y();
    }

    bool operator !=(const ivec2& v) const 
    {
	    return !(*this == v);
    }

    bool operator<(const ivec2& v) const
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
	  GLint mVec[2];

  };

  inline ivec2 operator*(GLint f, const ivec2& v)
  {
    return v * f;
  }

  inline GLint dot(const ivec2& v1, const ivec2& v2)
  {
    return v1.x()*v2.x() + v1.y()*v2.y();
  }

}

#endif
