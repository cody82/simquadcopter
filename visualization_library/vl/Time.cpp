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

#include "vl/Time.hpp"
#include <ctime>
#include <cmath>
#ifdef WIN32
  #include <windows.h>
#else
  #include <sys/time.h> // gettimeofday
#endif

using namespace vl;

Time::Time()
{
  mStart = -1;

#ifdef WIN32
  SYSTEMTIME local_time;
  GetLocalTime(&local_time);
  mYear = local_time.wYear;
  mMonth = local_time.wMonth;
  mDayOfWeek = local_time.wDayOfWeek;
  mDayOfMonth = local_time.wDay;
  mHour = local_time.wHour;
  mMinute = local_time.wMinute;
  mSecond = local_time.wSecond;
  mMicrosecond = local_time.wMilliseconds * 1000;
#else
  time_t secs;
  time(&secs);
  tm* date = localtime( &secs );

  mYear = date->tm_year + 1900;
  mMonth = date->tm_mon;
  mDayOfWeek = date->tm_wday;
  mDayOfMonth = date->tm_mday;
  mHour = date->tm_hour;
  mMinute = date->tm_min;
  mSecond = date->tm_sec;

  struct timeval tv;
  gettimeofday( &tv, NULL );
  mMicrosecond = tv.tv_usec;
#endif
}

double Time::timerSeconds()
{
#ifdef WIN32

  LARGE_INTEGER Frequency;
  LARGE_INTEGER PerformanceCount;
  BOOL has_timer = QueryPerformanceFrequency( &Frequency );
  if (has_timer)
  {
    QueryPerformanceCounter( &PerformanceCount );
    return (double)PerformanceCount.QuadPart/Frequency.QuadPart;
  }
  else
  {
    return (double)GetTickCount() / 1000.0;
  }
#else
  struct timeval tv;
  gettimeofday( &tv, NULL );
  return (double) tv.tv_sec + (double) tv.tv_usec * 0.000001;
#endif
}

