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

#ifndef Say_INCLUDE_DEFINE
#define Say_INCLUDE_DEFINE

#include <vector>
#include <string>

namespace vl
{

  class SayArg
  {
  friend class Say;
  public:
    SayArg();

    explicit SayArg(const char* d);

    explicit SayArg(std::string d);

    explicit SayArg(double d);

    explicit SayArg(float d);

    explicit SayArg(unsigned char d);

    explicit SayArg(signed char d);

    explicit SayArg(unsigned short d);

    explicit SayArg(signed short d);

    explicit SayArg(unsigned int d);

    explicit SayArg(signed int d);

    explicit SayArg(unsigned long d);

    explicit SayArg(signed long d);

    explicit SayArg(unsigned long long d);

    explicit SayArg(signed long long d);

  protected:
    void init();

    std::string str;
    double float64;
    unsigned long long ulonglong;
    signed   long long slonglong;

    enum
    {
      NO_TYPE,
      STRING,
      FLOAT64,
      ULONGLONG,
      SLONGLONG
    } type;

  };

  class Say: public std::vector<SayArg>
  {
  public:
    std::string format_string;

    Say(const std::string& fstr)
    {
      format_string = fstr;
    }

    Say& operator<<(const SayArg& p)
    {
      push_back(p);
      return *this;
    }

    template <typename T>
    Say& operator<<(T p)
    {
      push_back(SayArg(p));
      return *this;
    }

    operator std::string()
    {
      return parse(*this);
    }

    std::string str() const
    {
      return parse(*this);
    }

  protected:
    std::string parse( const Say& pset ) const;

    std::string euronotation(std::string str, int base) const;
      
    std::string format(unsigned long long n, int base, int field, int decimals, int align, int fill, int plus, int finalizer, int eur) const;

    std::string format(signed long long nn, int base, int field, int decimals, int align, int fill, int plus, int finalizer, int eur) const;
      
    std::string format(double num, int base, int field, int decimals, int align, int fill, int plus, int finalizer, int eur) const;

    std::string pipeline(std::string str, int base, int field, int decimals, int finalizer, int align, int eur, int fill, int negative, int plus) const;
  };
}

#endif
