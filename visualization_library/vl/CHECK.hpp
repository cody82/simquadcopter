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

#ifndef CHECK_INCLUDED
#define CHECK_INCLUDED

#define THROW_INT3_ON_FAILED_CHECK 1
#define EXIT_ON_FAILED_CHECK 0
#define HANG_ON_FAILED_CHECK 0

#include "vl/config.hpp"
#include <string>

namespace vl
{
  extern void (*check_reaction)(void);
  void check_reaction_int3();
  void check_reaction_exit();
  void check_reaction_hang();
  void check_reaction_continue();
  void check_failed(const char*, const char* file, int line);
}

namespace vl
{
  extern std::string logfile_name;

#if VL_CHECK_ENABLED
#define CHECK(expr) { if(!(expr)) ::vl::check_failed(#expr,__FILE__,__LINE__); }

#else
  #define CHECK(expr) {}

#endif

}

#endif
