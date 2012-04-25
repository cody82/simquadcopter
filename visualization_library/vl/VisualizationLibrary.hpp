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

#ifndef VisualizationLibrary_INCLUDE_DEFINE
#define VisualizationLibrary_INCLUDE_DEFINE

#include <string>

namespace vl
{
  extern std::string logfile_name;
  extern int glcheck_level;
  extern bool can_use_opengl;
  void visualization_library_init();
  void visualization_library_shutdown();
  std::string envVarValue(const std::string& name);
}

#endif
