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

#include "vl/KdTree.hpp"
#include <algorithm>

using namespace vl;

namespace 
{

  class sortX
  {
  public:
    sortX(int camera_slot): mCameraSlot(camera_slot) {}
    bool operator()(const ref<Actor>& a1, const ref<Actor>& a2) const
    {
      CHECK(a1->drawable(0))
      CHECK(a2->drawable(0))
      AABB b1 = a1->transform() ? a1->drawable(0)->aabb().transformed( a1->transform()->localToWorldMatrix(mCameraSlot)) : a1->drawable(0)->aabb();
      AABB b2 = a2->transform() ? a2->drawable(0)->aabb().transformed( a2->transform()->localToWorldMatrix(mCameraSlot)) : a2->drawable(0)->aabb();
      return b1.minCorner().x() < b2.minCorner().x();
    }
    int mCameraSlot;
  };

  class sortY
  {
  public:
    sortY(int camera_slot): mCameraSlot(camera_slot) {}
    bool operator()(const ref<Actor>& a1, const ref<Actor>& a2) const
    {
      CHECK(a1->drawable(0))
      CHECK(a2->drawable(0))
      AABB b1 = a1->transform() ? a1->drawable(0)->aabb().transformed( a1->transform()->localToWorldMatrix(mCameraSlot)) : a1->drawable(0)->aabb();
      AABB b2 = a2->transform() ? a2->drawable(0)->aabb().transformed( a2->transform()->localToWorldMatrix(mCameraSlot)) : a2->drawable(0)->aabb();
      return b1.minCorner().y() < b2.minCorner().y();
    }
    int mCameraSlot;
  };

  class sortZ
  {
  public:
    sortZ(int camera_slot): mCameraSlot(camera_slot) {}
    bool operator()(const ref<Actor>& a1, const ref<Actor>& a2) const
    {
      CHECK(a1->drawable(0))
      CHECK(a2->drawable(0))
      AABB b1 = a1->transform() ? a1->drawable(0)->aabb().transformed( a1->transform()->localToWorldMatrix(mCameraSlot)) : a1->drawable(0)->aabb();
      AABB b2 = a2->transform() ? a2->drawable(0)->aabb().transformed( a2->transform()->localToWorldMatrix(mCameraSlot)) : a2->drawable(0)->aabb();
      return b1.minCorner().z() < b2.minCorner().z();
    }
    int mCameraSlot;
  };
}

KdTree* KdTree::treeFromNonLeafyActors(int camera_slot, int max_depth, float limit_volume)
{
  std::vector< ref<Actor> > actors;
  harvestNonLeafActors(actors);
  ref<KdTree> newtree = new KdTree;
  newtree->compileTree(camera_slot, actors, max_depth, limit_volume);
  return newtree.get();
}

void KdTree::harvestNonLeafActors(std::vector< ref<Actor> >& actors)
{
  CHECK( (mActors.size() && (mChildN == 0 && mChildP == 0)) || !(mChildN == 0 && mChildP == 0) );

  if ( mChildN || mChildP )
  {
    for(int i=0; i<(int)mActors.size(); i++)
      actors.push_back(mActors[i]);
    mActors.clear();
  }

  if(mChildN) mChildN->harvestNonLeafActors( actors );
  if(mChildP) mChildP->harvestNonLeafActors( actors );
}

void KdTree::computeAABB(const std::vector< ref<Actor> >& actors, int camera_slot)
{
  mAABB.setEmpty();
  for(int i=0; i<(int)actors.size(); i++)
  {
    CHECK( actors[i]->drawable(0) )
    mAABB += actors[i]->transform() ? actors[i]->drawable(0)->aabb().transformed( actors[i]->transform()->localToWorldMatrix(camera_slot) ) : actors[i]->drawable(0)->aabb();
  }
}

