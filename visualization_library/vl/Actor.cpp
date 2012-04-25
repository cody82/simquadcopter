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

#include "vl/Actor.hpp"
#include "vl/Painter.hpp"
#include "vl/Camera.hpp"

using namespace vl;

void Actor::evaluateLOD(Camera* camera)
{
  if (mLODEvaluator)
  {
    setActiveLOD(mLODEvaluator->evaluate(this, camera));
    CHECK( drawable() )
  }
  else
    setActiveLOD(0);
}

void Actor::addUniform(Uniform* u)
{
  removeUniform(u->name());
  mUniform.push_back(u);
}

void Actor::removeUniform(Uniform* u)
{
  removeUniform(u->name());
}

void Actor::removeUniform(int i)
{
  CHECK( i<(int)mUniform.size() )
  mUniform.erase(mUniform.begin() + i);
}

void Actor::removeUniform(const std::string& name)
{
  for(int i=0; i<uniformCount(); ++i)
  {
    if(uniform(i)->name() == name)
    {
      mUniform.erase(mUniform.begin() + i);
      return;
    }
  }
}

const Uniform* Actor::uniform(int i) const
{
  return mUniform[i].get();
}

Uniform* Actor::uniform(int i)
{
  return mUniform[i].get();
}

int Actor::uniformCount()
{
  return (int)mUniform.size();
}

void Actor::removeAllUniform()
{
  mUniform.clear();
}

