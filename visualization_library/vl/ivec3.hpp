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

#ifndef ivec3_INCLUDE_DEFINE
#define ivec3_INCLUDE_DEFINE

#include "vl/ivec2.hpp"

namespace vl
{

  class ivec3 
  {
  public:

    explicit ivec3(GLuint rgb)
    {
      GLubyte r = GLubyte((rgb >> 16) & 0xFF);
      GLubyte g = GLubyte((rgb >> 8) & 0xFF);
      GLubyte b = GLubyte(rgb & 0xFF);
	    mVec[0] = r;
	    mVec[1] = g;
	    mVec[2] = b;
    }
    explicit ivec3(const ivec2& v, GLint z) 
    {
	    mVec[0] = v.x();
	    mVec[1] = v.y();
	    mVec[2] = z;
    }
    explicit ivec3(GLint x, GLint y, GLint z) 
    {
	    mVec[0] = x;
	    mVec[1] = y;
	    mVec[2] = z;
    }
    ivec3()
    {
	    mVec[0] = 0;
	    mVec[1] = 0;
	    mVec[2] = 0;
    }

	  GLint& x() { return mVec[0]; }

	  GLint& y() { return mVec[1]; }

	  GLint& z() { return mVec[2]; }

	  GLint& r() { return mVec[0]; }

	  GLint& g() { return mVec[1]; }

	  GLint& b() { return mVec[2]; }

	  GLint& s() { return mVec[0]; }

	  GLint& t() { return mVec[1]; }

	  GLint& p() { return mVec[2]; }

	  const GLint& x() const { return mVec[0]; }

	  const GLint& y() const { return mVec[1]; }

	  const GLint& z() const { return mVec[2]; }

	  const GLint& r() const { return mVec[0]; }

	  const GLint& g() const { return mVec[1]; }

	  const GLint& b() const { return mVec[2]; }

	  const GLint& s() const { return mVec[0]; }

	  const GLint& t() const { return mVec[1]; }

	  const GLint& p() const { return mVec[2]; }

	  const ivec3& xyz() const { return *this; }
	  const ivec3& stp() const { return *this; }
	  const ivec3& rgb() const { return *this; }

	  ivec2 xy() const { ivec2 v; v.x() = x(); v.y() = y(); return v; }
	  ivec2 st() const { return xy(); }

    GLint& operator[](unsigned int i) { CHECK(i<3); return ptr()[i]; }

    const GLint& operator[](unsigned int i) const { CHECK(i<3); return mVec[i]; }

	  GLint* ptr() { return mVec; }

	  const GLint* ptr() const { return mVec; }

    ivec3 operator=(const ivec3& v) 
    {
	    x() = v.x();
	    y() = v.y();
	    z() = v.z();
	    return v;
    }

    const ivec3& operator=(GLint f) 
    {
	    x() = f;
	    y() = f;
	    z() = f;
	    return *this;
    }

    ivec3 operator+(ivec3 v) const 
    {
	    return ivec3(x()+v.x(), y()+v.y(), z()+v.z());
    }

    const ivec3& operator+=(ivec3 v) 
    {
      *this = ivec3(x()+v.x(), y()+v.y(), z()+v.z());
      return *this;
    }

    ivec3 operator+(GLint f) const 
    {
	    return ivec3(x()+f, y()+f, z()+f);
    }

    ivec3 operator-() const 
    {
	    return ivec3(-x(), -y(), -z());
    }

    ivec3 operator-(ivec3 v) const 
    {
	    return ivec3(x()-v.x(), y()-v.y(), z()-v.z());
    }

    const ivec3& operator-=(ivec3 v) 
    {
      *this = ivec3(x()-v.x(), y()-v.y(), z()-v.z());
      return *this;
    }

    ivec3 operator-(GLint f) const 
    {
	    return ivec3(x()-f, y()-f, z()-f);
    }

    ivec3 operator*(const ivec3& v) const
    {
      return ivec3( x()*v.x(), y()*v.y(), z()*v.z() );
    }

    const ivec3& operator*=(const ivec3& v)
    {
      *this = *this * v;
      return *this;
    }

    ivec3 operator*(GLint f) const
    {
	    return ivec3(x()*f, y()*f, z()*f);
    }

    const ivec3& operator*=(GLint f)
    {
	    *this = *this * f;
      return *this;
    }

    ivec3 operator/(const ivec3& v) const
    {
      return ivec3( x()/v.x(), y()/v.y(), z()/v.z() );
    }

    const ivec3& operator/=(const ivec3& v)
    {
      *this = *this / v;
      return *this;
    }

    ivec3 operator/(GLint f) const
    {
	    return ivec3(x()/f, y()/f, z()/f);
    }

    const ivec3& operator/=(GLint f)
    {
	    *this = *this / f;
      return *this;
    }

    bool operator==(const ivec3& v) const
    {
	    return x() == v.x() && y() == v.y() && z() == v.z();
    }

    bool operator!=(const ivec3& v) const
    {
	    return x() != v.x() || y() != v.y() || z() != v.z();
    }

    bool operator<(const ivec3& v) const
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
	    if (x() || y() || z())
		    return false;
	    else
		    return true;
    }

  protected:
	  GLint mVec[3];
  };

  inline ivec3 operator*(GLint f, const ivec3& v)
  {
    return v * f;
  }

  inline GLint dot(const ivec3& v1, const ivec3& v2)
  {
    return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z();
  }

  inline ivec3 cross(const ivec3& v1, const ivec3& v2)
  {
    ivec3 t;
    t.x() = v1.y()*v2.z() - v1.z()*v2.y() ;
    t.y() = v1.z()*v2.x() - v1.x()*v2.z() ;
    t.z() = v1.x()*v2.y() - v1.y()*v2.x() ;
    return t;
  }

}

#endif
