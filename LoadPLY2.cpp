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

#include "LoadPLY2.hpp"

#include <string>
#include <iostream>
#include <sstream>
#include <istream>

using namespace std;

vl::ref<vl::Geometry> LoadPLY2::loadPLY(const std::string& plyfile)
{
  vl::ref<vl::Geometry> geom = new vl::Geometry;
  vl::ref<vl::GPUArrayVec3> verts = new vl::GPUArrayVec3;
  geom->setVertexArray( verts.get() );
  vl::ref<vl::GPUArrayVec2> texcoords;
  vl::ref<vl::GPUArrayVec3> normals;

  vl::ref<vl::DrawElementsUInt> polys = new vl::DrawElementsUInt(vl::PT_TRIANGLES);
  geom->addDrawCall( polys.get() );

  FILE* fin = fopen( plyfile.c_str(), "rt" );
  if(fin == NULL)
  {
    vl::Log::error( vl::Say("Could not open file: %s\n") << plyfile );
    return NULL;
  }

  char line[1024];
  memset(line,0,1024); fgets(line,1023,fin);
  if (strstr(line,"ply") == 0)
  {
    fclose(fin);
    vl::Log::error( vl::Say("Not a PLY file: %s\n") << plyfile );
    return NULL;
  }

  memset(line,0,1024); fgets(line,1023,fin);
  bool ascii_format = true;
  if (strstr(line,"format ascii 1.0"))
    ascii_format = true;

  else
  {
    fclose(fin);
    vl::Log::error( vl::Say("Not a PLY 1.0 file: %s\n") << plyfile );
    return NULL;
  }

  int vert_count = 0;
  while(!feof(fin))
  {
    fgets(line,1023,fin);
    if (strstr(line,"element vertex") == NULL)
      continue;
    else
    {
      vert_count = atoi(line+strlen("element vertex"));
      break;
    }
  }

  int property_index=0;
  int property_s=-1;
  int property_t=-1;
  int property_nx=-1;
  int property_ny=-1;
  int property_nz=-1;

  while(!feof(fin))
  {
    fgets(line,1023,fin);
    if (strstr(line,"property ") == NULL)
      break;
    else
    {
      char *p=line+strlen("property ");

      //skip data type (should be "float")
      while(*p!=' ')p++;
      p++;

      std::string str(p);

      //truncate newline
      if(str[str.length()-1]=='\n')str.erase(str.length()-1);

      std::cout << property_index << ": " << str << std::endl;

      //get property indices
      if(str=="s")property_s=property_index;
      else if(str=="t")property_t=property_index;
      else if(str=="nx")property_nx=property_index;
      else if(str=="ny")property_ny=property_index;
      else if(str=="nz")property_nz=property_index;

      property_index++;
    }
  }

  //create and assign property arrays
  if(property_s!=-1)
  {
    texcoords = new vl::GPUArrayVec2;
    geom->setTexCoordArray(0, texcoords.get());
  }
  if(property_nx!=-1)
  {
    normals = new vl::GPUArrayVec3;
    geom->setNormalArray(normals.get());
  }

  int face_count = 0;
  while(!feof(fin))
  {
    if (strstr(line,"element face") != NULL)
    {
      face_count = atoi(line+strlen("element face"));
      break;
    }
    fgets(line,1023,fin);
    if (strstr(line,"element face") == NULL)
      continue;
    else
    {
      face_count = atoi(line+strlen("element face"));
      break;
    }
  }

  while(!feof(fin))
  {
    fgets(line,1023,fin);
    if ( strstr(line,"end_header") )
      break;
  }

  if (ascii_format)
  {

    for(int i=0; i<vert_count; i++)
    {
      fgets(line,1023,fin);
      vl::vec3 v;
      sscanf( line, "%f %f %f", &v.x(), &v.y(), &v.z() );
      verts->push_back(v);

      std::stringstream ss(line);
      float f;
      vl::vec2 st;
      vl::vec3 n;
      for(int j=0;j<property_index;++j)
      {
         ss >> f;
         if(j==property_s)st.s()=f;
         else if(j==property_t)st.t()=f;
         else if(j==property_nx)n.x()=f;
         else if(j==property_ny)n.y()=f;
         else if(j==property_nz)n.z()=f;
      }
      if(property_s!=-1)texcoords->push_back(st);
      if(property_nx!=-1)normals->push_back(n);
    }

    for(int i=0; i<face_count; i++)
    {
      fgets(line,1023,fin);
      int n, a, b, c, d;
      sscanf( line, "%d", &n);
      if(n==3)
      {
        sscanf( line, "%d %d %d %d", &n, &a, &b, &c );
        /*polys->addIndex(a);
        polys->addIndex(c);
        polys->addIndex(b);*/
        polys->addIndex(b);
        polys->addIndex(c);
        polys->addIndex(a);
      }
      else if(n==4)
      {
        sscanf( line, "%d %d %d %d %d", &n, &a, &b, &c, &d);
        /*polys->addIndex(a);
        polys->addIndex(c);
        polys->addIndex(b);
        polys->addIndex(a);
        polys->addIndex(d);
        polys->addIndex(c);*/
        polys->addIndex(b);
        polys->addIndex(c);
        polys->addIndex(a);
        polys->addIndex(c);
        polys->addIndex(d);
        polys->addIndex(a);
      }
    }
  }
  else
  {

  }

  fclose(fin);
  if (vl::VERBOSITY_LEVEL > 1)
    vl::Log::info( vl::Say("PLY: verts=%n, faces=%n\n") << vert_count << face_count );
  if (property_nx==-1)
    geom->computeSmoothNormals();
  return geom.get();
}
