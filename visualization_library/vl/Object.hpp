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

#ifndef Object_INCLUDE_DEFINE
#define Object_INCLUDE_DEFINE

#include <string.h>
#include "vl/config.hpp"
#include "../../3rdparty-req/ref.hpp"

#if DEBUG_LIVING_OBJECTS
#include <set>
#endif

#include <string>

namespace vl
{

  class Object 
  {
  friend void ref_add_ref(Object * p);
  friend void ref_release(Object * p);
  public:
    Object();
    Object(const Object&);
	  const Object& operator=(const Object&)
	  {

	    return *this;
	  }

	  int refCount() const { return mRefCount; }

    std::string name() const { return mName; }
    void setName(std::string name) { mName = name; }

  #if DEBUG_LIVING_OBJECTS
    static std::set< Object* > *mDebug_LivingObjects;
    static std::set< Object* >* debug_living_objects() 
    { 

      if (!mDebug_LivingObjects)
        mDebug_LivingObjects = new std::set< Object* >;
      return mDebug_LivingObjects;
    }
  #endif

  protected:
    virtual ~Object();
    std::string mName;

  private:
	  int mRefCount;
  };

  inline void ref_add_ref(Object * p)
  {
    p->mRefCount++;
  }

  inline void ref_release(Object * p)
  {
   p->mRefCount --;
   if (p->mRefCount == 0)
     delete p;
  }

  void visualization_library_init();
  void visualization_library_shutdown();
}

#endif