void KdTree::compileTree(int camera_slot, std::vector< ref<Actor> >& actors, int max_depth, float limit_volume)
{
  mChildN = NULL;
  mChildP = NULL;
  mActors.clear();
  mAABB.setEmpty();
  mPlane = Plane();

  if (actors.size() == 0)
    return;

  computeAABB(actors, camera_slot);

  if (actors.size() == 1 || max_depth == 0 || mAABB.volume() < limit_volume)
  {
    mActors = actors;
    return;
  }

  if ( !findBestPlane(mPlane, actors, max_depth, camera_slot) )
  {
    mActors = actors;
    return;
  }

  std::vector< ref<Actor> > actorsN;
  std::vector< ref<Actor> > actorsP;

  for(int i=0; i<(int)actors.size(); i++)
  {
    CHECK(actors[i]->drawable(0))
    switch( mPlane.classify(actors[i]->transform() ? actors[i]->drawable(0)->aabb().transformed( actors[i]->transform()->localToWorldMatrix(camera_slot)): actors[i]->drawable(0)->aabb()) )
    {
    case 0: mActors.push_back(actors[i]); break;
    case -1: actorsN.push_back(actors[i]); break;
    case +1: actorsP.push_back(actors[i]); break;
    }    
  }

  if (actorsN.size())
  {
    mChildN = new KdTree;
    mChildN->compileTree(camera_slot, actorsN, max_depth-1, limit_volume);
  }

  if (actorsP.size())
  {
    mChildP = new KdTree;
    mChildP->compileTree(camera_slot, actorsP, max_depth-1, limit_volume);
  }

}

int KdTree::scorePlane(const Plane& plane, const std::vector< ref<Actor> >& actors, int camera_slot)
{
  int cN=0, cC=0, cP=0;
  for(int i=0; i<(int)actors.size(); i++)
  {
    CHECK(actors[i]->drawable(0))
    AABB taabb = actors[i]->transform() ? actors[i]->drawable(0)->aabb().transformed( actors[i]->transform()->localToWorldMatrix(camera_slot)) : actors[i]->drawable(0)->aabb();
    switch( plane.classify(taabb) )
    {
    case -1: cN++; break;
    case 0:  cC++; break;
    case +1: cP++; break;
    }
  }
  return cC + ::abs(cN-cP);
}

bool KdTree::findBestPlane(Plane& plane, std::vector< ref<Actor> >& actors, int depth, int camera_slot)
{
  AABB taabb;
  int median = (int)actors.size() / 2;
  if (depth%3 == 0)
  {
    std::sort(actors.begin(), actors.end(), sortX(camera_slot));
    CHECK(actors[median]->drawable(0))
    taabb = actors[median]->transform() ? actors[median]->drawable(0)->aabb().transformed( actors[median]->transform()->localToWorldMatrix(camera_slot)) : actors[median]->drawable(0)->aabb();
    plane = Plane(taabb.minCorner().x(), vec3d(1,0,0));
    if (scorePlane(plane, actors, camera_slot) == (int)actors.size())
    {
      std::sort(actors.begin(), actors.end(), sortY(camera_slot));
      CHECK(actors[median]->drawable(0))
      taabb = actors[median]->transform() ? actors[median]->drawable(0)->aabb().transformed( actors[median]->transform()->localToWorldMatrix(camera_slot)) : actors[median]->drawable(0)->aabb();
      plane = Plane(taabb.minCorner().y(), vec3d(0,1,0));
      if (scorePlane(plane, actors, camera_slot) == (int)actors.size())
      {
        std::sort(actors.begin(), actors.end(), sortZ(camera_slot));
        CHECK(actors[median]->drawable(0))
        taabb = actors[median]->transform() ? actors[median]->drawable(0)->aabb().transformed( actors[median]->transform()->localToWorldMatrix(camera_slot)) : actors[median]->drawable(0)->aabb();
        plane = Plane(taabb.minCorner().z(), vec3d(0,0,1));
        if (scorePlane(plane, actors, camera_slot) == (int)actors.size())
          return false;
      }
    }
  }
  else
  if (depth%3 == 1)
  {
    std::sort(actors.begin(), actors.end(), sortY(camera_slot));
    CHECK(actors[median]->drawable(0))
    taabb = actors[median]->transform() ? actors[median]->drawable(0)->aabb().transformed( actors[median]->transform()->localToWorldMatrix(camera_slot)) : actors[median]->drawable(0)->aabb();
    plane = Plane(taabb.minCorner().y(), vec3d(0,1,0));
    if (scorePlane(plane, actors, camera_slot) == (int)actors.size())
    {
      std::sort(actors.begin(), actors.end(), sortZ(camera_slot));
      CHECK(actors[median]->drawable(0))
      taabb = actors[median]->transform() ? actors[median]->drawable(0)->aabb().transformed( actors[median]->transform()->localToWorldMatrix(camera_slot)) : actors[median]->drawable(0)->aabb();
      plane = Plane(taabb.minCorner().z(), vec3d(0,0,1));
      if (scorePlane(plane, actors, camera_slot) == (int)actors.size())
      {
        std::sort(actors.begin(), actors.end(), sortX(camera_slot));
        CHECK(actors[median]->drawable(0))
        taabb = actors[median]->transform() ? actors[median]->drawable(0)->aabb().transformed( actors[median]->transform()->localToWorldMatrix(camera_slot)) : actors[median]->drawable(0)->aabb();
        plane = Plane(taabb.minCorner().x(), vec3d(1,0,0));
        if (scorePlane(plane, actors, camera_slot) == (int)actors.size())
          return false;
      }
    }
  }
  else
  if (depth%3 == 2)
  {
    std::sort(actors.begin(), actors.end(), sortZ(camera_slot));
    CHECK(actors[median]->drawable(0))
    taabb = actors[median]->transform() ? actors[median]->drawable(0)->aabb().transformed( actors[median]->transform()->localToWorldMatrix(camera_slot)) : actors[median]->drawable(0)->aabb();
    plane = Plane(taabb.minCorner().z(), vec3d(0,0,1));
    if (scorePlane(plane, actors, camera_slot) == (int)actors.size())
    {
      std::sort(actors.begin(), actors.end(), sortX(camera_slot));
      CHECK(actors[median]->drawable(0))
      taabb = actors[median]->transform() ? actors[median]->drawable(0)->aabb().transformed( actors[median]->transform()->localToWorldMatrix(camera_slot)) : actors[median]->drawable(0)->aabb();
      plane = Plane(taabb.minCorner().x(), vec3d(1,0,0));
      if (scorePlane(plane, actors, camera_slot) == (int)actors.size())
      {
        std::sort(actors.begin(), actors.end(), sortY(camera_slot));
        CHECK(actors[median]->drawable(0))
        taabb = actors[median]->transform() ? actors[median]->drawable(0)->aabb().transformed( actors[median]->transform()->localToWorldMatrix(camera_slot)) : actors[median]->drawable(0)->aabb();
        plane = Plane(taabb.minCorner().y(), vec3d(0,1,0));
        if (scorePlane(plane, actors, camera_slot) == (int)actors.size())
          return false;
      }
    }
  }

  return true;
}

