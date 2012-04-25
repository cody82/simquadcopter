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

#ifndef KdTreeCuller_INCLUDE_DEFINE
#define KdTreeCuller_INCLUDE_DEFINE

#include "vl/KdTree.hpp"
#include "vl/Culler.hpp"
#include "vl/Camera.hpp"
#include "vl/ObjectVector.hpp"

namespace vl
{

  class KdTreeCuller: public Culler
  {
  public:
    KdTreeCuller(): mKdTree(new KdTree), mMaxDepth(100), mLimitVolume(0) { }

    void executeCulling();

    void cullKdTree(KdTree* tree);

    void clearDynamicActors();

    void addDynamicActors(const std::vector< Actor* >& actors );

    void addDynamicActor(Actor* actor);

    void removeDynamicActor(Actor* actor);

    bool isDynamicActor(Actor* actor);

    void clearStaticActors();

    void addStaticActors(const std::vector< ref<Actor> >& actors );

    void addStaticActor(Actor* actor);

    void removeStaticActor(Actor* actor);

    bool isStaticActor(Actor* actor);

    void setLimitVolume(float volume) { mLimitVolume = volume; }
    float limitVolume() const { return mLimitVolume; }
   
    void setMaxDepth(int max_depth) { mMaxDepth = max_depth; }
    int maxDepth() const { return mMaxDepth; }

    void setActorList( ActorList* actor_list ) { mActorList = actor_list; }
    ActorList* actorList() { return mActorList.get(); }

    void prepareForCulling();

    KdTree* kdtree() { return mKdTree.get(); }

  protected:
    std::set< ref<Actor> > mStaticActors;
    std::set< ref<Actor> > mDynamicActors;
    std::vector< ref<Actor> > mDynamicActorList;
    ref<KdTree> mKdTree;
    ref< ActorList > mActorList;
    int mMaxDepth;
    float mLimitVolume;
  };
}

#endif
