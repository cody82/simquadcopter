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

#ifndef Time_INCLUDE_DEFINE
#define Time_INCLUDE_DEFINE

namespace vl
{

  class Time
  {
  public:
    Time();

    int year() const { return mYear; }
    int month() const { return mMonth; }
    int dayOfWeek() const { return mDayOfWeek; }
    int dayOfMonth() const { return mDayOfMonth; }
    int hour() const { return mHour; }
    int minute() const { return mMinute; }
    int second() const { return mSecond; }
    int microsecond() const { return mMicrosecond; }

    static double timerSeconds();

    void start() { mStart = timerSeconds(); }
    void stop()  { mStart = -1.0; }
    bool isStarted() const { return mStart != -1; }
    double elapsed() const { return mStart >= 0 ? timerSeconds() - mStart : -1; }

  protected:
    int mYear; // 1601 through 30827.
    int mMonth; // 1..12
    int mDayOfWeek; // 0 = Sunday, 6 = Saturday
    int mDayOfMonth; // 1..31
    int mHour; // 0..23
    int mMinute; // 0..59
    int mSecond; // 0..59
    int mMicrosecond; // 0 ... 999999

    double mStart;
  };
}

#endif
