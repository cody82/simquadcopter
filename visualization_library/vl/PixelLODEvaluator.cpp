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

#include "vl/PixelLODEvaluator.hpp"
#include "vl/Camera.hpp"

using namespace vl;

int PixelLODEvaluator::evaluate(Actor* actor, Camera* camera)
{
  if (mPixelRangeSet.empty())
    return 0;

  AABB aabb = actor->transform() ? actor->drawable(0)->aabb().transformed( actor->transform()->localToWorldMatrix( camera->renderStream() ) ) : actor->drawable(0)->aabb();

  vec3d corner[] = 
  {
    vec3d(aabb.minCorner().x(), aabb.minCorner().y(), aabb.minCorner().z()),
    vec3d(aabb.minCorner().x(), aabb.maxCorner().y(), aabb.minCorner().z()),
    vec3d(aabb.maxCorner().x(), aabb.maxCorner().y(), aabb.minCorner().z()),
    vec3d(aabb.maxCorner().x(), aabb.minCorner().y(), aabb.minCorner().z()),
    vec3d(aabb.minCorner().x(), aabb.minCorner().y(), aabb.maxCorner().z()),
    vec3d(aabb.minCorner().x(), aabb.maxCorner().y(), aabb.maxCorner().z()),
    vec3d(aabb.maxCorner().x(), aabb.maxCorner().y(), aabb.maxCorner().z()),
    vec3d(aabb.maxCorner().x(), aabb.minCorner().y(), aabb.maxCorner().z())
  };

  mat4d proj_matrix = camera->projectionMatrix() * camera->inverseViewMatrix();

  aabb.setEmpty();

  for(int i=0; i<8; ++i)
  {
    vec4d out = proj_matrix * vec4d(corner[i],1);

    if (out.w() == 0.0f)
      continue;

    out.x() /= out.w();
    out.y() /= out.w();
    out.z() /= out.w();

    out.x() = out.x() * 0.5f + 0.5f;
    out.y() = out.y() * 0.5f + 0.5f;
    out.z() = out.z() * 0.5f + 0.5f;

    out.x() = out.x() * camera->viewport()->width()  + camera->viewport()->x();
    out.y() = out.y() * camera->viewport()->height() + camera->viewport()->y();

    aabb.addPoint(out.xyz());
  }

  double pixels = aabb.width() * aabb.height();

  int i=0;
  for(; i<(int)mPixelRangeSet.size(); ++i)
  {
    if (pixels>mPixelRangeSet[mPixelRangeSet.size() - 1 - i])
      return i;
  }

  return i; // == mPixelRangeSet.size()
}

void PixelLODEvaluator::setPixelRangeSet(const std::vector<float>& distance_set) 
{ 
  mPixelRangeSet = distance_set; 
  std::sort(mPixelRangeSet.begin(), mPixelRangeSet.end()); 
}

void PixelLODEvaluator::addPixelRange(float distance) 
{ 
  mPixelRangeSet.push_back(distance); 
  std::sort(mPixelRangeSet.begin(), mPixelRangeSet.end()); 
}

void PixelLODEvaluator::setPixelRange(int index, float distance) 
{ 
  mPixelRangeSet[index] = distance; 
  std::sort(mPixelRangeSet.begin(), mPixelRangeSet.end()); 
}

