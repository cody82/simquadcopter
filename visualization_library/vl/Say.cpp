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

#include "vl/Say.hpp"
#include <cmath>
#include <string.h>

using namespace vl;

SayArg::SayArg()
{
  init();
}

SayArg::SayArg(const char* d)
{
  init();
  if (d)
    str = d;
  type = STRING;
}

SayArg::SayArg(std::string d)
{
  init();
  str = d;
  type = STRING;
}

SayArg::SayArg(double d)
{
  init();
  float64 = d;
  type = FLOAT64;
}

SayArg::SayArg(float d)
{
  init();
  float64 = d;
  type = FLOAT64;
}

SayArg::SayArg(unsigned char d)
{
  init();
  ulonglong = d;
  type = ULONGLONG;
}

SayArg::SayArg(signed char d)
{
  init();
  slonglong = d;
  type = SLONGLONG;
}

SayArg::SayArg(unsigned short d)
{
  init();
  ulonglong = d;
  type = ULONGLONG;
}

SayArg::SayArg(signed short d)
{
  init();
  slonglong = d;
  type = SLONGLONG;
}

SayArg::SayArg(unsigned int d)
{
  init();
  ulonglong = d;
  type = ULONGLONG;
}

SayArg::SayArg(signed int d)
{
  init();
  slonglong = d;
  type = SLONGLONG;
}

SayArg::SayArg(unsigned long d)
{
  init();
  ulonglong = d;
  type = ULONGLONG;
}

SayArg::SayArg(signed long d)
{
  init();
  slonglong = d;
  type = SLONGLONG;
}

SayArg::SayArg(unsigned long long d)
{
  init();
  ulonglong = d;
  type = ULONGLONG;
}

SayArg::SayArg(signed long long d)
{
  init();
  slonglong = d;
  type = SLONGLONG;
}

void SayArg::init()
{
  type = NO_TYPE;
  float64 = 0;
  ulonglong = 0;
  slonglong = 0;
}

