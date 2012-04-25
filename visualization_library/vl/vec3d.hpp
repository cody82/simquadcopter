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

#ifndef vec3d_INCLUDE_DEFINE
#define vec3d_INCLUDE_DEFINE

#include "vl/vec2d.hpp"
#include "vl/vec3.hpp"

namespace vl
{

  class vec3d 
  {
  public:

    vec3 to_vec3() const
    {
      vec3 v;
      v.x() = (GLfloat)x();
      v.y() = (GLfloat)y();
      v.z() = (GLfloat)z();
      return v;
    }
    explicit vec3d(const vec3& v)
    {
      x() = v.x();
      y() = v.y();
      z() = v.z();
    }
    explicit vec3d(GLuint rgb)
    {
      GLubyte r = GLubyte((rgb >> 16) & 0xFF);
      GLubyte g = GLubyte((rgb >> 8) & 0xFF);
      GLubyte b = GLubyte(rgb & 0xFF);
	    mVec[0] = r / 255.0f;
	    mVec[1] = g / 255.0f;
	    mVec[2] = b / 255.0f;
    }
    explicit vec3d(const vec2d& v, GLdouble z) 
    {
	    mVec[0] = v.x();
	    mVec[1] = v.y();
	    mVec[2] = z;
    }
    explicit vec3d(GLdouble x, GLdouble y, GLdouble z) 
    {
	    mVec[0] = x;
	    mVec[1] = y;
	    mVec[2] = z;
    }
    vec3d()
    {
	    mVec[0] = 0.0f;
	    mVec[1] = 0.0f;
	    mVec[2] = 0.0f;
    }

	  GLdouble& x() { return mVec[0]; }

	  GLdouble& y() { return mVec[1]; }

	  GLdouble& z() { return mVec[2]; }

	  GLdouble& r() { return mVec[0]; }

	  GLdouble& g() { return mVec[1]; }

	  GLdouble& b() { return mVec[2]; }

	  GLdouble& s() { return mVec[0]; }

	  GLdouble& t() { return mVec[1]; }

	  GLdouble& p() { return mVec[2]; }

	  const GLdouble& x() const { return mVec[0]; }

	  const GLdouble& y() const { return mVec[1]; }

	  const GLdouble& z() const { return mVec[2]; }

	  const GLdouble& r() const { return mVec[0]; }

	  const GLdouble& g() const { return mVec[1]; }

	  const GLdouble& b() const { return mVec[2]; }

	  const GLdouble& s() const { return mVec[0]; }

	  const GLdouble& t() const { return mVec[1]; }

	  const GLdouble& p() const { return mVec[2]; }

	  const vec3d& xyz() const { return *this; }
	  const vec3d& stp() const { return *this; }
	  const vec3d& rgb() const { return *this; }

	  vec2d xy() const { vec2d v; v.x() = x(); v.y() = y(); return v; }
	  vec2d st() const { return xy(); }

    GLdouble& operator[](unsigned int i) { CHECK(i<3); return ptr()[i]; }

    const GLdouble& operator[](unsigned int i) const { CHECK(i<3); return mVec[i]; }

	  GLdouble* ptr() { return mVec; }

	  const GLdouble* ptr() const { return mVec; }

    vec3d operator=(const vec3d& v) 
    {
	    x() = v.x();
	    y() = v.y();
	    z() = v.z();
	    return v;
    }

    const vec3d& operator=(GLdouble f) 
    {
	    x() = f;
	    y() = f;
	    z() = f;
	    return *this;
    }

    vec3d operator+(vec3d v) const 
    {
	    return vec3d(x()+v.x(), y()+v.y(), z()+v.z());
    }

    const vec3d& operator+=(vec3d v) 
    {
      *this = vec3d(x()+v.x(), y()+v.y(), z()+v.z());
      return *this;
    }

    vec3d operator+(GLdouble f) const 
    {
	    return vec3d(x()+f, y()+f, z()+f);
    }

    vec3d operator-() const 
    {
	    return vec3d(-x(), -y(), -z());
    }

    vec3d operator-(vec3d v) const 
    {
	    return vec3d(x()-v.x(), y()-v.y(), z()-v.z());
    }

    const vec3d& operator-=(vec3d v) 
    {
      *this = vec3d(x()-v.x(), y()-v.y(), z()-v.z());
      return *this;
    }

    vec3d operator-(GLdouble f) const 
    {
	    return vec3d(x()-f, y()-f, z()-f);
    }

    vec3d operator*(const vec3d& v) const
    {
      return vec3d( x()*v.x(), y()*v.y(), z()*v.z() );
    }

    const vec3d& operator*=(const vec3d& v)
    {
      *this = *this * v;
      return *this;
    }

    vec3d operator*(GLdouble f) const
    {
	    return vec3d(x()*f, y()*f, z()*f);
    }

    const vec3d& operator*=(GLdouble f)
    {
	    *this = *this * f;
      return *this;
    }

    vec3d operator/(const vec3d& v) const
    {
      return vec3d( x()/v.x(), y()/v.y(), z()/v.z() );
    }

    const vec3d& operator/=(const vec3d& v)
    {
      *this = *this / v;
      return *this;
    }

    vec3d operator/(GLdouble f) const
    {
      f = 1.0f / f;
	    return vec3d(x()*f, y()*f, z()*f);
    }

    const vec3d& operator/=(GLdouble f)
    {
	    *this = *this / f;
      return *this;
    }

    GLdouble length() const { return sqrt( x()*x()+y()*y()+z()*z() ); }

    GLdouble lengthSquared() const { return x()*x()+y()*y()+z()*z(); } 

    const vec3d& normalize()
    {
	    GLdouble l = length();

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

    bool operator==(const vec3d& v) const
    {
	    return x() == v.x() && y() == v.y() && z() == v.z();
    }

    bool operator!=(const vec3d& v) const
    {
	    return x() != v.x() || y() != v.y() || z() != v.z();
    }

    bool operator<(const vec3d& v) const
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
	  GLdouble mVec[3];
  };

  inline vec3d operator*(GLdouble f, const vec3d& v)
  {
    return v * f;
  }

  inline GLdouble dot(const vec3d& v1, const vec3d& v2)
  {
    return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z();
  }

  inline vec3d cross(const vec3d& v1, const vec3d& v2)
  {
    vec3d t;
    t.x() = v1.y()*v2.z() - v1.z()*v2.y() ;
    t.y() = v1.z()*v2.x() - v1.x()*v2.z() ;
    t.z() = v1.x()*v2.y() - v1.y()*v2.x() ;
    return t;
  }

}

#endif
