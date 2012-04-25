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

#include "vl/Painter.hpp"
#include "vl/Drawable.hpp"
#include "vl/Camera.hpp"

using namespace vl;

void Painter::addActor(Actor* actor)
{
  CHECK( std::find(mActors.begin(), mActors.end(), actor) == mActors.end() )
	mActors.push_back(actor);
  actor->setScedeDraw(this);
}

void Painter::setActor(int index, Actor* actor)
{
  CHECK( index < (int)mActors.size() );
  mActors[index]->setScedeDraw(NULL);
  mActors[index] = actor;
  mActors[index]->setScedeDraw(this);
}

void Painter::removeActor(Actor* actor)
{
  std::vector< ref<Actor> >::iterator it = std::find(mActors.begin(), mActors.end(), actor);
  if (it != mActors.end())
  {
    (*it)->setScedeDraw(NULL);
    mActors.erase(it);
  }
}

void Painter::removeAllActors()
{
  for(unsigned i=0; i<mActors.size(); i++)
    mActors[i]->setScedeDraw(NULL);

  mActors.clear();
}

void Painter::removeActors(int index, int count)
{
  CHECK( index + count <= (int)mActors.size() );

  for(int i=index; i<index+count; i++)
    mActors[i]->setScedeDraw(NULL);

  for(int i=index+count, j=index; i<(int)mActors.size(); i++, j++)
    mActors[j] = mActors[i];

  mActors.resize( mActors.size() - count );
}

void Painter::evaluateLOD(Actor* actor, Camera* camera)
{
  if(mLODEvaluator)
  {
    CHECK( !mMultipassDrawLOD.empty() );
    mActiveLOD = mLODEvaluator->evaluate(actor, camera);
    CHECK( mActiveLOD < (int)mMultipassDrawLOD.size() )
  }
}

void Painter::addDrawPass(Shader* shader, int lod) 
{ 
  if ((int)mMultipassDrawLOD.size() <= lod)
    mMultipassDrawLOD.resize(lod+1);
  if (mMultipassDrawLOD[lod] == NULL)
    mMultipassDrawLOD[lod] = new MultipassDraw;
  mMultipassDrawLOD[lod]->addPass( DrawPass(shader) );
}

