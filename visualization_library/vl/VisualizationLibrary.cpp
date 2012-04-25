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

#include "vl/Log.hpp"
#include "vl/Say.hpp"
#include "vl/Time.hpp"
#include "vl/Log.hpp"
#include "vl/GlobalState.hpp"
#include "vl/../../3rdparty-req/freetype-2.3.5/include/ft2build.h"
#include FT_FREETYPE_H

namespace vl
{
  std::string logfile_name = "log.txt";

  int glcheck_level = 4;

  bool can_use_opengl = true;

  std::map<std::string, std::string>* mEnvVars = NULL;

  FT_Library FreeTypeLibrary = NULL;

  void setupEnvironmentVariables();

  void dump_env_vars()
  {
    Log::print("Environment variables:\n");
    for(std::map<std::string, std::string>::iterator it = mEnvVars->begin(); it != mEnvVars->end(); ++it)
    {
      Log::print( it->first + " = " + it->second + "\n" );
    }
    Log::print( "\n" );
  }

  void visualization_library_init()
  {
    setupEnvironmentVariables();

    Log::setLogger( new LogPrintf );

    Log::print("visualization_library_init()\n\n");

    FT_Error error = FT_Init_FreeType( &FreeTypeLibrary );
    if ( error )
    {
      Log::error("An error occurred during FreeType library initialization!\n");
      CHECK(0);
    }

    dump_env_vars();
  }

  void visualization_library_shutdown()
  {
    CHECK(FreeTypeLibrary);
    FT_Done_FreeType(FreeTypeLibrary);
    FreeTypeLibrary = NULL;

    can_use_opengl = false;

    GlobalState::shutdown();

    delete mEnvVars; mEnvVars = NULL;

    Log::print("visualization_library_shutdown()\n\n");

    Log::setLogger( NULL );

  #if DEBUG_LIVING_OBJECTS
    int i = 0;
    std::set< Object* >::iterator it = Object::debug_living_objects()->begin();
    CHECK( it == Object::debug_living_objects()->end());
    while( it != Object::debug_living_objects()->end() )
    {
      printf("[%d] %p '%s'\n", i, *it, (*it)->name().c_str());
      it++;
      i++;
    }
  #endif
  }

  void glcheck(int level, const char* file, int line)
  {
    if (glcheck_level < level)
      return;

    GLuint glerr = glGetError();
    if (glerr != GL_NO_ERROR)
    {
      std::string msg( (char*)gluErrorString(glerr) );
      Log::error( Say("glGetError() [%s:%n]: %s\n") << file << line << msg.c_str() );
      check_reaction();
    }
  }

  void (*check_reaction)(void) = check_reaction_int3;

  void check_reaction_int3()
  {
    #if defined(WIN32) && !defined(__MINGW32__)
	    __asm {int 3}
    #else
	    asm("int $0x3");
    #endif
  }

  void check_reaction_exit()
  {
    exit(24);
  }

  void check_reaction_hang()
  {
    while(true) {}
  }

  void check_reaction_continue()
  {

  }

  void check_failed(const char* expr, const char* file, int line)
  {
    Log::error( ::vl::Say("CHECK(%s) failed at %s:%n\n") << expr << file << line );
    fflush(stdout);
    fflush(stderr);
    ::vl::check_reaction();
  }

  std::string envVarValue(const std::string& name)
  {
    if (mEnvVars->find(name) != mEnvVars->end())
      return mEnvVars->find(name)->second;
    else
      return "";
  }

  void setEnvVar(const std::string& name, const std::string& value)
  {
    (*mEnvVars)[name] = value;
  }

  void setupEnvironmentVariables()
  {
    mEnvVars = new std::map<std::string, std::string>;

    char* val = NULL;

    val = ::getenv("VL_DATA_PATH");
    if (val)
    {
      setEnvVar( "VL_DATA_PATH", val );

    }
    else
    {
      setEnvVar( "VL_DATA_PATH", "../data" );

    }

    val = ::getenv("VL_LOGFILE_PATH");
    if (val)
    {
      setEnvVar( "VL_LOGFILE_PATH", val );

    }
    else
    {
      setEnvVar( "VL_LOGFILE_PATH", "./log.txt" );

    }

    logfile_name = envVarValue("VL_LOGFILE_PATH");    
  }
}
