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

#ifndef ivec4_INCLUDE_DEFINE
#define ivec4_INCLUDE_DEFINE

#include "vl/ivec3.hpp"

namespace vl
{

  class ivec4
  {
  public:

    explicit ivec4(GLuint rgb, GLubyte a=0xFF)
    {
      GLubyte r = GLubyte((rgb >> 16) & 0xFF);
      GLubyte g = GLubyte((rgb >> 8) & 0xFF);
      GLubyte b = GLubyte(rgb & 0xFF);
	    mVec[0] = r;
	    mVec[1] = g;
	    mVec[2] = b;
	    mVec[3] = a;
    }
    explicit ivec4(GLint x, GLint y, GLint z, GLint w=1) 
    {
	    mVec[0] = x;
	    mVec[1] = y;
	    mVec[2] = z;
	    mVec[3] = w;
    }
    explicit ivec4(const ivec3& v, GLint w) 
    {
	    mVec[0] = v.x();
	    mVec[1] = v.y();
	    mVec[2] = v.z();
	    mVec[3] = w;
    }
    explicit ivec4(const ivec2& v, GLint z, GLint w)
    {
	    mVec[0] = v.x();
	    mVec[1] = v.y();
	    mVec[2] = z;
	    mVec[3] = w;
    }
    explicit ivec4(const ivec2& v1, const ivec2& v2)
    {
	    mVec[0] = v1.x();
	    mVec[1] = v1.y();
	    mVec[2] = v2.x();
	    mVec[3] = v2.y();
    }
    ivec4()
    {
	    mVec[0] = 0;
	    mVec[1] = 0;
	    mVec[2] = 0;
	    mVec[3] = 1;
    }

	  GLint& x() { return mVec[0]; }

	  GLint& y() { return mVec[1]; }

	  GLint& z() { return mVec[2]; }

	  GLint& w() { return mVec[3]; }

	  GLint& r() { return mVec[0]; }

	  GLint& g() { return mVec[1]; }

	  GLint& b() { return mVec[2]; }

	  GLint& a() { return mVec[3]; }

	  GLint& s() { return mVec[0]; }

	  GLint& t() { return mVec[1]; }

	  GLint& p() { return mVec[2]; }

	  GLint& q() { return mVec[3]; }

	  const GLint& x() const { return mVec[0]; }

	  const GLint& y() const { return mVec[1]; }

	  const GLint& z() const { return mVec[2]; }

	  const GLint& w() const { return mVec[3]; }

	  const GLint& r() const { return mVec[0]; }

	  const GLint& g() const { return mVec[1]; }

	  const GLint& b() const { return mVec[2]; }

	  const GLint& a() const { return mVec[3]; }

	  const GLint& s() const { return mVec[0]; }

	  const GLint& t() const { return mVec[1]; }

	  const GLint& p() const { return mVec[2]; }

	  const GLint& q() const { return mVec[3]; }

	  const ivec4& xyzw() const { return *this; }
	  const ivec4& stpq() const { return *this; }
	  const ivec4& rgba() const { return *this; }

	  ivec3 xyz() const { ivec3 v; v.x() = x(); v.y() = y(); v.z() = z(); return v; }
	  ivec3 stp() const { return xyz(); }
	  ivec3 rgb() const { return xyz(); }

	  ivec2 xy() const { ivec2 v; v.x() = x(); v.y() = y(); return v; }
	  ivec2 st() const { return xy(); }

    GLint& operator[](unsigned int i) { CHECK(i<4); return ptr()[i]; }

    const GLint& operator[](unsigned int i) const { CHECK(i<4); return mVec[i]; }

	  GLint* ptr() { return mVec; }

	  const GLint* ptr() const { return mVec; }

    ivec4 operator=(const ivec4& v) 
    {
	    x() = v.x();
	    y() = v.y();
	    z() = v.z();
	    w() = v.w();
	    return v;
    }

    ivec4 operator=(GLint f) 
    {
	    x() = f;
	    y() = f;
	    z() = f;
	    w() = f;
	    return *this;
    }

    ivec4 operator+(const ivec4& v) const 
    {
	    return ivec4(x()+v.x(), y()+v.y(), z()+v.z(), w()+v.w());
    }

    const ivec4& operator+=(const ivec4& v) 
    {
      *this = *this + v;
	    return *this;
    }

    ivec4 operator+(GLint f) const 
    {
	    return ivec4(x()+f, y()+f, z()+f, w()+f);
    }

    ivec4 operator-() const 
    {
	    return ivec4(-x(), -y(), -z(), -w());
    }

    ivec4 operator-(const ivec4& v) const 
    {
	    return ivec4(x()-v.x(), y()-v.y(), z()-v.z(), w()-v.w());
    }

    const ivec4& operator-=(const ivec4& v) 
    {
	    *this = *this - v;
	    return *this;
    }

    ivec4 operator-(GLint f) const 
    {
	    return ivec4(x()-f, y()-f, z()-f, w()-f);
    }

    ivec4 operator*(const ivec4& v) const
    {
      return ivec4(x()*v.x(), y()*v.y(), z()*v.z(), w()*v.w());
    }

    const ivec4& operator*=(const ivec4& v)
    {
      *this = *this * v;
      return *this;
    }

    ivec4 operator*(GLint f) const
    {
	    return ivec4(x()*f, y()*f, z()*f, w()*f);
    }

    const ivec4& operator*=(GLint f)
    {
	    *this = *this * f;
	    return *this;
    }

    ivec4 operator/(const ivec4& v) const
    {
      return ivec4(x()/v.x(), y()/v.y(), z()/v.z(), w()/v.w());
    }

    const ivec4& operator/=(const ivec4& v)
    {
      *this = *this / v;
      return *this;
    }

    ivec4 operator/(GLint f) const
    {
	    return ivec4(x()/f, y()/f, z()/f, w()/f);
    }

    const ivec4& operator/=(GLint f)
    {
	    *this = *this / f;
	    return *this;
    }

    bool operator==(const ivec4& v) const 
    {
	    return x() == v.x() && y() == v.y() && z() == v.z() && w() == v.w();
    }

    bool operator !=(const ivec4& v) const 
    {
	    return !(*this == v);
    }

    bool operator<(const ivec4& v) const
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
	  GLint mVec[4];
  };

  inline ivec4 operator*(GLint f, const ivec4& v)
  {
    return v * f;
  }

  inline GLint dot(const ivec4& v1, const ivec4& v2)
  {
    return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z() + v1.w()*v2.w();
  }

}

#endif