std::string Say::parse( const Say& pset ) const
{
  std::string out;
  std::string fmt = pset.format_string;

  int param_idx = 0;

  int eur = -1;
  int base = -1;
  int field = -1;
  int decimals = -1;
  int align = -1; 
  int fill = -1;
  int plus = -1;
  
  int fmtstart = -1;

  bool fmtdata = false;
  for(int i=0; i<(int)fmt.length(); i++)
  {
    int ch = (i<(int)fmt.length()) ? fmt[i] : -1;
    int next_ch = (i<(int)fmt.length()-1) ? (fmt[i+1]) : -1;
    int nnext_ch = (i<(int)fmt.length()-2) ? (fmt[i+2]) : -1;

    if (!fmtdata)
    {
      if (ch == '%' && next_ch == '%')
      {
        out.push_back('%');
        i++;
      }
      else
      if (ch == '%')
      {
        if (param_idx < (int)pset.size())
        {
          fmtdata = true;
          fmtstart = i;
        }
        else
        {
          out += " !!!too few parameters: %";
        }
      }
      else
      if (ch >= 0)
      {
        out.push_back((char)ch);
      }
    }
    else
    {

      if(eur == -1)
      {
        if (ch == '$')
        {
          eur = 1;
          continue;
        }
      }

      if (base == -1)
      {
        switch(ch)
        {
        case 'b': base = 2;  break;
        case 'o': base = 8;  break;
        case 'd': base = 10; break;
        case 'h': base = 16; break;
        }
        if (base != -1)
        {
          if (eur == -1)
            eur = 0;
          continue;
        }
      }

      if (plus == -1)
      {
        switch(ch)
        {
        case '+': plus = 1; break;
        }
        if (plus != -1)
        {
          if (base == -1)
            base = 10;
          if (eur == -1)
            eur = 0;
          continue;
        }
      }

      if (fill == -1)
      {
        switch(ch)
        {
        case '0': fill = '0'; break;
        case ' ': fill = ' '; break;
        }
        if (fill != -1)
        {
          if (base == -1)
            base = 10;
          if (plus == -1)
            plus = 0;
          if (eur == -1)
            eur = 0;
          continue;
        }
      }

      if (field == -1)
      {
        if (ch >= '0' && ch <= '9')
        {
          field = ch - '0';
          if (next_ch >= '0' && next_ch <= '9')
          {
            field = field*10 + next_ch - '0';
            i++;
          }
        }

        if (field != -1)
        {
          if (base == -1)
            base = 10;
          if (plus == -1)
            plus = 0;
          if (fill == -1)
            fill = ' ';
          if (eur == -1)
            eur = 0;
          continue;
        }
      }

      if (decimals == -1)
      {
        if(ch == '.')
        {
          if (next_ch >= '0' && next_ch <= '9')
          {
            decimals = next_ch - '0';
            i++;
            if (nnext_ch >= '0' && nnext_ch <= '9')
            {
              decimals = decimals*10 + nnext_ch - '0';
              i++;
            }
          }
        }

        if (decimals != -1)
        {
          if (base == -1)
            base = 10;
          if (plus == -1)
            plus = 0;
          if (fill == -1)
            fill = ' ';
          if (field == -1)
            field = 0;
          if (eur == -1)
            eur = 0;
          continue;
        }
      }

      if (align == -1)
      {
        if(ch == '=')
          align = 0;
        if(ch == '<')
          align = 1;
        if(ch == '>')
          align = 2;

        if (align != -1)
        {
          if (base == -1)
            base = 10;
          if (plus == -1)
            plus = 0;
          if (fill == -1)
            fill = ' ';
          if (field == -1)
            field = 0;
          if (eur == -1)
            eur = 0;
          if (decimals == -1)
          {
            switch(pset[param_idx].type)
            {
            case SayArg::FLOAT64: decimals = 6; break;
            default: decimals = 0; break;
            }
          }
          continue;
        }
      }

      const SayArg& p = pset[param_idx];

      if (ch == 'c')
      {
        if (fmtstart != i-1)
          out += " !!! '%c' doesn't need arguments !!! ";

        switch(p.type)
        {
        case SayArg::FLOAT64: out += (char)p.float64; break;
        case SayArg::SLONGLONG: out += (char)p.slonglong; break;
        case SayArg::ULONGLONG: out += (char)p.ulonglong; break;
        default:
          out += " !!! wrong argument type for '%c' !!! ";
          break;
        }

      }
      else
      if (ch == 's')
      {
        if (fmtstart != i-1)
          out += " !!! '%s' doesn't need arguments !!! ";

        switch(p.type)
        {
        case SayArg::STRING: out += p.str; break;
        default:
          out += " !!! wrong argument type for '%s' !!! ";
          break;
        }

      }
      else
      if (ch == 'n' || ch == 'N' || ch == 'e' || ch == 'E')
      {

        if (param_idx<(int)pset.size())
        {
          if (decimals == -1)
          {
            switch(p.type)
            {
            case SayArg::FLOAT64: decimals = 6; break;
            default: decimals = 0; break;
            }
          }

          if (base == -1)
            base = 10;
          if (field == -1)
            field = 0;
          if (decimals == -1)
            decimals = 0;
          if (fill == -1)
            fill = ' ';
          if (plus == -1)
            plus = 0;
          if (align == -1)
            align = 2;
          if (eur == -1)
            eur = 0;

          switch(p.type)
          {
          case SayArg::FLOAT64: out += format(p.float64, base, field, decimals, align, fill, plus, ch, eur) ; break;
          case SayArg::SLONGLONG: out += format(p.slonglong, base, field, decimals, align, fill, plus, ch, eur) ; break;
          case SayArg::ULONGLONG: out += format(p.ulonglong, base, field, decimals, align, fill, plus, ch, eur) ; break;
          default: 
            out += " !!! wrong argument type for '%n' !!! ";
            break;
          }
        }
        else
        {
          out += " !!!missing parameter!!! ";
          if (ch != -1)
            i--;
        }
      }
      else
      {
        out += " !!!format error: unexpected '";
        out += (char)ch;
        out += "' !!! ";
      }

      fmtdata = false;
      align = -1;
      base = -1;
      field = -1;
      decimals = -1;
      align = -1; 
      fill = -1;
      plus = -1;
      eur = -1;

      param_idx++;
    }
  }

  if (fmtdata)
  {
    out += " !!!truncated format!!! ";
    param_idx++;
  }

  if (param_idx < (int)pset.size())
    out += " !!!too many parameters!!! ";

  return out;

}

