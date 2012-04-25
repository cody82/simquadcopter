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

#include "vl/KdTreeCuller.hpp"

using namespace vl;

void KdTreeCuller::executeCulling()
{
  CHECK(mActorList)
  mActorList->clear();

  for (int i=0; i<(int)mDynamicActorList.size(); i++)
  {
    AABB taabb;
    if ( mDynamicActorList[i]->transform() ) 
      mDynamicActorList[i]->drawable(0)->aabb().transformed( taabb, mDynamicActorList[i]->transform()->localToWorldMatrix( camera()->renderStream() ) );
    else
      taabb = mDynamicActorList[i]->drawable(0)->aabb();

    if ( camera()->frustumCull(taabb) == false )
    {
      mActorList->push_back(mDynamicActorList[i]);
    }
  }

  cullKdTree(mKdTree.get());

}

void KdTreeCuller::cullKdTree(KdTree* tree)
{
  if ( camera()->frustumCull( tree->aabb() ) == false )
  {
    for(int i=0; i<(int)tree->actorCount(); i++)
      if ( tree->actor(i)->enabled() )
        mActorList->push_back( tree->actor(i) );

    if (tree->childN())
      cullKdTree( tree->childN() );

    if (tree->childP())
      cullKdTree( tree->childP() );
  }
}

void KdTreeCuller::clearDynamicActors()
{
  mDynamicActors.clear();
}

void KdTreeCuller::addDynamicActors(const std::vector< Actor* >& actors )
{
  for(int i=0; i<(int)actors.size(); i++)
    addDynamicActor(actors[i]);
}

void KdTreeCuller::addDynamicActor(Actor* actor)
{
  mDynamicActors.insert(actor);
}

void KdTreeCuller::removeDynamicActor(Actor* actor)
{
  mDynamicActors.erase(actor);
}

bool KdTreeCuller::isDynamicActor(Actor* actor)
{
  return mDynamicActors.find(actor) != mDynamicActors.end();
}

void KdTreeCuller::clearStaticActors()
{
  mStaticActors.clear();
}

void KdTreeCuller::addStaticActors(const std::vector< ref<Actor> >& actors )
{
  for(int i=0; i<(int)actors.size(); i++)
    addStaticActor(actors[i].get());
}

void KdTreeCuller::addStaticActor(Actor* actor)
{
  mStaticActors.insert(actor);
}

void KdTreeCuller::removeStaticActor(Actor* actor)
{
  mStaticActors.erase(actor);
}

bool KdTreeCuller::isStaticActor(Actor* actor)
{
  return mStaticActors.find(actor) != mStaticActors.end();
}

void KdTreeCuller::prepareForCulling()
{
  CHECK(mCamera);

  mDynamicActorList.clear();
  std::set< ref<Actor> >::iterator act = mDynamicActors.begin();
  while( act != mDynamicActors.end() )
  {
    mDynamicActorList.push_back( *act  );
    ++act;
  }

  std::vector< ref<Actor> > actors;
  act  = mStaticActors.begin();
  while( act  != mStaticActors.end() )
  {
    actors.push_back( *act );
    ++act ;
  }

  std::set< ref<Transform> > root_transforms;
  for(int i=0; i<(int)actors.size(); i++)
  {
    ref<Transform> root = actors[i]->transform();
    while(root)
    {
      if ( !root->parent() )
      {
        root_transforms.insert(root);
      }
      root = root->parent();
    }
  }
  for(int i=0; i<(int)mDynamicActorList.size(); i++)
  {
    ref<Transform> root = mDynamicActorList[i]->transform();
    while(root)
    {
      if ( !root->parent() )
      {
        root_transforms.insert(root);
      }
      root = root->parent();
    }
  }

  std::set< ref<Transform> >::iterator tra = root_transforms.begin();
  while( tra != root_transforms.end() )
  {
    (*tra)->computeLocalToWorldMatrixRecursive(camera());
    ++tra;
  }

  mKdTree->compileTree(mCamera->renderStream(), actors, mMaxDepth, mLimitVolume);
}

