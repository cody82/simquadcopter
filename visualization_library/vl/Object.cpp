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

#include "vl/Object.hpp"
#include "vl/Log.hpp"
#include "vl/Say.hpp"
#include "vl/Time.hpp"
#include "vl/GlobalState.hpp"
#include <sstream>

using namespace vl;

#if DEBUG_LIVING_OBJECTS
std::set< Object* > *Object::mDebug_LivingObjects = NULL;
#endif

Object::Object()
{
  mRefCount = 0;
#if DEBUG_LIVING_OBJECTS
  debug_living_objects()->insert(this);

#endif
}

Object::Object(const Object&)
{

  mRefCount = 0;
#if DEBUG_LIVING_OBJECTS
  debug_living_objects()->insert(this);

#endif
}

Object::~Object()
{
	if (mRefCount)
    Log::bug(Say("Object '%s' is being deleted having still %n references!\n"
                 "This is probably due to an explicit call to the 'delete' operator.\n"
                 "Never call 'delete' on an object derived from the class 'Object'.\n"
                 "The program will most probably crash very soon.\n") << mName << mRefCount);
#if DEBUG_LIVING_OBJECTS
  debug_living_objects()->erase(this);

#endif
}