void KdTree::extractActors(std::vector< ref<Actor> >& actors) const
{
  actors.clear();
  getActorsRecursive(actors);
}

bool KdTree::removeActor(Actor* actor)
{
  for(int i=0; i<(int)mActors.size(); i++)
  {
    if (mActors[i] == actor)
    {
      mActors.erase( mActors.begin() + i );
      return true;
    }
  }

  if ( childN() )
    if (childN()->removeActor(actor))
      return true;

  if ( childP() )
    if (childP()->removeActor(actor))
      return true;

  return false;
}

void KdTree::insertActor(Actor* actor, int camera_slot )
{
  CHECK(actor->drawable(0))
  if (childN() == 0 && childP() == 0)
  {
    mActors.push_back(actor);
    return;
  }
  else
  {
    AABB taabb = actor->transform() ? actor->drawable(0)->aabb().transformed( actor->transform()->localToWorldMatrix( camera_slot )) : actor->drawable(0)->aabb();
    switch( mPlane.classify(taabb) )
    {
    case -1: if (!mChildN) mChildN = new KdTree; mChildN->insertActor(actor, camera_slot); break;
    case 0:  mActors.push_back(actor); break;
    case +1: if (!mChildP) mChildP = new KdTree; mChildP->insertActor(actor, camera_slot); break;
    }
  }
}

void KdTree::rebuildTree(int camera_slot, int max_depth, float limit_volume)
{
  std::vector< ref<Actor> > actors;
  extractActors(actors);
  compileTree(camera_slot, actors, max_depth, limit_volume);
}

void KdTree::getActorsRecursive(std::vector< ref<Actor> >&actors) const
{
  for(int i=0; i<actorCount(); i++)
    actors.push_back(const_cast<Actor*>(actor(i)) );
  if (childN()) 
    childN()->getActorsRecursive(actors);
  if (childP()) 
    childP()->getActorsRecursive(actors);
}

