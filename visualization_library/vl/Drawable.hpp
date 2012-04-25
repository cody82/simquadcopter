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

#ifndef Drawable_INCLUDE_DEFINE
#define Drawable_INCLUDE_DEFINE

#include "vl/Object.hpp"
#include "vl/Transform.hpp"
#include "vl/AABB.hpp"

namespace vl
{

  class Actor;
  class Drawable: public Object
  {
  public:
    Drawable(): mAABBDirty(true) {}
	  virtual ~Drawable() {}
    virtual void draw(Actor*, int thread, unsigned int tex_units) = 0;

    virtual void computeAABB() = 0;
    void setAABBDirty(bool dirty) { mAABBDirty = dirty; }
    bool aabbDirty() const { return mAABBDirty; }
    void setAABB( const AABB& aabb ) 
    {
      mAABB = aabb; 
      mAABBDirty = false;
    }
    const AABB& aabb()
    { 
      if (mAABBDirty)
      {
        computeAABB();
        mAABBDirty = false;
      }
      return mAABB; 
    }

  protected:
    AABB mAABB;
    bool mAABBDirty;
  };
}

#endif
