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

#ifndef GLUT_Window_INCLUDE_DEFINE
#define GLUT_Window_INCLUDE_DEFINE

#include "vl/OpenGLWidget.hpp"
#include "vl/vec3d.hpp"
#include "vl/vec4.hpp"
#include <GL/freeglut.h>
#include <map>

namespace vlut
{
  class Program;
}

namespace vlGLUT
{
  class GLUT_Window: public vl::OpenGLWidget
  {
  public:
    ~GLUT_Window()
    {
      quit();
    }

    GLUT_Window(const std::string& title, const vl::OpenGLContextInfo& info, int x, int y, int width, int height);

    virtual void setMouseVisible(bool visible);

    virtual void setMousePosition(int x, int y);

    void update();

    void quit();

    void updateOverlay();

    void swapBuffers();

    void setWindowTitle(const std::string& title) ;

    int handle() const { return mHandle; }

    void setPosition(int x, int y) ;

    void setSize(int w, int h) ;

    vl::ivec2 position() const;

    vl::ivec2 size() const;

  protected:
    void initKeymap();

    vl::EKey mapAsciiKey(unsigned char ascii);

    static vl::EKey mapSpecialKey(int special_key);

    void updateModifiers();

    static void glut_keyboard_func(unsigned char ch, int x, int y);

    static void glut_keyboard_up_func(unsigned char ch, int x, int y);

    static void glut_special_func(int key, int x, int y);

    static void glut_special_up_func(int key, int x, int y);

    static void glut_mouse_func(int button, int state, int x, int y);

    static void glut_motion_func(int x, int y);

    static void glut_passive_motion_func(int x, int y);

    static void glut_mouse_wheel_func(int a, int rotation, int c, int d);

    static void glut_visibility_func(int visibility);

    static void glut_reshape_func(int w, int h);

    static void glut_display_func();

    static void glut_wmclose_func();

    static void glut_close_func();

  protected:
    int mHandle;
    std::map<unsigned char, vl::EKey> mKeymap;

    static std::map< int, GLUT_Window* > mWinMap;
    bool mGlewOK;
  };

  void atexit_visualization_library_shutdown();

  vl::ref<vlGLUT::GLUT_Window> open_GLUT_Window(const std::string& title, vlut::Program* program, const vl::OpenGLContextInfo& info, int x, int y, int width, int height, vl::vec4 bkcolor, vl::vec3d pos, vl::vec3d look);
}

#endif
