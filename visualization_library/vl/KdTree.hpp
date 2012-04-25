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

#ifndef KdTree_INCLUDE_DEFINE
#define KdTree_INCLUDE_DEFINE

#include "vl/AABB.hpp"
#include "vl/Actor.hpp"
#include "vl/math3D.hpp"
#include "vl/Plane.hpp"

namespace vl
{

  class KdTree: public Object
  {
  public:

    KdTree* treeFromNonLeafyActors(int camera_slot, int max_depth=100, float limit_volume=0);

    void harvestNonLeafActors(std::vector< ref<Actor> >& actors);

    void computeAABB(const std::vector< ref<Actor> >& actors, int camera_slot);

    void compileTree(int camera_slot, std::vector< ref<Actor> >& actors, int max_depth=100, float limit_volume=0);

    int scorePlane(const Plane& plane, const std::vector< ref<Actor> >& actors, int camera_slot);

    bool findBestPlane(Plane& plane, std::vector< ref<Actor> >& actors, int depth, int camera_slot);

    const AABB& aabb() const { return mAABB; }

    const Plane& plane() const { return mPlane; }

    int actorCount() const { return (int)mActors.size(); }

    Actor* actor(int i) { return mActors[i].get(); }
    const Actor* actor(int i) const { return mActors[i].get(); }

    KdTree* childN() { return mChildN.get(); }
    const KdTree* childN() const { return mChildN.get(); }

    KdTree* childP() { return mChildP.get(); }
    const KdTree* childP() const { return mChildP.get(); }

    void extractActors(std::vector< ref<Actor> >& actors) const;

    bool removeActor(Actor* actor);

    void insertActor(Actor* actor, int camera_slot );

    void rebuildTree(int camera_slot, int max_depth=100, float limit_volume=0);

  protected:
    void getActorsRecursive(std::vector< ref<Actor> >&actors) const;

  protected:
    AABB mAABB;
    Plane mPlane;
    std::vector< ref<Actor> > mActors;
    ref<KdTree> mChildN;
    ref<KdTree> mChildP;
  };

}

#endif
