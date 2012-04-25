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

#ifndef Flags_INCLUDE_DEFINE
#define Flags_INCLUDE_DEFINE

namespace vl
{
  template <typename T>
  class Flags
  {
  public:
    typedef T EFlagsType;

    Flags()
    {
      mValue = 0;
    }
    Flags(T flag)
    {
      mValue = flag;
    }
    Flags(const Flags& flag)
    {
      mValue = flag.mValue;
    }

    inline operator unsigned int() const { return mValue; }

    Flags operator|(const Flags& flag) const { Flags f; f.mValue = mValue | flag.mValue; return f; }
    Flags operator|(T flag) const { Flags f; f.mValue = mValue | flag; return f; }
    Flags operator&(const Flags& flag) const { Flags f; f.mValue = mValue & flag.mValue; return f; }
    Flags operator&(T flag) const { Flags f; f.mValue = mValue & flag; return f; }
    Flags operator^(const Flags& flag) const { Flags f; f.mValue = mValue ^ flag.mValue; return f; }
    Flags operator^(T flag) const { Flags f; f.mValue = mValue ^ flag; return f; }
    Flags operator~() const { Flags f; f.mValue = ~mValue; return f; }

    const Flags& operator|=(const Flags& flag) { mValue = mValue | flag.mValue; return f; }
    const Flags& operator|=(T flag) { mValue = mValue | flag; return f; }
    const Flags& operator&=(const Flags& flag) { mValue = mValue & flag.mValue; return f; }
    const Flags& operator&=(T flag) { mValue = mValue & flag; return f; }
    const Flags& operator^=(const Flags& flag) { mValue = mValue ^ flag.mValue; return f; }
    const Flags& operator^=(T flag) { mValue = mValue ^ flag; return f; }

  protected:
    unsigned int mValue;
  };

  #define VL_DECLARE_FLAGS(Type, Name) typedef Flags<Type> Name;

  #define VL_DECLARE_FLAGS_OPERATORS(Name) \
  inline Flags<Name::EFlagsType> operator|(Name::EFlagsType f1, Name::EFlagsType f2) { return Flags<Name::EFlagsType>(f1) | Flags<Name::EFlagsType>(f2); } \
  inline Flags<Name::EFlagsType> operator|(Name::EFlagsType f1, Flags<Name::EFlagsType> f2) { return f2 | f1; } \
  inline Flags<Name::EFlagsType> operator&(Name::EFlagsType f1, Name::EFlagsType f2) { return Flags<Name::EFlagsType>(f1) & Flags<Name::EFlagsType>(f2); } \
  inline Flags<Name::EFlagsType> operator&(Name::EFlagsType f1, Flags<Name::EFlagsType> f2) { return f2 & f1; } \
  inline Flags<Name::EFlagsType> operator^(Name::EFlagsType f1, Name::EFlagsType f2) { return Flags<Name::EFlagsType>(f1) ^ Flags<Name::EFlagsType>(f2); } \
  inline Flags<Name::EFlagsType> operator^(Name::EFlagsType f1, Flags<Name::EFlagsType> f2) { return f2 ^ f1; }
}

#endif
