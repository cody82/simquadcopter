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

#ifndef OpenGL_INCLUDE_DEFINE
#define OpenGL_INCLUDE_DEFINE

#include "vl/CHECK.hpp"
#include <cstdio> // FILE

#include "GL/glew.h"
#ifdef WIN32
  #include "GL/wglew.h"
  #undef min
  #undef max
#endif

namespace vl
{

  void glcheck(int level, const char* file, int line);

  void print_system_info(FILE*);

  extern std::string logfile_name;

  extern int glcheck_level;

  extern bool can_use_opengl;

  #if VL_GLCHECK_ENABLED

    #define GLCHECK4() ::vl::glcheck(4, __FILE__, __LINE__);
    #define GLCHECK3() ::vl::glcheck(3, __FILE__, __LINE__);
    #define GLCHECK2() ::vl::glcheck(2, __FILE__, __LINE__);
    #define GLCHECK1() ::vl::glcheck(1, __FILE__, __LINE__);

    #define CLEAR_GL_ERROR() { glGetError(); }

  #else

	  #define GLCHECK4() ;
    #define CLEAR_GL_ERROR() ;

  #endif

  void init_glew();
}

#endif
