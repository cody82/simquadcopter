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
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace vl;

const char* VISUALIZATION_LIBRARY_LICENSE = "VisualizationLibrary.com#PRJBX-PHOOH-PKWQY-UHRQZ-HNBNF-UVQNQ";

const char* MD5_SUM = "ef2a15557864944f0be93d81f2c539e0";

namespace vl
{
  void MD5(const char *string, unsigned char sum[33]);

  class License
  {
  public:
    bool CheckLicense(const char* license)
    {
      unsigned char sum[33];
      MD5(license, sum);
      return memcmp(MD5_SUM, sum, 33) == 0;
    }

    License()
    {
      if ( CheckLicense(VISUALIZATION_LIBRARY_LICENSE) )
      {
        VISUALIZATION_LIBRARY_LICENSE = NULL;
      }
      else
      {
        char* msg = 
        "Invalid license.\n"
        "\n"
        "Visit our website www.visualizationlibrary.com to obtain a valid license.\n"
        "\n"
        "Note that we provide FREE licenses for NON COMMERCIAL OPENSOURCE applications, which makes Visualization Library perfect for personal and educational use.\n"
        "\n"
        "Even if you don't match any of the above mentioned categories ask to our sales office for a personalized offer and special discounts: \n"
        "\n"
        "sales@visualizationlibary.com\n"
        "\n"
        "or visit the page:\n" 
        "\n"
        "www.visualizationlibrary.com/sales.html" 
        "\n";
        printf(msg);
        Time time;
        time.start();
        while(time.elapsed() < 15) {}
        exit(8);
      }
    }
  };
}

vl::License* license = new vl::License;