std::string Say::euronotation(std::string str, int base) const
{
  std::string tmp;
  int pos = (int)str.length();
  if (strchr(str.c_str(), '.'))
  {
    while(pos--)
    {
      if (str[pos] == '.')
      {
        tmp.insert(tmp.begin(), ',');
        break;
      }
      tmp.insert(tmp.begin(), str[pos]);
    }
    if (pos < 0)
      pos = (int)str.length();
  }

  int count = 0;
  int wait = 3;
  if (base == 2)
    wait = 4;
  if (base == 16)
    wait = 2;
  while(pos--)
  {
    if (count && count % wait == 0)
    {
      tmp.insert(tmp.begin(), '.');
    }
    tmp.insert(tmp.begin(), str[pos]);
    count ++;
  }

  return tmp;
}
  
std::string Say::format(unsigned long long n, int base, int field, int decimals, int align, int fill, int plus, int finalizer, int eur) const
{
  if (field < 0)
    field = -field;

  if (field > 1024)
    field = 1024;

  if (decimals < 0)
    decimals = -decimals;
  if (decimals > 20)
    decimals = 20;

  if (align != 0 && align != 1 && align != 2)
    align = 0;

  if (base > 16)
    base = 16;

  if (base < 2)
    base = 2;

  std::string str;

  char* hex = "0123456789abcdef";

  int k = base;
  do
  {
    int x = (int)(n % base);
    int c = x/(k/base);
    str.insert(str.begin(), hex[c]);
    n = n  / base;
  }
  while(n);

  if (decimals)
  {
    str.push_back('.');
    int i = decimals;
    while(i--)
      str.push_back('0');
  }

  bool negative = false;

  return pipeline(str, base, field, decimals, finalizer, align, eur, fill, negative, plus); 
}

std::string Say::format(signed long long nn, int base, int field, int decimals, int align, int fill, int plus, int finalizer, int eur) const
{
  if (field < 0)
    field = -field;

  if (field > 1024)
    field = 1024;

  if (decimals < 0)
    decimals = -decimals;
  if (decimals > 20)
    decimals = 20;

  if (align != 0 && align != 1 && align != 2)
    align = 0;

  if (base > 16)
    base = 16;

  if (base < 2)
    base = 2;

  std::string str;

  char* hex = "0123456789abcdef";

  bool negative = nn < 0;
  unsigned long long n;

  if (nn<0 && -nn<0) // overflow
    n = (unsigned long long)nn;
  else
  if (nn<0)
    n = - nn;
  else
    n = nn;

  int k = base;
  do
  {
    int x = (int)(n % base);
    int c = x/(k/base);
    str.insert(str.begin(), hex[c]);
    n = n  / base;
  }
  while(n);
  
  if (decimals)
  {
    str.push_back('.');
    int i = decimals;
    while(i--)
      str.push_back('0');
  }

  return pipeline(str, base, field, decimals, finalizer, align, eur, fill, negative, plus); 
}
  
std::string Say::format(double num, int base, int field, int decimals, int align, int fill, int plus, int finalizer, int eur) const
{
  if (field < 0)
    field = -field;
  if (field > 1024)
    field = 1024;

  if (decimals < 0)
    decimals = -decimals;
  if (decimals > 20)
    decimals = 20;

  if (align != 0 && align != 1 && align != 2)
    align = 0;

  if (base > 16)
    base = 16;

  if (base < 2)
    base = 2;

  std::string str;

  char* hex = "0123456789abcdef";

  double f = num;

  float tmp = (float)f;
  unsigned char *nan= (unsigned char*)&tmp;
  const char* sign = nan[3] >= 128 ? "-" : "+";
  unsigned char exp = (nan[3] << 1) + (nan[2] >> 7);
  nan[2] &= 127;
  unsigned int frac = nan[0] + (nan[1] << 8) + (nan[2] << 16);

  bool negative = false;
  if (exp == 255 && frac == 0)
  {
    return std::string(sign) + "#INF";
  }
  else
  if (exp == 255 && frac != 0)
  {
    return "#NAN";
  }
  else
  {

    if (finalizer == 'n' || finalizer == 'N')
    {
      double fp = f - floor(f); 
      double eps = base/2;
      int dec = decimals;
      do
      {
        if ( !(dec--) )
          break;

        int c = (int)(fp * base);
        fp = fp * base - c;

        eps /= base;

        if (c<0 || c>15)
        {
          return "#ERR";
        }

        if (dec == 0) // round only if all the decimals are here
        {

          f += eps/base;
          break;
        }
      }
      while(fp>0);
    }
      
    if (f < 0)
    {
      f = -f;
      negative = true;
    }
    double n = floor(f);

    int count = 0; 
    unsigned int base2 = base*base;
    unsigned int base3 = base*base*base;
    unsigned int base4 = base*base*base*base;
    unsigned int base5 = base*base*base*base*base;
    unsigned int base6 = base*base*base*base*base*base;
    unsigned int base7 = base*base*base*base*base*base*base; // maximum number in base 16
    while (floor(n))
    {
      if (n>=base7)
      {
        n /= base7;
        count+=7;
      }
      else
      if (n>=base6)
      {
        n /= base6;
        count+=6;
      }
      else
      if (n>=base5)
      {
        n /= base5;
        count+=5;
      }
      else
      if (n>=base4)
      {
        n /= base4;
        count+=4;
      }
      else
      if (n>=base3)
      {
        n /= base3;
        count+=3;
      }
      else
      if (n>=base2)
      {
        n /= base2;
        count+=2;
      }
      else
      {
        n = n / base;
        count++;
      }
    }

    double eps = (base / 2.0) / base;
    for(int i=0; i<count; i++)
    {
      eps /= base;
    }
    n+=eps;

    if (count)
    {
      do
      {
        int c = (int)(n * (double)base);
        n = n * (double)base - floor(n * (double)base);
        int next = (int)(n * base);

        if (c<0 || c>15 || next<0 || next>15)
        {
          return "#ERR";
        }

        str.push_back(hex[c]);
      }
      while(--count);
    }
    else
      str.push_back('0');

    str.push_back('.');

    double fp = f - floor(f);
    do
    {
      int c = (int)(fp * base);
      fp = fp * base - c;

      if (c<0 || c>15)
      {
        return "#ERR";
      }

      str.push_back(hex[c]);
    }
    while(fp>0);

    return pipeline(str, base, field, decimals, finalizer, align, eur, fill, negative, plus);
  }
}

