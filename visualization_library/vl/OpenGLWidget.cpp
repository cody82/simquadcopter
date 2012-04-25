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

#include "vl/OpenGLWidget.hpp"
#include "vl/OpenGL.hpp"
#include "vl/CHECK.hpp"
#include <algorithm>

using namespace vl;

namespace 
{
  bool sort_listeners( const ref<EventListener>& a, const ref<EventListener>& b)
  {
    return a->rank() < b->rank();
  }
}

void EventListener::setRank(int rank) 
{
  mRank = rank;
  if ( openglWidget() )
    openglWidget()->sortEventListeners();
}

int EventListener::rank() const 
{
  return mRank;
}

OpenGLWidget* EventListener::openglWidget() { return mOpenGLWidget; }

void OpenGLWidget::sortEventListeners()
{
  std::sort(mEventListener.begin(), mEventListener.end(), sort_listeners);
}

void OpenGLWidget::addEventListerner(EventListener* el)
{
  CHECK( el->mOpenGLWidget == NULL );
  mEventListener.push_back(el);
  el->mOpenGLWidget = this;
  el->boundToOpenGLWidget(this);
  sortEventListeners();
}

void OpenGLWidget::removeEventListerner(EventListener* el)
{
  CHECK( el->mOpenGLWidget != NULL );
  if( std::find(mEventListener.begin(), mEventListener.end(), el) != mEventListener.end() )
  {
    mEventListener.erase( std::find(mEventListener.begin(), mEventListener.end(), el) );
    el->mOpenGLWidget = NULL;
  }
}

void OpenGLWidget::removeAllEventListerners()
{
  std::vector< ref<EventListener> >::iterator it = mEventListener.begin();
  for(; it != mEventListener.end(); it++)
    it->get()->mOpenGLWidget = NULL;

  mEventListener.clear();
}

bool OpenGLWidget::isKeyPressed(EKey key)
{
  return mKeyboard.find(key) != mKeyboard.end();
}

void OpenGLWidget::setVSyncEnabled(bool enable)
{
#ifdef WIN32
  if (WGLEW_EXT_swap_control)
    wglSwapIntervalEXT(enable?1:0);
#endif
}

bool OpenGLWidget::vsyncEnabled() const
{
#ifdef WIN32
  if (WGLEW_EXT_swap_control)
    return wglGetSwapIntervalEXT() != 0;
  else
    return false;
#else
  return false;
#endif

}

