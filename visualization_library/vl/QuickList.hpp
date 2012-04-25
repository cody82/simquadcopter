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

#ifndef QuickList_INCLUDE_DEFINE
#define QuickList_INCLUDE_DEFINE

#include "vl/Object.hpp"

namespace vl
{

  template <typename T>
  class QuickList
  {
  public:
    QuickList()
    {
      List = NULL;
      FreeList = NULL;
      FreeElements = 0;
      UsedElements = 0;
    }

    ~QuickList()
    {
      Element* el = List;
      while(el)
      {
        Element* next = el->Next;
        delete el;
        el = next;
      }

      el = FreeList;
      while(el)
      {
        Element* next = el->Next;
        delete el;
        el = next;
      }
    }

	  class Element: public Object
	  {
	  friend class QuickList;
    public:
      Element()
      {
        Next = NULL;
        Prev = NULL;
      }

      ~Element()
      {

      }

      Element* next() const
      {
        return Next;
		  }

		  Element* prev() const
		  {
		    return Prev;
		  }

	    T Data;

	  protected:
	    Element* Next;
	    Element* Prev;
	  };

	  Element* addToFront(T data)
	  {
	    Element* el = NULL;
		  if (FreeList == NULL)
		  {

		    el = new Element;

		  }
		  else
		  {

			  el = FreeList;
			  FreeList = FreeList->Next;
        FreeElements--;
		  }
      UsedElements++;

		  el->Data = data;

		  el->Prev = NULL;
		  el->Next = List;
		  if (List)
			  List->Prev = el;
		  List = el;
		  return el;
	  }

	  Element* addToFront()
	  {
	    Element* el = NULL;
		  if (FreeList == NULL)
		  {

		    el = new Element;

		  }
		  else
		  {

			  el = FreeList;
			  FreeList = FreeList->Next;
        FreeElements--;
		  }
      UsedElements++;

		  el->Prev = NULL;
		  el->Next = List;
		  if (List)
			  List->Prev = el;
		  List = el;
		  return el;
	  }

	  void freeElement(Element* el)
	  {

	    if (el == FreeList)
	      return;

      FreeElements++;
      UsedElements--;

	    if (el->Prev == NULL) // am the head
	      List = el->Next;
	    else
	      el->Prev->Next = el->Next;

	    if (el->Next)
	     el->Next->Prev = el->Prev;

		  el->Prev = NULL;
		  el->Next = FreeList;
		  if (FreeList)
			  FreeList->Prev = el;
		  FreeList = el;
	  }

	  Element* begin()
	  {
	    return List;
	  }

	  void clear()
	  {

      if (List == NULL)
        return;
      else
      if (FreeList == NULL)
      {
        FreeList = List;
        List = NULL;
      }
      else

      if (FreeElements < UsedElements)
      {

	      Element* el = FreeList;
        while(el->Next)
	        el = el->Next;
        el->Next = List;
        if (List)
          List->Prev = el;
        List = NULL;
      }
      else

      {

	      Element* el = List;
        while(el->Next)
	        el = el->Next;
        el->Next = FreeList;
        if (FreeList)
          FreeList->Prev = el;
        FreeList = List;
        List = NULL;
      }

      FreeElements += UsedElements;
      UsedElements = 0;
	  }

	  int countFreeElements()
	  {
	    Element* el = FreeList;
	    int count = 0;
	    while(el)
	    {
	      count ++;
	      el = el->next();
	    }

	    return count;
	  }

	  int countUsedElements()
	  {
	    Element* el = List;
	    int count = 0;
	    while(el)
	    {
	      count ++;
	      el = el->next();
	    }

	    return count;
	  }

    int freeElements() const { return FreeElements; }
    int usedElements() const { return UsedElements; }

  protected:
	  Element* List;
	  Element* FreeList;
    int FreeElements;
    int UsedElements;
  };

  template <class T>
  class QuickAllocator
  {
  public:
    typedef ref<T> PtrT;
    QuickList< PtrT > List;

  public:
    virtual ~QuickAllocator() {}

    ref<T> allocate()
    {
      typename QuickList< PtrT >::Element * el;
      el = List.addToFront();
      if (!el->Data)
        el->Data = new T;
      return el->Data;
    }
    void clear()
    {
      List.clear();
    }
  };

}

#endif
