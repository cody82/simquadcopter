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

#ifndef AABB_INCLUDE_DEFINE
#define AABB_INCLUDE_DEFINE

#include "vl/vec3d.hpp"
#include "vl/mat4d.hpp"

namespace vl
{

  class AABB {
  public:
	  AABB();
	  AABB( const vec3d& center, double radius );
	  AABB( const vec3d& pt1, const vec3d& pt2, double displace=0);
    void setEmpty() { mMin = 1; mMax = -1; }
	  bool isEmpty() const;
	  void enlarge(double displace);
	  void addPoint(const vec3d& v);
	  void addPoint(const vec3d& v, double radius);
	  bool intersects(const AABB & bb) const;
	  vec3d clip(const vec3d& v, bool clipx=true, bool clipy=true, bool clipz=true) const;
	  bool isInside(const vec3d& v, bool clipx=true, bool clipy=true, bool clipz=true) const;
	  double height() const;
	  double width() const;
	  double depth() const;
	  AABB operator+(const AABB& aabb) const;
	  const AABB& operator+=(const AABB& other)
    {
      *this = *this + other;
      return *this;
    }
    vec3d center() const;
    double area() const
    {
      if (isEmpty())
        return 0;
      else 
        return width()*height()*depth();
    }
    double longestSideLength() const
    {
      double side = width();
      if (height() > side)
        side = height();
      if (depth() > side)
        side = depth();
      return side;
    }
    void transformed(AABB& aabb, const mat4d& mat) const 
    {
      aabb.setEmpty();
      if ( !isEmpty() )
      {
        aabb.addPoint( mat * vec3d(minCorner().x(), minCorner().y(), minCorner().z()) );
        aabb.addPoint( mat * vec3d(minCorner().x(), maxCorner().y(), minCorner().z()) );
        aabb.addPoint( mat * vec3d(maxCorner().x(), maxCorner().y(), minCorner().z()) );
        aabb.addPoint( mat * vec3d(maxCorner().x(), minCorner().y(), minCorner().z()) );
        aabb.addPoint( mat * vec3d(minCorner().x(), minCorner().y(), maxCorner().z()) );
        aabb.addPoint( mat * vec3d(minCorner().x(), maxCorner().y(), maxCorner().z()) );
        aabb.addPoint( mat * vec3d(maxCorner().x(), maxCorner().y(), maxCorner().z()) );
        aabb.addPoint( mat * vec3d(maxCorner().x(), minCorner().y(), maxCorner().z()) );
      }
    }
    AABB transformed(const mat4d& mat) const 
    {
      AABB aabb;
      if ( !isEmpty() )
      {
        aabb.addPoint( mat * vec3d(minCorner().x(), minCorner().y(), minCorner().z()) );
        aabb.addPoint( mat * vec3d(minCorner().x(), maxCorner().y(), minCorner().z()) );
        aabb.addPoint( mat * vec3d(maxCorner().x(), maxCorner().y(), minCorner().z()) );
        aabb.addPoint( mat * vec3d(maxCorner().x(), minCorner().y(), minCorner().z()) );
        aabb.addPoint( mat * vec3d(minCorner().x(), minCorner().y(), maxCorner().z()) );
        aabb.addPoint( mat * vec3d(minCorner().x(), maxCorner().y(), maxCorner().z()) );
        aabb.addPoint( mat * vec3d(maxCorner().x(), maxCorner().y(), maxCorner().z()) );
        aabb.addPoint( mat * vec3d(maxCorner().x(), minCorner().y(), maxCorner().z()) );
      }
      return aabb;
    }
    const vec3d& minCorner() const { return mMin; }
    const vec3d& maxCorner() const { return mMax; }
    void setMinCorner(const vec3d& v) { mMin = v; }
    void setMaxCorner(const vec3d& v) { mMax = v; }
    double volume() const { return width() * height() * depth(); }

    template <typename T>
    void compute(const T* verts, int size, int components)
    {
      CHECK(components>=2 && components <=4)

      setEmpty();

	    if (size == 0)
		    return;

	    mMin.x() = mMax.x() = (double)verts[0];
	    mMin.y() = mMax.y() = (double)verts[1];
      if (components >= 3)
        mMin.z() = mMax.z() = (double)verts[2];
      else 
        mMin.z() = mMax.z() = 0;

	    for(int i=components; i<size; i+=components)
	    {
		    if ( mMax.x() < (double)verts[i+0] ) mMax.x() = (double)verts[i+0];
		    if ( mMax.y() < (double)verts[i+1] ) mMax.y() = (double)verts[i+1];

		    if ( components >=3 && mMax.z() < (double)verts[i+2] ) mMax.z() = (double)verts[i+2];

		    if ( mMin.x() > (double)verts[i+0] ) mMin.x() = (double)verts[i+0];
		    if ( mMin.y() > (double)verts[i+1] ) mMin.y() = (double)verts[i+1];

		    if ( components >=3 && mMin.z() > (double)verts[i+2] ) mMin.z() = (double)verts[i+2];
	    }
    }

  protected:
	  vec3d mMin;
	  vec3d mMax;
  };
}

#endif
