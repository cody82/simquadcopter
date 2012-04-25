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

#ifndef Plane_INCLUDE_DEFINE
#define Plane_INCLUDE_DEFINE

#include "vl/Object.hpp"
#include "vl/vec3d.hpp"
#include "vl/Transform.hpp"
namespace vl
{
  class AABB;

  class Plane: public Object
  {
  public:
	  Plane(double o=0.0f, vec3d n=vec3d(0,0,0)): mNormal(n), mOrigin(o) {}
	  Plane(const vec3d& o, const vec3d& n);
	  double distance(const vec3d &v) const;

    int classify(const AABB&) const;
    bool isOutside(const AABB&) const;
    const vec3d& normal() const { return mNormal; }
    double origin() const { return mOrigin; }
    void setNormal(const vec3d& normal) { mNormal = normal; }
    void setOrigin(double origin) { mOrigin = origin; }

    void applyPlane(int plane, const mat4d& mat);

    void followTransform(Transform* transform);
    Transform* followedTransform();

  protected:
	  vec3d mNormal;
	  double mOrigin;
    ref<Transform> mFollowedTransform;
  };

}

#endif
