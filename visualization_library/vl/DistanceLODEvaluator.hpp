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

#ifndef DistanceLODEvaluator_INCLUDE_DEFINE
#define DistanceLODEvaluator_INCLUDE_DEFINE

#include "vl/Actor.hpp"

namespace vl
{

  class DistanceLODEvaluator: public LODEvaluator
  {
  public:
    DistanceLODEvaluator() {}
    virtual int evaluate(Actor* actor, Camera* camera)
    {
      if (mDistanceRangeSet.empty())
        return 0;

      vec3d center = actor->transform() ? actor->transform()->localToWorldMatrix( camera->renderStream() ) * actor->drawable(0)->aabb().center() : actor->drawable(0)->aabb().center();
      double dist = (camera->viewMatrix().getT() - center).length();

      int i=0;
      for(; i<(int)mDistanceRangeSet.size(); ++i)
      {
        if (dist<mDistanceRangeSet[i])
          return i;
      }

      return i; // == mDistanceRangeSet.size()
    }

    const std::vector<double>& distanceRangeSet() { return mDistanceRangeSet; }
    void clearDistanceRangeSet() { mDistanceRangeSet.clear(); }
    int distanceRangeCount() const { return (int)mDistanceRangeSet.size(); }
    double distanceRange(int index) const { return mDistanceRangeSet[index]; }

    void setDistanceRangeSet(const std::vector<double>& distance_set) { mDistanceRangeSet = distance_set; std::sort(mDistanceRangeSet.begin(), mDistanceRangeSet.end()); }
    void addDistanceRange(double distance) { mDistanceRangeSet.push_back(distance); std::sort(mDistanceRangeSet.begin(), mDistanceRangeSet.end()); }
    void setDistanceRange(int index, double distance) { mDistanceRangeSet[index] = distance; std::sort(mDistanceRangeSet.begin(), mDistanceRangeSet.end()); }

  protected:
    std::vector<double> mDistanceRangeSet;
  };
}

#endif
