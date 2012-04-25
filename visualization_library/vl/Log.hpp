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

#ifndef Log_INCLUDE_DEFINE
#define Log_INCLUDE_DEFINE

#include "vl/Object.hpp"
#include <string>

#ifndef NDEBUG
  #define PrintDebug(msg) { Log::print(LogDebug, msg); }
#else
	#define PrintDebug(msg) {}
#endif

namespace vl
{

  typedef enum 
  { 
    LogSilent, 
    LogDebug, 
    LogInfo, 
    LogWarning, 
    LogAlert, 
    LogError, 
    LogBug 
  } ELogLevel;

  class Log: public Object
  {
  public:
	  static void print(ELogLevel level, std::string log);
    static void print(std::string log);
	  static void debug(std::string log);
	  static void info(std::string log);
	  static void warning(std::string log);
	  static void alert(std::string log);
	  static void error(std::string log);
	  static void bug(std::string log);
    static void setLogger(Log* logger) { mLogger = logger; }
    static Log* logger() { return mLogger.get(); }

  protected:
	  static ref<Log> mLogger;
	  virtual void print_implementation(ELogLevel level, std::string log) = 0;
  };

  class LogPrintf: public Log
  {
  protected:
	  virtual void print_implementation(ELogLevel level, std::string log);
  };
}

#endif
