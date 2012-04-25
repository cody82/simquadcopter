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

#ifndef math3D_INCLUDE_DEFINE
#define math3D_INCLUDE_DEFINE

#include "vl/mat4d.hpp"
#include "vl/vec4d.hpp"
#include "vl/mat3d.hpp"
#include "vl/vec3d.hpp"
#include "vl/mat2d.hpp"
#include "vl/vec2d.hpp"
#include <cmath>
#include <cstdio>
#include <memory.h>
#include <vector>

namespace vl
{
  extern const double PI;
  extern const double DEG_TO_RAD;
  extern const double RAD_TO_DEG;

  class Plane;
  class AABB;

  class Ray 
  {
  public:
	  Ray(vec3d o=vec3d(0,0,0), vec3d d=vec3d(0,0,0)): Origin(o), Direction(d) {}
	  vec3d Origin;
	  vec3d Direction;
  };

  class Cylinder 
  {
  public:
	  Cylinder(vec3d o=vec3d(0,0,0), double h=1, double r=1): Origin(o), Height(h), Radius(r) {}
	  vec3d Origin;
	  double Height;
	  double Radius;
  };

  class Extrusion 
  {
  public:
	  Extrusion(double b=0, double h=0): Bottom(b), Height(h) {}
	  std::vector<vec3d> Silhouette;
	  double Bottom;
	  double Height;
  };

  double random01();
  double randomMinMax(double min, double max);
  int greaterEqualPow2(int n);
  int smallerEqualPow2(int n);
  void transform( const mat4d& m, std::vector<vec3d> &vec);

  bool rayCircleIntersection(const Ray& ray, const vec3d &cir_centre, double cir_radius, double *pt);
  bool rayPlaneIntersection( const Ray& ray, const Plane& plane, double *pt);
  bool rayAABBIntersection( const Ray& ray, const AABB& aabb, double *pt);
  bool rayCylinderIntersection( const Ray& ray, const Cylinder& aabb, double *pt);
  bool rayConvexPolygonIntersection( const Ray& ray, const std::vector<vec3d>& poly, double *pt);
  bool pointInPolygonTest_XZ( const vec3d& p, std::vector<vec3d> poly );
  bool rayExtrusionIntersection( const Ray& ray, const Extrusion& extrusion, double *pt);

}

#endif
