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

#ifndef quat_INCLUDE_DEFINE
#define quat_INCLUDE_DEFINE

#include "vl/mat4d.hpp"

namespace vl
{

  class quat
  {
  public:

    quat()
    {
      setNoRotation();
    }

    explicit quat(const vec4d& v)
    {
      mVec[0] = v.x();
      mVec[1] = v.y();
      mVec[2] = v.z();
      mVec[3] = v.w();
    }

    explicit quat(GLdouble x, GLdouble y, GLdouble z, GLdouble w)
    {
      mVec[0] = x;
      mVec[1] = y;
      mVec[2] = z;
      mVec[3] = w;
    }

    explicit quat(GLdouble degrees, const vec3d& axis)
    {
      fromAxisAngle(degrees, axis);
    }

    bool operator==(const quat& q) const
    {
      return x() == q.x() && y() == q.y() && z() == q.z() && w() == q.w();
    }

    bool operator!=(const quat& q) const
    {
      return !(*this == q);
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

    quat setZero()
    {
      mVec[0] = 0;
      mVec[1] = 0;
      mVec[2] = 0;
      mVec[3] = 0;
      return *this;
    }

    quat setNoRotation()
    {
      mVec[0] = 0;
      mVec[1] = 0;
      mVec[2] = 0;
      mVec[3] = 1;
      return *this;
    }

    const quat& fromVectors(const vec4d& from, const vec4d& to)
    {
      *this = fromVectors( from.xyz(), to.xyz() );
      return *this;
    }

    const quat& fromVectors(const vec3d& from, const vec3d& to);

    quat fromMatrix(const mat4d& m);

    quat fromEulerXYZ(GLdouble degX, GLdouble degY, GLdouble degZ );

    quat fromEulerZYX(GLdouble degZ, GLdouble degY, GLdouble degX );

    quat fromAxisAngle(GLdouble degrees, const vec3d& axis);

    void toAxisAngle( vec4d& axis, GLdouble& angle ) const;

    mat4d toMatrix() const;

    const vec4d xyzw() const
    {
      return vec4d( x(), y(), z(), w() );
    }

    GLdouble& x() { return mVec[0]; }

	  GLdouble& y() { return mVec[1]; }

	  GLdouble& z() { return mVec[2]; }

	  GLdouble& w() { return mVec[3]; }

	  const GLdouble& x() const { return mVec[0]; }

	  const GLdouble& y() const { return mVec[1]; }

	  const GLdouble& z() const { return mVec[2]; }

	  const GLdouble& w() const { return mVec[3]; }

    quat operator*(GLdouble real) const
    {
      quat t = *this;
      t.x() *= real;
      t.y() *= real;
      t.z() *= real;
      t.w() *= real;
      return t;
    }

    const quat& operator*=(GLdouble real)
    {
      *this = *this * real;
      return *this;
    }

    quat operator/(GLdouble real) const
    {
      quat t = *this;
      real = 1.0 / real;
      t.x() *= real;
      t.y() *= real;
      t.z() *= real;
      t.w() *= real;
      return t;
    }

    const quat& operator/=(GLdouble real)
    {
      *this = *this / real;
      return *this;
    }

    quat operator+(const quat& q) const
    {
      quat t = *this;
      t.x() += q.x();
      t.y() += q.y();
      t.z() += q.z();
      t.w() += q.w();
      return t; 
    }

    const quat& operator+=(const quat& q)
    {
      *this = *this + q;
      return *this;
    }

    quat operator-(const quat& q) const
    {
      quat t = *this;
      t.x() -= q.x();
      t.y() -= q.y();
      t.z() -= q.z();
      t.w() -= q.w();
      return t; 
    }

    const quat& operator-=(const quat& q)
    {
      *this = *this - q;
      return *this;
    }

    quat operator-() const
    {
      return quat(-x(), -y(), -z(), -w());
    }

    GLdouble dot(const quat& q) const
    {
      return x()*q.x() + y()*q.y() + z()*q.z() + w()*q.w();
    }

    quat normalize();

    quat getNormalized() const { quat t = *this; t.normalize(); return t; }

    GLdouble length() const;

    GLdouble lengthSquared() const
    {
      return this->dot(*this);
    }

    quat conjugate() const
    {
      return quat(-x(), -y(), -z(), w());
    }

    quat inverse() const
    {
      return conjugate() / lengthSquared();
    }

    quat slerp( GLdouble t, const quat& a, const quat& b ) const;

    quat squad(GLdouble t, const quat& a, const quat& p, const quat& q, const quat& b) const
    {
      return slerp(2.0*t*(1.0-t), slerp(t,a,b), slerp(t,p,q));
    }

    quat nlerp( GLdouble t, const quat& a, const quat& b ) const
    {
	    quat q;
      q = a + (b - a) * t;
      q.normalize();
      return q;
    }
    
  protected:
    GLdouble mVec[4];
  };

  inline quat operator*(GLdouble r, const quat& q)
  {
    return q * r;
  }

  inline quat operator*(const quat& q1, const quat& q2)
  {
    quat q;
    q.x() = q1.w() * q2.x() + q1.x() * q2.w() + q1.y() * q2.z() - q1.z() * q2.y();
    q.y() = q1.w() * q2.y() + q1.y() * q2.w() + q1.z() * q2.x() - q1.x() * q2.z();
	  q.z() = q1.w() * q2.z() + q1.z() * q2.w() + q1.x() * q2.y() - q1.y() * q2.x();
    q.w() = q1.w() * q2.w() - q1.x() * q2.x() - q1.y() * q2.y() - q1.z() * q2.z();
    return q;
  }

  inline vec3d operator*(const quat&q, const vec3d& v)
  {

    GLdouble x2 = q.x() * q.x();
    GLdouble y2 = q.y() * q.y();
    GLdouble z2 = q.z() * q.z();
    GLdouble xy = q.x() * q.y();
    GLdouble xz = q.x() * q.z();
    GLdouble yz = q.y() * q.z();
    GLdouble wx = q.w() * q.x();
    GLdouble wy = q.w() * q.y();
    GLdouble wz = q.w() * q.z();

    vec3d r;
    r.x() = ( v.x()*(1.0f - 2.0f * (y2 + z2)) + v.y()*(2.0f * (xy - wz)) + v.z()*(2.0f * (xz + wy)) );
    r.y() = ( v.x()*(2.0f * (xy + wz)) + v.y()*(1.0f - 2.0f * (x2 + z2)) + v.z()*(2.0f * (yz - wx)) );
    r.z() = ( v.x()*(2.0f * (xz - wy)) + v.y()*(2.0f * (yz + wx)) + v.z()*(1.0f - 2.0f * (x2 + y2)) );
    return r;
  }

  inline vec4d operator*(const quat&q, const vec4d& v)
  {
    return vec4d( q * v.xyz(), v.w() );
  }

}

#endif
