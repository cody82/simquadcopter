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

#ifndef SDLAdapter_INCLUDE_DEFINE
#define SDLAdapter_INCLUDE_DEFINE

#include "vl/OpenGLWidget.hpp"
#include "vl/vec4.hpp"
#include "vl/vec3d.hpp"
#include <SDL.h>

namespace vlut
{
  class Program;
}

namespace vlSDL
{
  void sdlMainLoop();

  class SDL_Window: public vl::OpenGLWidget
  {
  public:
    SDL_Window(const std::string& title, const vl::OpenGLContextInfo& info, int width=640, int height=480);

    ~SDL_Window();

    virtual void swapBuffers();

    void translateEvent(SDL_Event * ev);

    void quit();

    void setWindowTitle(const std::string&);

    void setGrabKeyboard(bool grab)
    {
      mGrabKeyboard = mGrabMouse = grab;
      SDL_WM_GrabInput(grab ? SDL_GRAB_ON : SDL_GRAB_OFF);
    }

    void setGrabMouse(bool grab)
    {
      mGrabKeyboard = mGrabMouse = grab;
      SDL_WM_GrabInput(grab ? SDL_GRAB_ON : SDL_GRAB_OFF);
    }

    void setMouseVisible(bool visible)
    {
      mMouseVisible = visible;
      SDL_ShowCursor(visible ? SDL_ENABLE : SDL_DISABLE);
    }

    void setMousePosition(int x, int y)
    {
      SDL_WarpMouse((Uint16)x, (Uint16)y);
    }

    void update();

    static SDL_Surface* screen() { return mScreen; }

  protected:
    static SDL_Surface* mScreen;
  };

  vl::ref<vlSDL::SDL_Window> open_SDL_Window(const std::string& title, vlut::Program* program, const vl::OpenGLContextInfo& settings, int width, int height, vl::vec4 bkcolor, vl::vec3d pos, vl::vec3d look);
}

#endif
