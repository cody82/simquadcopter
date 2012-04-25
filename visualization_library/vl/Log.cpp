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
#include "vl/CHECK.hpp"
#include <cstdio>

using namespace vl;

ref<Log> Log::mLogger;

void Log::print(ELogLevel level, std::string log)
{
  mLogger->print_implementation(level, log);
}

void Log::print(std::string log) { mLogger->print_implementation(LogSilent, log); }

void Log::debug(std::string log) { print(LogDebug, log); }

void Log::info(std::string log) { print(LogInfo, log); }

void Log::warning(std::string log) { print(LogWarning, log); }

void Log::alert(std::string log) { print(LogAlert, log); }

void Log::error(std::string log) { print(LogError, log); }

void Log::bug(std::string log) { print(LogBug, log); }

void LogPrintf::print_implementation(ELogLevel level, std::string log)
{
  if (log.empty())
    return;

	switch(level)
	{
	case LogSilent: break;
  case LogDebug:   log = "#  debug: "   + log; break;
  case LogInfo:    log = "#  info: "    + log; break;
  case LogWarning: log = "#  warning: " + log; break;
  case LogAlert:   log = "#  alert: "   + log; break;
  case LogError:   log = "#  error: "   + log; break;
  case LogBug:     log = "#  bug: "     + log; break;
	}

  fprintf( stdout,"%s", log.c_str() );

  FILE*fout = fopen(::vl::logfile_name.c_str(),"at"); 
  fprintf(fout,"%s", log.c_str()); 
  fclose(fout);
}

