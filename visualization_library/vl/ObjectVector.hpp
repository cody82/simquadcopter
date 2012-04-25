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

#ifndef ObjectVector_INCLUDE_DEFINE
#define ObjectVector_INCLUDE_DEFINE

#include "vl/Object.hpp"
#include <vector>

namespace vl
{

  template <typename T>
  class ObjectVector: public Object
  {
  public:
    void push_back( const T& data ) { mVector.push_back(data); }
    int size() const { return (int)mVector.size(); }
    bool empty() const { return mVector.empty(); }
    void clear() { mVector.clear(); }
    const T& back() const { mVector.back(); }
    T& back() { mVector.back(); }
    void reserve(int capacity) { mVector.reserve(capacity); }
    int capacity() const { return (int)mVector.capacity(); }
    const T& operator[](int i) const { return mVector[i]; }
    T& operator[](int i) { return mVector[i]; }
    const T& element(int i) const { return mVector[i]; }
    T& element(int i) { return mVector[i]; }
    void eraseElement(const T& data)
    {
      typename std::vector<T>::iterator it = std::find(mVector.begin(), mVector.end(), data);
      if (it != mVector.end())
        mVector.erase(it);
    }
    void eraseElementAt(int index)
    {
      if (index < mVector.size())
        mVector.erase(mVector.begin() + index);
    }
    void sort()
    {
      std::sort(mVector.begin(), mVector.end());
    }

  protected:
    std::vector< T > mVector;
  };

  typedef ObjectVector< ref<Actor> > ActorList;

}

#endif