std::string Say::pipeline(std::string str, int base, int field, int decimals, int finalizer, int align, int eur, int fill, int negative, int plus) const
{

  int shift = 0;

  if (finalizer == 'e' || finalizer == 'E')
  {
    int ptpos = (int)str.length(); // point position
    int nzpos = -1; // non zero position
    for(int i=0; i<(int)str.size(); i++)
    {
      if(str[i] != '0' && nzpos == -1 && str[i] != '.')
        nzpos = i;
      else
      if (str[i] == '.')
        ptpos = i;
    }

    if (nzpos == -1)
      shift = 0;
    else
      shift = ptpos - nzpos - ( (ptpos > nzpos) ? 1 : 0 );

    str.erase(str.begin() + ptpos);

    while( str.length() && str[0] == '0' )
      str.erase(str.begin());

    if (str.size() == 1)
      str.push_back('0');
    if (str.size() == 0)
      str = "00";

    str.insert(str.begin()+1, '.');
  }

  if (strchr(str.c_str(), '.') == NULL)
    str += ".0";
  int pos = (int)(strchr(str.c_str(), '.') - str.c_str());

  int decs = (int)str.length() - pos -1;

  if (decs > decimals)
  {

    int dot = decimals == 0 ? 1 : 0;
    str.resize(str.length() - (decs - decimals + dot));
  }
  else
  {

    int i = decimals - decs;
    while(i--)
      str.push_back('0');
  }

  if (finalizer == 'e' || finalizer == 'E')
  {
    str += 'e';
    str += format((signed long long)shift, base, 0, 0, 2, 0, 1, 0,0);
  }
  else

  if (eur)
    str = euronotation(str, base);

  int right = (field - (int)str.length()) / 2;
  right = right < 0 ? 0 : right;

  int left =  (field - (int)str.length()) - right;
  left = left < 0 ? 0 : left;

  if (align == 1) // left
  {
    right += left;
    left = 0;
  }
  else
  if (align == 2) // right
  {
    left += right;
    right = 0;
  }

  for(int i=0; i<left; i++)
    str.insert(str.begin(), (char)fill);

  for(int i=0; i<right; i++)
    str.push_back((char)fill);

  if (negative)
  {
    if (left)
      str.erase(str.begin());
    else
    if (right)
      str.resize(str.size()-1);

    str.insert(str.begin(), '-');
  }
  else
  if(plus)
  {
    if (left)
      str.erase(str.begin());
    else
    if (right)
      str.resize(str.size()-1);

    str.insert(str.begin(), '+');
  }

  if (finalizer == 'N' || finalizer == 'E')
  {
    for(int i=0; i<(int)str.length(); i++)
      if (str[i] >= 'a' && str[i] <= 'z')
        str[i] = str[i] - 'a' + 'A';
  }

  return str;
}

