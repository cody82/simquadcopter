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

#ifndef OpenGLWidget_INCLUDE_DEFINE
#define OpenGLWidget_INCLUDE_DEFINE

#include "vl/Object.hpp"
#include "vl/ivec2.hpp"
#include "vl/EventListener.hpp"
#include <vector>
#include <set>

namespace vl
{

  class OpenGLWidget: public EventListener
  {
  public:
    OpenGLWidget(): mWidth(0), mHeight(0), mMouseVisible(true),  mGrabMouse(false), mGrabKeyboard(false), mContinuousUpdate(true), mIgnoreNextMouseMoveEvent(false)
    {
      addEventListerner( new EventForwarder(this) );
    }

    virtual ~OpenGLWidget() { removeAllEventListerners(); }

    virtual bool grabKeyboard() const { return mGrabKeyboard; }

    virtual bool grabMouse() const { return mGrabMouse; }

    virtual bool mouseVisible() const { return mMouseVisible; }

    virtual void setGrabKeyboard(bool) { }

    virtual void setGrabMouse(bool) { }

    virtual void setMouseVisible(bool) { }

    virtual void swapBuffers() = 0;

    virtual void setMousePosition(int x, int y) = 0;

    bool isKeyPressed(EKey key);

    void setVSyncEnabled(bool enable);

    bool vsyncEnabled() const;

    void setContinuousUpdate(bool continuous) { mContinuousUpdate = continuous; }

    bool continuousUpdate() const { return mContinuousUpdate; }

    void sortEventListeners();

    void addEventListerner(EventListener* el);

    void removeEventListerner(EventListener* el);

    void removeAllEventListerners();

    const OpenGLContextInfo& openglWidgetInfo() const { return mGLContextInfo; }
    void setOpenGLWidgetInfo(const OpenGLContextInfo& info) { mGLContextInfo = info; }

    void ignoreNextMouseMoveEvent() { mIgnoreNextMouseMoveEvent = true; }

    int width() const { return mWidth; }

    int height() const { return mHeight; }

  protected:
    std::vector< ref<EventListener> > mEventListener;
    std::set<EKey> mKeyboard;
    OpenGLContextInfo mGLContextInfo;
    int mWidth;
    int mHeight;
    bool mMouseVisible;
    bool mGrabMouse;
    bool mGrabKeyboard;
    bool mContinuousUpdate;
    bool mIgnoreNextMouseMoveEvent;
  };

}

#endif
